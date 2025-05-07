#include "dyarr/dyarr.h"
#include "utils.h"
#include <stdlib.h>

uint8_t array_create(array **result, size_t initial_cap, size_t item_size)
{
    uint8_t exit_code = ARR_NO_ERR;
    INFO("[DYARR] creating array");
    IS_NULL(result, ARR_ERR_INVALARG, "DYARR");
    if (initial_cap == 0 || item_size == 0)
    {
        ERR("cannot create 0 sized array, do not pass 0 size arguments.");
        exit_code = ARR_ERR_INVALARG;
        goto cleanup;
    }

    if (initial_cap % item_size != 0)
    {
        WARN("permanently unused memory initializing array at %zu "
             "initial_cap: %zu,\n please initialize the array with "
             "initial_cap devisable by item_size",
             initial_cap, item_size);
    }

    array *_result = NULL;
    

    _result = malloc(sizeof(array));
    IS_NULL(_result, ARR_ERR_ALLOC, "DYARR");

    _result->items = malloc(initial_cap * item_size);
    IS_NULL(_result->items, ARR_ERR_ALLOC, "DYARR");

    _result->cap = initial_cap;
    _result->item_size = item_size;
    _result->count = 0;

cleanup:
    return exit_code;
}

uint8_t array_destroy(array *arr)
{
    INFO("[DYARR] destroying array");
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    FREE(arr->items, free);
    FREE(arr, free);

cleanup:
    return exit_code;
}

uint8_t array_put(array *arr, void *item)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(item, ARR_ERR_INVALARG, "DYARR");
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    if (arr->count == arr->cap)
    {
        // TODO comtimue
    }

cleanup:
    return exit_code;
}

uint8_t array_put_array(array *arr, array *arr_to_put)
{
    uint8_t exit_code = ARR_NO_ERR;

cleanup:
    return exit_code;
}

uint8_t array_pop(array *arr)
{
    uint8_t exit_code = ARR_NO_ERR;

cleanup:
    return exit_code;
}
