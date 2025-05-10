#ifndef MODULES_WINDOW_H
#define MODULES_WINDOW_H

// clang-format off
#define WIN_NO_ERR              0
#define WIN_ERR_GLFWERR         1
#define WIN_ERR_ALLOC           2
#define WIN_ERR_INVALARG        3

#define WINEVENT_KEY_DOWN            0
#define WINEVENT_KEY_RELEASED        1
#define WINEVENT_MOUSE_BUTTONDOWN    2
#define WINEVENT_MOUSE_BUTTONUP      3
#define WINEVENT_MOUSE_MOTION        4
// TODO mouse wheel lol #define WINEVENT_MOUSE_WHEEL        5
// clang-format on

#include "event/event.h"
#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct key_info
{
    int32_t key;
    int32_t scancode;
    int32_t mods;
} key_info;

typedef struct mb_info
{
    int32_t button;
    int32_t mods;
} mb_info;

typedef struct mp_info
{
    double x, y;
} mp_info;

typedef struct input_device
{
    event *key_down_event, *key_released_event;
    event *mouse_motion_event, *mb_down_event, *mb_released_event;
    event_system *event_sys;
    key_info *keyinfo;
    mb_info *mbinfo;
    mp_info *mpinfo;

} input_device;

typedef struct window
{
    GLFWwindow *data;
    input_device *input_device;
    int32_t width, height;

} window;

typedef GLFWframebuffersizefun win_framebuffersizefun;
typedef GLFWwindow win_data;

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
                      window **result);

/**
 * Returns 1 if the window should close.
 */
uint8_t window_closing(window *win);

/**
 * Clears the screen and swaps the front and back buffers.
 * Returns WIN_NO_ERR on success
 */
uint8_t window_swap_buffers(window *win);

/**
 * Polls for window events.
 * Return WIN_NO_ERR on success
 */
uint8_t window_poll_events(void);

/**
 * Deallocates window object and destroys glfw context
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t window_destroy(window *win);

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
uint8_t window_key_pressed(window *window, int key);

/**
 * Checks if target key is repeated, returns true if it is, false
 * otherwise.
 */
uint8_t window_key_repeated(window *window, int key);

/**
 * Checks if target key is released, returns true if it is, false
 * otherwise.
 */
uint8_t window_key_released(window *window, int key);

/**
 * Checks if target mouse button is pressed, returns true if it is,
 * false otherwise.
 */
uint8_t window_mousebutton_pressed(window *window, int button);

/**
 * Checks if target mouse button is released, returns true if it is,
 * false otherwise.
 */
uint8_t window_mousebutton_released(window *window, int button);

/**
 * Returns mouse's x and y position in their respective result pointers.
 * Returns 0 on success.
 */
uint8_t window_get_mouse_pos(window *window, double *x_result,
                             double *y_result);
// input_device_
/**
 * Creates input_device heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t input_device_create(input_device **result);

/**
 * Frees input_device heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t input_device_destroy(input_device *dev);

// key_info_
/**
 * Creates key_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t key_info_create(int32_t key, int32_t scancode, int32_t mods,
                        key_info **result);
/**
 * Frees key_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t key_info_destroy(key_info *keyinfo);

// mb_info_
/**
 * Creates key_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mb_info_create(int32_t button, int32_t mods, mb_info **result);

/**
 * Frees mb_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mb_info_destroy(mb_info *mbinfo);

// mp_info_
/**
 * Creates mp_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mp_info_create(double x, double y, mp_info **result);

/**
 * Creates mp_info heap object.
 * Returns WIN_NO_ERR on success, 0< on failure.
 */
uint8_t mp_info_destroy(mp_info *mpinfo);

#endif
