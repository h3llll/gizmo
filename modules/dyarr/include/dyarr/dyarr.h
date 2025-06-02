#ifndef MODULES_DYARR_H
#define MODULES_DYARR_H

// clang-format off
#define ARR_NO_ERR              0
#define ARR_ERR_INVALARG        1
#define ARR_ERR_ALLOC           2
#define ARR_ERR_SIZE_MISMATCH   3

#define array_get(arr, type, index)                         \
    ((type)(arr)->items[index])

// clang-format on

#include <stddef.h>
#include <stdint.h>

typedef struct array
{
    void *items;
    size_t count;
    size_t cap;
    size_t item_size;
} array;

// Returns a string representation of given error code
char *array_err_str(uint8_t code);

// Returns a heap allocated array in the result pointer, which only
// accepts items of specified item size.
// Note that you can't exceed SIZE_MAX, be careful with high item_size/cap
// Returns ARR_NO_ERR on success, 0< otherwise.
uint8_t array_create(array **result, size_t cap, size_t item_size);

// Deallocates the array and all items of the array.
// Returns ARR_NO_ERR on success, 0< otherwise.
uint8_t array_destroy(array **arr);

// Places given item at the end of the array, stretching the array
// when needed, while only accepting items of size: array->item_size.
// Returns ARR_NO_ERR on success, 0< otherwise.
uint8_t array_put(array *arr, void *item, size_t item_size);

// Copies arr_to_put memory directly at the end of target_arr
// after stretching it to fit arr_to_put if needed, only if both arrays
// have the same arr*->item_size.
// Returns ARR_NO_ERR on success, 0< otherwise.
uint8_t array_put_array(array *arr, array *arr_to_put);

// Reduces the array item count by count and strips the array removing
// extra unused memory(in this case, the former items). (if you want to
// free its memory, use array_strip() after) Returns ARR_NO_ERR on success,
// 0< otherwise.
uint8_t array_pop(array *arr, size_t count_to_pop);

// Resets the array's item count so you can use preallocated memory
// again without having to reallocate/deallocate the array.
// As stated, DOES NOT FREE MEMORY.
// Returns ARR_NO_ERR on success, 0< otherwise.
uint8_t array_reset(array *arr);

// Strips off unused memory in the array to tightly pack its members.
// Returns ARR_NO_ERR on success, 0< otherwise.
uint8_t array_strip(array *arr);

#endif // MODULES_DYARR_H
