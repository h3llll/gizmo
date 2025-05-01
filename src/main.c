#include "event/event.h"
#include "renderer/renderer.h"
#include "utils.h"
#include "window/window.h"
#include <stdio.h>

void fb_cb(win_data *windata, int32_t width, int32_t height)
{
    renderer_set_viewport(0, 0, width, height);
}

void key_cb(event *ev)
{
    if (ev->type != WINEVENT_KEY_DOWN)
        return;

    key_info *key_info = ev->data;
    printf("haha key: %c \n", key_info->key);
}

int main(void)
{
    printf("hello gizmo!\n");
    window_module_init();
    window *window;
    window_create(400, 400, "hi", &window);
    window_set_fb_callback(window, fb_cb);

    // Never initialize renderer module before creating a window
    renderer_module_init((void *)window_get_proc);

    renderer *renderer;
    renderer_create(&renderer);
    renderer_colorf(renderer, 1.0f, 1.0f, 1.0f, 1.0f);

    event_system_register(window->input_device->event_sys, key_cb);

    ERR("haha error");
    WARN("warn");
    while (!window_closing(window))
    {
        window_poll_events();
        renderer_clear(renderer);
        window_swap_buffers(window);
    }

    window_destroy(window);
    window_module_deinit();
    renderer_destroy(renderer);
    return 0;
}
