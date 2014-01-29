/* 
 * File:   mylog.h
 * Author: mchaberski
 *
 * Created on July 15, 2013, 10:46 AM
 */

#ifndef MYLOG_H
#define	MYLOG_H

#include <cassert>
#include <cstdio>
#include <vector>
#include <string>

namespace mylog
{

enum Level {    
    ALL = 1000,
    TRACE = 900,
    DEBUG = 800,
    INFO = 500,
    WARN = 400,
    ERROR = 200,
    FATAL = 0
};

const Level LEVELS[] = {
    ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL
};

const char* GetName(Level level);

class Logger
{
public:
	Logger(const std::string& name);
	void Log(Level msg_level, const char * fmt, ...);
	Level set_level(Level log_level);
	Level level();
	bool IsLevelEnabled(Level log_level);
	void set_output(FILE * ofile);
    const std::string& name();
private:
    const std::string name_;
	Level level_;
	FILE* output_;
};

class LoggerSet
{
public:
    LoggerSet();
    ~LoggerSet();
    Logger* Get(const std::string& name);
private:
    std::vector<Logger*> loggers;
};

class Logs {
public:
	static Logger& GetLogger();
    static Logger& GetLogger(const std::string& name);
    bool ParseLevel(const std::string& level_name, Level* level);
	static void log_msg(Level msg_level, const char * fmt, ...);
    static const char* LEVEL_NAME_ALL;
    static const char* LEVEL_NAME_TRACE;
    static const char* LEVEL_NAME_DEBUG;
    static const char* LEVEL_NAME_INFO;
    static const char* LEVEL_NAME_WARN;
    static const char* LEVEL_NAME_ERROR;
    static const char* LEVEL_NAME_FATAL;
    static const Level DEFAULT_LOG_LEVEL = INFO;
private:
    static LoggerSet logger_set;
    static bool IsGlobal(const std::string& name);
	static Logger global_logger;
    static const std::string kGlobalName;
};

} // end namespace mylog


#endif	/* MYLOG_H */

