#include <print>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>
#include <chrono>
#include <ctime>
#include <string>

#include "Logger.h"

class ImplLogger {
public:
    static ImplLogger &instance();

    void setLogFilePath(std::string_view path);

    void addToQueue(const std::string &message);

    void Stop();

    void Start();

    [[nodiscard]] std::string_view getLogFilePath() const;

private:
    ImplLogger() = default;

    void run();

    static std::string getCurrentTimeString() ;

    std::string_view mLogFilePath;
    std::queue<std::string> mLogQueue;
    std::mutex mLogMutex;
    std::thread mThread;
    bool mRunning = true;
};

ImplLogger &ImplLogger::instance() {
    static ImplLogger mInstance{};
    return mInstance;
}

void ImplLogger::setLogFilePath(const std::string_view path) {
    mLogFilePath = path;
}

void ImplLogger::addToQueue(const std::string &message) {
    std::lock_guard<std::mutex> lock(mLogMutex);
    mLogQueue.push("["+getCurrentTimeString() + "]" + message);
}

void ImplLogger::Stop() {
    std::lock_guard<std::mutex> lock(mLogMutex);
    mRunning = false;
    mThread.join();
}


void ImplLogger::Start() {
    mThread = std::thread(&ImplLogger::run, this);
}

std::string_view ImplLogger::getLogFilePath() const {
    return mLogFilePath;
}


void ImplLogger::run() {
    while (mRunning || !mLogQueue.empty()) {
        if (!mLogQueue.empty()) {
            std::printf("%s", mLogQueue.front().c_str());
            mLogQueue.pop();
        }
    }
}

std::string ImplLogger::getCurrentTimeString() {
    using namespace std::chrono;
    const auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::Setup(const std::string_view logFilePath) {
    if (ImplLogger::instance().getLogFilePath().empty())
        ImplLogger::instance().setLogFilePath(logFilePath);
    ImplLogger::instance().Start();
}

void Logger::Stop() {
    ImplLogger::instance().Stop();
}


void Logger::Info() {
}

void Logger::Warn(const std::string_view message) {
    ImplLogger::instance().addToQueue("[WARN] " + std::string(message) + "\n");
}

void Logger::Debug() {
}

void Logger::Err() {
}
