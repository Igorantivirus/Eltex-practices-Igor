# =====================================================================
#  Makefile проекта «Калькулятор» (Calculator)
# ---------------------------------------------------------------------
#  Сборка через make (вместо CMake):
#    - 5 математических операций собираются в ОТДЕЛЬНЫЕ динамические
#      библиотеки libSum.so, libSub.so, libMul.so, libDiv.so, libPow.so
#      — все в одну папку build/libs (подгружаются приложением в runtime
#      через dlopen/dlsym);
#    - приложение (Calculator) и тесты линкуются со статикой ядра
#      (utils + parse + calc + BaseCalcBTInit).
#
#  Основные команды:
#    make           — собрать всё (5 библиотек, приложение, тесты);
#    make libs      — собрать только динамические библиотеки операций;
#    make run       — собрать и запустить приложение (аргумент: build/libs);
#    make test      — собрать и запустить все тесты (unity);
#    make clean     — удалить артефакты сборки.
# =====================================================================

# ---------------------------------------------------------------------
# Общие настройки
# ---------------------------------------------------------------------
CC       := cc
CPPFLAGS := -Iinclude
CFLAGS   := -std=c23 -Wall -Wextra
# Прикладной бинарник и тесты: dl (dlopen/dlsym) и math (pow, nan, isnan)
LDLIBS   := -ldl -lm
LDFLAGS  :=

# Отладочная сборка по умолчанию (переопределить: make DEBUG=0)
DEBUG ?= 1
ifeq ($(DEBUG),1)
CFLAGS += -g -O0
else
CFLAGS += -O2
endif

# Каталог сборки: объектные файлы и артефакты — в build/
BUILD_DIR  := build
# Каталог динамических библиотек операций (одна папка для всех)
LIBS_DIR   := $(BUILD_DIR)/libs

# ---------------------------------------------------------------------
# Динамические библиотеки математических операций: lib<Name>.so
# ---------------------------------------------------------------------
PLUGIN_NAMES := Sum Sub Mul Div Pow
PLUGIN_SO    := $(addprefix $(LIBS_DIR)/lib,$(addsuffix .so,$(PLUGIN_NAMES)))

# ---------------------------------------------------------------------
# Ядро проекта (статически линкуется в приложение и тесты)
# ---------------------------------------------------------------------
CORE_OBJS := $(BUILD_DIR)/utils/BinTree.o \
             $(BUILD_DIR)/utils/String.o \
             $(BUILD_DIR)/utils/DynamicArray.o \
             $(BUILD_DIR)/parse/StringView.o \
             $(BUILD_DIR)/parse/Parser.o \
             $(BUILD_DIR)/calc/Callbacks.o \
             $(BUILD_DIR)/calc/CalcTree.o \
             $(BUILD_DIR)/calc/DynamicLibrarys.o \
             $(BUILD_DIR)/calc/LibraryContext.o \
             $(BUILD_DIR)/ui/BaseCalcBTInit.o

# ---------------------------------------------------------------------
# Прикладное приложение: Calculator (main + ui/Calc + ядро)
# ---------------------------------------------------------------------
APP      := $(BUILD_DIR)/Calculator
APP_OBJS := $(BUILD_DIR)/main.o \
             $(BUILD_DIR)/ui/Calc.o

# ---------------------------------------------------------------------
# Фреймворк тестирования Unity (внешняя зависимость, версия 2.7.0)
# ---------------------------------------------------------------------
UNITY_URL  := https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v2.7.0.zip
UNITY_DIR  := $(BUILD_DIR)/unity-src
UNITY_INCS := $(UNITY_DIR)/src
UNITY_OBJS := $(BUILD_DIR)/unity/unity.o

# ---------------------------------------------------------------------
# Тесты (объектные файлы зеркалят структуру src/tests/...)
# ---------------------------------------------------------------------
TEST_OBJS := $(BUILD_DIR)/tests/utils/String.o \
             $(BUILD_DIR)/tests/utils/DynamicArray.o \
             $(BUILD_DIR)/tests/parse/Parser.o \
             $(BUILD_DIR)/tests/calc/CalcTree.o \
             $(BUILD_DIR)/tests/CalcTest.o
TEST_BINS := $(TEST_OBJS:.o=.test)

# ---------------------------------------------------------------------
# Целевые правила
# ---------------------------------------------------------------------
.PHONY: all
all: $(PLUGIN_SO) $(APP) $(TEST_BINS)

