#include <iostream>
#include <spdlog/spdlog.h>

#include "rendering/window.hpp"

using namespace FNAFWorldEngine;

int main(int argv, char** args) {
    Rendering::Window* window = new Rendering::Window();

    if (!window->createWindow(1280, 720, "FNAF World Engine")) {
        spdlog::error("Error creating window");
    }

    return 0;
}