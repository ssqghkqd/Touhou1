#include <utils/Logger.hpp>
#include <utils/Time.hpp>
#include <iomanip>
#include "core/App.hpp"
#include <format>
#include <print>

namespace thLogger
{
    bool is_debug = false;
    void Log(const Level level, const std::string &msg, const std::source_location &loc)
    {
        std::string lv;
        switch (level)
        {
            case Debug:
            if (!is_debug)
                return;
            lv = "Debug";
            break;
        case Info:
            lv = "Info";
            break;
        case Warning:
            lv = "Warning";
            break;
        case Error:
            lv = "Error";
            break;
        case Critical:
            lv = "Critical";
            break;
        default:
            lv = "";
            break;
        }
        std::string timeStr = th::Time::getTimeStr();
        std::print("[{}] [{}] {}:{} - {}\n",
                                 timeStr, lv, loc.file_name(), loc.line(), msg);
        if (level == Critical)
            throw std::runtime_error("Critical error: " + msg);
    }

    void debug(const std::string &msg, const std::source_location &loc)
    {
        Log(Debug, msg, loc);
    }

    void info(const std::string &msg, const std::source_location &loc)
    {
        Log(Info, msg, loc);
    }

    void warning(const std::string &msg, const std::source_location &loc)
    {
        Log(Warning, msg, loc);
    }

    void error(const std::string &msg, const std::source_location &loc)
    {
        Log(Error, msg, loc);
    }

    void critical(const std::string &msg, const std::source_location &loc)
    {
        Log(Critical, msg, loc);
    }

    void toggleDebug()
    {
        is_debug = !is_debug;
    }

} // namespace thLogger
