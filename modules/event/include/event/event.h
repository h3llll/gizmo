#ifndef MODULES_EVENT_H
#define MODULES_EVENT_H

#include <stdint.h>

#define EVENT_NO_ERR 0
#define EVENT_ERR_INVALARG 1
#define EVENT_ERR_ALLOC 2

typedef struct event event_t;
typedef void event_callback_t(event_t *);
typedef struct event_system event_system_t;

// Returns a string representation of given error code.
char *event_err_str(uint8_t code);

// Creates a heap allocated event object that its type is given type in the result pointer.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_create(uint32_t type, event_t **result);

// Inputs given data struct into event's data field.
// Note that given event doesn't gain the ownership of the loaded data,
// it would still have to be freed by the caller.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_load(event_t *event, void *data);

// Gets target event's type.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_get_type(event_t *event, uint8_t *type);

// Gets the target event's data pointer.
// Caller of this function shouldn't free or modify this pointer as this is 
// the event creator's responsibility.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_get_data(event_t *event, const void **data);

// Creates a heap allocated event_system object.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_system_create(event_system_t **result);

// Registers given callback function to listen to any event fires which
// happen through given event_system.
// Returns EVENT_NO_ERR on success, 0< otherwise.
//
// FIXME IT DOESN'T CHECK IF GIVEN CALLBACK HAS ALREADY BEEN REGISTERED!!
uint8_t event_system_register(event_system_t *event_sys,
                              event_callback_t *callback);

// TODO NEITHER DONE NOR DEFINED YET!! 
// Unregisters given callback function.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_system_unregister(event_system_t *event_sys,
                              event_callback_t *callback);

// Fires given event through given event_system.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_system_fire(event_system_t *event_sys, event_t *event);

// Frees given event_system.
// Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_system_destroy(event_system_t *event_sys);

// Frees given event.
// Note that it doesn't free the data loaded through event_load to given
// event. Returns EVENT_NO_ERR on success, 0< otherwise.
uint8_t event_destroy(event_t *event);

#endif // MODULES_EVENT_H
