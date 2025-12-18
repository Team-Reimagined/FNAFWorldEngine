#include <iostream>
#include <cstdlib>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "rendering/window.hpp"

using namespace FNAFWorldEngine;

void setupLogger() {
    try {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/recent.txt", true);
        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("FNAFWorldEngine", sinks.begin(), sinks.end());
        spdlog::register_logger(logger);
        spdlog::set_default_logger(logger);
    } catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

int main(int argv, char** args) {
    setupLogger();
    std::atexit([]() {
        spdlog::shutdown();
    });

    Rendering::Window* window = new Rendering::Window();

    if (!window->createWindow(1280, 720, "FNAF World Engine")) {
        spdlog::error("Error creating window");
    }

    return 0;
}