#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include "mylog.h"
#include <string>

using namespace std;
using namespace mylog;

const string Logs::kGlobalName = "";
Logger Logs::global_logger(Logs::kGlobalName);
LoggerSet Logs::logger_set;

LoggerSet::LoggerSet() 
    : loggers()
{}

Logger* LoggerSet::Get(const string& name)
{
    for (size_t i = 0; i < loggers.size(); i++) {
        if (name.compare(loggers[i]->name()) == 0) {
            return loggers[i];
        }
    }
    Logger* logger = new Logger(name);
    loggers.push_back(logger);
    return logger;
}

LoggerSet::~LoggerSet()
{
    for (size_t i = 0; i < loggers.size(); i++) {
        Logger* logger = loggers[i];
        delete logger;
    }
}

Logger& Logs::GetLogger()
{
    return global_logger;
}

Logger& Logs::GetLogger(const string& name)
{
    if (kGlobalName.compare(name) == 0) {
        return global_logger;
    }
    Logger* loggerp = logger_set.Get(name);
    return *loggerp;
}

/**
 * Parses a log level integer from string. Returns DEFAULT_LOG_LEVEL on
 * any error, otherwise the LOG_* constant is returned.
 */
bool Logs::ParseLevel(const string& level_name, Level* level) {
	if (level_name.compare("ALL") == 0) {
		*level = ALL;
	} else if (level_name.compare("TRACE") == 0) {
		*level = TRACE;
	} else if (level_name.compare("DEBUG") == 0) {
		*level = DEBUG;
	} else if (level_name.compare("INFO") == 0) {
		*level = INFO;
	} else if (level_name.compare("WARN") == 0) {
		*level = WARN;
	} else if (level_name.compare("ERROR") == 0) {
		*level = ERROR;
	} else if (level_name.compare("FATAL") == 0) {
		*level = FATAL;
	} else {
		return false;
	}
	return true;
}

Logger::Logger(const string& name) 
        : name_(name), 
          level_(Logs::DEFAULT_LOG_LEVEL), 
          output_(stderr)
{
}

const string& Logger::name()
{
    return name_;
}

void Logger::Log(Level level, const char* fmt, ...)
{
	if (IsLevelEnabled(level)) {
		va_list argptr;
		va_start(argptr, fmt);
		vfprintf(output_, fmt, argptr);
		va_end(argptr);
	}
}

Level Logger::set_level(Level level)
{
	Level old = level_;
	level_ = level;
	return old;
}

Level Logger::level()
{
	return level_;
}

bool Logger::IsLevelEnabled(Level level)
{
	return level_ >= level;
}

void Logger::set_output(FILE* ofile)
{
	output_ = ofile;
}

const char* GetName(Level level)
{
    switch (level) {
        case ALL: return "ALL";
        case TRACE: return "TRACE";
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        case FATAL: return "FATAL";
        default: return "UNRECOGNIZED";
    }    
    assert(false);
}
