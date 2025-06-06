#ifndef MODULES_WINDOW_H
#define MODULES_WINDOW_H

#include "event/event.h"
// clang-format off
#define WIN_NO_ERR              0
#define WIN_ERR_GLFWERR         1
#define WIN_ERR_ALLOC           2
#define WIN_ERR_INVALARG        3

#define WIN_EVENT_DIFF          20

#define WIN_ERR_EVENT_ALLOC     EVENT_ERR_ALLOC     +   WIN_EVENT_DIFF
#define WIN_ERR_EVENT_INVALARG  EVENT_ERR_INVALARG  +   WIN_EVENT_DIFF

#define WINEVENT_KEY_DOWN            0
#define WINEVENT_KEY_RELEASED        1
#define WINEVENT_MOUSE_BUTTONDOWN    2
#define WINEVENT_MOUSE_BUTTONUP      3
#define WINEVENT_MOUSE_MOTION        4
#define WINEVENT_MOUSE_SCROLL        5
// clang-format on

#include "keys.h"
#include <GLFW/glfw3.h>
#include <stdint.h>

struct key_info
{
    int32_t key;
    int32_t scancode;
    int32_t mods;
};

struct mb_info
{
    int32_t button;
    int32_t mods;
};

struct mp_info
{
    double x, y;
};

struct ms_info
{
    double xoffset, yoffset;
};

typedef struct key_info key_info_t;

typedef struct mb_info mb_info_t;

typedef struct mp_info mp_info_t;

typedef struct ms_info ms_info_t;

typedef struct input_device input_device_t;

typedef struct window window_t;

typedef GLFWframebuffersizefun win_framebuffersizefun_t;

typedef event_callback_t window_input_callback_t;

// window_
/**
 * Initializes GLFW.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t window_module_init(void);

/**
 * Initializes a window on heap with the given width, height, and title in
 * the result pointer.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t window_create(int32_t width, int32_t height, const char *title,
                      window_t **result);

/**
 * Returns 1 if the window should close.
 */
uint8_t window_closing(window_t *win);

/**
 * Clears the screen and swaps the front and back buffers.
 * Returns WIN_NO_ERR on success
 */
uint8_t window_swap_buffers(window_t *win);

/**
 * Polls for window events.
 * Return WIN_NO_ERR on success
 */
uint8_t window_poll_events(void);

/**
 * Deallocates window object and destroys glfw context
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t window_destroy(window_t **win);

/**
 * Terminates GLFW.
 * Returns WIN_NO_ERR on succes.
 */
uint8_t window_module_deinit(void);

/**
 * Returns procedure address from procedure name
 */
const void *window_get_proc(const char *proc_name);

/**
 * Checks if target key is pressed, returns true if it is, false otherwise.
 */
uint8_t window_key_pressed(window_t *window, int key);

/**
 * Checks if target key is repeated, returns true if it is, false
 * otherwise.
 */
uint8_t window_key_repeated(window_t *window, int key);

/**
 * Checks if target key is released, returns true if it is, false
 * otherwise.
 */
uint8_t window_key_released(window_t *window, int key);

/**
 * Checks if target mouse button is pressed, returns true if it is,
 * false otherwise.
 */
uint8_t window_mousebutton_pressed(window_t *window, int button);

/**
 * Checks if target mouse button is released, returns true if it is,
 * false otherwise.
 */
uint8_t window_mousebutton_released(window_t *window, int button);

/**
 * Returns mouse's x and y position in their respective result pointers.
 * Returns WIN_NO_ERR on success.
 */
uint8_t window_get_mouse_pos(window_t *window, double *x_result,
                             double *y_result);
/**
 * Registers given function to be called whenever an input event is fired
 * with an event argument. Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t window_reg_input_callback(window_t *win,
                                  window_input_callback_t *callback);
/**
 * Returns given window's width and height in their respective result
 * pointer. Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t window_get_dimentions(window_t *win, int32_t *result_width,
                              int32_t *result_height);

// input_device_
/**
 * Creates input_device heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t input_device_create(input_device_t **result);

/**
 * Frees input_device heap object and NULLs its pointer.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t input_device_destroy(input_device_t **dev);

// key_info_
/**
 * Creates key_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t key_info_create(int32_t key, int32_t scancode, int32_t mods,
                        key_info_t **result);
/**
 * Frees key_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t key_info_destroy(key_info_t **keyinfo);

// mb_info_
/**
 * Creates key_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mb_info_create(int32_t button, int32_t mods, mb_info_t **result);

/**
 * Frees mb_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mb_info_destroy(mb_info_t **mbinfo);

// mp_info_
/**
 * Creates mp_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mp_info_create(double x, double y, mp_info_t **result);

/**
 * Creates mp_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mp_info_destroy(mp_info_t **mpinfo);

// ms_info_
/**
 * Creates ms_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t ms_info_create(double xoffset, double yoffset, ms_info_t **result);

/**
 * Creates mp_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t ms_info_destroy(ms_info_t **msinfo);

#endif
