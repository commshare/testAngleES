#include "gl_utils.h"
#include "gl_scene.h"

const char * GetGLErrorString(GLenum error)
{
  const char *str;
  switch (error)
  {
    case GL_NO_ERROR:
      str = "GL_NO_ERROR";
      break;
    case GL_INVALID_ENUM:
      str = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      str = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      str = "GL_INVALID_OPERATION";
      break;
#if defined __gl_h_ || defined __gl3_h_
    case GL_OUT_OF_MEMORY:
      str = "GL_OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      str = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
#endif
#if defined __gl_h_
    case GL_STACK_OVERFLOW:
      str = "GL_STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      str = "GL_STACK_UNDERFLOW";
      break;
    case GL_TABLE_TOO_LARGE:
      str = "GL_TABLE_TOO_LARGE";
      break;
#endif
    default:
      str = "(ERROR: Unknown Error Enum)";
      break;
  }
  return str;
}

const char* strVertexType(GLenum type)
{
  if (type == GL_VERTEX_SHADER) {
    return "vertex";
  } else if (type == GL_FRAGMENT_SHADER) {
    return "fragment";
  }
  return "unkown type";
}

void maskStencil(const glm::mat4 &global_trans, GloveShapeBase *shape, GLint ref)
{
  glStencilFunc(GL_NEVER, ref, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
  glStencilMask(0xFF);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  shape->preRender(global_trans);
  shape->render(global_trans);
  shape->postRender(global_trans);
  glStencilMask(0x00);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  //glStencilFunc(GL_EQUAL, 1, 0xFF);
}

#if GLOVE_ENABLE_THREAD_AFFINITY_CHECK
std::thread::id g_glove_affinity_thread;
#endif

#ifdef __ANDROID__
void eglPrintConfig(EGLDisplay display, EGLConfig config)
{
    const int attributes[] = {
        EGL_BUFFER_SIZE,
        EGL_ALPHA_SIZE,
        EGL_BLUE_SIZE,
        EGL_GREEN_SIZE,
        EGL_RED_SIZE,
        EGL_DEPTH_SIZE,
        EGL_STENCIL_SIZE,
        EGL_CONFIG_CAVEAT,
        EGL_CONFIG_ID,
        EGL_LEVEL,
        EGL_MAX_PBUFFER_HEIGHT,
        EGL_MAX_PBUFFER_PIXELS,
        EGL_MAX_PBUFFER_WIDTH,
        EGL_NATIVE_RENDERABLE,
        EGL_NATIVE_VISUAL_ID,
        EGL_NATIVE_VISUAL_TYPE,
        0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
        EGL_SAMPLES,
        EGL_SAMPLE_BUFFERS,
        EGL_SURFACE_TYPE,
        EGL_TRANSPARENT_TYPE,
        EGL_TRANSPARENT_RED_VALUE,
        EGL_TRANSPARENT_GREEN_VALUE,
        EGL_TRANSPARENT_BLUE_VALUE,
        0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
        0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
        0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
        0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
        EGL_LUMINANCE_SIZE,
        EGL_ALPHA_MASK_SIZE,
        EGL_COLOR_BUFFER_TYPE,
        EGL_RENDERABLE_TYPE,
        0x3042 // EGL10.EGL_CONFORMANT
    };
    const char* names[] = {
        "EGL_BUFFER_SIZE",
        "EGL_ALPHA_SIZE",
        "EGL_BLUE_SIZE",
        "EGL_GREEN_SIZE",
        "EGL_RED_SIZE",
        "EGL_DEPTH_SIZE",
        "EGL_STENCIL_SIZE",
        "EGL_CONFIG_CAVEAT",
        "EGL_CONFIG_ID",
        "EGL_LEVEL",
        "EGL_MAX_PBUFFER_HEIGHT",
        "EGL_MAX_PBUFFER_PIXELS",
        "EGL_MAX_PBUFFER_WIDTH",
        "EGL_NATIVE_RENDERABLE",
        "EGL_NATIVE_VISUAL_ID",
        "EGL_NATIVE_VISUAL_TYPE",
        "EGL_PRESERVED_RESOURCES",
        "EGL_SAMPLES",
        "EGL_SAMPLE_BUFFERS",
        "EGL_SURFACE_TYPE",
        "EGL_TRANSPARENT_TYPE",
        "EGL_TRANSPARENT_RED_VALUE",
        "EGL_TRANSPARENT_GREEN_VALUE",
        "EGL_TRANSPARENT_BLUE_VALUE",
        "EGL_BIND_TO_TEXTURE_RGB",
        "EGL_BIND_TO_TEXTURE_RGBA",
        "EGL_MIN_SWAP_INTERVAL",
        "EGL_MAX_SWAP_INTERVAL",
        "EGL_LUMINANCE_SIZE",
        "EGL_ALPHA_MASK_SIZE",
        "EGL_COLOR_BUFFER_TYPE",
        "EGL_RENDERABLE_TYPE",
        "EGL_CONFORMANT"
    };
    int value = 0;
    for (int i = 0; i < sizeof(attributes)/sizeof(int); i++) {
        int attribute = attributes[i];
        const char* name = names[i];
        if (eglGetConfigAttrib(display, config, attribute, &value)) {
            LOGI("  %s: %d", name, value);
        } else {
            while (eglGetError() != EGL_SUCCESS);
        }
    }
}

const char *eglGetErrorString(EGLint error_code)
{
  switch(error_code) {
    case EGL_NOT_INITIALIZED:
      return "EGL_NOT_INITIALIZED: EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
    case EGL_BAD_ACCESS:
      return "EGL_BAD_ACCESS: EGL cannot access a requested resource (for example a context is bound in another thread)";
    case EGL_BAD_ALLOC:
      return "EGL_BAD_ALLOC: EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE:
      return "EGL_BAD_ATTRIBUTE: An unrecognized attribute or attribute value was passed in the attribute list.";
    case EGL_BAD_CONTEXT:
      return "EGL_BAD_CONTEXT: An EGLContext argument does not name a valid EGL rendering context.";
    case EGL_BAD_CONFIG:
      return "EGL_BAD_CONFIG: An EGLConfig argument does not name a valid EGL frame buffer configuration.";
    case EGL_BAD_CURRENT_SURFACE:
      return "EGL_BAD_CURRENT_SURFACE: The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY:
      return "EGL_BAD_DISPLAY: An EGLDisplay argument does not name a valid EGL display connection.";
    case EGL_BAD_SURFACE:
      return "EGL_BAD_SURFACE: An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH:
      return "EGL_BAD_MATCH: Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER:
      return "EGL_BAD_PARAMETER: One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP:
      return "EGL_BAD_NATIVE_PIXMAP: A NativePixmapType argument does not refer to a valid native pixmap.";
    case EGL_BAD_NATIVE_WINDOW:
      return "EGL_BAD_NATIVE_WINDOW: A NativeWindowType argument does not refer to a valid native window.";
    case EGL_CONTEXT_LOST:
      return "EGL_CONTEXT_LOST: A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
    default:
      return "unknown EGL error";
  }
}

void glDummyBindVertexArrayOES (GLuint array) {}
void glDummyDeleteVertexArraysOES (GLsizei n, const GLuint *arrays) {}
void glDummyGenVertexArraysOES (GLsizei n, GLuint *arrays) {}
GLboolean glDummyIsVertexArrayOES (GLuint array) {return false;}

PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES = &glDummyGenVertexArraysOES;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES = &glDummyBindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES = &glDummyDeleteVertexArraysOES;
PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES = &glDummyIsVertexArrayOES;

void loadVAOFunctions() {
  auto gva = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress ( "glGenVertexArraysOES" );
  if (gva != nullptr) {
    LOGD("device has glGenVertexArraysOES supported!");
    glGenVertexArraysOES = gva;
  }
  auto bva = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress ( "glBindVertexArrayOES" );
  if (bva != nullptr) {
    LOGD("device has glBindVertexArrayOES supported!");
    glBindVertexArrayOES = bva;
  }
  auto dva = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress ( "glDeleteVertexArraysOES" );
  if (dva != nullptr) {
    LOGD("device has glDeleteVertexArraysOES supported!");
    glDeleteVertexArraysOES = dva;
  }
  auto iva = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress ( "glIsVertexArrayOES" );
  if (iva != nullptr) {
    LOGD("device has glIsVertexArrayOES supported!");
    glIsVertexArrayOES = iva;
  }
}

#endif //__ANDROID__
