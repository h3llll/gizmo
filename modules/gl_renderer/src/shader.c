#include "libraries.h"
#include RENDERER_SHADER_INCLUDE
#include GLAD_INCLUDE
#include RENDERER_INCLUDE

#define UTIL_IMP
#include "utils.h"

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
    char log[512] = "no err";
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
                      shader_t **result)
{
    uint8_t exit_code = SHADER_NO_ERR;

    uint32_t vert_shader = 0, frag_shader = 0, program = 0;
    char *vert_src = NULL, *frag_src = NULL;
    shader_t *_result = NULL;

    INFO("[RENDERER->SHADER] creating shader");
    IS_NULL(vert_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]",
            "\'vert_path\' argument is NULL");
    IS_NULL(frag_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]",
            "\'frag_path\' argument is NULL");

    // MAKING AN IO MODULE RN
    // TODO
    // io_load_file(vert_path, &vert_src);
    // io_load_file(vert_path, &frag_src);

    _result = malloc(sizeof(shader_t));
    IS_NULL(_result, SHADER_ERR_ALLOC, "RENDERER->SHADER",
            "malloc failed");

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

uint8_t shader_create_fallback(shader_t **result)
{
    uint8_t exit_code = SHADER_NO_ERR;

    uint32_t vert_shader = 0, frag_shader = 0, program = 0;
    shader_t *_result = NULL;

    INFO("[RENDERER->SHADER] creating shader");

    _result = malloc(sizeof(shader_t));
    IS_NULL(_result, SHADER_ERR_ALLOC, "RENDERER->SHADER",
            "malloc failed");

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

uint8_t shader_use(shader_t *shader)
{
    uint8_t exit_code = SHADER_NO_ERR;
    IS_NULL(shader, SHADER_ERR_INVALARG, "RENDERER->SHADER",
            "\'shader\' argument is NULL");
    glUseProgram(shader->prog);
    return exit_code;

cleanup:
    return exit_code;
}

uint8_t shader_strip(shader_t *shader)
{
    uint8_t exit_code = SHADER_NO_ERR;
    IS_NULL(shader, SHADER_ERR_INVALARG, "RENDERER->SHADER",
            "\'shader\' argument is NULL");

    glDeleteShader(shader->vert);
    glDeleteShader(shader->frag);

    shader->vert = 0;
    shader->frag = 0;

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t shader_destroy(shader_t **shader)
{
    uint8_t exit_code = SHADER_NO_ERR;

    IS_NULL(shader, SHADER_ERR_INVALARG, "RENDERER->SHADER",
            "\'shader\' argument is NULL");

    shader_t *_shader = *shader;
    IS_NULL(_shader, SHADER_ERR_INVALARG, "RENDERER->SHADER",
            "\'shader\' argument is a pointer to NULL");

    glDeleteShader(_shader->vert);
    glDeleteShader(_shader->frag);
    glDeleteProgram(_shader->prog);

    _shader->vert = 0;
    _shader->frag = 0;
    _shader->prog = 0;

    FREE(*shader, free);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t shader_uniform_vec2f(shader_t *shader, const char *name, float x,
                             float y)
{
    int exit_code = RENDERER_NO_ERR;
    IS_NULL(shader, RENDERER_ERR_INVALARG, "RENDERER->SHADER", "\'shader\' argument is NULL");
    IS_NULL(name, RENDERER_ERR_INVALARG, "RENDERER->SHADER", "\'name\' argument is NULL");

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
