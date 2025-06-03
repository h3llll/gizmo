#include "libraries.h"
#include WINDOW_INCLUDE
#include "event/event.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdlib.h>

char *window_err_str(uint8_t code)
{
    switch (code)
    {
    case WIN_NO_ERR:
        return "success";
        break;

    case WIN_ERR_GLFWERR:
        return "glfw error";
        break;

    case WIN_ERR_ALLOC:
        return "memory allocation failed";
        break;

    case WIN_ERR_INVALARG:
        return "a NULL pointer was given as an argument";
        break;

    case WIN_ERR_EVENT_ALLOC:
        return "an event function failed to allocate memroy";
        break;

    default:
        return "invalid error code";
    }
}

// Callback definitions
void glfw_err_callback(int err, const char *desc)
{
    ERR("[WINDOW] GLFW err\n code: %d\n desc: %s\n", err, desc);
}

void glfw_size_callback(GLFWwindow *glfw_win, int width, int height)
{
    window_t *window = glfwGetWindowUserPointer(glfw_win);
    window->width = width;
    window->height = height;
    INFO("[WINDOW] %p window size changed: x: %d, y: %d\n", glfw_win,
         width, height);
}

void glfw_close_callback(GLFWwindow *glfw_win)
{
    INFO("[WINDOW] %p window is closing..\n", glfw_win);
    glfwSetWindowShouldClose(glfw_win, GLFW_TRUE);
}

// below is unconfirmed, just a 30~% probability
// NOTE IMPORTANT TODO HELP VERY IMPORTANT DONT FORGET
//
// sharing the data pointer (window->input_device->keyinfo)
// or (window->input_device->mbinfo) causes a segfault while freeing the
// input device
// asan:
//     #4 0x7b3469e6f47f in event_destroy ../modules/event/src/event.c:151
//    #5 0x7b3469e885ba in input_device_destroy
//    ../modules/window/src/window.c:401
//   #6 0x7b3469e88963 in window_destroy ../modules/window/src/window.c:240
//   #7 0x5e42ba8cf557 in main ../src/main.c:48

// (use after free, double free or something irdk)
//
// quick solution : just use different input_device fields
//
// long term solution : make a good data sharing system and data structs
// and shit

void glfw_key_callback(GLFWwindow *glfw_win, int key, int scancode,
                       int action, int mods)
{
    window_t *window = glfwGetWindowUserPointer(glfw_win);
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
        break;
    }

    case GLFW_RELEASE:
    {
        event_load(window->input_device->key_released_event,
                   window->input_device->keyinfo);
        event_system_fire(window->input_device->event_sys,
                          window->input_device->key_released_event);
        break;
    }
    }
}

void glfw_mb_callback(GLFWwindow *glfw_win, int button, int action,
                      int mods)
{
    window_t *window = glfwGetWindowUserPointer(glfw_win);

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
        break;
    }

    case GLFW_RELEASE:
    {
        event_load(window->input_device->mb_released_event,
                   window->input_device->mbinfo);
        event_system_fire(window->input_device->event_sys,
                          window->input_device->mb_released_event);
        break;
    }
    }
}

void glfw_mp_callback(GLFWwindow *glfw_win, double x, double y)
{
    window_t *window = glfwGetWindowUserPointer(glfw_win);

    window->input_device->mpinfo->x = x;
    window->input_device->mpinfo->y = y;

    event_load(window->input_device->mouse_motion_event,
               window->input_device->mpinfo);

    event_system_fire(window->input_device->event_sys,
                      window->input_device->mouse_motion_event);
}

void glfw_ms_callback(GLFWwindow *glfw_win, double xoffset, double yoffset)
{
    printf("scall");
    window_t *window = glfwGetWindowUserPointer(glfw_win);

    window->input_device->msinfo->xoffset = xoffset;
    window->input_device->msinfo->yoffset = yoffset;

    event_load(window->input_device->mouse_scroll_event,
               window->input_device->msinfo);

    event_system_fire(window->input_device->event_sys,
                      window->input_device->mouse_scroll_event);
}

