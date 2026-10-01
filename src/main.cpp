#include "engine/VulkanContext.h"

#include <exception>
#include <iostream>

int main() {
    try {
        VulkanContext context;
        context.run();
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}