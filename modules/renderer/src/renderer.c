#include "renderer/renderer.h"
#include "glad/glad.h"
#include "shader.h"
#include "utils.h"
#include <stddef.h>
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

    IS_NULL(result, RENDERER_ERR_INVALARG, "RENDERER");

    vertex *vert_arr = NULL;
    uint32_t ind_arr = NULL;
    shader *shader = NULL;
    renderer *_result = NULL;
    uint32_t VBO, VAO;

    if (vert_path == NULL)
    {
        INFO("[RENDERER] no vert_path was given, using builtin(wip)");
    }
    if (frag_path == NULL)
    {
        INFO("[RENDERER] no frag_path was given, using builtin(wip)");
    }

    RET_ON_FAIL(shader_create(vert_path, frag_path, &shader),
                SHADER_ERR_ALLOC, RENDERER_ERR_ALLOC, "RENDERER");
    shader_use(shader);

    _result = malloc(sizeof(renderer));
    IS_NULL(_result, RENDERER_ERR_ALLOC, "RENDERER");

    vert_arr = malloc(MAX_FRAME_FLOATS * sizeof(float));
    ind_arr = malloc(MAX_FRAME_INDICES * sizeof(uint32_t));

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    glEnableVertexAttribArray(0);

    _result->col = (colorf){
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 1,
    };

    _result->VAO = VAO;
    _result->VBO = VBO;

    _result->vertex_array = vert_arr;
    

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

    renderer->col.r = r;
    renderer->col.g = g;
    renderer->col.b = b;
    renderer->col.a = a;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_colori(renderer *renderer, int r, int g, int b, int a)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] setting renderer color from int");
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");

    renderer->col.r = (float)r / 255.0f;
    renderer->col.g = (float)g / 255.0f;
    renderer->col.b = (float)b / 255.0f;
    renderer->col.a = (float)a / 255.0f;

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
