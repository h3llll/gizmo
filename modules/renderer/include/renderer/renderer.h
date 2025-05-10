#ifndef MODULES_RENDERER_H
#define MODULES_RENDERER_H

// clang-format off
#include "dyarr/dyarr.h"
#define RENDERER_NO_ERR         0 // no error
#define RENDERER_ERR_ALLOC      1 // error allocating memory
#define RENDERER_ERR_GLADFAIL   2 // glad error occured
#define RENDERER_ERR_INVALARG   3 // null/bad argument

#define FLOATS_PER_VERTEX    11
#define MAX_FRAME_VERTICES  1000000
#define MAX_FRAME_FLOATS    (FLOATS_PER_VERTEX * MAX_FRAME_VERTICES)
#define MAX_FRAME_INDICES  (MAX_FRAME_VERTICES * 3)

#define TO_NDC_X(px, screen_w)\
    (((float)(px) / screen_w) * 2.0f - 1.0f)

#define TO_NDC_Y(py, screen_h)\
    (1.0f - ((float)(py) / screen_h) * 2.0f)

// clang-format on

#include "shader.h"
#include <stdint.h>

// Int representation of color, tiny bit slower to use since it is devided
// by 255 passing through renderer_colori
typedef struct colori
{
    int8_t r, g, b, a;
} colori;

// Float representation of color, tiny bit faster to use
// since there's no division needed to transform
typedef struct colorf
{
    float r, g, b, a;
} colorf;

typedef struct vertex
{
    float x, y, z;    // pos
    float r, g, b, a; // col
    float u, v;       // text
    float nx, ny, nz; // norm
} vertex;

// Holds renderer information such as color and buffer objects.
typedef struct renderer
{
    array *vertex_array;
    array *index_array;
    shader *shader;
    colorf col;
    uint32_t VAO, VBO, EBO;
    int32_t v_width, v_height;

} renderer;
/**
 * Initializes gl loader with the procedure address getter argument.
 * Returns RENDERER_NO_ERR on success, 0< on failure.
 *
 * Important note : THIS WILL FAIL IF NO WINDOW WAS CREATED BEFOREHAND
 * IF IT RETURNS RENDERER_ERR_GLADFAIL CHECK THE FOLLOWING :
 * 1 - if you manually sat the context major and minor versions make sure
 * they match with the provided glad headers
 *
 * 2 - check if a window is created beforehand and if there's a valid gl
 * context
 *
 * 3 - check if the provided glad headers include extra extensions
 *
 * 4 - check if your drivers are compatible with given glad version
 */
uint8_t renderer_module_init(void *(*get_proc_address_func)(const char *));

/**
 * Initializes a renderer to the result pointer.
 * Returns RENDERER_NO_ERR on success, 0< on failure.
 */
uint8_t renderer_create(renderer **result, const char *vert_path,
                        const char *frag_path);

/**
 * Sets the renderer's colors to the specified channel values.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_colori(renderer *renderer, int r, int g, int b, int a);

/**
 * Sets the renderer's colors to the specified channel values.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_colorf(renderer *renderer, float r, float g, float b,
                        float a);

/**
 * Clears the screen with the renderer's color.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_clear(renderer *renderer);

/**
 * Sets renderer viewport.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_set_viewport(int32_t x, int32_t y, int32_t width,
                              int32_t height);

// Binds GPU buffers and objects and resets vertex/index arrays.
// Returns RENDERER_NO_ERR on success, 0< otherwise.
uint8_t renderer_draw_begin(renderer *renderer);

// Puts rectangle shaped vertices in target renderer's vertex/index array
// Returns RENDERER_NO_ERR on success, 0< otherwise.
uint8_t renderer_draw_rect(renderer *renderer, int32_t x, int32_t y,
                           uint32_t width, uint32_t height);

// Resets GPU buffers and objects after buffering vertex/index array
// contents to the GPU via said buffers then drawing.
// Returns RENDERER_NO_ERR on success, 0< otherwise.
uint8_t renderer_draw_end(renderer *renderer);

/**
 * Frees the renderer's memory.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_destroy(renderer *renderer);

#endif
