/* 
 * File:   mylog.h
 * Author: mchaberski
 *
 * Created on July 15, 2013, 10:46 AM
 */

#ifndef MYLOG_H
#define	MYLOG_H

#include <cstdlib>
#include <cstdio>

namespace mylog
{

const int ALL = 1000;
const int TRACE = 900;
const int DEBUG = 800;
const int INFO = 500;
const int WARN = 400;
const int ERROR = 200;
const int FATAL = 0;

struct loglevel {
	const char * levelName;
	int value;
};

typedef struct loglevel LogLevel;

const int NUM_STD_LEVELS = 7;
const int DEFAULT_LOG_LEVEL = INFO;

const int ERROR_LOG_IO = 250;
const int ERROR_INVALID_LOG_STREAM = 251;

enum LOG_MODE { MYLOG_WRITE, MYLOG_APPEND };

extern int ParseLogLevel(const char * str);
extern void Log(const int msg_level, const char * fmt, ...);
extern int SetLogLevel(const int log_level);
extern bool IsLevelEnabled(const int log_level);
extern int UseStream(FILE * ofile);
extern int OpenLogFile(const char * logfile_pathname, int log_mode);
extern int CloseLogFile();

} // end namespace mylog


#endif	/* MYLOG_H */

