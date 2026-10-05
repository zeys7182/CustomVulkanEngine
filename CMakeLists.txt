cmake_minimum_required(VERSION 3.20)

project(MyVulkanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glm URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip)
FetchContent_MakeAvailable(glm)

add_executable(MyVulkanEngine
    src/main.cpp
    src/engine/VulkanContext.cpp
)

target_link_libraries(MyVulkanEngine PRIVATE Vulkan::Vulkan glfw glm::glm)