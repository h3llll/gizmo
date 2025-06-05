#include "event/event.h"
#include "libraries.h"
#include RENDERER_INCLUDE
#include "utils.h"
#include WINDOW_INCLUDE

void ms_cb(event_t *ev)
{
    uint8_t type;
    event_get_type(ev, &type);
    if (type != WINEVENT_MOUSE_SCROLL)
        return;

    const ms_info_t *ms_info;

    event_get_data_ptr(ev, (const void **)&ms_info);

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
    window_reg_input_callback(window, ms_cb);

    renderer_t *renderer;
    WARN("%d", renderer_create(&renderer, "", ""));
    int32_t x = 1;
    int32_t y = 1;
    int32_t win_width;
    int32_t win_height;

    while (!window_closing(window))
    {
        window_get_dimentions(window, &win_width, &win_height);
        renderer_set_viewport(renderer, 0, 0, win_width, win_height);

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

    renderer_destroy(&renderer);
    window_destroy(&window);
    window_module_deinit();
    return 0;
}
