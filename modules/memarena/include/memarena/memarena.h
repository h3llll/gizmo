#ifndef MODULES_MEMARENA_H
#define MODULES_MEMARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// clang-format off
#define ARENA_NO_ERR        0
#define ARENA_ERR_ALLOC     1
#define ARENA_ERR_FULL      2
#define ARENA_ERR_INVALARG  3
// clang-format on

typedef struct arena
{
    void *mem;
    size_t offset;
    size_t size;
} arena;

// Returns a heap arena object in result pointer.
// Returns 0 on success, 0< on failure.
uint8_t arena_create(arena **result, size_t size);

// Returns a pointer to arena heap allocated memory in result pointer
// Returns 0 on success, 0< on failure.
uint8_t arena_alloc(arena *arena, void **result, size_t size);

// Deallocates targetted arena's heap memory
// Returns 0 on success, 0< on failure.
uint8_t arena_destroy(arena *arena);

#endif
