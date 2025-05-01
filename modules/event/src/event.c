#include "event/event.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

void event_err_str(uint8_t code)
{
    // TODO: uhh errors n shit turn fucking error codes into fucking
    // strings or something idk switch statement or something
}

uint8_t event_create(uint8_t type, event **result)
{
    uint8_t exit_code = EVENT_NO_ERR;

    INFO("[EVENT] creating event");
    IS_NULL(result, EVENT_ERR_INVALARG, "EVENT");

    event *_result = malloc(sizeof(event));
    IS_NULL(_result, EVENT_ERR_ALLOC, "EVENT");

    _result->type = type;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_load(event *event, void *data)
{
    uint8_t exit_code = EVENT_NO_ERR;

    INFO("[EVENT] loading event with data");
    IS_NULL(data, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT");

    event->data = data;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_create(event_system **result)
{
    uint8_t exit_code = EVENT_NO_ERR;

    event_system *_result = NULL;
    event_callback **listeners = NULL;

    INFO("[EVENT] creating event_system");
    IS_NULL(result, EVENT_ERR_INVALARG, "EVENT");

    _result = malloc(sizeof(event_system));
    IS_NULL(_result, EVENT_ERR_ALLOC, "EVENT");

    uint16_t listeners_size = 1;

    listeners = malloc(listeners_size * sizeof(event_callback *));
    IS_NULL(listeners, EVENT_ERR_ALLOC, "EVENT");

    _result->listeners = listeners;
    _result->size = listeners_size;
    _result->count = 0;

    *result = _result;
    return exit_code;

cleanup:
    FREE(_result, free);
    return exit_code;
}

uint8_t event_system_register(event_system *event_sys,
                              event_callback *callback)
{
    uint8_t exit_code = EVENT_NO_ERR;

    INFO("[EVENT] registering function to event_system");
    IS_NULL(callback, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT");

    if (event_sys->count == event_sys->size)
    {
        event_callback **new_listeners =
            realloc(event_sys->listeners,
                    (event_sys->size * 2) * sizeof(event_callback *));

        IS_NULL(new_listeners, EVENT_ERR_ALLOC, "EVENT");

        event_sys->listeners = new_listeners;
        event_sys->size *= 2;
    }

    event_sys->listeners[event_sys->count] = callback;
    event_sys->count += 1;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_fire(event_system *event_sys, event *event)
{
    uint8_t exit_code = EVENT_NO_ERR;
    
    INFO("[EVENT] firing event to event_system");
    
    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT");
    IS_NULL(event->data, EVENT_ERR_INVALARG, "EVENT");

    for (int i = 0; i < event_sys->count; i++)
    {
        event_sys->listeners[i](event);
    }

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_system_destroy(event_system *event_sys)
{
    uint8_t exit_code = EVENT_NO_ERR;
    
    INFO("[EVENT] destroying event_system");
    IS_NULL(event_sys, EVENT_ERR_INVALARG, "EVENT");

    free(event_sys->listeners);
    free(event_sys);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t event_destroy(event *event)
{
    uint8_t exit_code = EVENT_NO_ERR;
    
    INFO("[EVENT] destroying event");
    IS_NULL(event, EVENT_ERR_INVALARG, "EVENT");

    FREE(event->data, free);
    free(event);

    return exit_code;

cleanup:
    return exit_code;
}
