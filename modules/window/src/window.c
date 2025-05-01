#include "window/window.h"
#include "event/event.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdlib.h>

// TODO cleanup goto statements to free if a function failed but after
// memory allocation, god i forgot that exists
// aaugudsuaghuaghsagdjsalfsajldaf

// Callback definitions
void glfw_err_callback(int err, const char *desc)
{
    ERR("[WINDOW] GLFW err\n code: %d\n desc: %s\n", err, desc);
}

void window_size_callback(GLFWwindow *glfw_win, int width, int height)
{
    INFO("[WINDOW] %p window size changed: x: %d, y: %d\n", glfw_win,
         width, height);
}

void window_close_callback(GLFWwindow *glfw_win)
{
    INFO("[WINDOW] %p window is closing..\n", glfw_win);
    glfwSetWindowShouldClose(glfw_win, GLFW_TRUE);
}

void window_key_callback(GLFWwindow *glfw_win, int key, int scancode,
                         int action, int mods)
{
    window *window = glfwGetWindowUserPointer(glfw_win);
    window->input_device->keyinfo->key = key;
    window->input_device->keyinfo->scancode = scancode;
    window->input_device->keyinfo->mods = mods;

    switch (action)
    {

    case GLFW_PRESS:
    {
        event_load(window->input_device->key_down_event,
                   window->input_device->keyinfo);
        event_system_fire(window->input_device->event_sys,
                          window->input_device->key_down_event);
        INFO("[WINDOW] fired key down event");
        break;
    }

    case GLFW_RELEASE:
    {
        event_load(window->input_device->key_released_event,
                   window->input_device->keyinfo);
        event_system_fire(window->input_device->event_sys,
                          window->input_device->key_released_event);
        INFO("[WINDOW] fired key release event");

        break;
    }
    }
}

void window_mb_callback(GLFWwindow *glfw_win, int button, int action,
                        int mods)
{
    window *window = glfwGetWindowUserPointer(glfw_win);

    window->input_device->mbinfo->button = button;
    window->input_device->mbinfo->mods = mods;

    switch (action)
    {

    case GLFW_PRESS:
    {
        event_load(window->input_device->mb_down_event,
                   window->input_device->mbinfo);
        event_system_fire(window->input_device->event_sys,
                          window->input_device->mb_down_event);
        INFO("[WINDOW] fired mouse button down event");
        break;
    }

    case GLFW_RELEASE:
    {
        event_load(window->input_device->mb_released_event,
                   window->input_device->mbinfo);
        event_system_fire(window->input_device->event_sys,
                          window->input_device->mb_released_event);
        INFO("[WINDOW] fired mouse button release event");
        break;
    }
    }
}

void window_mp_callback(GLFWwindow *glfw_win, double x, double y)
{
    window *window = glfwGetWindowUserPointer(glfw_win);

    window->input_device->mpinfo->x = x;
    window->input_device->mpinfo->y = y;

    event_load(window->input_device->mouse_motion_event,
               window->input_device->mpinfo);

    event_system_fire(window->input_device->event_sys,
                      window->input_device->mouse_motion_event);
    INFO("[WINDOW] fired mouse motion event");
}

uint8_t window_set_fb_callback(window *window, win_framebuffersizefun func)
{
    uint8_t exit_code = WIN_NO_ERR;
    INFO("[WINDOW] setting framebuffer size callback");
    IS_NULL(window, WIN_NO_ERR, "WINDOW");
    glfwSetFramebufferSizeCallback(window->data, func);
cleanup:
{
    return exit_code;
}
}

// window_
uint8_t window_module_init(void)
{

    INFO("[WINDOW] setting error callback");
    glfwSetErrorCallback(glfw_err_callback);

    INFO("[WINDOW] initiating glfw");
    if (glfwInit() == GLFW_FALSE)
    {
        return WIN_ERR_GLFWERR;
    }
    return WIN_NO_ERR;
}

