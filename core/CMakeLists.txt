cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(FWECore VERSION 1.0.0 LANGUAGES CXX)

option(FWE_CORE_BUILD_TESTS "Build Core Library Tests" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.4
)
FetchContent_MakeAvailable(SDL3)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
)
target_include_directories(${PROJECT_NAME} PRIVATE include/)

if (FWE_CORE_BUILD_TESTS)
    file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
    add_executable(fwe_core_test ${TEST_SOURCES})
    target_link_libraries(fwe_core_test PRIVATE FWECore)
endif()