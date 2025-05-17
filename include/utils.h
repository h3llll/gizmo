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
    fprintf(stderr, ANSI_YELLOW "[WARN] " fmt ANSI_RESET, ##__VA_ARGS__)
#else
#define WARN(fmt, ...)
#endif // ACTIVE_LOG_LEVEL >= LOG_LEVEL_WARN

#if ACTIVE_LOG_LEVEL >= LOG_LEVEL_ERR
#define ERR(fmt, ...)                                                     \
    fprintf(stderr, ANSI_RED "[ERR] " fmt ANSI_RESET, ##__VA_ARGS__)
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

#ifdef UTIL_IMP

// clang-format off
#define UTIL_NO_ERR         0
#define UTIL_ERR_INVALARG   1
#define UTIL_ERR_ALLOC      2
#define UTIL_ERR_IO         3
// clang-format on

#include <stdlib.h>

static inline uint8_t read_file(const char *path, char **result)
{
    uint8_t exit_code = UTIL_NO_ERR;
    FILE *file = NULL;
    char *_result = NULL;
    long size = 0;

    INFO("[UTILS] reading file %s", path);

    file = fopen(path, "rb"); // use "rb" for binary compatibility
    IS_NULL(file, UTIL_ERR_IO, "UTIL");

    if (fseek(file, 0, SEEK_END) != 0)
    {
        ERR("[UTIL] failed to seek to end of file");
        exit_code = UTIL_ERR_IO;
        goto cleanup;
    }

    size = ftell(file);
    if (size < 0)
    {
        ERR("[UTIL] failed to get file size");
        exit_code = UTIL_ERR_IO;
        goto cleanup;
    }

    rewind(file); // or fseek(file, 0, SEEK_SET);

    _result = malloc(size + 1); // +1 for null-terminator
    IS_NULL(_result, UTIL_ERR_ALLOC, "UTIL");

    size_t read = fread(_result, 1, size, file);
    if (read != (size_t)size)
    {
        ERR("[UTIL] only read %zu of %ld bytes", read, size);
        exit_code = UTIL_ERR_IO;
        goto cleanup;
    }

    _result[size] = '\0';
    *result = _result;

    fclose(file);
    return exit_code;

cleanup:
    FREE(file, fclose);
    FREE(_result, free);
    return exit_code;
}
#endif // UTIL_IMP

#endif // UTILS_H
