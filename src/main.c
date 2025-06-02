#include "event/event.h"
#include "renderer/renderer.h"
#include "utils.h"
#include "libraries.h"
#include WINDOW_INCLUDE

void ms_cb(event_t *ev)
{
    uint8_t type;
    event_get_type(ev, &type);
    if (type != WINEVENT_MOUSE_SCROLL)
        return;

    const ms_info_t *ms_info;

    event_get_data(ev, (const void **)&ms_info);

    printf("mouse scroll: xoff: %f, yoff: %f", ms_info->xoffset,
           ms_info->yoffset);
}

int main(void)
{
    window_module_init();
    window_t *window;
    window_create(400, 400, "hi", &window);

    // Never initialize renderer module before creating a window
    renderer_module_init((void *)window_get_proc);
    event_system_register(window->input_device->event_sys, ms_cb);

    renderer_t *renderer;
    WARN("%d", renderer_create(&renderer, "", ""));
    int32_t x = 1;
    int32_t y = 1;

    while (!window_closing(window))
    {
        renderer_set_viewport(renderer, 0, 0, window->width,
                              window->height);
        window_poll_events();

        if (window_key_pressed(window, KEY_D))
        {
            x += 2;
        }
        if (window_key_pressed(window, KEY_A))
        {
            x -= 2;
        }

        if (window_key_pressed(window, KEY_W))
        {
            y -= 2;
        }
        if (window_key_pressed(window, KEY_S))
        {
            y += 2;
        }

        renderer_draw_begin(renderer);
        {
            renderer_colorf(renderer, 0.5f, 0.5f, 0.5f, 1.0f);
            renderer_clear(renderer);

            renderer_colorf(renderer, 0, 0, 0, 1);
            renderer_draw_rect(renderer, x, y, 40, 40);
        }
        renderer_draw_end(renderer);
        window_swap_buffers(window);
    }

    renderer_destroy(renderer);
    window_destroy(window);
    window_module_deinit();
    return 0;
}
