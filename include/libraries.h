#ifndef LIBRARIES_H
#define LIBRARIES_H

#define GL_INCLUDE "renderer/GL_renderer.h"
// #define VK_INCLUDE 

#ifdef USE_GLFW
#define WINDOW_INCLUDE "window/glfw_window.h"
#else /* USE_GLFW */
#define WINDOW_INCLUDE "sdl_window/include/window/window.h"
#endif /* USE_GLFW */

//#ifdef USE_GL 
// NOTE: still unused.
// just gl for now
#define RENDERER_INCLUDE "gl_renderer/include/renderer/renderer.h"
#define RENDERER_SHADER_INCLUDE "gl_renderer/include/shader.h"
#define GLAD_INCLUDE "gl_renderer/include/glad/glad.h"
//#else /* USE_GL */
//#define RENDERER_INCLUDE "renderer/vk_renderer.h"
//#endif /* USE_GL */

#endif /* LIBRARIES_H */
