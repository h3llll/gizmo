#include "renderer/renderer.h"
#include "dyarr/dyarr.h"
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

static uint8_t setup_gl(renderer *r, const char *vert_path,
                        const char *frag_path)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    uint32_t VBO, VAO, EBO;
    shader *shader = NULL;

    // TODO DON'T FUCKING FORGET THIS SHIT NIGGA
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

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_FRAME_FLOATS * sizeof(float), NULL,
                 GL_STREAM_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 MAX_FRAME_INDICES * sizeof(uint32_t), NULL,
                 GL_STREAM_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vec3 pos = (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // vec2 textcord = (u, v)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)offsetof(vertex, u));
    glEnableVertexAttribArray(1);

    // vec3 norm = (nx, ny, nz)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)offsetof(vertex, nx));
    glEnableVertexAttribArray(2);

    r->VAO = VAO;
    r->VBO = VBO;
    r->EBO = EBO;
    r->shader = shader;

cleanup:
    return exit_code;
}

static uint8_t setup_arr(renderer *r)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    array *vert_arr = NULL;
    array *ind_arr = NULL;

    IS_NULL(r, RENDERER_ERR_INVALARG, "RENDERER(INTERNAL)");

    RET_ON_FAIL(
        array_create(&vert_arr, MAX_FRAME_VERTICES * 0.1, sizeof(vertex)),
        ARR_ERR_ALLOC, RENDERER_ERR_ALLOC, "RENDERER");

    RET_ON_FAIL(
        array_create(&ind_arr, MAX_FRAME_INDICES * 0.1, sizeof(uint32_t)),
        ARR_ERR_ALLOC, RENDERER_ERR_ALLOC, "RENDERER");

    r->vertex_array = vert_arr;
    r->index_array = ind_arr;

    return exit_code;

cleanup:
    array_destroy(vert_arr);
    return exit_code;
}

uint8_t renderer_create(renderer **result, const char *vert_path,
                        const char *frag_path)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] creating renderer");
    renderer *_result = NULL;

    IS_NULL(result, RENDERER_ERR_INVALARG, "RENDERER");

    _result = malloc(sizeof(renderer));
    IS_NULL(_result, RENDERER_ERR_ALLOC, "RENDERER");

    _result->col = (colorf){
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 1,
    };

    RET_ON_FAIL(setup_gl(_result, vert_path, frag_path),
                RENDERER_ERR_ALLOC, RENDERER_ERR_ALLOC,
                "RENDERER->INTERNAL");

    RET_ON_FAIL(setup_arr(_result), RENDERER_ERR_ALLOC, RENDERER_ERR_ALLOC,
                "RENDERER->INTERNAL");

    *result = _result;
    return exit_code;

cleanup:
    renderer_destroy(_result);
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
    glClearColor(renderer->col.r, renderer->col.g, renderer->col.b,
                 renderer->col.a);
    glClear(GL_COLOR_BUFFER_BIT);

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

    FREE(renderer->vertex_array, array_destroy);
    FREE(renderer->index_array, array_destroy);
    FREE(renderer->shader, shader_destroy);

    glDeleteVertexArrays(1, &renderer->VAO);
    glDeleteBuffers(1, &renderer->VBO);
    glDeleteBuffers(1, &renderer->EBO);

    renderer->VAO = 0;
    renderer->VBO = 0;
    renderer->EBO = 0;

    FREE(renderer, free);

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t renderer_draw_rect(renderer *renderer, int32_t x, int32_t y,
                           uint32_t width, uint32_t height)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");
    array *vertices = NULL;

    array_create(); // TODO continue ts

cleanup:
    return exit_code;
}

uint8_t renderer_draw_begin(renderer *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");

    array_reset(renderer->vertex_array);
    array_reset(renderer->index_array);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
    glBindVertexArray(renderer->VAO);

cleanup:
    return exit_code;
}

uint8_t renderer_draw_end(renderer *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER");

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    renderer->vertex_array->count *
                        renderer->vertex_array->item_size,
                    renderer->vertex_array->items);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    renderer->index_array->count *
                        renderer->index_array->item_size,
                    renderer->index_array->items);

    glDrawElements(GL_TRIANGLES, renderer->index_array->count,
                   GL_UNSIGNED_INT, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

cleanup:
    return exit_code;
}
