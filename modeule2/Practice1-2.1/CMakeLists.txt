cmake_minimum_required(VERSION 3.25)

project(Tests VERSION 0.1.0 LANGUAGES C)

# =================================================== #
# ==================== НАСТРОЙКА ==================== #
# =================================================== #

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CPP_FILES
    src/main.c

    src/utils/String.c
    src/utils/DynamicArray.c

    src/teleboock/Contact.c
    src/teleboock/Teleboock.c

    src/ui/menu/ElementMenu.c
    src/ui/menu/InputOutput.c
    src/ui/menu/MainMenu.c

    src/ui/EditSArrayMenu.c
    src/ui/EditContactMenu.c
    src/ui/MainMenu.c
)

# =================================================== #
# ============ ПОДКЛЮЧЕНИЕ ЗАВИСИМОСТЕЙ ============= #
# =================================================== #

# =================================================== #
# =============== СОЗДАНИЕ БИНАРНИКОВ =============== #
# =================================================== #

add_executable(${EXECUTABLE_NAME} ${CPP_FILES})

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
