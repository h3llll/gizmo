#include "libraries.h"
#include RENDERER_INCLUDE
#include GLAD_INCLUDE
#include "dyarr/dyarr.h"
#include RENDERER_SHADER_INCLUDE
#include "utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct colori
{
    int8_t r, g, b, a;
};

struct colorf
{
    float r, g, b, a;
};

struct vertex
{
    float x, y, z;
    float r, g, b, a;
    float u, v;
    float nx, ny, nz;
};

struct camera
{

};

struct renderer
{
    array *vertex_array;
    array *index_array;
    shader_t *shader;
    colorf_t col;
    uint32_t VAO, VBO, EBO;
    int32_t v_width, v_height;

};

void APIENTRY gl_err_callback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message,
                              const void *user_param)
{
    (void)user_param; // satisfying unused_param warning
    (void)id;
    (void)type;
    (void)length;
    (void)source;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        ERR("[RENDERER] OPENGL FAILED: %s", message);
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        WARN("[RENDERER] OPENGL FAILED: %s", message);
        break;

    default:
        INFO("[RENDERER] OPENGL : %s", message);
        break;
    }
}

char *renderer_err_str(uint8_t code)
{
    switch (code)
    {
    case RENDERER_ERR_INVALARG:
        return "NULL argument was passed";
        break;

    case RENDERER_ERR_ALLOC:
        return "memory allocation failed";
        break;

    case RENDERER_ERR_GLADFAIL:
        return "GLAD failed to load";
        break;

    case RENDERER_NO_ERR:
        return "success";
        break;

    default:
        return "invalid error code";
        break;
    }
}

uint8_t renderer_module_init(void *(*get_proc_address_func)(const char *))
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] initializing glad\n");
    IS_NULL(get_proc_address_func, RENDERER_ERR_INVALARG, "RENDERER",
            "get_proc_address_func\'");
    RET_ON_FAIL(gladLoadGLLoader(get_proc_address_func),
                0, // GLAD returns 0 on fail idk why
                RENDERER_ERR_GLADFAIL, "RENDERER");

    const char *version = (const char *)glGetString(GL_VERSION);
    if (version)
    {
        INFO("[RENDERER] OPENGL: version: %s", version);
    }
    else
    {
        ERR("[RENDERER] OPENGL FAILED: failed to retrieve gl version");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_err_callback, NULL);

    return exit_code;

cleanup:
    return exit_code;
}

static uint8_t setup_gl(renderer_t *r, const char *vert_path,
                        const char *frag_path)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    uint32_t VBO, VAO, EBO;
    shader_t *shader = NULL;

    if (shader_create(vert_path, frag_path, &shader) > 0)
    {
        WARN("[RENDERER->SHADER] couldn't create shader, using fallback");
        RET_ON_FAIL(shader_create_fallback(&shader),
                    SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                    "RENDERER->SHADER");
    }
    shader_use(shader);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_FRAME_FLOATS * sizeof(float), NULL,
                 GL_STREAM_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 MAX_FRAME_INDICES * sizeof(uint32_t), NULL,
                 GL_STREAM_DRAW);

    // vec3 pos = (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // vec4 col = (r, g, b, a)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                          (void *)offsetof(vertex_t, r));
    glEnableVertexAttribArray(1);

    // vec2 textcord = (u, v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                          (void *)offsetof(vertex_t, u));
    glEnableVertexAttribArray(2);

    // vec3 norm = (nx, ny, nz)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                          (void *)offsetof(vertex_t, nx));
    glEnableVertexAttribArray(3);

    r->VAO = VAO;
    r->VBO = VBO;
    r->EBO = EBO;

    r->shader = shader;

cleanup:
    return exit_code;
}

static uint8_t setup_arr(renderer_t *r)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    array *vert_arr = NULL;
    array *ind_arr = NULL;

    IS_NULL(r, RENDERER_ERR_INVALARG, "RENDERER->INTERNAL",
            "UNDEFINED BEHAVOUR: renderer pointer passed to setup_arr is "
            "NULL");

    uint8_t arr_ret_code = array_create(
        &vert_arr, MAX_FRAME_VERTICES * 0.5, sizeof(vertex_t));

    if (arr_ret_code != ARR_NO_ERR)
    {
        ERR("[RENDERER] failed to create vertex array");
    }
    arr_ret_code =
        array_create(&ind_arr, MAX_FRAME_VERTICES * 0.5, sizeof(vertex_t));

    if (arr_ret_code != ARR_NO_ERR)
    {
        ERR("[RENDERER] failed to create index array");
    }

    r->vertex_array = vert_arr;
    r->index_array = ind_arr;

    return exit_code;

cleanup:
    FREE_PTR(&vert_arr, array_destroy);
    FREE_PTR(&ind_arr, array_destroy);
    return exit_code;
}

