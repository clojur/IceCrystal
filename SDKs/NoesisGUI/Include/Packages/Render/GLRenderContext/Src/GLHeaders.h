////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_GLHEADERS_H__
#define __RENDER_GLHEADERS_H__


#define NS_RENDERER_USE_WGL 0
#define NS_RENDERER_USE_EGL 0
#define NS_RENDERER_USE_EAGL 0
#define NS_RENDERER_USE_NSGL 0
#define NS_RENDERER_USE_GLX 0


#include <NsCore/Noesis.h>


#ifdef NS_PLATFORM_MOBILE
    #define NS_RENDERER_OPENGL_ES 1
#else
    #define NS_RENDERER_OPENGL 1
#endif


#if NS_RENDERER_OPENGL

    #define GL_APIENTRY APIENTRY
    #define GL_APIENTRYP APIENTRYP

    #if defined(NS_PLATFORM_WINDOWS)
        #undef NS_RENDERER_USE_WGL
        #define NS_RENDERER_USE_WGL 1
        #include <windows.h>
        #include <GL/gl.h>
        #include "glext.h"

    #elif defined(NS_PLATFORM_OSX)
        #undef NS_RENDERER_USE_NSGL
        #define NS_RENDERER_USE_NSGL 1
        #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
        #include <OpenGL/gl3.h>
        #include <OpenGL/gl3ext.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
        
    #elif defined(NS_PLATFORM_LINUX)
        #undef NS_RENDERER_USE_GLX
        #define NS_RENDERER_USE_GLX 1
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>

    #else
        #error Platform not supported
    #endif

#elif NS_RENDERER_OPENGL_ES

    #if defined(NS_PLATFORM_IPHONE)
        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES2/glext.h>
        #include <OpenGLES/ES3/glext.h>
        #undef NS_RENDERER_USE_EAGL
        #define NS_RENDERER_USE_EAGL 1

    #elif defined(NS_PLATFORM_ANDROID)
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
        #undef NS_RENDERER_USE_EGL
        #define NS_RENDERER_USE_EGL 1

    #else
        #error Platform not supported
    #endif
#endif

