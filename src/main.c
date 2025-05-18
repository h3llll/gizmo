#include "renderer/renderer.h"
#include "utils.h"
#include "window/window.h"
#include "window/keys.h"

int main(void)
{
    window_module_init();
    window_t *window;
    window_create(400, 400, "hi", &window);

    // Never initialize renderer module before creating a window
    renderer_module_init((void *)window_get_proc);

    renderer_t *renderer;
    WARN("%d", renderer_create(&renderer, "", ""));
    int32_t x = 1;
    int32_t y = 1;

    while (!window_closing(window))
    {
        renderer_set_viewport(renderer, 0, 0, window->width, window->height);
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