uint8_t renderer_create(renderer_t **result, const char *vert_path,
                        const char *frag_path)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    INFO("[RENDERER] creating renderer");
    renderer_t *_result = NULL;

    IS_NULL(result, RENDERER_ERR_INVALARG, "RENDERER",
            "\'result\' argument is NULL");

    _result = malloc(sizeof(renderer_t));
    IS_NULL(_result, RENDERER_ERR_ALLOC, "RENDERER", "malloc failed");

    _result->col = (colorf_t){
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 1,
    };

    uint8_t init_ret_code = setup_gl(_result, vert_path, frag_path);
    if (init_ret_code != RENDERER_NO_ERR)
    {
        ERR("[RENDERER] renderer object creation failed: GL initiation: "
            "%s",
            renderer_err_str(init_ret_code));
    }

    init_ret_code = setup_arr(_result);
    if (init_ret_code != RENDERER_NO_ERR)
    {
        ERR("[RENDERER] renderer object creation failed: array "
            "initiation: "
            "%s",
            renderer_err_str(init_ret_code));
    }

    *result = _result;
    return exit_code;

cleanup:
    FREE_PTR(&_result, renderer_destroy);
    return exit_code;
}

uint8_t renderer_colorf(renderer_t *renderer, float r, float g, float b,
                        float a)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER",
            "\'renderer\' argument is NULL");

    renderer->col.r = r;
    renderer->col.g = g;
    renderer->col.b = b;
    renderer->col.a = a;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_colori(renderer_t *renderer, uint8_t r, uint8_t g,
                        uint8_t b, uint8_t a)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER",
            "\'renderer\' argument is NULL");

    renderer->col.r = (float)r / 255.0f;
    renderer->col.g = (float)g / 255.0f;
    renderer->col.b = (float)b / 255.0f;
    renderer->col.a = (float)a / 255.0f;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_clear(renderer_t *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER",
            "\'renderer\' argument is NULL");
    glClearColor(renderer->col.r, renderer->col.g, renderer->col.b,
                 renderer->col.a);
    glClear(GL_COLOR_BUFFER_BIT);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t renderer_set_viewport(renderer_t *renderer, int32_t x, int32_t y,
                              int32_t width, int32_t height)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    renderer->v_width = width;
    renderer->v_height = height;
    shader_uniform_vec2f(renderer->shader, "viewport", width, height);
    glViewport(x, y, width, height);

    return exit_code;
}

uint8_t renderer_destroy(renderer_t **renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER", "\'renderer\' pointer is NULL");

    renderer_t *renderer_ptr = *renderer;
    IS_NULL(renderer_ptr, RENDERER_ERR_INVALARG, "RENDERER", "\'renderer\' is a pointer to NULL");

    INFO("[RENDERER] destroying renderer\n");

    FREE_PTR(&renderer_ptr->vertex_array, array_destroy);
    FREE_PTR(&renderer_ptr->index_array, array_destroy);

    FREE_PTR(&renderer_ptr->shader, shader_destroy);

    glDeleteVertexArrays(1, &renderer_ptr->VAO);
    glDeleteBuffers(1, &renderer_ptr->VBO);
    glDeleteBuffers(1, &renderer_ptr->EBO);

    renderer_ptr->VAO = 0;
    renderer_ptr->VBO = 0;
    renderer_ptr->EBO = 0;

    FREE(renderer_ptr, free);

    return exit_code;
cleanup:
    return exit_code;
}

uint8_t renderer_draw_rect(renderer_t *renderer, int32_t x, int32_t y,
                           uint32_t width, uint32_t height)
{
    uint8_t exit_code = RENDERER_NO_ERR;

    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER", "\'renderer\' argument is NULL");

    float r = renderer->col.r;
    float g = renderer->col.g;
    float b = renderer->col.b;
    float a = renderer->col.a;

    float x0 = x;
    float y0 = y;
    float x1 = x + width;
    float y1 = y + height;

    vertex_t verts[] = {
        {x0, y0, 0, r, g, b, a, 0, 0, 0, 0, 0},
        {x1, y0, 0, r, g, b, a, 0, 0, 0, 0, 0},
        {x1, y1, 0, r, g, b, a, 0, 0, 0, 0, 0},
        {x0, y1, 0, r, g, b, a, 0, 0, 0, 0, 0},
    };

    uint32_t inds[] = {0, 1, 2, 2, 3, 0};

    for (size_t i = 0; i < 4; i++)
    {
        //array_put(renderer->vertex_array, &verts[i], sizeof(vertex_t));
    }

    size_t base_index = renderer->vertex_array->count - 4;

    for (size_t i = 0; i < 6; i++)
    {
        uint32_t adjusted_index = base_index + inds[i];
        //array_put(renderer->index_array, &adjusted_index,
        //         sizeof(uint32_t));
    }

cleanup:
    return exit_code;
}

uint8_t renderer_draw_begin(renderer_t *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER", "\'renderer\' argument is NULL");

    array_reset(renderer->vertex_array);
    array_reset(renderer->index_array);

    glBindVertexArray(renderer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);

cleanup:
    return exit_code;
}

uint8_t renderer_draw_end(renderer_t *renderer)
{
    uint8_t exit_code = RENDERER_NO_ERR;
    IS_NULL(renderer, RENDERER_ERR_INVALARG, "RENDERER", "\'renderer\' argument is NULL");

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    renderer->vertex_array->count *
                        renderer->vertex_array->item_size,
                    renderer->vertex_array->items);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    renderer->index_array->count *
                        renderer->index_array->item_size,
                    renderer->index_array->items);

    shader_use(renderer->shader);
    glFinish();
    glDrawElements(GL_TRIANGLES, renderer->index_array->count,
                   GL_UNSIGNED_INT, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

cleanup:
    return exit_code;
}
