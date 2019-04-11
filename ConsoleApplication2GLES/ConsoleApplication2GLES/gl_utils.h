#ifndef __GLOVE_GL_UTILS_H__
#define __GLOVE_GL_UTILS_H__

#ifdef __APPLE__

#include <TargetConditionals.h>
# if TARGET_OS_IPHONE != 1
#   include <OpenGL/gl2.h>
#define GLOVE_OSX_WORKAROUND

# else //end of OS X

#define glversion 2
#define using_openGL_ES 1

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

# endif //end of iOS

#elif defined(__ANDROID__) //end of __APPLE__

#define glversion 2
#define using_openGL_ES 1

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer

#define GLhalf GLshort

//VAO OES extensions must be load with eglGetProcAddress
extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
extern PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;

void loadVAOFunctions();

#else
#error unsupported platform
#endif

#ifdef using_openGL_ES
#define GLdouble double
#define GL_RED GL_LUMINANCE

//VAO are OES extension in iOS and Android
#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glIsVertexArray glIsVertexArrayOES;

#endif

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#ifdef __ANDROID__

void eglPrintConfig(EGLDisplay display, EGLConfig config);
const char *eglGetErrorString(EGLint error_code);

//for Android
#include <android/log.h>
#define TAG "glove"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)

#elif TARGET_OS_IPHONE != 1
//XXX:why the hell asl is not working in iOS

#include <asl.h>
#define TAG "glove"
#define LOGD(format, ...) asl_log(NULL, NULL, ASL_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LOGI(format, ...) asl_log(NULL, NULL, ASL_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOGW(format, ...) asl_log(NULL, NULL, ASL_LEVEL_WARNING, format, ##__VA_ARGS__)
#define LOGE(format, ...) asl_log(NULL, NULL, ASL_LEVEL_ERR, format, ##__VA_ARGS__)
#define LOGF(format, ...) asl_log(NULL, NULL, ASL_LEVEL_CRIT, format, ##__VA_ARGS__)

#else

#define LOGD(format, ...) printf(format"\n", ##__VA_ARGS__)
#define LOGI(format, ...) printf(format"\n", ##__VA_ARGS__)
#define LOGW(format, ...) printf(format"\n", ##__VA_ARGS__)
#define LOGE(format, ...) printf(format"\n", ##__VA_ARGS__)
#define LOGF(format, ...) printf(format"\n", ##__VA_ARGS__)

#endif

#define GLOVE_DISABLE_COPY(x) \
  x(const x&) = delete; \
  const x& operator=(const x&) = delete


const char * GetGLErrorString(GLenum error);
const char* strVertexType(GLenum type);

#define GLOVE_ENABLE_THREAD_AFFINITY_CHECK 0

#if GLOVE_ENABLE_THREAD_AFFINITY_CHECK

#include <thread>

extern std::thread::id g_glove_affinity_thread;

#define GLOVE_SET_AFFINITY_THREAD() \
  do { \
    auto neo_tid = std::this_thread::get_id();\
    if(neo_tid != g_glove_affinity_thread) { \
      LOGD("glove rendering thread changed, from %x to %x", g_glove_affinity_thread, neo_tid);\
      g_glove_affinity_thread = neo_tid;\
    } \
  } while(0)

#define GLOVE_CHECK_THREAD_AFFINITY(m) \
  do { \
    if(g_glove_affinity_thread != std::this_thread::get_id()) { \
      LOGE("%s called in wrong thread, shall be %x instead of %x", m, g_glove_affinity_thread, std::this_thread::get_id());\
    } \
  } while(0)

#else

#define GLOVE_SET_AFFINITY_THREAD() do{}while(0)
#define GLOVE_CHECK_THREAD_AFFINITY(m) do{}while(0)

#endif //GLOVE_ENABLE_THREAD_AFFINITY_CHECK

