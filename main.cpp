#include <print>
#include "Log.h"
int main() {
    using namespace Logger;
    Log::Setup("./log.txt", false, true);
    Log::Info("Starting software...");
    Log::Warn("Something went wrong");
    Log::Warn("Something wrong");
    Log::Debug("Debug line");
    Log::Info("Informational message");
    Log::Err("Critical message");
    Log::Stop();
}