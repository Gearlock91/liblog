#include <print>
#include "Logger.h"
int main() {
    Logger::Setup("");
    Logger::Warn("Something whent wrong");
    Logger::Warn("Something wrong");
    Logger::Stop();
}