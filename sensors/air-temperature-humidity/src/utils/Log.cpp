#include "Log.h"

#include <Arduino.h>

void Log::init()
{
    Serial.begin(115200);
}

void Log::log(LogLevel logLevel, const char *message, va_list args)
{
    static const char* logLevelPrefix[] = {
        "[FATAL] ",
        "[ERROR] ",
        "[WARNING] ",
        "[INFO] ",
        "[DEBUG] ",
        "[TRACE] "
    };
    Serial.print(logLevelPrefix[logLevel]);

    Serial.printf(message, args);
    Serial.println();
}

#define LOG_FUNCTION(level, logLevel) \
    void Log::level(const char *message, ...) \
    { \
        va_list args; \
        va_start(args, message); \
        log(logLevel, message, args); \
        va_end(args); \
    }

LOG_FUNCTION(fatal, FATAL)
LOG_FUNCTION(error, ERROR)
LOG_FUNCTION(warn, WARNING)
LOG_FUNCTION(info, INFO)
LOG_FUNCTION(debug, DEBUG)
LOG_FUNCTION(trace, TRACE)
