#include "renderer/renderer.h"
#include "window/window.h"
#include "window/keys.h"

int main(void)
{
    window_module_init();
    window *window;
    window_create(400, 400, "hi", &window);

    // Never initialize renderer module before creating a window
    renderer_module_init((void *)window_get_proc);

    renderer *renderer;
    renderer_create(&renderer, "", "");
    int32_t x = 1;
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

        renderer_draw_begin(renderer);
        {
            renderer_colorf(renderer, 0.5f, 0.5f, 0.5f, 1.0f);
            renderer_clear(renderer);

            renderer_colorf(renderer, 0, 0, 0, 1);
            renderer_draw_rect(renderer, x, 0, 40, 40);

            renderer_draw_rect(renderer, x, 30, 600, 600);

        }
        renderer_draw_end(renderer);
        window_swap_buffers(window);
    }

    window_destroy(window);
    window_module_deinit();
    renderer_destroy(renderer);
    return 0;
}
