#include "event/event.h"
#include "dyarr/dyarr.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

void event_err_str(uint8_t code)
{
    // TODO: uhh errors n shit turn fucking error codes into fucking
    // strings or something idk switch statement or something
}

uint8_t event_create(uint8_t type, event_t **result)
{
    uint8_t exit_code = EVENT_NO_ERR;

    INFO("[EVENT] creating event");
    IS_NULL(result, EVENT_ERR_INVALARG, "EVENT");

    event_t *_result = malloc(sizeof(event_t));
    IS_NULL(_result, EVENT_ERR_ALLOC, "EVENT");

    _result->type = type;
    _result->data = NULL;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_load(event_t *event, void *data)
{
    uint8_t exit_code = EVENT_NO_ERR;

    IS_NULL(data, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT");

    event->data = data;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_create(event_system_t **result)
{
    uint8_t exit_code = EVENT_NO_ERR;

    event_system_t *_result = NULL;
    array *listeners = NULL;

    INFO("[EVENT] creating event_system");
    IS_NULL(result, EVENT_ERR_INVALARG, "EVENT");

    _result = malloc(sizeof(event_system_t));
    IS_NULL(_result, EVENT_ERR_ALLOC, "EVENT");

    uint16_t listeners_size = 3;

    RET_ON_FAIL(array_create(&listeners, listeners_size,
                             sizeof(event_callback_t *)),
                ARR_ERR_ALLOC, EVENT_ERR_ALLOC, "EVENT");

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

    INFO("[EVENT] registering function to event_system");
    IS_NULL(callback, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT");

    array_put(event_sys->listeners, callback, sizeof(event_callback_t *));
    event_sys->listeners->count += 1;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_fire(event_system_t *event_sys, event_t *event)
{
    uint8_t exit_code = EVENT_NO_ERR;

    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event->data, EVENT_ERR_INVALARG, "EVENT");

    for (size_t i = 0; i < event_sys->listeners->count; i++)
    {
        array_get(event_sys->listeners, event_callback_t *, i)(event);
    }

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_destroy(event_system_t *event_sys)
{
    uint8_t exit_code = EVENT_NO_ERR;

    INFO("[EVENT] destroying event_system");
    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT");

    FREE_PTR(event_sys->listeners, array_destroy);
    FREE(event_sys, free);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_destroy(event_t *event)
{
    uint8_t exit_code = EVENT_NO_ERR;

    INFO("[EVENT] destroying event");
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT");

    FREE(event, free);

    return exit_code;

cleanup:
    return exit_code;
}