void glfw_fb_callback(GLFWwindow *glfw_win, int width, int height)
{
    INFO("[WINDOW] resized window");
    window_t *window = glfwGetWindowUserPointer(glfw_win);
    window->width = width;
    window->height = height;
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
                      window_t **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    INFO("[WINDOW] creating window");

    GLFWwindow *windata = NULL;
    window_t *_result = NULL;
    input_device_t *input_dev = NULL;

    *result = NULL;

    // Hint applications
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    // Window creation
    windata = glfwCreateWindow(width, height, title, NULL, NULL);
    IS_NULL(windata, WIN_ERR_GLFWERR, "WINDOW",
            "glfw failed to create window");

    glfwMakeContextCurrent(windata);

    // Callbacks
    glfwSetWindowCloseCallback(windata, glfw_close_callback);
    glfwSetKeyCallback(windata, glfw_key_callback);
    glfwSetMouseButtonCallback(windata, glfw_mb_callback);
    glfwSetCursorPosCallback(windata, glfw_mp_callback);
    glfwSetFramebufferSizeCallback(windata, glfw_fb_callback);
    // Returning the handle
    _result = malloc(sizeof(window_t));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW", "malloc failed");
    uint8_t input_dev_code = input_device_create(&input_dev);

    if (input_dev_code != WIN_NO_ERR)
    {
        ERR("[WINDOW] failed to create input device %s",
            window_err_str(input_dev_code));

        exit_code = input_dev_code;
        goto cleanup;
    }

    _result->input_device = input_dev;
    _result->data = windata;
    _result->width = width;
    _result->height = height;

    *result = _result;

    glfwSetWindowUserPointer(windata, _result);

    return exit_code;

cleanup:
{
    FREE(_result, free);
    FREE(windata, glfwDestroyWindow);
    return exit_code;
}
}

uint8_t window_closing(window_t *win)
{
    uint8_t exit_code = WIN_NO_ERR;
    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");

    return glfwWindowShouldClose(win->data);
cleanup:
    return exit_code;
}

uint8_t window_swap_buffers(window_t *win)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");
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

uint8_t window_destroy(window_t **win)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");

    window_t *win_ptr = *win;
    IS_NULL(win_ptr, WIN_ERR_INVALARG, "WINDOW",
            "\'win\' argument points to NULL");

    glfwDestroyWindow(win_ptr->data);

    uint8_t input_dev_code = input_device_destroy(&win_ptr->input_device);
    if (input_dev_code != WIN_NO_ERR)
    {
        WARN("[WINDOW] failed to free input device: %s, freeing window.",
             window_err_str(input_dev_code));
    }

    FREE(win_ptr, free);

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

uint8_t window_key_pressed(window_t *win, int key)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument was NULL");
    return glfwGetKey(win->data, key) == GLFW_PRESS;

cleanup:
    return exit_code;
}

uint8_t window_key_repeated(window_t *win, int key)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");
    return glfwGetKey(win->data, key) == GLFW_REPEAT;

cleanup:
    return exit_code;
}

uint8_t window_key_released(window_t *win, int key)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");
    return glfwGetKey(win->data, key) == GLFW_RELEASE;

cleanup:
    return exit_code;
}

uint8_t window_mousebutton_pressed(window_t *win, int button)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");
    return glfwGetMouseButton(win->data, button) == GLFW_PRESS;

cleanup:
    return exit_code;
}

uint8_t window_mousebutton_released(window_t *win, int button)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");
    return glfwGetMouseButton(win->data, button) == GLFW_RELEASE;

cleanup:
    return exit_code;
}

uint8_t window_get_mouse_pos(window_t *win, double *x_result,
                             double *y_result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(win, WIN_ERR_INVALARG, "WINDOW", "\'win\' argument is NULL");
    IS_NULL(x_result, WIN_ERR_INVALARG, "WINDOW",
            "\'x_result\' argument is NULL");
    IS_NULL(y_result, WIN_ERR_INVALARG, "WINDOW",
            "\'y_result\' argument is NULL");

    glfwGetCursorPos(win->data, x_result, y_result);
    return WIN_NO_ERR;

cleanup:
    return exit_code;
}

