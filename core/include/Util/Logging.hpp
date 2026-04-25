#pragma once

#include <chrono>
#include <format>
#include <string>
#include <stdio.h>

namespace FWE::Util {
    class Logging {
    public:
        template <typename... Args>
        static void info(std::format_string<Args...> fmt, Args&&... args) {
            std::string str = std::format(fmt, std::forward<Args>(args)...);
            printf("[%s]: %s \x1b[0m \n", Logging::getCurrentTime().c_str(), str.c_str());
        }

        template <class... Args>
        static void warn(std::format_string<Args...> fmt, Args&&... args) {
            std::string str = std::format(fmt, std::forward<Args>(args)...);
            printf("\e[0;33m[%s]: %s \x1b[0m \n", Logging::getCurrentTime().c_str(), str.c_str());
        }

        template <class... Args>
        static void error(std::format_string<Args...> fmt, Args&&... args) {
            std::string str = std::format(fmt, std::forward<Args>(args)...);
            printf("\e[0;31m[%s]: %s \x1b[0m \n", Logging::getCurrentTime().c_str(), str.c_str());
        }

    private:
        static std::string getCurrentTime() {
            return std::format("{:%T}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())).c_str();
        }
    };
}