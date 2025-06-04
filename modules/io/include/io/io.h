#ifndef MODULES_IO_H
#define MODULES_IO_H

#include <stddef.h>
#include <stdint.h>

// clang-format off
#define IO_NO_ERR       0
#define IO_ERR_INVALARG 1
#define IO_ERR_IO       3
#define IO_ERR_ALLOC    4
// clang-format on

typedef struct file file_t;

// Reads a file.
// Returns IO_NO_ERR on success, 0< otherwise.
uint8_t io_file_read(file_t **result, const char *path);

// Frees a file.
// Returns IO_NO_ERR on success, 0< otherwise.
uint8_t io_file_free(file_t **file);

#endif /* MODULES_IO_H */