// input_device_
uint8_t input_device_create(input_device_t **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    input_device_t *_result = NULL;

    _result = malloc(sizeof(input_device_t));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW", "malloc failed");
    uint8_t event_exit_code = EVENT_NO_ERR;
    uint8_t info_exit_code = WIN_NO_ERR;

    event_exit_code =
        event_create(WINEVENT_KEY_DOWN, &_result->key_down_event);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create key_down_event: %s",
            event_err_str(event_exit_code));
        exit_code = event_exit_code;
        goto cleanup;
    }

    event_exit_code =
        event_create(WINEVENT_KEY_RELEASED, &_result->key_released_event);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create key_released_event: %s",
            event_err_str(event_exit_code));
        exit_code = event_exit_code;

        goto cleanup;
    }

    event_exit_code =
        event_create(WINEVENT_KEY_RELEASED, &_result->mb_down_event);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create mb_down_event: %s",
            event_err_str(event_exit_code));
        exit_code = event_exit_code;
        goto cleanup;
    }

    event_exit_code =
        event_create(WINEVENT_KEY_RELEASED, &_result->mb_released_event);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create mb_released_event: %s",
            event_err_str(event_exit_code));
        exit_code = event_exit_code;
        goto cleanup;
    }

    event_exit_code =
        event_create(WINEVENT_KEY_RELEASED, &_result->mouse_motion_event);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create mouse_motion_event: %s",
            event_err_str(event_exit_code));
        exit_code = event_exit_code;
        goto cleanup;
    }

    event_exit_code =
        event_create(WINEVENT_KEY_RELEASED, &_result->mouse_scroll_event);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create mouse_scroll_event: %s",
            event_err_str(event_exit_code));
        exit_code = event_exit_code;
        goto cleanup;
    }

    event_exit_code = event_system_create(&_result->event_sys);
    if (event_exit_code != EVENT_NO_ERR)
    {
        ERR("[WINDOW] failed to create event_sys: %s",
            event_err_str(event_exit_code));

        exit_code = event_exit_code;
        goto cleanup;
    }

    info_exit_code = key_info_create(0, 0, 0, &_result->keyinfo);
    if (info_exit_code != WIN_NO_ERR)
    {
        ERR("[WINDOW] failed to create keyinfo: %s",
            window_err_str(info_exit_code));
        exit_code = info_exit_code;
        goto cleanup;
    }

    info_exit_code = mb_info_create(0, 0, &_result->mbinfo);
    if (info_exit_code != WIN_NO_ERR)
    {
        ERR("[WINDOW] failed to create mbinfo: %s",
            window_err_str(info_exit_code));
        exit_code = info_exit_code;
        goto cleanup;
    }

    info_exit_code = mp_info_create(0, 0, &_result->mpinfo);
    if (info_exit_code != WIN_NO_ERR)
    {
        ERR("[WINDOW] failed to create mpinfo: %s",
            window_err_str(info_exit_code));
        exit_code = info_exit_code;
        goto cleanup;
    }

    info_exit_code = ms_info_create(0, 0, &_result->msinfo);
    if (info_exit_code != WIN_NO_ERR)
    {
        ERR("[WINDOW] failed to create msinfo: %s",
            window_err_str(info_exit_code));
        exit_code = info_exit_code;
        goto cleanup;
    }
    *result = _result;

    return exit_code;

cleanup:
{
    input_device_destroy(&_result);
    return exit_code;
}
}

