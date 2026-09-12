#pragma once
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace astar::logging {

enum class Level { DEBUG, INFO, WARN, ERROR };

inline std::string level_str(Level l) {
    switch(l) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO:  return "INFO";
        case Level::WARN:  return "WARN";
        case Level::ERROR: return "ERROR";
    }
    return "INFO";
}

inline Level level_from_string(const std::string& s) {
    if (s == "DEBUG") return Level::DEBUG;
    if (s == "WARN")  return Level::WARN;
    if (s == "ERROR") return Level::ERROR;
    return Level::INFO;
}

inline std::atomic<Level>& min_level() {
    static std::atomic<Level> level = [] {
        const char* env = std::getenv("ASTAR_LOG_LEVEL");
        return env ? level_from_string(env) : Level::INFO;
    }();
    return level;
}

inline void set_min_level(Level l) { min_level().store(l); }

inline bool enabled(Level l) { return l >= min_level().load(); }

inline void log(Level level,
                const std::string& message,
                const std::string& service    = "astar",
                const std::string& trace_id   = "",
                const std::string& span_id    = "")
{
    if (!enabled(level)) return;

    auto now = std::chrono::system_clock::now();
    auto ts  = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now.time_since_epoch()).count();

    std::ostringstream out;
    out << "{"
        << "\"timestamp\":"  << ts                    << ","
        << "\"level\":\""    << level_str(level)      << "\","
        << "\"service\":\""  << service               << "\","
        << "\"message\":\""  << message               << "\"";

    if (!trace_id.empty())
        out << ",\"trace_id\":\"" << trace_id << "\"";
    if (!span_id.empty())
        out << ",\"span_id\":\""  << span_id  << "\"";

    out << "}";

    if (level >= Level::WARN)
        std::cerr << out.str() << "\n";
    else
        std::cout << out.str() << "\n";
}

} // namespace astar::logging


#define ASTAR_LOG(lvl, msg)                                                    \
    do {                                                                       \
        if (astar::logging::enabled(lvl)) astar::logging::log(lvl, msg);       \
    } while (false)

#define LOG_DEBUG(msg) ASTAR_LOG(astar::logging::Level::DEBUG, msg)
#define LOG_INFO(msg)  ASTAR_LOG(astar::logging::Level::INFO,  msg)
#define LOG_WARN(msg)  ASTAR_LOG(astar::logging::Level::WARN,  msg)
#define LOG_ERROR(msg) ASTAR_LOG(astar::logging::Level::ERROR, msg)