#pragma once
#include <string_view>

namespace Logger {

    class Log {
    public:
        /**
         * @breif Starts logger thread and provides logger options, should only be used once before all other logger calls.
         * @param logFilePath optional - path where the log file should be created
         * @param colored optional - If terminal output should be colored according to severity
         * @param debug optional - If debug lines should be printed
         */
        static void Setup(std::string_view logFilePath, bool colored = false, bool debug = false);

        /**
         * @brief Severity level of informational messages
         * @param message Informational message to print
         */
        static void Info(std::string_view message);

        /**
         * @brief Severity level of Warning messages
         * @param message Warning message to print
         */
        static void Warn(std::string_view message);

        /**
         * @breif Severity level of Debug messages
         * @param message Debug message to print
         */
        static void Debug(std::string_view message);

        /**
         * @brief Severity level of Error messages
         * @param message Error message to print
         */
        static void Err(std::string_view message);

        /**
         * @brief Stops the logger thread, should be called when exiting the program
         */
        static void Stop();
    };
}
