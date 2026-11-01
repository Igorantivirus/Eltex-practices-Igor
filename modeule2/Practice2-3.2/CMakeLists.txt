cmake_minimum_required(VERSION 3.25)

project(rch VERSION 0.1.0 LANGUAGES C)

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
    src/utils/Rand.c
    src/utils/UInt32Array.c

    src/ui/InputArgs.c
    src/ui/Parser.c
    src/ui/Commands.c

    src/stat/Statistick.c
)

target_include_directories(${CORE_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set_target_properties(${CORE_NAME} PROPERTIES
    ${ALL_TARGETS_PROPERTIES}
)
target_link_libraries(${CORE_NAME} PRIVATE
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:m>
)

# =================================================== #
# =================== ПРИЛОЖЕНИЕ ==================== #
# =================================================== #

set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(${EXECUTABLE_NAME}
    src/main.c
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${CORE_NAME}
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:m>
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

    target_compile_definitions(unity PUBLIC UNITY_INCLUDE_DOUBLE)

    function(add_unity_test test_name)
        add_executable(${test_name} ${ARGN})

        target_link_libraries(${test_name} PRIVATE
            ${CORE_NAME}
            unity
        )

        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()

    add_unity_test(rand_test
        src/tests/utils/Rand.c
    )

    add_unity_test(parser_test
        src/tests/ui/Parser.c
    )
    add_unity_test(input_args_test
        src/tests/ui/InputArgs.c
    )
    add_unity_test(commands_test
        src/tests/ui/Commands.c
    )

    add_unity_test(statistick_test
        src/tests/stat/Statistick.c
    )
endif()
