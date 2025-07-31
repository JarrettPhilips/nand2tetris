cmake_minimum_required(VERSION 3.14)
project(nand2tetris)

set(CMAKE_CXX_STANDARD 17)

# Your source files
add_executable(assembler assembler.cpp)
target_include_directories(assembler PUBLIC ${CMAKE_SOURCE_DIR})

# Download Catch2 (v3+)
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.3
)
FetchContent_MakeAvailable(catch2)

# Enable testing and discover tests
enable_testing()
include(Catch)


add_executable(assembler_test assembler_test.cpp assembler.cpp)
target_compile_definitions(assembler_test PRIVATE TEST_MODE)
target_link_libraries(assembler_test PRIVATE Catch2::Catch2WithMain)

# Register tests
catch_discover_tests(assembler_test)