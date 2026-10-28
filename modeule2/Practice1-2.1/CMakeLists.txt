cmake_minimum_required(VERSION 3.25)

project(Teleboock VERSION 0.1.0 LANGUAGES C)

# Применяется ко всем нашим целям.
set(ALL_TARGETS_PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

# =================================================== #
# ================== Ядро проекта =================== #
# =================================================== #

set(CORE_NAME "${PROJECT_NAME}_core")

add_library(${CORE_NAME} STATIC
    src/utils/String.c
    src/utils/DynamicArray.c

    src/teleboock/Contact.c
    src/teleboock/Teleboock.c
)

target_include_directories(${CORE_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set_target_properties(${CORE_NAME} PROPERTIES
    ${ALL_TARGETS_PROPERTIES}
)

# =================================================== #
# =================== ПРИЛОЖЕНИЕ ==================== #
# =================================================== #

set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(${EXECUTABLE_NAME}
    src/main.c

    src/ui/menu/ElementMenu.c
    src/ui/menu/InputOutput.c
    src/ui/menu/MainMenu.c

    src/ui/EditSArrayMenu.c
    src/ui/EditContactMenu.c
    src/ui/MainMenu.c
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CORE_NAME}
)

# =================================================== #
# ====================== ТЕСТЫ ====================== #
# =================================================== #

set(TESTING_NAME "${PROJECT_NAME}_tests")

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        unity
        URL https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v2.7.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(unity)

    function(add_unity_test test_name)
        add_executable(${test_name} ${ARGN})

        target_link_libraries(${test_name} PRIVATE
            ${CORE_NAME}
            unity
        )

        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()

    add_unity_test(string_test
        src/tests/utils/String.c
    )
    add_unity_test(dynamic_array_test
        src/tests/utils/DynamicArray.c
    )

    add_unity_test(contact_test
        src/tests/teleboock/Contact.c
    )
    add_unity_test(teleboock_test
        src/tests/teleboock/Teleboock.c
    )
endif()