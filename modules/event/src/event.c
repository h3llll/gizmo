#include "event/event.h"
#include "dyarr/dyarr.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

struct event
{
    uint32_t type;
    void *data;
};

struct event_system
{
    array *listeners;
};

char *event_err_str(uint8_t code)
{
    switch (code)
    {
    case EVENT_NO_ERR:
        return "success";
        break;

    case EVENT_ERR_INVALARG:
        return "a given pointer argument is NULL.\n";
        break;

    case EVENT_ERR_ALLOC:
        return "memory allocation failed";
        break;

    default:
        return "unknown error code";
    }
}

uint8_t event_create(uint32_t type, event_t **result)
{
    uint8_t exit_code = EVENT_NO_ERR;
    IS_NULL(result, EVENT_ERR_INVALARG, "EVENT",
            "given result pointer is NULL");

    event_t *_result = malloc(sizeof(event_t));
    IS_NULL(_result, EVENT_ERR_ALLOC, "EVENT", "malloc failed");

    _result->type = type;
    _result->data = NULL;

    *result = _result;

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t event_get_type(event_t *event, uint8_t *type)
{
    uint8_t exit_code = EVENT_NO_ERR;
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT",
            "\'event\' argument is NULL");
    IS_NULL(type, EVENT_ERR_INVALARG, "EVENT",
            "\'type\' argument is NULL");

    *type = event->type;

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t event_load(event_t *event, void *data)
{
    uint8_t exit_code = EVENT_NO_ERR;
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT",
            "\'event\' argument is NULL");
    IS_NULL(data, EVENT_ERR_INVALARG, "EVENT",
            "\'data\' argument is NULL");

    event->data = data;

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t event_get_data(event_t *event, const void **data)
{
    uint8_t exit_code = EVENT_NO_ERR;
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT",
            "\'event\' argument is NULL");
    IS_NULL(data, EVENT_ERR_INVALARG, "EVENT",
            "\'data\' argument is NULL");

    IS_NULL(event->data, EVENT_ERR_INVALARG, "EVENT",
            "given event object doesn't hold any data");
    *data = event->data;

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t event_system_create(event_system_t **result)
{
    uint8_t exit_code = EVENT_NO_ERR;

    event_system_t *_result = NULL;
    array *listeners = NULL;
    size_t initial_listeners = 2;
    uint8_t arr_code = array_create(&listeners, initial_listeners,
                                    sizeof(event_callback_t));
    if (arr_code != ARR_NO_ERR)
    {
        ERR("failed to create dynamic array: %s", array_err_str(arr_code));
        goto cleanup;
    }

    _result = malloc(sizeof(event_system_t));
    IS_NULL(_result, EVENT_ERR_ALLOC, "EVENT", "malloc failed");

    _result->listeners = listeners;
    *result = _result;

    return exit_code;
cleanup:
    FREE(_result, free);
    return exit_code;
}

uint8_t event_system_register(event_system_t *event_sys,
                              event_callback_t *callback)
{
    uint8_t exit_code = EVENT_NO_ERR;
    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT",
            "\'event_sys\' argument is NULL");
    IS_NULL(callback, EVENT_ERR_INVALARG, "EVENT",
            "\'callback\' argument is NULL");

    array_put(event_sys->listeners, callback, sizeof(callback));

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_fire(event_system_t *event_sys, event_t *event)
{
    uint8_t exit_code = EVENT_NO_ERR;

    for (size_t i = 0; i < event_sys->listeners->count; i++)
    {
        event_callback_t callback =
            array_get(event_sys->listeners, event_callback_t, i); // syntax error here ffs TODO ERROR README BAD DANGEROUS SEX
    }

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t event_system_destroy(event_system_t *event_sys)
{
}

uint8_t event_destroy(event_t *event)
{
}
