#include "logger.h"
#ifdef _WIN32
#include <windows.h>
#endif

void EnableANSIColors()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE)
        return;

    DWORD dwMode = 0;

    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);
#endif
}

void Logger::Init()
{
        EnableANSIColors();
}

fmt::color Logger::GetColor(LogLevel level)
{
switch (level)
{
        case LogLevel::Trace: return fmt::color::light_gray;
        case LogLevel::Info:  return fmt::color::green;
        case LogLevel::Warn:  return fmt::color::yellow;
        case LogLevel::Error: return fmt::color::red;
        case LogLevel::Fatal: return fmt::color::crimson;
}

return fmt::color::white;
}

std::string_view Logger::ToString(LogLevel level)
{
switch (level)
{
        case LogLevel::Trace: return "TRACE";
        case LogLevel::Info:  return "INFO";
        case LogLevel::Warn:  return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Fatal: return "FATAL";
}

return "UNKNOWN";
}
