//
//  fsmklog.h
//  ScribbleLab FSMK Core
//
//  Copyright (c) 2024, ScribbleLabApp LLC
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef fsmklog_h
#define fsmklog_h

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <syslog.h>
#include <unistd.h>

#define FSMK_MSG_BUFFER_SIZE 1024

/**
 * @enum fsmk_log_type_t
 * @brief Log levels used in the logging system.
 */
typedef enum {
    FSMK_LOG_TYPE_DEBUG,            ///< Debugging level, for verbose and detailed logs.
    FSMK_LOG_TYPE_INFO,             ///< Informational level, for general operational messages.
    FSMK_LOG_TYPE_ERROR,            ///< Error level, for non-critical issues.
    FSMK_LOG_TYPE_FAULT,            ///< Fault level, for critical system errors.
    FSMK_LOG_CATEGORY_CRITICAL,     ///< Critical issues requiring immediate attention.
    FSMK_LOG_CATEGORY_SECURITY,     ///< Security-related messages or alerts.
    FSMK_LOG_CATEGORY_PERFORMANCE,  ///< Performance-related information or metrics.
    FSMK_LOG_CATEGORY_NETWORK,      ///< Network-related logs.
    FSMK_LOG_CATEGORY_STORAGE,      ///< Logs related to storage or filesystem operations.
    FSMK_LOG_CATEGORY_SYSTEM,       ///< System-level events or messages.
    FSMK_LOG_CATEGORY_USER_ACTION,  ///< Logs capturing user actions or interactions.
} fsmk_log_type_t;

/**
 * @struct fsmk_subsystem_t
 * @brief Represents a logging subsystem.
 */
typedef struct {
    const char *subsystem;  ///< Name of the subsystem.
} fsmk_log_subsystem_t;

/**
 * @brief Logs a formatted message with details.
 *
 * @param type Log level (debug, info, error, fault).
 * @param function Name of the function logging the message.
 * @param line Line number in the source code.
 * @param format Format string for the log message.
 * @param ... Variable arguments matching the format string.
 */
void _fsmklog(
    fsmk_log_type_t type,
    const char *function,
    int line,
    const char *format,
    ...
);

/**
 * @brief Writes a raw log message directly to a file.
 *
 * @param message The message to log.
 */
void _fsmklogf(const char *message);

/**
 * @brief Logs a message with a va_list for extended flexibility.
 *
 * @param type Log level (debug, info, error, fault).
 * @param function Name of the function logging the message.
 * @param line Line number in the source code.
 * @param format Format string for the log message.
 * @param subsystem Pointer to a `fsmk_subsystem_t` structure.
 * @param args Variable arguments in `va_list` form.
 */
void _fsmklogv(
    fsmk_log_type_t type,
    const char *function,
    int line,
    const char *format,
    fsmk_log_subsystem_t *subsystem,
    va_list args
);

void _fsmk_log_page_size(void);

/**
 * @brief Macro to log a formatted message with automatic function and line details.
 *
 * @param level Log level.
 * @param fmt Format string.
 * @param ... Variable arguments matching the format string.
 */
#define fsmk_log(level, fmt, ...) _fsmklog(level, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief Macro to log a formatted message to a file.
 *
 * @param fmt Format string.
 * @param ... Variable arguments matching the format string.
 */
#define fsmk_log_f(fmt, ...) { \
    char message[FSMK_MSG_BUFFER_SIZE]; \
    snprintf(message, sizeof(message), "%s [Line %d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
    _fsmklogf(message); \
    _fsmklog(LOG_LEVEL_INFO, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
}

/**
 * @brief Macro to log a formatted message with a `va_list`.
 *
 * @param fmt Format string.
 * @param ... Variable arguments matching the format string.
 */
#define fsmk_log_v(fmt, ...) { \
    va_list args; \
    va_start(args, fmt); \
    _fsmklogv(LOG_LEVEL_INFO, __PRETTY_FUNCTION__, __LINE__, fmt, args); \
    va_end(args); \
}

#define fsmk_log_info(log, format, ...) { \
    _fsmklog(FSMK_LOG_TYPE_INFO, __PRETTY_FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
}

#ifdef DEBUG
/**
 * @brief Macro to log debug-level messages (enabled only in debug builds).
 *
 * @param fmt Format string.
 * @param ... Variable arguments matching the format string.
 */
#define fsmk_log_debug(fmt, ...) _fsmklog(level, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define fsmk_log_debug(fmt, ...)
#endif

#define fsmk_log_page_size _fsmk_log_page_size

#endif /* fsmklog_h */