uint8_t window_create(int32_t width, int32_t height, const char *title,
                      window **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    INFO("[WINDOW] creating window");

    GLFWwindow *windata = NULL;
    window *_result = NULL;
    input_device *input_dev = NULL;

    *result = NULL;

    // Hint applications
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // gl 3.3

    // Window creation
    windata = glfwCreateWindow(width, height, title, NULL, NULL);
    IS_NULL(windata, WIN_ERR_GLFWERR, "WINDOW");

    glfwMakeContextCurrent(windata);

    // Callbacks
    glfwSetWindowCloseCallback(windata, window_close_callback);
    glfwSetKeyCallback(windata, window_key_callback);
    glfwSetMouseButtonCallback(windata, window_mb_callback);
    glfwSetCursorPosCallback(windata, window_mp_callback);

    // Returning the handle
    _result = malloc(sizeof(window));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(input_device_create(&input_dev), WIN_ERR_ALLOC,
                WIN_ERR_ALLOC, "WINDOW");

    _result->input_device = input_dev;
    _result->data = windata;

    *result = _result;

    glfwSetWindowUserPointer(windata, _result);

    return exit_code;

cleanup:
{
    FREE(_result, free);
    if (windata != NULL)
        glfwDestroyWindow(windata);
    return exit_code;
}
}

uint8_t window_closing(window *win)
{
    uint8_t exit_code = WIN_NO_ERR;
    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW");

    return glfwWindowShouldClose(win->data);
cleanup:
    return exit_code;
}

uint8_t window_swap_buffers(window *win)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW");
    glfwSwapBuffers(win->data);

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t window_poll_events(void)
{
    glfwPollEvents();

    return WIN_NO_ERR;
}

uint8_t window_destroy(window *win)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW");

    glfwDestroyWindow(win->data);
    input_device_destroy(win->input_device);
    free(win);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t window_module_deinit(void)
{
    glfwTerminate();

    return WIN_NO_ERR;
}

const void *window_get_proc(const char *proc_name)
{
    return glfwGetProcAddress(proc_name);
}

uint8_t window_key_pressed(window *window, int key)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(window, WIN_ERR_INVALARG, "WINDOW");
    return glfwGetKey(window->data, key) == GLFW_PRESS;

cleanup:
    return exit_code;
}

uint8_t window_key_repeated(window *window, int key)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(window, WIN_ERR_INVALARG, "WINDOW");
    return glfwGetKey(window->data, key) == GLFW_REPEAT;

cleanup:
    return exit_code;
}

uint8_t window_key_released(window *window, int key)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(window, WIN_ERR_INVALARG, "WINDOW");
    return glfwGetKey(window->data, key) == GLFW_RELEASE;

cleanup:
    return exit_code;
}

uint8_t window_mousebutton_pressed(window *window, int button)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(window, WIN_ERR_INVALARG, "WINDOW");
    return glfwGetMouseButton(window->data, button) == GLFW_PRESS;

cleanup:
    return exit_code;
}

uint8_t window_mousebutton_released(window *window, int button)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(window, WIN_ERR_INVALARG, "WINDOW");
    return glfwGetMouseButton(window->data, button) == GLFW_RELEASE;

cleanup:
    return exit_code;
}

uint8_t window_get_mouse_pos(window *window, double *x_result,
                             double *y_result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(window, WIN_ERR_INVALARG, "WINDOW");
    IS_NULL(x_result, WIN_ERR_INVALARG, "WINDOW");
    IS_NULL(y_result, WIN_ERR_INVALARG, "WINDOW");

    glfwGetCursorPos(window->data, x_result, y_result);
    return WIN_NO_ERR;

cleanup:
    return exit_code;
}

