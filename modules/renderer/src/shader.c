#include "shader.h"
#include "glad/glad.h"
#include "renderer/renderer.h"

#define UTIL_IMP
#include "utils.h"

// TODO : PUT IN THE VERTEX SHADER !! CRITICAL !!! IMPORTANT !!! CRAZY
// !!!!! NEEDED !!! NOTE !!!!
#define TO_NDC_X(px, screen_w) (((float)(px) / screen_w) * 2.0f - 1.0f)

#define TO_NDC_Y(py, screen_h) (1.0f - ((float)(py) / screen_h) * 2.0f)

static const char *base_vert_sh =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec4 aCol;\n"
    "layout(location = 2) in vec2 aUv;\n"
    "layout(location = 3) in vec3 aNorm;\n"

    "out vec2 vUV;\n"
    "out vec4 col;\n"

    "uniform vec2 viewport;\n"

    "void main()\n"
    "{\n" 
    "float ndc_x = (aPos.x / viewport.x) * 2.0 - 1.0;\n"
    "float ndc_y = 1.0 - (aPos.y / viewport.y) * 2.0;\n"
    "gl_Position = vec4(ndc_x, ndc_y, aPos.z, 1.0);\n"
    "col = aCol;\n"
    "}\n";

static const char *base_frag_sh = "#version 330 core\n"
                                  "in vec2 vUv;\n"
                                  "in vec4 col;\n"
                                  "out vec4 FragColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "FragColor = col;\n"
                                  "}\n";

uint8_t shader_compile(uint32_t shader, int32_t type)
{
    int32_t success;
    char log[512] = {'n', 'o', ' ', 'e', 'r', 'r', 'o', 'r', '\n'};
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        switch (type)
        {
        case GL_VERTEX_SHADER:
            ERR("[RENDERER->SHADER] failed to compile vertex shader:\n %s",
                log);

            break;
        case GL_FRAGMENT_SHADER:
            ERR("[RENDERER->SHADER] failed to compile fragment shader:\n"
                "%s",
                log);

            break;
        }

        return SHADER_ERR_COMPILATION;
    }

    return SHADER_NO_ERR;
}

uint8_t shader_create(const char *vert_path, const char *frag_path,
                      shader **result)
{
    uint8_t exit_code = SHADER_NO_ERR;

    uint32_t vert_shader = 0, frag_shader = 0, program = 0;
    char *vert_src = NULL, *frag_src = NULL;
    shader *_result = NULL;

    INFO("[RENDERER->SHADER] creating shader");
    IS_NULL(vert_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]");
    IS_NULL(frag_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]");

    RET_ON_FAIL(read_file(vert_path, &vert_src), UTIL_ERR_IO,
                SHADER_ERR_IO, "RENDERER->SHADER");
    RET_ON_FAIL(read_file(frag_path, &frag_src), UTIL_ERR_IO,
                SHADER_ERR_IO, "RENDERER->SHADER");

    _result = malloc(sizeof(shader));
    IS_NULL(_result, SHADER_ERR_ALLOC, "RENDERER->SHADER");

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    glShaderSource(vert_shader, 1, (const char *const *)&vert_src, NULL);
    glShaderSource(frag_shader, 1, (const char *const *)&frag_src, NULL);

    RET_ON_FAIL(shader_compile(vert_shader, GL_VERTEX_SHADER),
                SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                "RENDERER->SHADER");

    RET_ON_FAIL(shader_compile(frag_shader, GL_FRAGMENT_SHADER),
                SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                "RENDERER->SHADER");

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    int32_t success;
    char log[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        ERR("[RENDERER->SHADER] failed to link program:\n%s", log);
        exit_code = SHADER_ERR_COMPILATION;
    }

    _result->prog = program;
    _result->vert = vert_shader;
    _result->frag = frag_shader;

    *result = _result;

    FREE(vert_src, free);
    FREE(frag_src, free);

    return exit_code;

cleanup:
    if (vert_shader != 0)
    {
        glDeleteShader(vert_shader);
    }
    if (frag_shader != 0)
    {
        glDeleteShader(frag_shader);
    }
    FREE(vert_src, free);
    FREE(frag_src, free);
    return exit_code;
}

uint8_t shader_create_fallback(shader **result)
{
    uint8_t exit_code = SHADER_NO_ERR;

    uint32_t vert_shader = 0, frag_shader = 0, program = 0;
    shader *_result = NULL;

    INFO("[RENDERER->SHADER] creating shader");

    _result = malloc(sizeof(shader));
    IS_NULL(_result, SHADER_ERR_ALLOC, "RENDERER->SHADER");

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    glShaderSource(vert_shader, 1, (const char *const *)&base_vert_sh,
                   NULL);
    glShaderSource(frag_shader, 1, (const char *const *)&base_frag_sh,
                   NULL);

    RET_ON_FAIL(shader_compile(vert_shader, GL_VERTEX_SHADER),
                SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                "RENDERER->SHADER");

    RET_ON_FAIL(shader_compile(frag_shader, GL_FRAGMENT_SHADER),
                SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                "RENDERER->SHADER");

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    int32_t success;
    char log[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        ERR("[RENDERER->SHADER] failed to link program:\n%s", log);
        exit_code = SHADER_ERR_COMPILATION;
    }

    _result->prog = program;
    _result->vert = vert_shader;
    _result->frag = frag_shader;

    *result = _result;

    return exit_code;

cleanup:
    if (vert_shader != 0)
    {
        glDeleteShader(vert_shader);
    }
    if (frag_shader != 0)
    {
        glDeleteShader(frag_shader);
    }
    return exit_code;
}

uint8_t shader_use(shader *shader)
{
    uint8_t exit_code = SHADER_NO_ERR;
    IS_NULL(shader, SHADER_ERR_INVALARG, "RENDERER->SHADER");
    glUseProgram(shader->prog);
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t shader_strip(shader *shader)
{
    uint8_t exit_code = SHADER_NO_ERR;
    IS_NULL(shader, SHADER_ERR_INVALARG, "RENDERER->SHADER");

    glDeleteShader(shader->vert);
    glDeleteShader(shader->frag);

    shader->vert = 0;
    shader->frag = 0;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t shader_destroy(shader *shader)
{
    uint8_t exit_code = SHADER_NO_ERR;
    IS_NULL(shader, SHADER_ERR_INVALARG, "RENDERER->SHADER");

    glDeleteShader(shader->vert);
    glDeleteShader(shader->frag);
    glDeleteProgram(shader->prog);

    shader->vert = 0;
    shader->frag = 0;
    shader->prog = 0;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t shader_uniform_vec2f(shader *shader, const char *name, float x,
                             float y)
{
    int exit_code = RENDERER_NO_ERR;
    IS_NULL(shader, RENDERER_ERR_INVALARG, "RENDERER");
    int32_t uniform_loc = glGetUniformLocation(shader->prog, name);

    if (uniform_loc == -1)
    {
        ERR("[RENDERER->SHADER] FAILED TO LOCATE VIEWPORT UNIFORM SHADER "
            "%s",
            name);
        exit_code = SHADER_ERR_INTERNAL;
        goto cleanup;
    }
    glUniform2f(uniform_loc, x, y);

    return exit_code;
cleanup:
    return exit_code;
}
