#pragma once

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <chrono>
#include <mutex>
#include <string_view>

enum class LogLevel
{
        Trace,
        Info,
        Warn,
        Error,
        Fatal
};

class Logger
{
public:
        static void Init();

        template<typename... Args>
        static void Trace(fmt::format_string<Args...> fmtStr, Args&&... args)
        {
        Log(
                LogLevel::Trace,
                fmtStr,
                std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Info(fmt::format_string<Args...> fmtStr, Args&&... args)
        {
        Log(
                LogLevel::Info,
                fmtStr,
                std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(fmt::format_string<Args...> fmtStr, Args&&... args)
        {
        Log(
                LogLevel::Warn,
                fmtStr,
                std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(fmt::format_string<Args...> fmtStr, Args&&... args)
        {
        Log(
                LogLevel::Error,
                fmtStr,
                std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Fatal(fmt::format_string<Args...> fmtStr, Args&&... args)
        {
        Log(
                LogLevel::Fatal,
                fmtStr,
                std::forward<Args>(args)...);
        }

private:
        template<typename... Args>
        static void Log(
        LogLevel level,
        fmt::format_string<Args...> fmtStr,
        Args&&... args)
        {
        std::lock_guard lock(s_Mutex);

        auto now = std::chrono::system_clock::now();

        fmt::memory_buffer buffer;

        fmt::format_to(
                std::back_inserter(buffer),
                "[{:%H:%M:%S}] ",
                now);

        auto color = GetColor(level);

        fmt::format_to(
                std::back_inserter(buffer),
                "[{}] ",
                ToString(level));

        fmt::format_to(
                std::back_inserter(buffer),
                fmtStr,
                std::forward<Args>(args)...);

        fmt::print(
                fg(color),
                "{}\n",
                fmt::to_string(buffer));
        }

        static fmt::color GetColor(LogLevel level);
        static std::string_view ToString(LogLevel level);

private:
        static inline std::mutex s_Mutex;
};