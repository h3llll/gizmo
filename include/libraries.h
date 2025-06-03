#ifndef LIBRARIES_H
#define LIBRARIES_H

// ----------- WINDOW

#ifdef USE_GLFW
#define WINDOW_INCLUDE "window/glfw_window.h"
#else /* USE_GLFW */
#define WINDOW_INCLUDE "sdl_window/include/window/window.h"
#endif /* USE_GLFW */

// ----------- RENDERER

//#ifdef USE_GL 
// NOTE: still unused.
// just gl for now
#define RENDERER_INCLUDE "gl_renderer/include/renderer/renderer.h"
#define RENDERER_SHADER_INCLUDE "gl_renderer/include/shader.h"
#define GLAD_INCLUDE "gl_renderer/include/glad/glad.h"
//#else /* USE_GL */
//#define RENDERER_INCLUDE "renderer/vk_renderer.h"
//#endif /* USE_GL */

// ----------- EVENT
#define EVENT_INCLUDE "event/event.h"

// ----------- DYARR
#define DYARR_INCLUDE "dyarr/dyarr.h"


#endif /* LIBRARIES_H */
