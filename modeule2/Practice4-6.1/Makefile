# =====================================================================
#  Makefile проекта «Телефонная книга» (Teleboock)
# ---------------------------------------------------------------------
#  Сборка через make (вместо CMake):
#    - утилиты (включая структуры и функции двусвязного упорядоченного
#      списка) собираются в ОТДЕЛЬНУЮ динамическую библиотеку libutils.so;
#    - приложение (Teleboock) и тесты линкуются против libutils.so.
#
#  Основные команды:
#    make           — собрать всё (libutils.so, приложение, тесты);
#    make lib       — собрать только динамическую библиотеку;
#    make run       — собрать и запустить приложение;
#    make test      — собрать и запустить все тесты (unity);
#    make clean     — удалить артефакты сборки.
# =====================================================================

# ---------------------------------------------------------------------
# Общие настройки
# ---------------------------------------------------------------------
CC       := cc
CPPFLAGS := -Iinclude
CFLAGS   := -std=c23 -Wall -Wextra
LDFLAGS  :=

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
# Динамическая библиотека утилит: libutils.so
# ---------------------------------------------------------------------
LIB_NAME := utils
LIB      := $(BUILD_DIR)/lib$(LIB_NAME).so
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

# Динамическая библиотека libutils.so
.PHONY: lib
lib: $(LIB)
$(LIB): $(LIB_OBJS)
	$(CC) -shared -o $@ $(LIB_OBJS)

# Приложение: main + ui + teleboock, линкуется против libutils.so
$(APP): $(APP_OBJS) $(TELEBOCK_OBJS) $(LIB)
	$(CC) $(CPPFLAGS) -o $@ $(APP_OBJS) $(TELEBOCK_OBJS) -L$(BUILD_DIR) -l$(LIB_NAME) $(LDFLAGS)

# Тесты: линкуются против libutils.so.
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

# Библиотека: обязательный -fPIC (разделяемый код)
$(BUILD_DIR)/utils/%.o: src/utils/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $< -o $@

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
.PHONY: run
run: $(APP)
	cd $(BUILD_DIR) && LD_LIBRARY_PATH=$(CURDIR)/$(BUILD_DIR):$$LD_LIBRARY_PATH ./Teleboock

.PHONY: test
test: $(TEST_BINS)
	@for t in $(TEST_BINS); do \
		echo "=== $$t ==="; \
		LD_LIBRARY_PATH=$(CURDIR)/$(BUILD_DIR) $$t || exit 1; \
	done

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

