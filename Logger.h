// Logger.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <string_view>


class Logger {
public:
    static void Setup(std::string_view logFilePath);
    static void Info();
    static void Warn(std::string_view message);
    static void Debug();
    static void Err();
    static void Stop();
};
