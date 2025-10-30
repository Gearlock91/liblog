
#include <print>
#include "Logger.h"
#include <string_view>

class ImplLogger {
public:
    ImplLogger& instance();
    void setLogFilePath(std::string_view path);
private:
    ImplLogger() = default;
    std::string_view mLogFilePath;

};

ImplLogger& ImplLogger::instance() {
    static ImplLogger mInstance{};
    return mInstance;
}

void ImplLogger::setLogFilePath(std::string_view path) {
    mLogFilePath = path;
}

void Logger::Setup(std::string_view logFilePath)
{
    ImplLogger::instance();
}

void Logger::Info()
{
    
}

void Logger::Warn() 
{
    std::println("Somethingelse");
}

void Logger::Err()
{
}
