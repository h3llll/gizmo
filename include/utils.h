#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define ANSI_RED "\x1b[31m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET "\x1b[0m"

#ifdef DEBUG_BUILD
// clang-format off
#define LOG_LEVEL_ALL   3
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERR   1
#define LOG_LEVEL_NONE  0
// clang-format on
#ifndef ACTIVE_LOG_LEVEL
#define ACTIVE_LOG_LEVEL LOG_LEVEL_ALL
#endif

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_ALL
#define INFO(fmt, ...) fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)
#else
#define INFO(fmt, ...)
#endif

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_WARN
#define WARN(fmt, ...)                                                    \
    fprintf(stderr,                                                       \
            ANSI_YELLOW "[WARN] " fmt                                     \
                        " (file: %s, func: %s, line: %d)\n" ANSI_RESET,   \
            ##__VA_ARGS__, __FILE__, __func__, __LINE__)
#else
#define WARN(fmt, ...)
#endif

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_ERR
#define ERR(fmt, ...)                                                     \
    fprintf(stderr,                                                       \
            ANSI_RED "[ERR]  " fmt                                        \
                     " (file: %s, func: %s, line: %d)\n" ANSI_RESET,      \
            ##__VA_ARGS__, __FILE__, __func__, __LINE__)
#else
#define ERR(fmt, ...)
#endif

#else

#define INFO(fmt, ...)
#define WARN(fmt, ...)
#define ERR(fmt, ...)

#endif
#endif

#define IS_NULL(ptr, code, module)                                        \
    do                                                                    \
    {                                                                     \
        if ((ptr) == NULL)                                                \
        {                                                                 \
            ERR("[%s] NULL POINTER ERR", module);                         \
            exit_code = code;                                             \
            goto cleanup;                                                 \
        }                                                                 \
    } while (0)

#define RET_ON_FAIL(fncall, fail_code, ret_code, module)                  \
    do                                                                    \
    {                                                                     \
        if ((fncall) == fail_code)                                        \
        {                                                                 \
            ERR("[%s] FUNCTION CALL " #fncall " FAILED", module);         \
            exit_code = ret_code;                                         \
            goto cleanup;                                                 \
        }                                                                 \
    } while (0)

#define FREE(ptr, free_fn)                                                \
    do                                                                    \
    {                                                                     \
        if (ptr != NULL)                                                  \
        {                                                                 \
            ptr = NULL;                                                   \
            free_fn(ptr);                                                 \
        }                                                                 \
    } while (0)
