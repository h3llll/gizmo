#include "shader.h"
#include "glad/glad.h"
#include "utils.h"
#include <GL/glext.h>

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
    INFO("[RENDERER->SHADER] creating shader");
    IS_NULL(vert_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]");
    IS_NULL(frag_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]");

    const char *const *vert_src, *const *frag_src;

    // read_file(vert_path, vert_src);
    // read_file(frag_path, frag_src);

    uint32_t vert_shader, frag_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert_shader, 1, vert_src, NULL);
    glShaderSource(frag_shader, 1, frag_src, NULL);

    RET_ON_FAIL(shader_compile(vert_shader, GL_VERTEX_SHADER),
                SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                "RENDERER->SHADER");
    RET_ON_FAIL(shader_compile(frag_shader, GL_FRAGMENT_SHADER),
                SHADER_ERR_COMPILATION, SHADER_ERR_COMPILATION,
                "RENDERER->SHADER");

cleanup:
    return exit_code;
}
