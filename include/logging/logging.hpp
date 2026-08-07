#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>

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

inline void log(Level level,
                const std::string& message,
                const std::string& service    = "astar",
                const std::string& trace_id   = "",
                const std::string& span_id    = "")
{
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

#define LOG_INFO(msg)  astar::logging::log(astar::logging::Level::INFO,  msg)
#define LOG_WARN(msg)  astar::logging::log(astar::logging::Level::WARN,  msg)
#define LOG_ERROR(msg) astar::logging::log(astar::logging::Level::ERROR, msg)
#define LOG_DEBUG(msg) astar::logging::log(astar::logging::Level::DEBUG, msg)

} // namespace astar::logging