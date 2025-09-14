#pragma once

#include <string>
#include <source_location>

namespace thLogger
{
    enum Level
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    void Log(const Level level, const std::string &msg,
             const std::source_location &loc = std::source_location::current());
    void debug(const std::string &msg,
               const std::source_location &loc = std::source_location::current());
    void info(const std::string &msg,
              const std::source_location &loc = std::source_location::current());
    void warning(const std::string &msg,
                 const std::source_location &loc = std::source_location::current());
    void error(const std::string &msg,
               const std::source_location &loc = std::source_location::current());
    void critical(const std::string &msg,
                  const std::source_location &loc = std::source_location::current());
    void toggleDebug();

} // namespace th
