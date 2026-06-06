cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(FWECore VERSION 1.0.0 LANGUAGES CXX)

option(FWE_CORE_BUILD_TESTS "Build Core Library Tests" ON)

option(FWE_SHADER_COMPILATION "Compile Shaders" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.4
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
    GIT_TAG release-3.2.0
)
FetchContent_MakeAvailable(SDL3_mixer)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    vk-bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG v1.4.349
)
FetchContent_MakeAvailable(vk-bootstrap)

FetchContent_Declare(
    VulkanMemoryAllocator
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(VulkanMemoryAllocator)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.92.7
)
FetchContent_MakeAvailable(imgui)

find_package(Vulkan)

add_library(imgui STATIC 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC
    Vulkan::Vulkan
    SDL3::SDL3
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    SDL3_mixer::SDL3_mixer
    glm::glm
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
    Vulkan::Vulkan
    imgui
)

target_include_directories(${PROJECT_NAME} PUBLIC include/ ${vulkanmemoryallocator_SOURCE_DIR}/include)

if(FWE_SHADER_COMPILATION)
    find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
    
    file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/shaders/*.frag"
        "${CMAKE_SOURCE_DIR}/shaders/*.vert"
        "${CMAKE_SOURCE_DIR}/shaders/*.comp"
        )
    
    foreach(GLSL ${GLSL_SOURCE_FILES})
      message(STATUS "BUILDING SHADER")
      get_filename_component(FILE_NAME ${GLSL} NAME)
      set(SPIRV "${CMAKE_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
      message(STATUS ${GLSL})
      message(STATUS COMMAND " " ${GLSL_VALIDATOR} " -V " ${GLSL} " -o "  ${SPIRV})
      add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL})
      list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endforeach(GLSL)
    
    add_custom_target(
        Shaders ALL
        DEPENDS ${SPIRV_BINARY_FILES}
    )
endif()

if (FWE_CORE_BUILD_TESTS)
    file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
    add_executable(fwe_core_test ${TEST_SOURCES})
    target_link_libraries(fwe_core_test PRIVATE FWECore)
endif()
