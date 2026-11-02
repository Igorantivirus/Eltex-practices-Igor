#include <queue/PriorityCycleQueue.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Параметры моделирования
static const uint32_t simTicks = 5000u; // Тиков
static const uint32_t rngSeed = 228u;   // Сид
static const char    *csvPath = "simulation.csv";

static const int32_t priorityMin = 0;  // Минимальный приоритет
static const int32_t priorityMax = 15; // Максимальный приоритет

static const double pGenHeavy = 0.60; // Вероятность генерации события в фазе перегрузки
static const double pGenLight = 0.15; // Вероятность генерации события в фазе разгрузки

static const uint32_t durationMin = 1u; // Минимальное время обработки события
static const uint32_t durationMax = 4u; // Максимальное время обработки события

static const uint32_t highFillCount = 200u; // Порог заполнения, выше которого система считается перегруженной
static const int32_t  midPriority = 4;      // Порог приоритета, ниже которого события не извлекаются в спокойной фазе
static const uint32_t kPopPriority = 4u;    // Периодичность вызова pcqPopPriority в спокойной фазе (каждые k тиков)

static double randUnit(void)
{
    return (double)rand() / (double)RAND_MAX;
}

static int32_t randI32(int32_t lo, int32_t hi)
{
    return lo + rand() % (hi - lo + 1);
}

static uint32_t randU32(uint32_t lo, uint32_t hi)
{
    return lo + (uint32_t)rand() % (hi - lo + 1u);
}

// Статистика
typedef struct
{
    uint32_t pushedOk;          // Успешных pcqPush
    uint32_t pushedFail;        // Отказов pcqPush (переполнение)
                                //
    uint32_t poppedTotal;       // Всего успешных извлечений
    uint32_t poppedPop;         // Извлечено через pcqPop
    uint32_t poppedPrio;        // Извлечено через pcqPopPriority
    uint32_t poppedAtLeast;     // Извлечено через pcqPopAtLeast
    uint32_t poppedFallback;    // Извлечено через pcqPop как fallback после провала popAtLeast
                                //
    uint32_t failPrio;          // Пустых вызовов pcqPopPriority
    uint32_t failAtLeast;       // Пустых вызовов pcqPopAtLeast
                                //
    uint32_t idleTicks;         // Тиков, в которые очередь пуста и система простаивает
    uint32_t peakSize;          // Максимальный размер очереди за прогон
    uint32_t sumDurationPopped; // Суммарное время обработки всех извлечённых событий
    uint64_t busyTicks;         // Тиков, в которые система занята обработкой
} SimStats;

// Запись журнала в CSV
static void logHeader(FILE *f)
{
    fprintf(f, "tick,action,method,reqPriority,duration,resultPriority,ok\n");
}

static void logPush(FILE *f, uint32_t t, int32_t prio, int32_t dur, bool ok)
{
    fprintf(f, "%u,push,-,%d,%d,-,%d\n", t, prio, dur, (int)ok);
}

static void logPop(FILE *f, uint32_t t, const char *method, int32_t req, int32_t dur, int32_t got, bool ok)
{
    fprintf(f, "%u,pop,%s,%d,%d,%d,%d\n",
            t, method, req,
            ok ? dur : -1,
            ok ? got : -1,
            (int)ok);
}

static bool printStats(const SimStats *st, uint32_t finalSize)
{
    printf("\n========== Итоги симуляции ==========\n");
    printf("Успешных добавлений (pushedOk)           : %u\n", st->pushedOk);
    printf("Отказов при добавлении (pushedFail)      : %u\n", st->pushedFail);
    printf("Всего извлечено (poppedTotal)            : %u\n", st->poppedTotal);
    printf("  через pcqPop                           : %u\n", st->poppedPop);
    printf("  через pcqPopPriority                   : %u\n", st->poppedPrio);
    printf("  через pcqPopAtLeast                    : %u\n", st->poppedAtLeast);
    printf("  через pcqPop (fallback после AtLeast)  : %u\n", st->poppedFallback);
    printf("Пустых вызовов pcqPopPriority (failPrio) : %u\n", st->failPrio);
    printf("Пустых вызовов pcqPopAtLeast (failAtLeast): %u\n", st->failAtLeast);
    printf("Тиков простоя (idleTicks)                : %u\n", st->idleTicks);
    printf("Тиков занятости (busyTicks)              : %llu\n", (unsigned long long)st->busyTicks);
    printf("Суммарное время обработки                : %u\n", st->sumDurationPopped);
    printf("Пиковый размер очереди (peakSize)        : %u\n", st->peakSize);
    printf("Итоговый размер очереди (finalSize)      : %u\n", finalSize);

    const bool balanceOk = (st->pushedOk == st->poppedTotal + finalSize);
    printf("\nИнвариант pushedOk == poppedTotal + finalSize : %s\n",
           balanceOk ? "OK" : "НАРУШЕН");
    return balanceOk;
}

