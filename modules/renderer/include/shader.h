#ifndef MODULES_RENDERER_SHADER_H
#define MODULES_RENDERER_SHADER_H

#include <stdint.h>

// clang-format off
#define SHADER_NO_ERR           0
#define SHADER_ERR_INVALARG     1
#define SHADER_ERR_COMPILATION  2
#define SHADER_ERR_ALLOC        3
#define SHADER_ERR_IO           4
#define SHADER_ERR_INTERNAL     5
// clang-format on

typedef struct shader
{
    uint32_t prog, vert, frag;
} shader;

// Returns a shader heap object through result pointer.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_create(const char *vert_path, const char *frag_path,
                      shader **result);

// Returns a shader heap object through result pointer.
// though this version uses hardcoded basic fallback shaders.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_create_fallback(shader **result);

// Attaches shader program.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_use(shader *shader);

// Strips the shaders off of the struct, freeing all shaders.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_strip(shader *shader);

// Uniforms a 2D vector of floats to the shader.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_uniform_vec2f(shader *shader, const char *name, float x,
                             float y);

// Deallocates the GPU shaders if they exist and destroys the program.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_destroy(shader *shader);

#endif // MODULES_RENDERER_SHADER_H
