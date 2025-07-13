# Archivos de test
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

# Archivos del sistema sin main.cpp
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES "${PROJECT_SOURCE_DIR}/src/main.cpp")

# Ejecutable de pruebas
add_executable(tests_runner
    ${SRC_FILES}
    ${TEST_SOURCES}
)

target_include_directories(tests_runner PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(tests_runner PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests_runner)