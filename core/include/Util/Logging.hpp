#pragma once

#include <chrono>
#include <format>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <iostream>

namespace FWE::Util {
    class Logging {
    public:
        template <typename... Args>
        static void info(std::format_string<Args...> fmt, Args&&... args) {
            std::string str = std::format(fmt, std::forward<Args>(args)...);
            std::string log = std::format("[{}]: {} \x1b[0m \n", getCurrentTime(), str);
            getLogFile() << std::format("[{}]: {}", getCurrentTime(), str) << std::endl;
            std::cout << log << std::endl;
        }

        template <class... Args>
        static void warn(std::format_string<Args...> fmt, Args&&... args) {
            std::string str = std::format(fmt, std::forward<Args>(args)...);
            std::string log = std::format("\e[0;33m[{}]: {} \x1b[0m \n", getCurrentTime(), str);
            getLogFile() << std::format("[{}]: {}", getCurrentTime(), str) << std::endl;
            std::cout << log << std::endl;
        }

        template <class... Args>
        static void error(std::format_string<Args...> fmt, Args&&... args) {
            std::string str = std::format(fmt, std::forward<Args>(args)...);
            std::string log = std::format("\e[0;31m[{}]: {} \x1b[0m \n", getCurrentTime(), str);
            getLogFile() << std::format("[{}]: {}", getCurrentTime(), str) << std::endl;
            std::cout << log << std::endl;
        }

    private:
        static std::string getCurrentTime() {
            return std::format("{:%T}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())).c_str();
        }

        static std::ofstream& getLogFile() {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::ostringstream time;
            time << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

            if (!std::filesystem::exists("logs/")) {
                std::filesystem::create_directory("logs/");
            }

            std::string filename = std::format("logs/log-{}.txt", time.str());
            static std::ofstream logFile(filename);
            if (!logFile.is_open()) {
                logFile.open(filename, std::fstream::out | std::fstream::trunc);
                logFile <<"\n";
                logFile.close();
            }
            return logFile;
        }
    };
}