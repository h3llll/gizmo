#include "dyarr/dyarr.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// TODO (most likely) this module causes a nested??(asan says so) segfault
//
// I HAVE NO IDEA WHY

char *array_err_str(uint8_t code)
{
    switch (code)
    {
    case ARR_NO_ERR:
        return "no error";
        break;

    case ARR_ERR_SIZE_MISMATCH:
        return "given item size doesn't match required item size";
        break;

    case ARR_ERR_ALLOC:
        return "allocation failed";
        break;

    case ARR_ERR_INVALARG:
        return "given argument is NULL";
        break;

    default:
        return "invalid error code";
    }
}
uint8_t array_create(array **result, size_t initial_item_cap,
                     size_t item_size)
{
    uint8_t exit_code = ARR_NO_ERR;
    INFO("[DYARR] creating array");

    array *_result = NULL;
    void *items = NULL;

    IS_NULL(result, ARR_ERR_INVALARG, "DYARR",
            "\'result\' argument is NULL");

    if (initial_item_cap == 0 || item_size == 0)
    {
        ERR("cannot create 0 sized array, do not pass 0 size arguments.");
        exit_code = ARR_ERR_INVALARG;
        goto cleanup;
    }

    if (initial_item_cap > SIZE_MAX / item_size)
    {
        ERR("size overflow when calculating array allocation");
        exit_code = ARR_ERR_ALLOC;
        goto cleanup;
    }

    _result = malloc(sizeof(array));
    IS_NULL(_result, ARR_ERR_ALLOC, "DYARR", "malloc failed");

    items = malloc(initial_item_cap * item_size);
    IS_NULL(items, ARR_ERR_ALLOC, "DYARR", "malloc failed");

    _result->items = items;
    items = NULL;

    _result->cap = initial_item_cap;
    _result->item_size = item_size;
    _result->count = 0;

    *result = _result;
    _result = NULL;

    return exit_code;

cleanup:
    FREE(_result, free);
    FREE(items, free);
    return exit_code;
}

uint8_t array_destroy(array **arr)
{
    INFO("[DYARR] destroying array");
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR", "\'arr\' argument is NULL");
    IS_NULL(*arr, ARR_ERR_INVALARG, "DYARR",
            "invalid pointer to NULL \'arr\' (arr points to NULL)");
    IS_NULL((*arr)->items, ARR_ERR_INVALARG, "DYARR",
            "internal error detected, (*arr)->items is NULL, this is "
            "undefined behavour");

    FREE((*arr)->items, free);
    FREE(*arr, free);

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t array_put(array *arr, void *item, size_t item_size)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(item, ARR_ERR_INVALARG, "DYARR", "\'item\' argument is NULL");
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR", "\'arr\' argument is NULL");

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
        IS_NULL(new_arr_items, ARR_ERR_ALLOC, "DYARR",
                "\'realloc failed\'");
        arr->items = new_arr_items;
    }

    void *target_location =
        (char *)arr->items + arr->count * arr->item_size;
    memcpy(target_location, item, arr->item_size);
    arr->count++;

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t array_put_array(array *arr, array *arr_to_put)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr_to_put, ARR_ERR_INVALARG, "DYARR",
            "\'arr_to_put\' argument is NULL");
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR", "\'arr\' argument is NULL");

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

        IS_NULL(new_arr_items, ARR_ERR_ALLOC, "DYARR",
                "\'realloc failed\'");
        arr->items = new_arr_items;
    }

    void *target_location =
        (char *)arr->items + arr->count * arr->item_size;

    memcpy(target_location, arr_to_put->items,
           arr_to_put->count * arr->item_size);

    arr->count += arr_to_put->count;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t array_pop(array *arr, size_t count_to_pop)
{
    uint8_t exit_code = ARR_NO_ERR;

    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR", "\'arr\' argument is NULL");

    if (count_to_pop == 0)
    {
        WARN("won't pop 0 items, skipping pop");
        exit_code = ARR_ERR_INVALARG;
        goto cleanup;
    }

    if (arr->count < count_to_pop)
    {
        WARN("given item count (%zu) is greater than given array item "
             "count (%zu), freeing all items", count_to_pop, arr->count);
        array_reset(arr);
        array_strip(arr);
        goto cleanup;
    }
    arr->count -= count_to_pop;
    
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t array_reset(array *arr)
{
    uint8_t exit_code = ARR_NO_ERR;
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR", "\'arr\' argument is NULL");

    arr->count = 0;

cleanup:
    return exit_code;
}

uint8_t array_strip(array *arr)
{
    uint8_t exit_code = ARR_NO_ERR;
    IS_NULL(arr, ARR_ERR_INVALARG, "DYARR", "\'arr\' argument is NULL");

    void *new_arr_items = realloc(arr->items, arr->count * arr->item_size);
    IS_NULL(new_arr_items, ARR_ERR_ALLOC, "DYARR", "realloc failed");
    arr->cap = arr->count;

    arr->items = new_arr_items;

cleanup:
    return exit_code;
}