uint8_t input_device_destroy(input_device_t **dev)
{
    uint8_t exit_code = WIN_NO_ERR;
    IS_NULL(dev, WIN_ERR_INVALARG, "WINDOW", "\'dev\' argument is NULL");

    input_device_t *dev_ptr = *dev;
    IS_NULL(dev_ptr, WIN_ERR_INVALARG, "WINDOW",
            "\'dev\' is a pointer to NULL");

    FREE_PTR(&dev_ptr->event_sys, event_system_destroy);

    FREE_PTR(&dev_ptr->key_down_event, event_destroy);
    FREE_PTR(&dev_ptr->key_released_event, event_destroy);
    FREE_PTR(&dev_ptr->mb_down_event, event_destroy);
    FREE_PTR(&dev_ptr->mb_released_event, event_destroy);
    FREE_PTR(&dev_ptr->mouse_motion_event, event_destroy);
    FREE_PTR(&dev_ptr->mouse_scroll_event, event_destroy);
    FREE_PTR(&dev_ptr->keyinfo, key_info_destroy);
    FREE_PTR(&dev_ptr->mbinfo, mb_info_destroy);
    FREE_PTR(&dev_ptr->mpinfo, mp_info_destroy);
    FREE_PTR(&dev_ptr->msinfo, ms_info_destroy);
    FREE(dev_ptr, free);

    return exit_code;

cleanup:
    return exit_code;
}

// key_info_
uint8_t key_info_create(int32_t key, int32_t scancode, int32_t mods,
                        key_info_t **result)
{
    uint8_t exit_code = WIN_NO_ERR;
    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW",
            "\'result\' argument is NULL");

    key_info_t *_result = malloc(sizeof(key_info_t));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW", "malloc failed");

    _result->key = key;
    _result->scancode = scancode;
    _result->mods = mods;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t key_info_destroy(key_info_t **keyinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(keyinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'keyinfo\' argument is NULL");

    key_info_t *keyinfo_ptr = *keyinfo;
    IS_NULL(keyinfo_ptr, WIN_ERR_INVALARG, "WINDOW",
            "\'keyinfo\' is a pointer to NULL");

    FREE(keyinfo_ptr, free);

    return exit_code;

cleanup:
    return exit_code;
}

// mb_info_
uint8_t mb_info_create(int32_t button, int32_t mods, mb_info_t **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW",
            "\'result\' argument is NULL");

    mb_info_t *_result = malloc(sizeof(mb_info_t));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW", "malloc failed");

    _result->button = button;
    _result->mods = mods;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t mb_info_destroy(mb_info_t **mbinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(mbinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'mbinfo\' argument is NULL");
    IS_NULL(*mbinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'mbinfo\' argument points to NULL");

    FREE(*mbinfo, free);

    return exit_code;

cleanup:
    return exit_code;
}

// mp_info_
uint8_t mp_info_create(double x, double y, mp_info_t **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW",
            "\'result\' argument is NULL");

    mp_info_t *_result = malloc(sizeof(mp_info_t));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW", "malloc failed");

    _result->x = x;
    _result->y = y;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t mp_info_destroy(mp_info_t **mpinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(mpinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'mpinfo\' argument is NULL");
    IS_NULL(*mpinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'mpinfo\' points to NULL");
    FREE(*mpinfo, free);

    return exit_code;
cleanup:
    return exit_code;
}

// ms_info_
uint8_t ms_info_create(double xoffset, double yoffset, ms_info_t **result)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(result, WIN_ERR_INVALARG, "WINDOW",
            "\'result\' argument is NULL");

    ms_info_t *_result = malloc(sizeof(ms_info_t));
    IS_NULL(_result, WIN_ERR_ALLOC, "WINDOW", "malloc failed");

    _result->xoffset = xoffset;
    _result->yoffset = yoffset;

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t ms_info_destroy(ms_info_t **msinfo)
{
    uint8_t exit_code = WIN_NO_ERR;

    IS_NULL(msinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'msinfo\' argument is NULL");
    IS_NULL(*msinfo, WIN_ERR_INVALARG, "WINDOW",
            "\'msinfo\' is a pointer to NULL");
    FREE(*msinfo, free);

    return exit_code;
cleanup:
    return exit_code;
}