// Моделирование
int main(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    srand(rngSeed);

    FILE *csvFile = fopen(csvPath, "w");
    if (!csvFile)
    {
        fprintf(stderr, "Не удалось открыть файл %s\n", csvPath);
        return 1;
    }

    PriorityCycleQueue pcq = pcqInit(priorityMin, priorityMax);
    SimStats           st = {0};

    logHeader(csvFile);

    printf("========== Симуляция очереди с приоритетом ==========\n");
    printf("Сид ГПСЧ                  : %u\n", rngSeed);
    printf("Тиков симуляции           : %u\n", simTicks);
    printf("Вероятность генерации     : %.2f (перегрузка) / %.2f (разгрузка)\n", pGenHeavy, pGenLight);
    printf("Диапазон времени обработки: [%u..%u]\n", durationMin, durationMax);
    printf("Порог перегрузки          : %u\n", highFillCount);
    printf("Порог popAtLeast          : %d\n", midPriority);
    printf("Период popPriority        : каждые %u тиков\n", kPopPriority);
    printf("Файл журнала              : %s\n\n", csvPath);

    const uint32_t phaseSplit = simTicks / 2u;
    uint64_t       busyUntil = 0;

    for (uint32_t t = 0; t < simTicks; ++t)
    {

        if ((uint64_t)t < busyUntil)
            st.busyTicks++;

        /* --- Генерация нового события --- */
        const double pGen = (t < phaseSplit) ? pGenHeavy : pGenLight;
        if (randUnit() < pGen)
        {
            const int32_t prio = randI32(priorityMin, priorityMax);
            const int32_t dur = (int32_t)randU32(durationMin, durationMax);

            const bool ok = pcqPush(&pcq, (QType)dur, prio);
            logPush(csvFile, t, prio, dur, ok);
            if (ok)
                st.pushedOk++;
            else
                st.pushedFail++;
        }

        if (pcq.size > st.peakSize)
            st.peakSize = pcq.size;

        // Если система свободна - континую
        if ((uint64_t)t < busyUntil)
            continue;

        QType   d = 0;
        int32_t p = 0;
        bool    ok = false;

        if (pcq.size > highFillCount)
        {
            /* Перегрузка: берём строго самого важного */
            ok = pcqPop(&pcq, &d, &p);
            logPop(csvFile, t, "pop", -1, (int)d, p, ok);
            if (ok)
                st.poppedPop++;
        }
        else if ((t % kPopPriority) == 0u)
        {
            /* Каждые k тиков пробуем точный приоритет */
            const int32_t req = randI32(priorityMin, priorityMax);
            ok = pcqPopPriority(&pcq, &d, req);
            logPop(csvFile, t, "popPriority", req, (int)d, p, ok);
            if (ok)
                st.poppedPrio++;
            else
                st.failPrio++;
        }
        else
        {
            /* Спокойная фаза: не ниже порога, с fallback на pcqPop */
            const int32_t req = midPriority;
            ok = pcqPopAtLeast(&pcq, &d, req, &p);
            if (ok)
            {
                logPop(csvFile, t, "popAtLeast", req, (int)d, p, true);
                st.poppedAtLeast++;
            }
            else
            {
                logPop(csvFile, t, "popAtLeast", req, 0, 0, false);
                st.failAtLeast++;
                ok = pcqPop(&pcq, &d, &p);
                logPop(csvFile, t, "pop(fallback)", -1, (int)d, p, ok);
                if (ok)
                    st.poppedFallback++;
            }
        }

        if (ok)
        {
            st.poppedTotal++;
            st.sumDurationPopped += (uint32_t)d;
            busyUntil = (uint64_t)t + (uint64_t)d;
        }
        else
        {
            st.idleTicks++;
        }
    }

    const bool balanceOk = printStats(&st, pcq.size);

    pcqFree(&pcq);
    fclose(csvFile);
    return balanceOk ? 0 : 1;
}
