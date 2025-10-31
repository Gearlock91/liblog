#include <print>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>
#include <chrono>
#include <string>

#include "Log.h"

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class ImplLogger {
public:
    static ImplLogger& instance();

    void setLogFilePath(std::string_view path);

    void addToQueue(LogLevel level, const std::string& message);

    void setColored(bool colored);

    void setDebug(bool debug);

    void stop();

    void start();

    [[nodiscard]] bool getColored() const;

    [[nodiscard]] std::string_view getLogFilePath() const;

    [[nodiscard]] bool isDebug() const;

    [[nodiscard]] std::string getLogLevelColor(LogLevel level) const;

    static std::string logLevelFormat(LogLevel level);

private:
    ImplLogger() = default;

    void run();

    static std::string getCurrentTimeString();

    std::string_view        mLogFilePath;
    std::queue<std::string> mLogQueue;
    std::mutex              mLogMutex;
    std::thread             mThread;
    bool                    mRunning = true;
    bool                    mColored = false;
    bool                    mDebug   = false;
};

std::string ImplLogger::logLevelFormat(const LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "[DEBUG] ";
        case LogLevel::INFO:
            return "[INFO ] ";
        case LogLevel::WARN:
            return "[WARN ] ";
        case LogLevel::ERROR:
            return "[ERROR] ";
    }
    return "UNKNOWN LOG LEVEL";
}

std::string ImplLogger::getLogLevelColor(const LogLevel level) const {
    std::string result;
    switch (level) {
        case LogLevel::DEBUG:
            result = mColored
                         ? std::string{
                             "\033[34m" + logLevelFormat(LogLevel::DEBUG) +
                             "\033[0m"}
                         : logLevelFormat(LogLevel::DEBUG);
            break;
        case LogLevel::INFO:
            result = mColored
                         ? std::string{
                             "\033[32m" + logLevelFormat(LogLevel::INFO) +
                             "\033[0m"}
                         : logLevelFormat(LogLevel::INFO);
            break;
        case LogLevel::WARN:
            result = mColored
                         ? std::string{
                             "\033[33m" + logLevelFormat(LogLevel::WARN) +
                             "\033[0m"}
                         : logLevelFormat(LogLevel::WARN);
            break;
        case LogLevel::ERROR:
            result = mColored
                         ? std::string{
                             "\033[31m" + logLevelFormat(LogLevel::ERROR) +
                             "\033[0m"}
                         : logLevelFormat(LogLevel::ERROR);
            break;
    }
    return result;
}

void ImplLogger::setDebug(const bool debug) {
    std::lock_guard<std::mutex> lock(mLogMutex);
    mDebug = debug;
}

bool ImplLogger::isDebug() const {
    return mDebug;
}

ImplLogger& ImplLogger::instance() {
    static ImplLogger mInstance{};
    return mInstance;
}

void ImplLogger::setLogFilePath(const std::string_view path) {
    mLogFilePath = path;
}

void ImplLogger::addToQueue(const LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mLogMutex);
    mLogQueue.push(
        "[" + getCurrentTimeString() + "]" + getLogLevelColor(level) + message +
        "\n");
}

void ImplLogger::setColored(const bool colored) {
    mColored = colored;
}

bool ImplLogger::getColored() const {
    return mColored;
}

void ImplLogger::stop() {
    std::lock_guard<std::mutex> lock(mLogMutex);
    mRunning = false;
    mThread.join();
}

void ImplLogger::start() {
    mThread = std::thread(&ImplLogger::run, this);
}

std::string_view ImplLogger::getLogFilePath() const {
    return mLogFilePath;
}

void ImplLogger::run() {
    FILE* file = std::fopen(mLogFilePath.data(), "a");
    while (mRunning || !mLogQueue.empty()) {
        if (!mLogQueue.empty()) {
            std::print("{}", mLogQueue.front());
            std::print(file, "{}", mLogQueue.front());
            mLogQueue.pop();
        }
    }
}

std::string ImplLogger::getCurrentTimeString() {
    using namespace std::chrono;
    return std::format("{:%F %X}",
                       zoned_time{current_zone(), system_clock::now()});
}

using namespace Logger;

void Log::Setup(const std::string_view logFilePath, const bool colored,
                const bool             debug) {
    if (ImplLogger::instance().getLogFilePath().empty())
        ImplLogger::instance().setLogFilePath(logFilePath);
    ImplLogger::instance().setColored(colored);
    ImplLogger::instance().start();
    ImplLogger::instance().setDebug(debug);
}

void Log::Stop() {
    ImplLogger::instance().stop();
}


void Log::Info(const std::string_view message) {
    ImplLogger::instance().addToQueue(LogLevel::INFO, std::string(message));
}

void Log::Warn(const std::string_view message) {
    ImplLogger::instance().addToQueue(LogLevel::WARN, std::string(message));
}

void Log::Debug(const std::string_view message) {
    if (ImplLogger::instance().isDebug()) {
        ImplLogger::instance().
            addToQueue(LogLevel::DEBUG, std::string(message));
    }
}

void Log::Err(const std::string_view message) {
    ImplLogger::instance().addToQueue(LogLevel::ERROR, std::string(message));
}