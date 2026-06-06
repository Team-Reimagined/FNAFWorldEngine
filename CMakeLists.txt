cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(FNAFWorldEngine VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(core/)
add_subdirectory(editor/)
add_subdirectory(runtime/)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")