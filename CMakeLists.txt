cmake_minimum_required(VERSION 3.20)

project(MyVulkanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_executable(MyVulkanEngine
    src/main.cpp
    )

target_link_libraries(MyVulkanEngine
    PRIVATE Vulkan::Vulkan
    )