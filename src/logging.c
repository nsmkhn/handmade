#include "logging.h"
#include <time.h>
#include <stdio.h>

static const char *levels[] = { "ERROR", "WARNING", "INFO", "DEBUG" };

void
log_event(const event_type event, const log_info_t *const info)
{
    time_t now;
    time(&now);
    
    switch (event)
    {
        case NULL_POINTER:
        {
            fprintf(info->stream, "%.19s [%s] %s:%u %s - unexpected null pointer\n",
                    ctime(&now), levels[info->level], info->filename, info->line, info->function);
        } break;
        
        case MALLOC_FAILURE:
        {
            fprintf(info->stream, "%.19s [%s] %s:%u %s - failed to allocate memory\n",
                    ctime(&now), levels[info->level], info->filename, info->line, info->function);
        } break;
        
        default:
        {
            fprintf(info->stream, "%.19s [%s] %s:%u %s - unknown event\n",
                    ctime(&now), levels[info->level], info->filename, info->line, info->function);
        } break;
    }
}

void
log_message(const char *const message, const log_info_t *const info)
{
    time_t now;
    time(&now);
    
    fprintf(info->stream, "%.19s [%s] %s:%u %s - %s\n",
            ctime(&now), levels[info->level], info->filename, info->line, info->function, message);
}