// input_device_
uint8_t input_device_create(input_device **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    input_device *_result = NULL;

    _result = malloc(sizeof(input_device));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(event_create(WINEVENT_KEY_DOWN, &_result->key_down_event),
                EVENT_ERR_ALLOC, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(
        event_create(WINEVENT_KEY_RELEASED, &_result->key_released_event),
        EVENT_ERR_ALLOC, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(
        event_create(WINEVENT_MOUSE_BUTTONDOWN, &_result->mb_down_event),
        EVENT_ERR_ALLOC, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(
        event_create(WINEVENT_MOUSE_BUTTONUP, &_result->mb_released_event),
        EVENT_ERR_ALLOC, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(
        event_create(WINEVENT_MOUSE_MOTION, &_result->mouse_motion_event),
        EVENT_ERR_ALLOC, WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(event_system_create(&_result->event_sys), EVENT_ERR_ALLOC,
                WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(key_info_create(0, 0, 0, &_result->keyinfo), WIN_ERR_ALLOC,
                WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(mb_info_create(0, 0, &_result->mbinfo), WIN_ERR_ALLOC,
                WIN_ERR_ALLOC, "WINDOW");

    RET_ON_FAIL(mp_info_create(0, 0, &_result->mpinfo), WIN_ERR_ALLOC,
                WIN_ERR_ALLOC, "WINDOW");

    *result = _result;

    return exit_code;

cleanup:
{
    FREE(_result->mbinfo, mb_info_destroy);
    FREE(_result->mpinfo, mp_info_destroy);
    FREE(_result->keyinfo, key_info_destroy);
    FREE(_result->event_sys, event_system_destroy);
    FREE(_result->key_down_event, event_destroy);
    FREE(_result->key_released_event, event_destroy);
    FREE(_result->mb_down_event, event_destroy);
    FREE(_result->mb_released_event, event_destroy);
    FREE(_result->mouse_motion_event, event_destroy);
    FREE(_result, free);

    return exit_code;
}
}

uint8_t input_device_destroy(input_device *dev)
{
    uint8_t exit_code = WIN_NO_ERR;
    IS_NULL(dev, WIN_ERR_INVALARG, "WINDOW");
    event_system_destroy(dev->event_sys);

    event_destroy(dev->key_down_event);
    event_destroy(dev->key_released_event);
    event_destroy(dev->mb_down_event);
    event_destroy(dev->mb_released_event);
    event_destroy(dev->mouse_motion_event);

    key_info_destroy(dev->keyinfo);
    mb_info_destroy(dev->mbinfo);
    mp_info_destroy(dev->mpinfo);
    free(dev);

    return exit_code;

cleanup:
    return exit_code;
}

// key_info_
uint8_t key_info_create(int32_t key, int32_t scancode, int32_t mods,
                        key_info **result)
{
    uint8_t exit_code = WIN_NO_ERR;
    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW");

    key_info *_result = malloc(sizeof(key_info));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW");

    _result->key = key;
    _result->scancode = scancode;
    _result->mods = mods;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t key_info_destroy(key_info *keyinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(keyinfo, WIN_ERR_INVALARG, "WINDOW");
    free(keyinfo);

    return exit_code;

cleanup:
    return exit_code;
}

// mb_info_
uint8_t mb_info_create(int32_t button, int32_t mods, mb_info **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW");

    mb_info *_result = malloc(sizeof(mb_info));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW");

    _result->button = button;
    _result->mods = mods;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t mb_info_destroy(mb_info *mbinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(mbinfo, WIN_ERR_INVALARG, "WINDOW");
    free(mbinfo);

    return exit_code;

cleanup:
    return exit_code;
}

// mp_info_
uint8_t mp_info_create(double x, double y, mp_info **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW");

    mp_info *_result = malloc(sizeof(mp_info));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW");

    _result->x = x;
    _result->y = y;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t mp_info_destroy(mp_info *mpinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(mpinfo, WIN_ERR_INVALARG, "WINDOW");
    free(mpinfo);

    return exit_code;
cleanup:
    return exit_code;
}
