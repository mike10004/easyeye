#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "mylog.h"

namespace mylog {

	int MyLog_Log_Level = DEFAULT_LOG_LEVEL;
	FILE * mylog_log_outstream = NULL;

	LogLevel createLogLevel(const char* levelName, const int value) {
		LogLevel level;
		level.levelName = levelName;
		level.value = value;
		return level;
	}

	LogLevel STD_LEVELS[] = {
			createLogLevel("TRACE", TRACE),
			createLogLevel("DEBUG", DEBUG),
			createLogLevel("ALL", ALL),
			createLogLevel("INFO", INFO),
			createLogLevel("ERROR", ERROR),
			createLogLevel("WARNING", WARN),
			createLogLevel("FATAL", FATAL)
	};

	LogLevel* GetStandardLevel(int i) {
		return &(STD_LEVELS[i]);
	}

    const char* GetStandardLevelName(int levelValue) {
        for (int i = 0; i < NUM_STD_LEVELS; i++) {
            LogLevel* level = GetStandardLevel(i);
            if (levelValue == level->value) {
                return level->levelName;
            }
        }
        return "CUSTOM";
    }
    
	/**
	 * Parses a log level integer from string. Returns DEFAULT_LOG_LEVEL on
	 * any error, otherwise the LOG_* constant is returned.
	 */
	int ParseLogLevel(const char * str) {
		if (str == NULL) {
			fprintf(stderr, "mylog: invalid log level (null); defaulting to %d\n", DEFAULT_LOG_LEVEL);
			return DEFAULT_LOG_LEVEL;
		}
		LogLevel* level;
		for (int i = 0; i < NUM_STD_LEVELS; i++) {
			level = GetStandardLevel(i);
			if (strcmp(str, level->levelName) == 0) {
				return level->value;
			}
		}
		fprintf(stderr, "mylog: invalid log level (%s); defaulting to %d\n", str, DEFAULT_LOG_LEVEL);
		return DEFAULT_LOG_LEVEL;
	}

	FILE* GetDefaultStream() {
		return stderr;
	}


	/**
	 * Print a message to the log at the specified level.
	 */
	void Log(const int msg_level, const char * fmt, ...) {
		// First check the logstream
		if (mylog_log_outstream == NULL) {
			mylog_log_outstream = GetDefaultStream();
		}

		// Log if mylog level is above the message level
		if (MyLog_Log_Level >= msg_level) {
            fprintf(mylog_log_outstream, "%s ", GetStandardLevelName(msg_level));
			va_list argptr;
			va_start(argptr, fmt);
			vfprintf(mylog_log_outstream, fmt, argptr);
			va_end(argptr);
		}
	}


	/**
	 * Set the log level and returns the new level.
	 */
	int SetLogLevel(const int log_level) {
		MyLog_Log_Level = log_level;
		return MyLog_Log_Level;
	}

	/** Returns true if messages at or above
	  * argument log_level would be printed to the log.
	  * Otherwise returns false.
	  */
	bool IsLevelEnabled(const int log_level) {
		return (MyLog_Log_Level >= log_level);
	}

	/**
	 * Opens a file for writing log messages.
	  * Returns zero (MYLOG_SUCCESS) on success, or nonzero on error.
	  * On error, resets log to stderr.
	  * The log_mode argument should be of the enum MYLOG_MODE type; if not
	  * equal to MYLOG_WRITE, then append mode is used.
	  * If the file is successfully opened, then log_to_stream
	  * is called with the new FILE* pointer. If a logfile is opened
	  * using this function, it should be closed with close_logfile().
	  */
	int OpenLogFile(const char * logfile_pathname, LOG_MODE log_mode) {
		int retcode;
		FILE * ofile;
		const char * log_mode_str = (log_mode == MYLOG_WRITE) ? "w" : "a";

		ofile = fopen(logfile_pathname, log_mode_str);
		if (ofile == NULL) {
			fprintf(stderr, "error opening logfile at pathname %s\n", logfile_pathname);
			retcode = ERROR_LOG_IO;
		} else {
			retcode = UseStream(ofile);
		}

		return retcode;
	}

	/**
	 * Initialize
	 */
	int UseStream(FILE * ofile) {
		int retcode;
		if (ofile != NULL) {
			mylog_log_outstream = ofile;
			retcode = EXIT_SUCCESS;
		} else {
			retcode = ERROR_INVALID_LOG_STREAM;
		}
		return retcode;
	}

	/** Closes the log file. Must be called at end of program if
	  * open_logfile was called.

	  * If the current
	  * log stream is stdout or stderr, the stream is not closed,
	  * and MYLOG_ERROR_INVALID_LOG_STREAM is returned. Otherwise,
	  * EXIT_SUCCESS is returned.
	  */
	int close_logfile() {
		int retcode;
		int close_retcode;
		if ((mylog_log_outstream != stdout) && (mylog_log_outstream != stderr) && (mylog_log_outstream != NULL)) {
			fflush(mylog_log_outstream);
			close_retcode = fclose(mylog_log_outstream);
			if (close_retcode != 0) {
				retcode = close_retcode;
			} else {
				retcode = EXIT_SUCCESS;
			}
		} else {
			retcode = ERROR_INVALID_LOG_STREAM;
		}
		return retcode;
	}
}
