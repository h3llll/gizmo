#include "renderer/renderer.h"
#include "glad/glad.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>

uint8_t renderer_module_init(void *(*get_proc_address_func)(const char *))
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] initializing glad\n");
    IS_NULL(get_proc_address_func, RENDERER_ERR_INVALARG, "RENDERER");
    RET_ON_FAIL(gladLoadGLLoader(get_proc_address_func), 0,
                RENDERER_ERR_GLADFAIL, "RENDERER");

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_create(renderer **result)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] creating renderer");
    IS_NULL(result, RENDERER_ERR_INVALARG, "RENDERER");

    renderer *_result = malloc(sizeof(renderer));
    IS_NULL(_result, RENDERER_ERR_ALLOC, "RENDERER");

    *result = _result;
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_colorf(renderer *renderer, float r, float g, float b,
                        float a)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    INFO("[RENDERER] setting renderer color from float");

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");

    renderer->r = r;
    renderer->g = g;
    renderer->b = b;
    renderer->a = a;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_colori(renderer *renderer, int r, int g, int b, int a)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] setting renderer color from int");
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");

    renderer->r = (float)r / 255.0f;
    renderer->g = (float)g / 255.0f;
    renderer->b = (float)b / 255.0f;
    renderer->a = (float)a / 255.0f;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_clear(renderer *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(renderer->r, renderer->g, renderer->b, renderer->a);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_set_viewport(int32_t x, int32_t y, int32_t width,
                              int32_t height)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    
    INFO("[RENDERER] setting viewport");
    glViewport(x, y, width, height);

    return exit_code;
}

uint8_t renderer_destroy(renderer *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");
    INFO("[RENDERER] destroying renderer\n");
    free(renderer);

    return RENDERER_NO_ERR;

cleanup:
    return exit_code;
}
