#include "logging.h"
#include <time.h>

static const char* severity_strings[] = { "ERROR", "WARNING", "INFO", "DEBUG" };

void
log(const log_info_t *const info)
{
    time_t now;
    time(&now);
    
    switch (info->error)
    {
        case NULL_POINTER:
        {
            fprintf(info->out, "%.19s [%s]: unexpected null pointer in file \"%s\", function \"%s\", line %u\n",
                    ctime(&now), severity_strings[info->severity], info->filename, info->function, info->line);
        } break;
        
        case MALLOC_FAILURE:
        {
            fprintf(info->out, "%.19s [%s]: failed to allocate memory in file \"%s\", function \"%s\", line %u\n",
                    ctime(&now), severity_strings[info->severity], info->filename, info->function, info->line);
        } break;
        
        default:
        {
            fprintf(info->out, "%.19s [%s]: unknown error in file \"%s\", function \"%s\", line %u\n",
                    ctime(&now), severity_strings[info->severity], info->filename, info->function, info->line);
        } break;
    }
}