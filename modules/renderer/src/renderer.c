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

uint8_t renderer_create(renderer **result, const char *vert_path,
                        const char *frag_path)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] creating renderer");
    
    char *vert_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    char *frag_src = "";
    renderer *_result = NULL;
    uint32_t VBO, vert_shader;

    if (vert_path == NULL)
    {
        INFO("[RENDERER] no vert_path was given, using builtin");
    }
    else 
    {
        read_file(vert_path, vert_src);
    }

    if (frag_path == NULL)
    {
        INFO("[RENDERER] no frag_path was given, using builtin");
    }
    else
    {
        read_file(frag_path, frag_src);
    }

    IS_NULL(result, RENDERER_ERR_INVALARG, "RENDERER");

    _result = malloc(sizeof(renderer));
    IS_NULL(_result, RENDERER_ERR_ALLOC, "RENDERER");

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    

    _result->r = 0.0f;
    _result->g = 0.0f;
    _result->b = 0.0f;
    _result->a = 1.0f;
    _result->VBO = VBO;

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

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t renderer_draw_rect(renderer *renderer, int32_t x, int32_t y,
                           int32_t width, int32_t height)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    // send verticies and indicies to renderer's vertex and index arrays
cleanup:
    return exit_code;
}

uint8_t renderer_draw_begin(renderer *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    // clear all past frame data, vertex array and everything
cleanup:
    return exit_code;
}

uint8_t renderer_draw_end(renderer *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    // buffer all data to opengl
cleanup:
    return exit_code;
}
