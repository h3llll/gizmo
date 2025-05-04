#ifndef MODULES_RENDERER_SHADER_H
#define MODULES_RENDERER_SHADER_H

#include <stdint.h>

#define SHADER_NO_ERR       0
#define SHADER_ERR_INVALARG 1

typedef struct shader
{
    uint32_t id;
} shader;

// Returns a shader heap object through result pointer.
// Returns SHADER_NO_ERR on success, 0< otherwise.
uint8_t shader_create(const char *vert_path, const char *frag_path,
                      shader *result);

#endif // MODULES_RENDERER_SHADER_H
