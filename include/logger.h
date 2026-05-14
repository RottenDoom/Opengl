/*
 * File: logger.h
 * Created: 2026-05-07
 * Description: Simple logger using fmtlib
* 
* MIT License
* 
* Copyright (c) 2026 Aditya Yadav
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*/



#ifndef LOGGER_H
#define LOGGER_H

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

#endif // LOGGER_H