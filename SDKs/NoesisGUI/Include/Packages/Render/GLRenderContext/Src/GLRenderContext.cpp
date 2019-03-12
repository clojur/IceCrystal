////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


//#undef NS_MINIMUM_LOG_LEVEL
//#define NS_MINIMUM_LOG_LEVEL 0


#include "GLRenderContext.h"
#include "BuildSettings.h"

#include <NsCore/Interface.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Category.h>
#include <NsCore/Log.h>
#include <NsCore/TypeId.h>
#include <NsCore/Category.h>
#include <NsCore/ReflectionImplement.h>
#include <NsRender/RenderDevice.h>
#include <NsRender/GLFactory.h>
#include <NsRender/Image.h>
#include <NsApp/Display.h>


using namespace Noesis;
using namespace NoesisApp;


#if NS_RENDERER_USE_WGL
    #define GL_IMPORT(_optional, _proto, _func) \
    { \
        _func = (_proto)wglGetProcAddress(#_func); \
        if (_func == 0) \
        { \
            _func = (_proto)GetProcAddress(mOGL32, #_func); \
        } \
        if (_func == 0 && !_optional) \
        { \
            NS_FATAL("wglGetProcAddress %s", #_func); \
        } \
    }
#elif NS_RENDERER_USE_EGL
    #define GL_IMPORT(_optional, _proto, _func) \
    { \
        _func = (_proto)eglGetProcAddress(#_func); \
        if (_func == 0 && !_optional) \
        { \
            NS_FATAL("eglGetProcAddress %s", #_func); \
        } \
    }
#elif NS_RENDERER_USE_GLX
    #define GL_IMPORT(_optional, _proto, _func) \
    { \
        _func = (_proto)glXGetProcAddress((const GLubyte*)#_func); \
        if (_func == 0 && !_optional) \
        { \
            NS_FATAL("eglGetProcAddress %s", #_func); \
        } \
    }
#endif

#ifdef NS_COMPILER_MSVC
    #define sscanf sscanf_s
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
GLRenderContext::GLRenderContext(): mGPUTime(0.0f)
{
#if NS_RENDERER_USE_WGL
    mSurface = 0;
    mContext = 0;
#elif NS_RENDERER_USE_EGL
    mDisplay = 0;
    mSurface = 0;
    mContext = 0;
#elif NS_RENDERER_USE_EAGL
    mContext = 0;
    mFBO = 0;
    mFBOResolved = 0;
    mColor = 0;
    mColorAA = 0;
    mStencil = 0;
#elif NS_RENDERER_USE_NSGL
    mContext = 0;
#elif NS_RENDERER_USE_GLX
    mDisplay = 0;
    mSurface = 0;
    mContext = 0;
#endif

    mClearColor[0] = 0.0f;
    mClearColor[1] = 0.0f;
    mClearColor[2] = 0.0f;
    mClearColor[3] = 0.0f;

    memset(mQueries, 0, sizeof(mQueries));

#if NS_RENDERER_USE_WGL
    wglGetProcAddress = 0;
    wglMakeCurrent = 0;
    wglCreateContext = 0;
    wglDeleteContext = 0;

    glBindFramebuffer = 0;
    glViewport = 0;
    glPixelStorei = 0;
    glReadPixels = 0;
    glGetIntegerv = 0;
    glGetString = 0;
    glGetStringi = 0;

    glGenQueries = 0;
    glDeleteQueries = 0;
    glBeginQuery = 0;
    glEndQuery = 0;
    glGetQueryObjectuiv = 0;
    glGetQueryObjectui64v = 0;
    GetQueryObjectui64v_ = 0;

    mOGL32 = LoadLibraryA("opengl32.dll");
    if (mOGL32 != 0)
    {
        wglGetProcAddress = (PFNWGLGETPROCADDRESSPROC)GetProcAddress(mOGL32, "wglGetProcAddress");
        wglMakeCurrent = (PFNWGLMAKECURRENTPROC)GetProcAddress(mOGL32, "wglMakeCurrent");
        wglCreateContext = (PFNWGLCREATECONTEXTPROC)GetProcAddress(mOGL32, "wglCreateContext");
        wglDeleteContext = (PFNWGLDELETECONTEXTPROC)GetProcAddress(mOGL32, "wglDeleteContext");
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
GLRenderContext::~GLRenderContext()
{
    mDevice.Reset();

    if (HaveQueries())
    {
        V(glDeleteQueries(NS_COUNTOF(mQueries), mQueries));
    }

#if NS_RENDERER_USE_WGL
    if (mContext != 0)
    {
        wglDeleteContext(mContext);
        wglMakeCurrent(0, 0);
    }

    if (mSurface != 0)
    {
        ReleaseDC(WindowFromDC(mSurface), mSurface);
    }

    if (mOGL32 != 0)
    {
        FreeLibrary(mOGL32);
    }

#elif NS_RENDERER_USE_EGL
    if (mDisplay != 0)
    {
        if (mSurface != 0)
        {
            eglDestroySurface(mDisplay, mSurface);
        }
        if (mContext != 0)
        {
            eglDestroyContext(mDisplay, mContext);
            eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        }
        eglTerminate(mDisplay);
    }

#elif NS_RENDERER_USE_EAGL
    V(glDeleteRenderbuffers(1, &mStencil));
    V(glDeleteRenderbuffers(1, &mColor));
    V(glDeleteRenderbuffers(1, &mColorAA));
    V(glDeleteFramebuffers(1, &mFBO));
    V(glDeleteFramebuffers(1, &mFBOResolved));

    if (mContext != 0)
    {
        EAGLDeleteContext(mContext);
        EAGLSetCurrentContext(0);
    }

#elif NS_RENDERER_USE_NSGL
    if (mContext != 0)
    {
        NSGLDeleteContext(mContext);
        NSGLClearCurrentContext();
    }

#elif NS_RENDERER_USE_GLX
   if (mContext != 0)
    {
        glXDestroyContext(mDisplay, mContext);
        glXMakeCurrent(mDisplay, 0, 0);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* GLRenderContext::Description() const
{
#if NS_RENDERER_OPENGL
    return "OpenGL";
#else
    return "OpenGL ES";
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t GLRenderContext::Score() const
{
    return 100;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool GLRenderContext::Validate() const
{
#if NS_RENDERER_USE_WGL
    return wglGetProcAddress != 0 && wglMakeCurrent != 0 && wglCreateContext != 0 &&
        wglDeleteContext != 0;
#else
    return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::Init(void* window, uint32_t& sampleCount, bool vsync, bool sRGB)
{
    NS_ASSERT(Validate());
    NS_UNUSED(sRGB);

    NS_LOG_DEBUG("Creating GL render context");

#if NS_RENDERER_USE_EAGL
    mContext = EAGLCreateContext(mRenderingAPI);
    if (mContext == 0)
    {
        NS_FATAL("EAGLCreateContext");
    }

    NS_ASSERT(mRenderingAPI == 2 || mRenderingAPI == 3);

    if (!EAGLSetCurrentContext(mContext))
    {
        NS_FATAL("EAGLSetCurrentContext");
    }

    V(glGenRenderbuffers(1, &mColor));
    V(glBindRenderbuffer(GL_RENDERBUFFER, mColor));
    if (!EAGLRenderbufferStorageFromDrawable(mContext, GL_RENDERBUFFER, window))
    {
        NS_FATAL("EAGLRenderbufferStorageFromDrawable");
    }

    GLsizei maxSamples;
    V(glGetIntegerv(GL_MAX_SAMPLES, &maxSamples));
    sampleCount = eastl::min_alt((GLsizei)sampleCount, maxSamples);

    GLsizei samples = sampleCount == 1 ? 0 : sampleCount;

    V(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &mWidth));
    V(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &mHeight));
    GLsizei w = mWidth, h = mHeight;

    V(glGenRenderbuffers(1, &mStencil));
    V(glBindRenderbuffer(GL_RENDERBUFFER, mStencil));
    if (mRenderingAPI == 3)
    {
        V(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_STENCIL_INDEX8, w, h));
    }
    else
    {
        V(glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, samples, GL_STENCIL_INDEX8, w, h));
    }

    const GLenum ColorAttachment = GL_COLOR_ATTACHMENT0;
    const GLenum StencilAttachment = GL_STENCIL_ATTACHMENT;

    if (samples > 1)
    {
        V(glGenRenderbuffers(1, &mColorAA));
        V(glBindRenderbuffer(GL_RENDERBUFFER, mColorAA));
        if (mRenderingAPI == 3)
        {
            V(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8_OES, w, h));
        }
        else
        {
            V(glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, samples, GL_RGBA8_OES, w, h));
        }

        V(glGenFramebuffers(1, &mFBO));
        V(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
        V(glFramebufferRenderbuffer(GL_FRAMEBUFFER, ColorAttachment, GL_RENDERBUFFER, mColorAA));
        V(glFramebufferRenderbuffer(GL_FRAMEBUFFER, StencilAttachment, GL_RENDERBUFFER, mStencil));
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            NS_FATAL("glCheckFramebufferStatus %d", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }

        V(glGenFramebuffers(1, &mFBOResolved));
        V(glBindFramebuffer(GL_FRAMEBUFFER, mFBOResolved));
        V(glFramebufferRenderbuffer(GL_FRAMEBUFFER, ColorAttachment, GL_RENDERBUFFER, mColor));
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            NS_FATAL("glCheckFramebufferStatus %d", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
    }
    else
    {
        V(glGenFramebuffers(1, &mFBO));
        V(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
        V(glFramebufferRenderbuffer(GL_FRAMEBUFFER, ColorAttachment, GL_RENDERBUFFER, mColor));
        V(glFramebufferRenderbuffer(GL_FRAMEBUFFER, StencilAttachment, GL_RENDERBUFFER, mStencil));
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            NS_FATAL("glCheckFramebufferStatus %d", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
    }

#elif NS_RENDERER_USE_NSGL
    mContext = NSGLCreateContext(sampleCount, vsync);
    if (mContext == 0)
    {
        NS_FATAL("NSGLCreateContext");
    }

    NSGLSetView(mContext, window);
    NSGLSetCurrentContext(mContext);

#elif NS_RENDERER_USE_WGL
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 0;
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = 0;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;
    uint32_t glMajorVersion;
    uint32_t glMinorVersion;

    {
        // Dummy context creation
        Ptr<Display> display_ = CreateDisplay();
        HWND hWnd = (HWND)display_->GetNativeHandle();
        HDC hdc = GetDC(hWnd);
        if (hdc == 0)
        {
            NS_FATAL("GetDC");
        }

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (pixelFormat == 0)
        {
            NS_FATAL("Not valid pixel format found");
        }

        if (!SetPixelFormat(hdc, pixelFormat, &pfd))
        {
            NS_FATAL("SetPixelFormat");
        }

        HGLRC context = wglCreateContext(hdc);
        if (context == 0)
        {
            NS_FATAL("wglCreateContext");
        }

        if (!wglMakeCurrent(hdc, context))
        {
            NS_FATAL("wglMakeCurrent");
        }

        GL_IMPORT(true, PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB);
        GL_IMPORT(true, PFNWGLGETPIXELFORMATATTRIBIVARBPROC, wglGetPixelFormatAttribivARB);
        GL_IMPORT(true, PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
        GL_IMPORT(true, PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT);
        GL_IMPORT(false, PFNGLGETSTRINGPROC, glGetString);

        const char* version = (const char*)glGetString(GL_VERSION);
        if (version == 0 || sscanf(version, "%d.%d", &glMajorVersion, &glMinorVersion) != 2)
        {
            glMajorVersion = 2;
            glMinorVersion = 1;
        }

        wglDeleteContext(context);
        wglMakeCurrent(0, 0);
        ReleaseDC(hWnd, hdc);
    }

    HWND hWnd = (HWND)window;
    mSurface = GetDC(hWnd);
    if (mSurface == 0)
    {
        NS_FATAL("GetDC");
    }

    int pixelFormat = 0;

    if (wglChoosePixelFormatARB != 0 && sampleCount > 1)
    {
        uint32_t samples = sampleCount;

        do
        {
            int attribs[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
#if RENDERTARGETS_16BPP
                WGL_COLOR_BITS_ARB, 16,
#else
                WGL_COLOR_BITS_ARB, 24,
#endif
                WGL_STENCIL_BITS_ARB, 8,
                WGL_SAMPLE_BUFFERS_ARB, 1,
                WGL_SAMPLES_ARB, (int)samples,
                0
            };

            unsigned int numFormats;
            if (!wglChoosePixelFormatARB(mSurface, attribs, NULL, 1, &pixelFormat, &numFormats))
            {
                pixelFormat = 0;
            }

            samples >>= 1;
        }
        while (pixelFormat == 0 && samples != 0);
    }
    else
    {
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cStencilBits = 8;
#if RENDERTARGETS_16BPP
        pfd.cColorBits = 16;
#else
        pfd.cColorBits = 24;
#endif
        pixelFormat = ChoosePixelFormat(mSurface, &pfd);
    }

    if (pixelFormat == 0)
    {
        NS_FATAL("Not valid pixel format found");
    }

    PIXELFORMATDESCRIPTOR pfd;
    if (!DescribePixelFormat(mSurface, pixelFormat, sizeof(pfd), &pfd))
    {
        NS_FATAL("DescribePixelFormat");
    }

    sampleCount = 1;

    if (wglGetPixelFormatAttribivARB != 0)
    {
        int attrib = WGL_SAMPLES_ARB;
        int value;
        if (wglGetPixelFormatAttribivARB(mSurface, pixelFormat, 0, 1, &attrib, &value))
        {
            sampleCount = value;
        }
    }

    NS_LOG_DEBUG(" PixelFormat: R%dG%dB%dA%d D%dS%d %dx", pfd.cRedBits, pfd.cGreenBits,
        pfd.cBlueBits, pfd.cAlphaBits, pfd.cDepthBits, pfd.cStencilBits, sampleCount);

    if (!SetPixelFormat(mSurface, pixelFormat, &pfd))
    {
        NS_FATAL("SetPixelFormat");
    }

    if (wglCreateContextAttribsARB != 0)
    {
        int contextFlags = glMajorVersion >= 3? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0;

#ifdef NS_DEBUG_CFG
        NS_LOG_WARNING(" Creating a Debug context");
        contextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif

        int attribs[] =
        {
            WGL_CONTEXT_FLAGS_ARB, contextFlags,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_MAJOR_VERSION_ARB, (int)glMajorVersion,
            WGL_CONTEXT_MINOR_VERSION_ARB, (int)glMinorVersion,
            0
        };

        mContext = wglCreateContextAttribsARB(mSurface, 0, attribs);
    }
    else
    {
        mContext = wglCreateContext(mSurface);
    }

    if (mContext == 0)
    {
        NS_FATAL("wglCreateContext");
    }

    if (!wglMakeCurrent(mSurface, mContext))
    {
        NS_FATAL("wglMakeCurrent");
    }

    if (wglSwapIntervalEXT != 0)
    {
        wglSwapIntervalEXT(vsync ? 1 : 0);
    }

    GL_IMPORT(false, PFNGLGETERRORPROC, glGetError);
    GL_IMPORT(false, PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    GL_IMPORT(false, PFNGLVIEWPORTPROC, glViewport);
    GL_IMPORT(false, PFNGLPIXELSTOREI, glPixelStorei);
    GL_IMPORT(false, PFNGLREADPIXELSPROC, glReadPixels);
    GL_IMPORT(false, PFNGLGETINTEGERVPROC, glGetIntegerv);
    GL_IMPORT(false, PFNGLGETSTRINGPROC, glGetString);
    GL_IMPORT(false, PFNGLENABLEPROC, glEnable);
    GL_IMPORT(false, PFNGLDISABLEPROC, glDisable);
    GL_IMPORT(false, PFNGLCLEARCOLORPROC, glClearColor);
    GL_IMPORT(false, PFNGLCLEARSTENCILPROC, glClearStencil);
    GL_IMPORT(false, PFNGLCLEARPROC, glClear);
    GL_IMPORT(false, PFNGLCOLORMASKPROC, glColorMask);
    GL_IMPORT(true, PFNGLGETSTRINGIPROC, glGetStringi);

    GL_IMPORT(true, PFNGLGENQUERIESPROC, glGenQueries);
    GL_IMPORT(true, PFNGLDELETEQUERIESPROC, glDeleteQueries);
    GL_IMPORT(true, PFNGLBEGINQUERYPROC, glBeginQuery);
    GL_IMPORT(true, PFNGLENDQUERYPROC, glEndQuery);
    GL_IMPORT(true, PFNGLGETQUERYOBJECTUIVPROC, glGetQueryObjectuiv);
    GL_IMPORT(true, PFNGLGETQUERYOBJECTUI64VPROC, glGetQueryObjectui64v);
    GL_IMPORT(true, PFNGLGETQUERYOBJECTUI64VEXTPROC, glGetQueryObjectui64vEXT);

    GL_IMPORT(true, PFNGLDEBUGMESSAGECONTROLPROC, glDebugMessageControl);
    GL_IMPORT(true, PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);
    GL_IMPORT(true, PFNGLDEBUGMESSAGECALLBACKARBPROC, glDebugMessageCallbackARB);

#elif NS_RENDERER_USE_EGL
    sampleCount = 1;
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mDisplay == EGL_NO_DISPLAY)
    {
        NS_FATAL("EGL_NO_DISPLAY");
    }

    if (!eglInitialize(mDisplay, 0, 0))
    {
        EGLError("eglInitialize");
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        EGLError("eglBindAPI");
    }
        
    const EGLint configAttribs[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_SWAP_BEHAVIOR_PRESERVED_BIT,

#if RENDERTARGETS_16BPP
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 5,
        EGL_BLUE_SIZE, 5,
#else
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
#endif

        EGL_ALPHA_SIZE, 0,
        EGL_DEPTH_SIZE, 0,
        EGL_STENCIL_SIZE, 0,

        EGL_NONE
    };

    EGLint numConfigs;
    EGLConfig config = 0;
    if (!eglChooseConfig(mDisplay, configAttribs, &config, 1, &numConfigs))
    {
        EGLError("eglChooseConfig");
    }
        
    if (numConfigs == 0)
    {
        const EGLint configAttribs[] =
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                
#if RENDERTARGETS_16BPP
            EGL_RED_SIZE, 5,
            EGL_GREEN_SIZE, 5,
            EGL_BLUE_SIZE, 5,
#else
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
#endif

            EGL_ALPHA_SIZE, 0,
            EGL_DEPTH_SIZE, 0,
            EGL_STENCIL_SIZE, 0,

            EGL_NONE
        };
            
        if (!eglChooseConfig(mDisplay, configAttribs, &config, 1, &numConfigs))
        {
            EGLError("eglChooseConfig");
        }

        if (numConfigs == 0)
        {
            NS_FATAL("eglChooseConfig: no valid config found");
        }
    }

    EGLint redSize;
    eglGetConfigAttrib(mDisplay, config, EGL_RED_SIZE, &redSize);
    EGLint greenSize;
    eglGetConfigAttrib(mDisplay, config, EGL_GREEN_SIZE, &greenSize);
    EGLint blueSize;
    eglGetConfigAttrib(mDisplay, config, EGL_BLUE_SIZE, &blueSize);
    EGLint alphaSize;
    eglGetConfigAttrib(mDisplay, config, EGL_ALPHA_SIZE, &alphaSize);
    EGLint depthSize;
    eglGetConfigAttrib(mDisplay, config, EGL_DEPTH_SIZE, &depthSize);
    EGLint stencilSize;
    eglGetConfigAttrib(mDisplay, config, EGL_STENCIL_SIZE, &stencilSize);

    NS_LOG_DEBUG(" PixelFormat: R%dG%dB%dA%d D%dS%d", redSize, greenSize, blueSize, alphaSize,
        depthSize, stencilSize);

    const EGLint surfacetAttribs[] =
    {
        EGL_NONE
    };

    mSurface = eglCreateWindowSurface(mDisplay, config, static_cast<EGLNativeWindowType>(window),
        surfacetAttribs);
    if(mSurface == EGL_NO_SURFACE)
    {
        EGLError("eglCreateWindowSurface");
    }
        
    const EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, contextAttribs);
    if (mContext == EGL_NO_CONTEXT)
    {
        EGLError("eglCreateContext");
    }

    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext))
    {
        EGLError("eglMakeCurrent");
    }

    // Hint that we want a discardable surface
    eglSurfaceAttrib(mDisplay, mSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_DESTROYED);

    eglSwapInterval(mDisplay, vsync ? 1 : 0);

    GL_IMPORT(true, PFNGLGETSTRINGIPROC, glGetStringi);

    GL_IMPORT(true, PFNGLGENQUERIESPROC, glGenQueries);
    GL_IMPORT(true, PFNGLDELETEQUERIESPROC, glDeleteQueries);
    GL_IMPORT(true, PFNGLBEGINQUERYPROC, glBeginQuery);
    GL_IMPORT(true, PFNGLENDQUERYPROC, glEndQuery);
    GL_IMPORT(true, PFNGLGETQUERYOBJECTUIVPROC, glGetQueryObjectuiv);

    GL_IMPORT(true, PFNGLDEBUGMESSAGECONTROLPROC, glDebugMessageControl);
    GL_IMPORT(true, PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);

#elif NS_RENDERER_USE_GLX
    sampleCount = 1;

    struct XWindow
    {
        ::Display* display;
        XVisualInfo* visualInfo;
        Window window;
    };

    XWindow* x = (XWindow*)window;
    mDisplay = x->display;
    mSurface = x->window;
    XVisualInfo* pixelFormat = x->visualInfo;
    
    int errorBase, eventBase;
    if (!glXQueryExtension(mDisplay, &errorBase, &eventBase))
    {
        NS_FATAL("GLX extension not supported");
    }

    int glxMajor, glxMinor;
    glXQueryVersion(mDisplay, &glxMajor, &glxMinor);
    NS_LOG_DEBUG(" Connected to X server '%s' %d.%d", XServerVendor(mDisplay), glxMajor,glxMinor);

    int rSize, gSize, bSize, aSize, zSize, sSize;
    glXGetConfig(mDisplay, pixelFormat, GLX_RED_SIZE, &rSize);
    glXGetConfig(mDisplay, pixelFormat, GLX_GREEN_SIZE, &gSize);
    glXGetConfig(mDisplay, pixelFormat, GLX_BLUE_SIZE, &bSize);
    glXGetConfig(mDisplay, pixelFormat, GLX_ALPHA_SIZE, &aSize);
    glXGetConfig(mDisplay, pixelFormat, GLX_DEPTH_SIZE, &zSize);
    glXGetConfig(mDisplay, pixelFormat, GLX_STENCIL_SIZE, &sSize);
    NS_LOG_DEBUG(" PixelFormat: R%dG%dB%dA%d D%dS%d", rSize, gSize, bSize, aSize, zSize, sSize);

    mContext = glXCreateContext(mDisplay, pixelFormat, None, true);
    if (mContext == 0)
    {
        NS_FATAL("glXCreateContext");
    }

    if (!glXIsDirect(mDisplay, mContext))
    {
        NS_LOG_WARNING(" Indirect GLX rendering context");
    }
    
    if (!glXMakeCurrent(mDisplay, mSurface, mContext))
    {
        NS_FATAL("glXMakeCurrent");
    }

    PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
    GL_IMPORT(true, PFNGLXSWAPINTERVALEXTPROC, glXSwapIntervalEXT);

    if (glXSwapIntervalEXT != 0)
    {
        glXSwapIntervalEXT(mDisplay, mSurface, vsync ? 1 : 0);
    }

#else
    #error Unknown Windowing System Interface
#endif

    FindExtensions();

    if (HaveQueries())
    {
        V(glGenQueries(NS_COUNTOF(mQueries), mQueries));
        mReadQuery = 0;
        mWriteQuery = 0;
    }

    mDevice = GLFactory::CreateDevice();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
RenderDevice* GLRenderContext::GetDevice() const
{
    return mDevice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::BeginRender()
{
#if GL_VERSION_3_3 || GL_ARB_timer_query || GL_EXT_timer_query
    if (HaveQueries())
    {
        VD(glBeginQuery(GL_TIME_ELAPSED, mQueries[mWriteQuery]));
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::EndRender()
{
#if GL_VERSION_3_3 || GL_ARB_timer_query || GL_EXT_timer_query
    if (HaveQueries())
    {
        VD(glEndQuery(GL_TIME_ELAPSED));

        mWriteQuery = (mWriteQuery + 1) % NS_COUNTOF(mQueries);
        // Disabled because this assert happens frequenty when the GPU stops processing frames
        //NS_ASSERT(mWriteQuery != mReadQuery);

        NS_ASSERT(mReadQuery < NS_COUNTOF(mQueries));
        GLuint ready = GL_TRUE;

        while (ready && mReadQuery != mWriteQuery)
        {
            GLuint query = mQueries[mReadQuery];
            VD(glGetQueryObjectuiv(query, GL_QUERY_RESULT_AVAILABLE, &ready));

            if (ready)
            {
                GLuint64 time;
                VD(GetQueryObjectui64v_(query, GL_QUERY_RESULT, &time));
                mReadQuery = (mReadQuery + 1) % NS_COUNTOF(mQueries);

                mGPUTime = (float)(double(time) / 1e06);
            }
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::Resize()
{
#if NS_RENDERER_USE_NSGL
    NSGLUpdate(mContext);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
float GLRenderContext::GetGPUTimeMs() const
{
    return mGPUTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::SetClearColor(float r, float g, float b, float a)
{
    mClearColor[0] = r;
    mClearColor[1] = g;
    mClearColor[2] = b;
    mClearColor[3] = a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::SetDefaultRenderTarget(uint32_t width, uint32_t height)
{
#if NS_RENDERER_USE_EAGL
    VD(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
#else
    VD(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#endif

    VD(glViewport(0, 0, width, height));

    VD(glDisable(GL_SCISSOR_TEST));
    VD(glColorMask(true, true, true, true));
    VD(glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], mClearColor[3]));
    VD(glClearStencil(0));
    VD(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<NoesisApp::Image> GLRenderContext::CaptureRenderTarget(RenderTarget* surface) const
{
    NS_UNUSED(surface);

    GLint viewport[4];
    V(glGetIntegerv(GL_VIEWPORT, viewport));

    uint32_t x = viewport[0];
    uint32_t y = viewport[1];
    uint32_t width = viewport[2];
    uint32_t height = viewport[3];

    Ptr<Image> image = *new Image(width, height);

    V(glPixelStorei(GL_PACK_ALIGNMENT, 1));
    V(glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image->Data()));

    Ptr<Image> dest = *new Image(width, height);

    const uint8_t* src = static_cast<const uint8_t*>(image->Data());
    src += (height - 1) * image->Stride();
    uint8_t* dst = static_cast<uint8_t*>(dest->Data());

    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            dst[4 * j + 0] = src[4 * j + 2];
            dst[4 * j + 1] = src[4 * j + 1];
            dst[4 * j + 2] = src[4 * j + 0];
            dst[4 * j + 3] = src[4 * j + 3];
        }

        src -= image->Stride();
        dst += dest->Stride();
    }

    return dest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::Swap()
{
#if NS_RENDERER_USE_WGL
    SwapBuffers(mSurface);

#elif NS_RENDERER_USE_EGL
    eglSwapBuffers(mDisplay, mSurface);

#elif NS_RENDERER_USE_EAGL
    if (mFBOResolved != 0)
    {
        VD(glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, mFBOResolved));
        VD(glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, mFBO));

        if (mRenderingAPI == 3)
        {
            const GLint w = mWidth, h = mHeight;
            VD(glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST));

            GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_STENCIL_ATTACHMENT };
            VD(glInvalidateFramebuffer(GL_READ_FRAMEBUFFER, 2, attachments));
        }
        else
        {
            VD(glResolveMultisampleFramebufferAPPLE());

            GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_STENCIL_ATTACHMENT };
            VD(glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER, 2, attachments));
        }
    }

    VD(glBindRenderbuffer(GL_RENDERBUFFER, mColor));
    VD(EAGLPresentRenderbuffer(mContext, GL_RENDERBUFFER));

#elif NS_RENDERER_USE_NSGL
    NSGLFlushBuffer(mContext);

#elif NS_RENDERER_USE_GLX
    glXSwapBuffers(mDisplay, mSurface);

#else
    #error Unknown Windowing System Interface
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t GLRenderContext::GLVersion() const
{
    uint32_t major;
    uint32_t minor;
    const char* version = (const char*)glGetString(GL_VERSION);
    NS_ASSERT(version != 0);

#if NS_RENDERER_OPENGL
    if (sscanf(version, "%d.%d", &major, &minor) != 2)
    {
        NS_DEBUG_BREAK;
        NS_LOG_WARNING("Unable to parse GL_VERSION '%s'", version);
    }
#else
    if (sscanf(version, "OpenGL ES %d.%d", &major, &minor) != 2)
    {
        char profile[2];
        if (sscanf(version, "OpenGL ES-%c%c %d.%d", profile, profile + 1, &major, &minor) != 4)
        {
            NS_DEBUG_BREAK;
            NS_LOG_WARNING("Unable to parse GL_VERSION '%s'", version);
        }
    }
#endif

    NS_LOG_DEBUG(" Version:  %d.%d (%s)", major, minor, version);
    NS_LOG_DEBUG(" Vendor:   %s", glGetString(GL_VENDOR));
    NS_LOG_DEBUG(" Renderer: %s", glGetString(GL_RENDERER));
    return 10 * major + minor;
}

#if defined(NS_DEBUG_CFG) && (GL_VERSION_4_3 || GL_ES_VERSION_3_2 || GL_KHR_debug || GL_ARB_debug_output)

namespace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
void GL_APIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint, GLenum severity,
    GLsizei length, const GLchar *message, const GLvoid *)
{
    const char* szSource;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
        {
            szSource = "SOURCE_API";
            break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        {
            szSource = "SOURCE_WINDOW_SYSTEM";
            break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
        {
            szSource = "SOURCE_SHADER_COMPILER";
            break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY:
        {
            szSource = "SOURCE_THIRD_PARTY";
            break;
        }
        case GL_DEBUG_SOURCE_APPLICATION:
        {
            szSource = "SOURCE_APPLICATION";
            break;
        }
        case GL_DEBUG_SOURCE_OTHER:
        {
            szSource = "SOURCE_OTHER";
            break;
        }
        default:
        {
            szSource = "SOURCE_UNKNOWN";
            break;
        }
    }

    const char* szType;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
        {
            szType = "TYPE_ERROR";
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        {
            szType = "TYPE_DEPRECATED_BEHAVIOR";
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        {
            szType = "TYPE_UNDEFINED_BEHAVIOR";
            break;
        }
        case GL_DEBUG_TYPE_PORTABILITY:
        {
            szType = "TYPE_PORTABILITY";
            break;
        }
        case GL_DEBUG_TYPE_PERFORMANCE:
        {
            szType = "TYPE_PERFORMANCE";
            break;
        }
        case GL_DEBUG_TYPE_OTHER:
        {
            szType = "TYPE_OTHER";
            break;
        }
        default:
        {
            szType = "TYPE_UNKNOWN";
            break;
        }
    }

    const char* szSeverity;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
        {
            szSeverity = "SEVERITY_HIGH";
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            szSeverity = "SEVERITY_MEDIUM";
            break;
        }
        case GL_DEBUG_SEVERITY_LOW:
        {
            szSeverity = "SEVERITY_LOW";
            break;
        }
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            szSeverity = "SEVERITY_NOTIFICATION";
            break;
        }
        default:
        {
            szSeverity = "SEVERITY_UNKNOWN";
            break;
        }
    }

    NS_LOG_DEBUG("[%s][%s][%s]", szSource, szType, szSeverity);

    if (length == 0)
    {
        NS_LOG_DEBUG(" %s", message);
    }
    else
    {
        // Strings with a specified lenght are not NULL terminated
        NS_LOG_DEBUG(" %.*s", length, message);
    }
}
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
void GLRenderContext::FindExtensions()
{
    uint32_t glVersion = GLVersion();

    struct Extension
    {
        enum Enum
        {
            ARB_timer_query,
            EXT_timer_query,
            KHR_debug,
            ARB_debug_output,

            Count
        };
    
        const char* name;
        bool supported;
    };

    Extension extensions[Extension::Count] =
    {
        { "GL_ARB_timer_query", false},
        { "EXT_timer_query", false},
        { "GL_KHR_debug", false},
        { "GL_ARB_debug_output", false}
    };

    if (glVersion >= NS_GL_VER(3,0))
    {
        GLint numExtensions = 0;
        V(glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions));

        for (GLint i = 0; i < numExtensions; i++)
        {
            const char* glExtension = (const char*)glGetStringi(GL_EXTENSIONS, i);

            for (uint32_t j = 0; j < Extension::Count; j++)
            {
                if (String::Equals(glExtension, extensions[j].name))
                {
                    extensions[j].supported = true;
                    break;
                }
            }
        }
    }
    else
    {
        const char* glExtensions = (const char*)glGetString(GL_EXTENSIONS);

        if (glExtensions != 0)
        {
            for (uint32_t i = 0; i < Extension::Count; i++)
            {
                if (String::FindFirst(glExtensions, extensions[i].name) != -1 )
                {
                    extensions[i].supported = true;
                }
            }
        }
    }

#if NS_RENDERER_OPENGL
    if (glVersion >= NS_GL_VER(3,3) || extensions[Extension::ARB_timer_query].supported)
#else
    if (extensions[Extension::ARB_timer_query].supported)
#endif
    {
        #if GL_VERSION_3_3 || GL_ARB_timer_query
            GetQueryObjectui64v_ = glGetQueryObjectui64v;
        #else
            NS_ASSERT_UNREACHABLE;
        #endif
    }
    else if (extensions[Extension::EXT_timer_query].supported)
    {
        #if GL_EXT_timer_query
            GetQueryObjectui64v_ = glGetQueryObjectui64vEXT;
        #else
            NS_ASSERT_UNREACHABLE;
        #endif
    }
    else
    {
        GetQueryObjectui64v_ = 0;
    }

    // DebugOutput
#if defined(NS_DEBUG_CFG) && (GL_VERSION_4_3 || GL_ES_VERSION_3_2 || GL_KHR_debug || GL_ARB_debug_output)

    void (GL_APIENTRYP DebugMessageControl_)(GLenum, GLenum, GLenum, GLsizei, const GLuint*,
        GLboolean) = 0;
    void (GL_APIENTRYP DebugMessageCallback_)(GLDEBUGPROC, const void*) = 0;

#if NS_RENDERER_OPENGL
    if (glVersion >= NS_GL_VER(4,3) || extensions[Extension::KHR_debug].supported)
#else
    if (glVersion >= NS_GL_VER(3,2))
#endif
    {
        #if GL_VERSION_4_3 || GL_ES_VERSION_3_2 || GL_KHR_debug
            DebugMessageControl_ = glDebugMessageControl;
            DebugMessageCallback_ = glDebugMessageCallback;
        #else
            NS_ASSERT_UNREACHABLE;
        #endif
    }
#if NS_RENDERER_OPENGL_ES
    else if (extensions[Extension::KHR_debug].supported)
    {
        #if GL_KHR_debug
            DebugMessageControl_ = glDebugMessageControlKHR;
            DebugMessageCallback_ = glDebugMessageCallbackKHR;
        #else
            NS_ASSERT_UNREACHABLE;
        #endif
    }
#endif

    if (DebugMessageControl_ != 0)
    {
        V(DebugMessageControl_(GL_DONT_CARE, GL_DEBUG_TYPE_MARKER, GL_DONT_CARE, 0, 0, GL_FALSE));
        V(DebugMessageControl_(GL_DONT_CARE, GL_DEBUG_TYPE_PUSH_GROUP, GL_DONT_CARE, 0, 0, GL_FALSE));
        V(DebugMessageControl_(GL_DONT_CARE, GL_DEBUG_TYPE_POP_GROUP, GL_DONT_CARE, 0, 0, GL_FALSE));
        V(DebugMessageCallback_((GLDEBUGPROC)DebugMessageCallback, 0));
        V(glEnable(GL_DEBUG_OUTPUT));
        V(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
    }
    else if (extensions[Extension::ARB_debug_output].supported)
    {
        #if GL_ARB_debug_output
            V(glDebugMessageCallbackARB((GLDEBUGPROCARB)DebugMessageCallback, 0));
            V(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB));
        #else
            NS_ASSERT_UNREACHABLE;
        #endif
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool GLRenderContext::HaveQueries() const
{
    return GetQueryObjectui64v_ != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(GLRenderContext)
{
    NsMeta<TypeId>("GLRenderContext");
    NsMeta<Category>("RenderContext");
}