#if NS_RENDERER_USE_WGL
    #include "wglext.h"
    typedef PROC (GL_APIENTRYP PFNWGLGETPROCADDRESSPROC)(LPCSTR lpszProc);
    typedef BOOL (GL_APIENTRYP PFNWGLMAKECURRENTPROC)(HDC hdc, HGLRC hglrc);
    typedef HGLRC (GL_APIENTRYP PFNWGLCREATECONTEXTPROC)(HDC hdc);
    typedef BOOL (GL_APIENTRYP PFNWGLDELETECONTEXTPROC)(HGLRC hglrc);
    typedef HDC (GL_APIENTRYP PFNWGLGETCURRENTDC)(VOID);
    typedef HGLRC (GL_APIENTRYP PFNWGLGETCURRENTCONTEXT)(VOID);
    typedef GLenum (GL_APIENTRYP PFNGLGETERRORPROC)(void);
    typedef void (GL_APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
    typedef void (GL_APIENTRYP PFNGLPIXELSTOREI)(GLenum pname, GLint param);
    typedef void (GL_APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
    typedef void (GL_APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint *params);
    typedef const GLubyte * (GL_APIENTRYP PFNGLGETSTRINGPROC) (GLenum name);
    typedef void (GL_APIENTRYP PFNGLENABLEPROC)(GLenum cap);
    typedef void (GL_APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
    typedef void (GL_APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    typedef void (GL_APIENTRYP PFNGLCLEARSTENCILPROC)(GLint s);
    typedef void (GL_APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
    typedef void (GL_APIENTRYP PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
#endif

#if NS_RENDERER_USE_EGL
    #include <EGL/egl.h>
    #include <EGL/eglext.h>

    typedef uint64_t GLuint64;

    #ifndef GL_ES_VERSION_3_0
        #define GL_ES_VERSION_3_0 1
        typedef const GLubyte *(GL_APIENTRYP PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);

        typedef void (GL_APIENTRYP PFNGLGENQUERIESPROC) (GLsizei n, GLuint *ids);
        typedef void (GL_APIENTRYP PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint *ids);
        typedef void (GL_APIENTRYP PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
        typedef void (GL_APIENTRYP PFNGLENDQUERYPROC) (GLenum target);
        typedef void (GL_APIENTRYP PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint *params);
    #endif

    #ifndef GL_ES_VERSION_3_2
        #define GL_ES_VERSION_3_2 1
        typedef void (GL_APIENTRYP PFNGLDEBUGMESSAGECONTROLPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
        typedef void (GL_APIENTRY  *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
        typedef void (GL_APIENTRYP PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
    #endif

    inline void EGLError(const char* what)
    {
        EGLint err = eglGetError();

        switch (err)
        {
            case EGL_SUCCESS: NS_FATAL("%s [EGL_SUCCESS]", what);
            case EGL_NOT_INITIALIZED: NS_FATAL("%s [EGL_NOT_INITIALIZED]", what);
            case EGL_BAD_ACCESS: NS_FATAL("%s [EGL_BAD_ACCESS]", what);
            case EGL_BAD_ALLOC: NS_FATAL("%s [EGL_BAD_ALLOC]", what);
            case EGL_BAD_ATTRIBUTE: NS_FATAL("%s [EGL_BAD_ATTRIBUTE]", what);
            case EGL_BAD_CONTEXT: NS_FATAL("%s [EGL_BAD_CONTEXT]", what);
            case EGL_BAD_CONFIG: NS_FATAL("%s [EGL_BAD_CONFIG]", what);
            case EGL_BAD_CURRENT_SURFACE: NS_FATAL("%s [EGL_BAD_CURRENT_SURFACE]", what);
            case EGL_BAD_DISPLAY: NS_FATAL("%s [EGL_BAD_DISPLAY]", what);
            case EGL_BAD_SURFACE: NS_FATAL("%s [EGL_BAD_SURFACE]", what);
            case EGL_BAD_MATCH: NS_FATAL("%s [EGL_BAD_MATCH]", what);
            case EGL_BAD_PARAMETER: NS_FATAL("%s [EGL_BAD_PARAMETER]", what);
            case EGL_BAD_NATIVE_PIXMAP: NS_FATAL("%s [EGL_BAD_NATIVE_PIXMAP]", what);
            case EGL_BAD_NATIVE_WINDOW: NS_FATAL("%s [EGL_BAD_NATIVE_WINDOW]", what);
            case EGL_CONTEXT_LOST: NS_FATAL("%s [EGL_CONTEXT_LOST]", what);
            default:  NS_FATAL("%s [0x%08x]", what, err);
        }
    }
#endif

#if NS_RENDERER_USE_EAGL
    #include "EAGL.h"
#endif

#if NS_RENDERER_USE_NSGL
    #include "NSGL.h"
#endif

#if NS_RENDERER_USE_GLX
    #include <GL/glx.h>
    #include <GL/glxext.h>
#endif


#define NS_GL_VER(major, minor) (major * 10 + minor)

#define V(exp) \
    NS_MACRO_BEGIN \
        while (glGetError() != GL_NO_ERROR); \
        exp; \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) \
        { \
            switch (err) \
            { \
                case GL_INVALID_ENUM: NS_FATAL("%s [GL_INVALID_ENUM]", #exp); \
                case GL_INVALID_VALUE: NS_FATAL("%s [GL_INVALID_VALUE]", #exp); \
                case GL_INVALID_OPERATION: NS_FATAL("%s [GL_INVALID_OPERATION]", #exp); \
                case GL_INVALID_FRAMEBUFFER_OPERATION: NS_FATAL("%s [GL_INVALID_FRAMEBUFFER_OPERATION]", #exp); \
                case GL_OUT_OF_MEMORY: NS_FATAL("%s [GL_OUT_OF_MEMORY]", #exp); \
                default: NS_FATAL("%s [0x%08x]", #exp, err); \
            } \
        } \
    NS_MACRO_END

#ifdef NS_DEBUG_CFG
    #define VD(exp) V(exp)
#else
    #define VD(exp) exp
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef GL_DEBUG_SOURCE_API
    #define GL_DEBUG_SOURCE_API 0x8246
#endif

#ifndef GL_DEBUG_SOURCE_WINDOW_SYSTEM
    #define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#endif

#ifndef GL_DEBUG_SOURCE_SHADER_COMPILER
    #define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#endif

#ifndef GL_DEBUG_SOURCE_THIRD_PARTY
    #define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#endif

#ifndef GL_DEBUG_SOURCE_APPLICATION
    #define GL_DEBUG_SOURCE_APPLICATION 0x824A
#endif

#ifndef GL_DEBUG_SOURCE_OTHER
    #define GL_DEBUG_SOURCE_OTHER 0x824B
#endif

#ifndef GL_DEBUG_TYPE_ERROR
    #define GL_DEBUG_TYPE_ERROR 0x824C
#endif

#ifndef GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
    #define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#endif

#ifndef GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
    #define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#endif

#ifndef GL_DEBUG_TYPE_PORTABILITY
    #define GL_DEBUG_TYPE_PORTABILITY 0x824F
#endif

#ifndef GL_DEBUG_TYPE_PERFORMANCE
    #define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#endif

#ifndef GL_DEBUG_TYPE_OTHER
    #define GL_DEBUG_TYPE_OTHER 0x8251
#endif

#ifndef GL_DEBUG_TYPE_MARKER
    #define GL_DEBUG_TYPE_MARKER 0x8268
#endif

#ifndef GL_DEBUG_TYPE_PUSH_GROUP
    #define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#endif

#ifndef GL_DEBUG_TYPE_POP_GROUP
    #define GL_DEBUG_TYPE_POP_GROUP 0x826A
#endif

#ifndef GL_DEBUG_SEVERITY_HIGH
    #define GL_DEBUG_SEVERITY_HIGH 0x9146
#endif

#ifndef GL_DEBUG_SEVERITY_MEDIUM
    #define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#endif

#ifndef GL_DEBUG_SEVERITY_LOW
    #define GL_DEBUG_SEVERITY_LOW 0x9148
#endif

#ifndef GL_DEBUG_SEVERITY_NOTIFICATION
    #define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#endif

#ifndef GL_NUM_EXTENSIONS
    #define GL_NUM_EXTENSIONS 0x821D
#endif

#ifndef GL_DEBUG_OUTPUT
    #define GL_DEBUG_OUTPUT 0x92E0
#endif

#ifndef GL_DEBUG_OUTPUT_SYNCHRONOUS
    #define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#endif

#endif
