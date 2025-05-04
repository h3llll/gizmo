#include "shader.h"
#include "glad/glad.h"
#include "utils.h"

uint8_t shader_compile(uint32_t shader, int32_t type)
{
    uint8_t success;
    char *log;
    // continue pls
    if (type == GL_VERTEX_SHADER)
    {
    }
}

uint8_t shader_create(const char *vert_path, const char *frag_path,
                      shader **result)
{
    uint8_t exit_code = SHADER_NO_ERR;
    INFO("[RENDERER->SHADER] creating shader");
    IS_NULL(vert_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]");
    IS_NULL(frag_path, SHADER_ERR_INVALARG, "[RENDERER->SHADER]");

    const char *const *vert_src, *frag_src;

    // read_file(vert_path, vert_src);
    // read_file(frag_path, frag_src);

    uint32_t vert_shader, frag_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert_shader, 1, vert_src, NULL);
    glShaderSource(frag_shader, 1, frag_src, NULL);

cleanup:
    return exit_code;
}
