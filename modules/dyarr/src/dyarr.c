#include "dyarr/dyarr.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

uint8_t array_create(array **result, size_t initial_cap, size_t item_size)
{
    uint8_t exit_code = ARR_NO_ERR;
    INFO("[DYARR] creating array");

    array *_result = NULL;

    IS_NULL(result, ARR_ERR_INVALARG, "DYARR");

    if (initial_cap > SIZE_MAX / item_size)
    {
        ERR("size overflow when calculating array allocation");
        exit_code = ARR_ERR_ALLOC;
        goto cleanup;
    }

    if (initial_cap == 0 || item_size == 0)
    {
        ERR("cannot create 0 sized array, do not pass 0 size arguments.");
        exit_code = ARR_ERR_INVALARG;
        goto cleanup;
    }

    if (initial_cap % item_size != 0)
    {
        WARN("permanently unused memory initializing array at cap(bytes): "
             "%zu "
             "wasted memory: %zu,\n please initialize the array with "
             "initial_cap devisable by item_size"
             ", also you could have used array_strip but i have no way to "
             "tell lol",
             initial_cap * item_size, item_size % initial_cap);
    }

    _result = malloc(sizeof(array));
    IS_NULL(_result, ARR_ERR_ALLOC, "DYARR");

    _result->items = malloc(initial_cap * item_size);
    IS_NULL(_result->items, ARR_ERR_ALLOC, "DYARR");

    _result->cap = initial_cap;
    _result->item_size = item_size;
    _result->count = 0;

    *result = _result;
    _result = NULL;

cleanup:
    FREE(_result, free);

    return exit_code;
}

uint8_t array_destroy(array *arr)
{
    INFO("[DYARR] destroying array");
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");
    IS_NULL(arr->items, ARR_ERR_INVALARG, "DYARR");

    FREE(arr->items, free);
    FREE(arr, free);

cleanup:
    return exit_code;
}

uint8_t array_put(array *arr, void *item, size_t item_size)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(item, ARR_ERR_INVALARG, "DYARR");
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    if (item_size != arr->item_size)
    {
        ERR("ignoring array_put, item(size: %zu) is not of size "
            "arr->item_size(size: %zu)",
            item_size, arr->item_size);

        exit_code = ARR_ERR_SIZE_MISMATCH;
        goto cleanup;
    }

    if (arr->count >= arr->cap)
    {
        INFO("[DYARR] resizing array");
        arr->cap *= 2;

        void *new_arr_items =
            realloc(arr->items, arr->cap * arr->item_size);
        IS_NULL(new_arr_items, ARR_ERR_ALLOC, "DYARR");
        arr->items = new_arr_items;
    }

    void *target_location =
        (char *)arr->items + arr->count * arr->item_size;
    memcpy(target_location, item, arr->item_size);
    arr->count++;

cleanup:
    return exit_code;
}

uint8_t array_put_array(array *arr, array *arr_to_put)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr_to_put, ARR_ERR_INVALARG, "DYARR");
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    if (arr_to_put->item_size != arr->item_size)
    {
        ERR("ignoring array_put_array, arr->item(size: %zu) is not of "
            "size "
            "arr_to_put->item_size(size: %zu)",
            arr->item_size, arr_to_put->item_size);

        exit_code = ARR_ERR_SIZE_MISMATCH;
        goto cleanup;
    }

    if (arr->cap < arr->count + arr_to_put->count)
    {
        INFO("[DYARR] resizing array");
        arr->cap = (arr->cap + arr_to_put->cap) * 2;

        void *new_arr_items =
            realloc(arr->items, arr->cap * arr->item_size);

        IS_NULL(new_arr_items, ARR_ERR_ALLOC, "DYARR");
        arr->items = new_arr_items;
    }

    void *target_location =
        (char *)arr->items + arr->count * arr->item_size;

    memcpy(target_location, arr_to_put->items,
           arr_to_put->count * arr->item_size);

    arr->count += arr_to_put->count;

cleanup:
    return exit_code;
}

uint8_t array_pop(array *arr, size_t count_to_pop)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    if (count_to_pop == 0)
    {
        WARN("won't pop 0 items, skipping pop");
        exit_code = ARR_ERR_INVALARG;
        goto cleanup;
    }

    if (arr->count <= count_to_pop)
    {
        ERR("can't free %zu items from array as array only has %zu items",
            count_to_pop, arr->count);
        exit_code = ARR_ERR_INVALARG;
        goto cleanup;
    }

    arr->count -= count_to_pop;

cleanup:
    return exit_code;
}

uint8_t array_reset(array *arr)
{
    uint8_t exit_code = ARR_NO_ERR;
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    arr->count = 0;

cleanup:
    return exit_code;
}

uint8_t array_strip(array *arr)
{
    uint8_t exit_code = ARR_NO_ERR;
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR");

    array *new_arr_items =
        realloc(arr->items, arr->count * arr->item_size);
    IS_NULL(new_arr_items, ARR_ERR_ALLOC, "DYARR");
    arr->cap = arr->count;

    arr->items = new_arr_items;

cleanup:
    return exit_code;
}
