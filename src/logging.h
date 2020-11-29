#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

typedef enum error
{
    NULL_POINTER,
    MALLOC_FAILURE
} error_t;

typedef enum severity
{
    ERROR,
    WARNING,
    INFO,
    DEBUG
} severity_t;

typedef struct log_info
{
    error_t error;
    severity_t severity;
    FILE *stream;
    const char *filename;
    const char *function;
    int unsigned line;
} log_info_t;

void
log(const log_info_t *const);

#endif //LOGGING_H