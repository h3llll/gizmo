#include "memarena/memarena.h"
#include "utils.h"

uint8_t arena_create(arena **result, size_t size)
{
    uint8_t exit_code = ARENA_NO_ERR;
    arena *_result = NULL;
    void *mem = NULL;

    if (size == 0)
    {
        WARN("[ARENA] requested size is 0, ignoring arena_creation");
        goto cleanup;
    }

    _result = malloc(sizeof(arena));
    IS_NULL(_result, ARENA_ERR_ALLOC, "MEMARENA");

    mem = malloc(size);
    IS_NULL(mem, ARENA_ERR_ALLOC, "MEMARENA");

    _result->mem = mem;
    _result->offset = 0;
    _result->size = size;

    *result = _result;

    return exit_code;

cleanup:
    FREE(_result, free);

    return exit_code;
}

uint8_t arena_alloc(arena *arena, void **result, size_t size)
{
    uint8_t exit_code = ARENA_NO_ERR;

    IS_NULL(result, ARENA_ERR_INVALARG, "ARENA");
    IS_NULL(arena, ARENA_ERR_INVALARG, "ARENA");

    if (size == 0)
    {
        WARN("[ARENA] requested size is 0, ignoring allocation");
        goto cleanup;
    }

    if (arena->offset + size > arena->size)
    {
        ERR("[ARENA] arena full, ignoring allocation (arena remaining "
            "size: %zu, "
            "requested size: %zu)",
            arena->size - arena->offset, size);

        exit_code = ARENA_ERR_FULL;
        goto cleanup;
    }

    void *_result = arena->mem + arena->offset;
    arena->offset += size;

    *result = _result;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t arena_destroy(arena *arena)
{
    uint8_t exit_code = ARENA_NO_ERR;
    IS_NULL(arena, ARENA_ERR_INVALARG, "ARENA");

    FREE(arena->mem, free);
    FREE(arena, free);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t arena_reset(arena *arena)
{
    uint8_t exit_code = ARENA_NO_ERR;
    IS_NULL(arena, ARENA_ERR_INVALARG, "ARENA");

    arena->offset = 0;

    return exit_code;

cleanup:
    return exit_code;
}
