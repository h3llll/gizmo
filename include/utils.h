#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
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

#endif // ACTIVE_LOG_LEVEL

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_ALL
#define INFO(fmt, ...) fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)
#else
#define INFO(fmt, ...)

#endif // ACTIVE_LOG_LEVEL >= LOG_LEVEL_ALL

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_WARN
#define WARN(fmt, ...)                                                    \
    fprintf(stderr,                                                       \
            ANSI_YELLOW "[WARN] " fmt                                     \
                        " (file: %s, func: %s, line: %d)\n" ANSI_RESET,   \
            ##__VA_ARGS__, __FILE__, __func__, __LINE__)
#else
#define WARN(fmt, ...)
#endif // ACTIVE_LOG_LEVEL >= LOG_LEVEL_WARN

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_ERR
#define ERR(fmt, ...)                                                     \
    fprintf(stderr,                                                       \
            ANSI_RED "[ERR]  " fmt                                        \
                     " (file: %s, func: %s, line: %d)\n" ANSI_RESET,      \
            ##__VA_ARGS__, __FILE__, __func__, __LINE__)
#else
#define ERR(fmt, ...)
#endif // ACTIVE_LOG_LEVEL >= LOG_LEVEL_ERR

#else

#define INFO(fmt, ...)
#define WARN(fmt, ...)
#define ERR(fmt, ...)
#endif // DEBUG_BUILD

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
            free_fn(ptr);                                                 \
            ptr = NULL;                                                   \
        }                                                                 \
    } while (0)

#define UTIL_IMP

#ifdef UTIL_IMP

#define UTIL_NO_ERR         0
#define UTIL_ERR_INVALARG   1
#define UTIL_ERR_ALLOC      2

#include <stdlib.h>

uint8_t static inline read_file(const char *path, char **result)
{
    uint8_t exit_code = UTIL_NO_ERR;

    INFO("[UTILS] reading file %s", path);
    size_t size = 22;
    char *_result = malloc(sizeof(char) * size);
    IS_NULL(_result, UTIL_ERR_ALLOC, "UTIL");

    return exit_code;

cleanup:
    return exit_code;
}

#endif // UTIL_IMP

#endif