# Динамические библиотеки операций (одна папка build/libs)
.PHONY: libs
libs: $(PLUGIN_SO)
$(LIBS_DIR)/lib%.so: src/libs/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared -o $@ $< -lm

# Приложение: main + ui + ядро, линкуется с dl и math
$(APP): $(APP_OBJS) $(CORE_OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(APP_OBJS) $(CORE_OBJS) $(LDLIBS) $(LDFLAGS)

# Тесты: линкуются с ядром + Unity + dl/math.
# Правила явные (не шаблонные), чтобы make не считал объектные файлы
# тестов «промежуточными» и не удалял их после сборки.
$(BUILD_DIR)/tests/utils/String.test: \
    $(BUILD_DIR)/tests/utils/String.o $(UNITY_OBJS) $(CORE_OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/utils/String.o $(UNITY_OBJS) $(CORE_OBJS) $(LDLIBS) $(LDFLAGS)

$(BUILD_DIR)/tests/utils/DynamicArray.test: \
    $(BUILD_DIR)/tests/utils/DynamicArray.o $(UNITY_OBJS) $(CORE_OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/utils/DynamicArray.o $(UNITY_OBJS) $(CORE_OBJS) $(LDLIBS) $(LDFLAGS)

$(BUILD_DIR)/tests/parse/Parser.test: \
    $(BUILD_DIR)/tests/parse/Parser.o $(UNITY_OBJS) $(CORE_OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/parse/Parser.o $(UNITY_OBJS) $(CORE_OBJS) $(LDLIBS) $(LDFLAGS)

$(BUILD_DIR)/tests/calc/CalcTree.test: \
    $(BUILD_DIR)/tests/calc/CalcTree.o $(UNITY_OBJS) $(CORE_OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/calc/CalcTree.o $(UNITY_OBJS) $(CORE_OBJS) $(LDLIBS) $(LDFLAGS)

$(BUILD_DIR)/tests/CalcTest.test: \
    $(BUILD_DIR)/tests/CalcTest.o $(UNITY_OBJS) $(CORE_OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/CalcTest.o $(UNITY_OBJS) $(CORE_OBJS) $(LDLIBS) $(LDFLAGS)

# Удобная цель: собрать все тесты.
.PHONY: tests
tests: $(TEST_BINS)

# ---------------------------------------------------------------------
# Компиляция
# ---------------------------------------------------------------------
# Общие правила: build/<path>.o <- src/<path>.c
$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Тесты: дополнительные include пути Unity + определение UNITY_INCLUDE_DOUBLE
$(BUILD_DIR)/tests/%.o: src/tests/%.c $(UNITY_DIR)/.stamp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(UNITY_INCS) -DUNITY_INCLUDE_DOUBLE -c $< -o $@

# Unity
$(BUILD_DIR)/unity/%.o: $(UNITY_DIR)/src/%.c $(UNITY_DIR)/.stamp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(UNITY_INCS) -DUNITY_INCLUDE_DOUBLE -c $< -o $@

# Загрузка Unity (wget + unzip); zip распаковывается в папку Unity-2.7.0
$(UNITY_DIR)/.stamp:
	@rm -rf $(UNITY_DIR) $(BUILD_DIR)/.unity-unpack
	@mkdir -p $(BUILD_DIR)/.unity-unpack
	@wget -q -O $(BUILD_DIR)/.unity.zip $(UNITY_URL)
	@unzip -oq $(BUILD_DIR)/.unity.zip -d $(BUILD_DIR)/.unity-unpack
	@mv $(BUILD_DIR)/.unity-unpack/Unity-2.7.0 $(UNITY_DIR)
	@rm -rf $(BUILD_DIR)/.unity-unpack $(BUILD_DIR)/.unity.zip
	@touch $@
$(UNITY_OBJS): $(UNITY_DIR)/.stamp

# ---------------------------------------------------------------------
# Вспомогательные цели
# ---------------------------------------------------------------------
# Приложение получает путь к папке с библиотеками первым аргументом (argv[1]).
.PHONY: run
run: $(APP) $(PLUGIN_SO)
	$(APP) $(LIBS_DIR)

.PHONY: test
test: $(TEST_BINS)
	@for t in $(TEST_BINS); do \
		echo "=== $$t ==="; \
		$$t || exit 1; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

