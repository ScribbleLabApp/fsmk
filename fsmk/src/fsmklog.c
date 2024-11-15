//
//  fsmklog.c
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

#include <fsmklog.h>

void _fsmklog(
    fsmk_log_type_t type,
    const char *function,
    int line,
    const char *format,
    ...
) {
    va_list args;
    va_start(args, format);
    
    time_t rawtime;
    struct tm * timeInfo;
    char timeStr[20];
    time(&rawtime);
    timeInfo = localtime(&rawtime);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeInfo);
    
    const char *lvlStr;
    switch (type) {
        case FSMK_LOG_TYPE_DEBUG: lvlStr = "[DEBUG]";
            break;
        case FSMK_LOG_TYPE_INFO: lvlStr = "[INFO]";
            break;
        case FSMK_LOG_TYPE_ERROR: lvlStr = "[ERROR]";
            break;
        case FSMK_LOG_TYPE_FAULT: lvlStr = "[FAULT]";
            break;
        default: lvlStr = "UNKNOWN";
            break;
    }
    
    printf("%s [%s] %s [Line %d] ", timeStr, lvlStr, function, line);
    vprintf(format, args);
    printf("\n");
    
    va_end(args);
}

void _fsmklogf(const char *message) {
    FILE *file = fopen("/Library/Logs/fsmk.log", "a");
    if (!file) {
        file = fopen("/Library/Logs/fsmk.log", "w");
        if (file) {
            fclose(file);
            file = fopen("/Library/Logs/fsmk.log", "a");
        }
    }
    
    if (file) {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

void _fsmklogv(
               fsmk_log_type_t type,
               const char *function,
               int line,
               const char *format,
               fsmk_log_subsystem_t *subsystem,
               va_list args
) {
    time_t rawtime;
    struct tm * timeinfo;
    char timeStr[20];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    const char *lvlStr;
    int syslogLvl;
    switch (type) {
        case FSMK_LOG_TYPE_DEBUG: lvlStr = "[DEBUG]"; syslogLvl = LOG_DEBUG; break;
        case FSMK_LOG_TYPE_INFO: lvlStr = "[INFO]"; syslogLvl = LOG_INFO; break;
        case FSMK_LOG_TYPE_ERROR: lvlStr = "[ERROR]"; syslogLvl = LOG_ERR; break;
        case FSMK_LOG_TYPE_FAULT: lvlStr = "[FAULT]"; syslogLvl = LOG_CRIT; break;
        default: lvlStr = "UNKNOWN"; syslogLvl = LOG_NOTICE; break;
    }
    
    printf("%s [%s] %s [Line %d] ", timeStr, lvlStr, function, line);
    vprintf(format, args);
    printf("\n");
    
    char logMessage[FSMK_MSG_BUFFER_SIZE];
    vsnprintf(logMessage, sizeof(logMessage), format, args);
    openlog(subsystem->subsystem, LOG_PID | LOG_CONS, LOG_USER);
    syslog(syslogLvl, "%s [%s] %s [Line %d] %s", timeStr, lvlStr, function, line, logMessage);
    closelog();
}

void fsmk_log_page_size(void) {
    long page_size = sysconf(_SC_PAGESIZE);
    fsmk_log(FSMK_LOG_TYPE_INFO, "Page size: %ld", page_size);
}
