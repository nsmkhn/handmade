#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
// TODO(niki): Optimize structs member ordering if possible.
typedef enum event_type
{
    NULL_POINTER,
    MALLOC_FAILURE
} event_type;

typedef enum log_level
{
    ERROR,
    WARNING,
    INFO,
    DEBUG
} log_level;

typedef struct log_info
{
    FILE *stream;
    log_level level;
    const char *filename;
    const char *function;
    int unsigned line;
} log_info_t;
// TODO(niki): Simplify logging api.
void
log_event(const event_type, const log_info_t *const);

void
log_message(const char *const, const log_info_t *const);

#endif //LOGGING_H