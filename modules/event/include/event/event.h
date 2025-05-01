#ifndef MODULES_EVENT_H
#define MODULES_EVENT_H

#include <stdint.h>

#define EVENT_NO_ERR 0
#define EVENT_ERR_INVALARG 1
#define EVENT_ERR_ALLOC 2

typedef struct event
{
    uint8_t type;
    void *data;

} event;

typedef void event_callback(event *);

typedef struct event_system
{
    event_callback **listeners;
    uint16_t count;
    uint16_t size;
} event_system;

uint8_t event_create(uint8_t type, event **result);
uint8_t event_load(event *event, void *data);
uint8_t event_system_create(event_system **result);
uint8_t event_system_register(event_system *event_sys,
                              event_callback *callback);
uint8_t event_system_fire(event_system *event_sys, event *event);
uint8_t event_system_destroy(event_system *event_sys);
uint8_t event_destroy(event *event);

#endif // MODULES_EVENT_H
