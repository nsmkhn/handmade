#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

inline void
log_np_error(const char *const file, const char *const function, const int unsigned line)
{
    fprintf(stderr, "%s: unexpected null pointer in function \"%s\", line %u\n", file, function, line);
}

inline void
log_malloc_error(const char *file, const char *function, const int unsigned line)
{
    fprintf(stderr, "%s: failed to allocate memory in function \"%s\", line %u\n", file, function, line);
}

#endif //LOGGING_H