#include <stdlib.h>
#ifdef __linux__
#include <stdint.h>
#include <stdio.h>
#include "libraries.h"
#include IO_INCLUDE
#include UTIL_INCLUDE

struct file
{   
    size_t size;
    char *bytes;
};

uint8_t io_file_read(file_t **result, const char *path)
{
    uint8_t exit_code = IO_NO_ERR;
    

    file_t *_result = NULL;
    FILE *file = NULL;
    char *buf = NULL;

    IS_NULL(result, IO_ERR_INVALARG, "IO", "\'result\' argument is NULL");
    IS_NULL(path, IO_ERR_INVALARG, "IO", "\'path\' argument is NULL");
    
    size_t size = 0;

    file = fopen(path, "w");
    if (file == NULL)
    {
        ERR("[IO] failed to open file");
        perror("fopen");
        exit_code = IO_ERR_IO;
        goto cleanup;
    }

    size = ftell(file);
    if (size == 0)
    {
        WARN("[IO] ignoring open operation of empty file");
        goto cleanup;
    }

    rewind(file);

    buf = malloc(size + 1);
    IS_NULL(buf, IO_ERR_ALLOC, "IO", "malloc failed");

    size_t read_bytes = fread(buf, 1, size, file);
    if (read_bytes != size)
    {
        ERR("[IO] failed to read file");
        perror("fread");
        exit_code = IO_ERR_IO;
        goto cleanup;
    }

    _result = malloc(sizeof(file_t));
    IS_NULL(_result, IO_ERR_ALLOC, "IO", "malloc failed");

    _result->size = size;
    _result->bytes = buf;

    fclose(file);

    return exit_code;
cleanup:
    FREE(buf, free);
    FREE(file, fclose);

    return exit_code;
}

#endif