#define glcheck_error(x) \
  do { \
    GLOVE_CHECK_THREAD_AFFINITY(#x);\
    x; \
    GLenum glerr = glGetError(); \
    if (glerr != GL_NO_ERROR) { \
      LOGW("[%s:%d]GLError %s: (%d) %s", __FILE__, __LINE__, #x, glerr, GetGLErrorString(glerr)); \
    } \
  } while(0)


#if 1
#define glcheck(x) \
  glcheck_error(x)
#else
#define glcheck(x) \
  do { \
    x; \
  } while(0)
#endif

#define GLOVE_EPSILON_VAL (1e-30f)

#define epsEqual(a, b) \
  glm::epsilonEqual((a), (b), GLOVE_EPSILON_VAL)


//TODO:handle other compiler...
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define GLOVE_LITTLE_ENDIAN
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define GLOVE_BIG_ENDIAN
#else
#error unkown endian or unsupported compiler!
#endif

//from google gears source
#if defined(_MSC_VER) && _MSC_VER >= 1400 && !defined(OS_WINCE)
#include <windows.h>
#else
#define ARRAYSIZE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
#endif

#define CONCAT_HELPER(x, y) x ## y

#define CONCAT(x, y) \
  CONCAT_HELPER(x, y)

#include <atomic>

enum {
  kShaderProgramType,
  kStateMachineEventType,
  kShapeType,
};

template<size_t Cata_>
class SeriesGenerator
{
public:
  static size_t entries() {return current_;}
  static size_t next() {return current_++;}

protected:
  static std::atomic<size_t> current_;
};


template<size_t Cata_>
std::atomic<size_t> SeriesGenerator<Cata_>::current_(0);

template<typename Assignee_, size_t Cata_>
class SeriesAssigner
  : public SeriesGenerator<Cata_>
{
public:
  static const size_t series;
};

template<typename Assignee_, size_t Cata_>
const size_t SeriesAssigner<Assignee_, Cata_>::series = SeriesAssigner<Assignee_, Cata_>::next();

template<size_t ...>
struct index_seq { };

template<size_t N, size_t ...S>
struct gen_index_seq : gen_index_seq<N-1, N-1, S...> { };

template<size_t ...S>
struct gen_index_seq<0, S...> {
    typedef index_seq<S...> type;
};

#include <nano_signal_slot.hpp>

template <typename Ret_, typename ...Args>
struct NanoSignalConnectionHelper
{
  template<typename Class_>
    static Class_* Extract(Ret_(Class_::*mem_fn)(Args...) const);
  template<typename Class_>
    static Class_* Extract(Ret_(Class_::*mem_fn)(Args...));
};


template <typename ...Args>
struct NanoSignalConnectionHelper<void, Args...>
{
    template<typename Class_>
    static Class_* Extract(void(Class_::*mem_fn)(Args...) const);
    template<typename Class_>
    static Class_* Extract(void(Class_::*mem_fn)(Args...));
};


template <typename Ret_, typename ...Args>
NanoSignalConnectionHelper<Ret_, Args...> *NanoSignalArgsExtractor(const Nano::Signal<Ret_(Args...)>&);

#define NANO_SIG_CONNECT(sig, slot, tar) \
  (sig).connect<std::decay<decltype(*std::decay<decltype(*NanoSignalArgsExtractor((sig)))>::type::Extract((slot)))>::type, (slot)>(tar);

#define NANO_SIG_DISCONNECT(sig, slot, tar) \
  (sig).disconnect<std::decay<decltype(*std::decay<decltype(*NanoSignalArgsExtractor((sig)))>::type::Extract((slot)))>::type, (slot)>(tar);

#define CALL_MEM_FN(o, fn) ((o).*(fn))


class GloveScene;
class GloveShapeBase;

void maskStencil(const glm::mat4 &global_trans, GloveShapeBase *shape, GLint ref);

template<typename Rect_>
void resetTexCoords(Rect_ *pThis)
{
  auto&& me = *pThis;
  me[0].texCoord = glm::vec2(0.0, 0.0);
  me[1].texCoord = glm::vec2(1.0, 0.0);
  me[2].texCoord = glm::vec2(0.0, 1.0);
  me[3].texCoord = glm::vec2(1.0, 1.0);
  pThis->vertices_dirty_ = true;
}

struct defer {
  defer(std::function<void()> todo)
    : todo_(todo)
  {
  }

  ~defer()
  {
    todo_();
  }

private:
  std::function<void()> todo_;
};


#define DTOR_TYPE_INFO_DEBUG() \
  DTOR_PRINT_INFO("%s ", typeid(*this).name(), 0)

#define DTOR_POLYGON_INFO_DEBUG() \
  DTOR_PRINT_INFO("shader=%s, vert_n=%lu, ", shader_type::name(), vertices_count_)


#if 0

#define DTOR_PRINT_INFO(format, info_exp, args...) \
  do { \
    auto && info = info_exp; \
    LOGD("[%lx]" format " destructing", (size_t)this, (info), args); \
    defer([&]{ \
      LOGD("[%lx]" format " destructed", (size_t)this, (info), args); \
    }); \
  } while(0)
#else
#define DTOR_PRINT_INFO(...)
#endif

const size_t kMaxZOrder       = 0x7FFFF;
const size_t kViewZOrderMask  = 0x7F000;
const size_t kViewZOrderShift = 12;
const size_t kMaxViewZOrder   = 0x7F;
const size_t kShapeZOrderMask = 0xFFF;
const size_t kMaxShapeZOrder  = 0xFFF;

static const GLfloat PI = glm::pi<GLfloat>();

#define TOKEN_CAT_HELPER(x, y) x ## y
#define TOKEN_CAT(x, y) TOKEN_CAT_HELPER(x, y)

#define AUTO_LOCK(lockable) \
  auto&& __attribute__((unused)) TOKEN_CAT(lock_, __LINE__) = lockable->auto_lock()


#define __uniform(n) \
  std::get<shader_type::n>(this->uniform_pack_)

#define GL_SAFE_DELETE(x) \
  do { \
    if((x) != nullptr) { \
      delete x; \
      x = nullptr; \
    } \
  } while(0)

#endif //__GLOVE_GL_UTILS_H__
