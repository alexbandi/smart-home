#ifndef LOG_H
#define LOG_H

#include <cstdarg>

enum LogLevel
{
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE
};
#define LOG_LEVELS_NUM LogLevel::TRACE

class Log
{
private:
    static void log(LogLevel logLevel, const char *message, va_list args);

public:
    static void init();
    static void fatal(const char *message, ...);
    static void error(const char *message, ...);
    static void warn(const char *message, ...);
    static void info(const char *message, ...);
    static void debug(const char *message, ...);
    static void trace(const char *message, ...);
};

#endif // LOG_H