#include "shader.h"
#include "glad/glad.h"
#include "utils.h"

uint8_t shader_compile(uint32_t shader, int32_t type)
{
    int32_t success;
    char log[512];
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

    RET_ON_FAIL(read_file(vert_path, &vert_src), UTIL_ERR_ALLOC,
                SHADER_ERR_ALLOC, "RENDERER->SHADER");
    RET_ON_FAIL(read_file(frag_path, &frag_src), UTIL_ERR_ALLOC,
                SHADER_ERR_ALLOC, "RENDERER->SHADER");

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
