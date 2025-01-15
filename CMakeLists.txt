# TODO 1: Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.16..3.31)

set(PROJECT_NAME EMU)
set(PROJECT_LIB lib${PROJECT_NAME})

# TODO 2: Create a project named Tutorial
project(${PROJECT_NAME} VERSION 0.0.1
    LANGUAGES CXX)

# TODO 7: Set the project version number as 1.0 in the above project command

# TODO 6: Set the variable CMAKE_CXX_STANDARD to 11
# and the variable CMAKE_CXX_STANDARD_REQUIRED to True
# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO 8: Use configure_file to configure and copy TutorialConfig.h.in to
# TutorialConfig.h
configure_file(TutorialConfig.h.in TutorialConfig.h)

# TODO 3: Add an executable called Tutorial to the project
# Hint: Be sure to specify the source file as tutorial.cxx
# add the executable

# Glob files
file(GLOB_RECURSE SRC_FILES lib/src/main/src/*.cpp)

add_library(${PROJECT_LIB} ${SRC_FILES})
target_include_directories(${PROJECT_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/main/include)

add_executable(${PROJECT_NAME} app.cpp ${SRC_FILES})

# target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/main/include)

# TODO 9: Use target_include_directories to include ${PROJECT_BINARY_DIR}
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
)

# Fetch Google Test as needed
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ## Unit Testing with Google Test
enable_testing()

# Glob files
file(GLOB_RECURSE SRC_UNIT_FILES lib/src/test/src/*.c*)

add_executable(
    unit_test
    lib/src/test/src/hello_test.cc ${SRC_FILES} ${SRC_UNIT_FILES}
)
target_include_directories(unit_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/main/include)

# target_include_directories(unit_test INTERFACE $<BUILD_INTERFACE:}lib/src/main/include >)
target_link_libraries(
    unit_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_test)
