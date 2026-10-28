cmake_minimum_required(VERSION 3.25)

project(Tests VERSION 0.1.0 LANGUAGES C)

# =================================================== #
# ==================== НАСТРОЙКА ==================== #
# =================================================== #

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(C_FILES
    src/utils/DynamicArray.c
    src/utils/String.c
    src/utils/BinTree.c
    src/utils/StringView.c
    src/utils/Arguments.c
    src/rules/Changer.c
    src/rules/ModeParse.c
    src/rules/ModeString.c
    src/ui/App.c
    src/ui/menu/InputOutput.c
    src/ui/menu/MainMenu.c
    src/ui/menu/ElementMenu.c
    src/main.c
)

# =================================================== #
# ============ ПОДКЛЮЧЕНИЕ ЗАВИСИМОСТЕЙ ============= #
# =================================================== #

# =================================================== #
# =============== СОЗДАНИЕ БИНАРНИКОВ =============== #
# =================================================== #

add_executable(${EXECUTABLE_NAME} ${C_FILES})

# =================================================== #
# ================= TARGET НАСТРОЙКА ================ #
# =================================================== #

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_include_directories(${EXECUTABLE_NAME} PUBLIC
    "include/"
)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC m)