# =====================================================================
#  Makefile проекта «Телефонная книга» (Teleboock)
# ---------------------------------------------------------------------
#  Сборка через make (вместо CMake):
#    - утилиты (включая структуры и функции двусвязного упорядоченного
#      списка) собираются в ОТДЕЛЬНУЮ статическую библиотеку libutils.a;
#    - приложение (Teleboock) и тесты линкуются со статикой libutils.a.
#
#  Основные команды:
#    make           — собрать всё (libutils.a, приложение, тесты);
#    make lib       — собрать только статическую библиотеку;
#    make run       — собрать и запустить приложение;
#    make test      — собрать и запустить все тесты (unity);
#    make clean     — удалить артефакты сборки.
# =====================================================================

# ---------------------------------------------------------------------
# Общие настройки
# ---------------------------------------------------------------------
CC       := cc
AR       := ar
CPPFLAGS := -Iinclude
CFLAGS   := -std=c23 -Wall -Wextra
LDFLAGS  :=
ARFLAGS  := rcs

# Отладочная сборка по умолчанию (переопределить: make DEBUG=0)
DEBUG ?= 1
ifeq ($(DEBUG),1)
CFLAGS += -g -O0
else
CFLAGS += -O2
endif

# Каталог сборки: объектные файлы и артефакты — в build/
BUILD_DIR := build

# ---------------------------------------------------------------------
# Статическая библиотека утилит: libutils.a
# ---------------------------------------------------------------------
LIB_NAME := utils
LIB      := $(BUILD_DIR)/lib$(LIB_NAME).a
LIB_OBJS := $(BUILD_DIR)/utils/String.o \
            $(BUILD_DIR)/utils/SortedList.o \
            $(BUILD_DIR)/utils/Compars.o

# ---------------------------------------------------------------------
# Модуль «телефонная книга»: статически линкуется в приложение и тесты
# ---------------------------------------------------------------------
TELEBOCK_OBJS := $(BUILD_DIR)/teleboock/Contact.o \
                 $(BUILD_DIR)/teleboock/Teleboock.o

# ---------------------------------------------------------------------
# Прикладное приложение: Teleboock
# ---------------------------------------------------------------------
APP      := $(BUILD_DIR)/Teleboock
APP_OBJS := $(BUILD_DIR)/main.o \
            $(BUILD_DIR)/ui/menu/ElementMenu.o \
            $(BUILD_DIR)/ui/menu/InputOutput.o \
            $(BUILD_DIR)/ui/menu/MainMenu.o \
            $(BUILD_DIR)/ui/EditSArrayMenu.o \
            $(BUILD_DIR)/ui/EditContactMenu.o \
            $(BUILD_DIR)/ui/MainMenu.o

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
             $(BUILD_DIR)/tests/utils/SortedList.o \
             $(BUILD_DIR)/tests/teleboock/Contact.o \
             $(BUILD_DIR)/tests/teleboock/Teleboock.o
TEST_BINS := $(TEST_OBJS:.o=.test)

# ---------------------------------------------------------------------
# Целевые правила
# ---------------------------------------------------------------------
.PHONY: all
all: $(LIB) $(APP) $(TEST_BINS)

# Статическая библиотека libutils.a
.PHONY: lib
lib: $(LIB)
$(LIB): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

# Приложение: main + ui + teleboock, линкуется со статикой libutils.a
$(APP): $(APP_OBJS) $(TELEBOCK_OBJS) $(LIB)
	$(CC) $(CPPFLAGS) -o $@ $(APP_OBJS) $(TELEBOCK_OBJS) -L$(BUILD_DIR) -l$(LIB_NAME) $(LDFLAGS)

# Тесты: линкуются со статикой libutils.a.
# Правила явные (не шаблонные), чтобы make не считал объектные файлы
# тестов «промежуточными» и не удалял их после сборки.
$(BUILD_DIR)/tests/utils/String.test: \
    $(BUILD_DIR)/tests/utils/String.o $(UNITY_OBJS) $(LIB)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/utils/String.o $(UNITY_OBJS) -L$(BUILD_DIR) -l$(LIB_NAME) $(LDFLAGS)

$(BUILD_DIR)/tests/utils/SortedList.test: \
    $(BUILD_DIR)/tests/utils/SortedList.o $(UNITY_OBJS) $(LIB)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/utils/SortedList.o $(UNITY_OBJS) -L$(BUILD_DIR) -l$(LIB_NAME) $(LDFLAGS)

# Тесты на teleboock: дополнительно линкуют модуль teleboock
$(BUILD_DIR)/tests/teleboock/Contact.test: \
    $(BUILD_DIR)/tests/teleboock/Contact.o $(TELEBOCK_OBJS) $(UNITY_OBJS) $(LIB)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/teleboock/Contact.o $(TELEBOCK_OBJS) $(UNITY_OBJS) -L$(BUILD_DIR) -l$(LIB_NAME) $(LDFLAGS)

$(BUILD_DIR)/tests/teleboock/Teleboock.test: \
    $(BUILD_DIR)/tests/teleboock/Teleboock.o $(TELEBOCK_OBJS) $(UNITY_OBJS) $(LIB)
	$(CC) $(CPPFLAGS) -o $@ $(BUILD_DIR)/tests/teleboock/Teleboock.o $(TELEBOCK_OBJS) $(UNITY_OBJS) -L$(BUILD_DIR) -l$(LIB_NAME) $(LDFLAGS)

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

# Библиотека: обычный код (статика, -fPIC не требуется)
$(BUILD_DIR)/utils/%.o: src/utils/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Тесты: дополнительные include пути Unity
$(BUILD_DIR)/tests/%.o: src/tests/%.c $(UNITY_DIR)/.stamp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(UNITY_INCS) -c $< -o $@

# Unity
$(BUILD_DIR)/unity/%.o: $(UNITY_DIR)/src/%.c $(UNITY_DIR)/.stamp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(UNITY_INCS) -c $< -o $@

# Загрузка Unity (wget + unzip); zip распаковывается в папку Unity-2.7.0
$(UNITY_DIR)/.stamp:
	@rm -rf $(UNITY_DIR) $(BUILD_DIR)/.unity-unpack
	@mkdir -p $(BUILD_DIR)/.unity-unpack
	wget -q -O $(BUILD_DIR)/.unity.zip $(UNITY_URL)
	unzip -oq $(BUILD_DIR)/.unity.zip -d $(BUILD_DIR)/.unity-unpack
	mv $(BUILD_DIR)/.unity-unpack/Unity-2.7.0 $(UNITY_DIR)
	rm -rf $(BUILD_DIR)/.unity-unpack $(BUILD_DIR)/.unity.zip
	touch $@
$(UNITY_OBJS): $(UNITY_DIR)/.stamp

# ---------------------------------------------------------------------
# Вспомогательные цели
# ---------------------------------------------------------------------
# Статика «зашита» в бинарник: запускать можно без LD_LIBRARY_PATH
.PHONY: run
run: $(APP)
	$(APP)

.PHONY: test
test: $(TEST_BINS)
	@for t in $(TEST_BINS); do \
		echo "=== $$t ==="; \
		$$t || exit 1; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

