////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_GLRENDERCONTEXT_H__
#define __RENDER_GLRENDERCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/Queue.h>
#include <NsRender/RenderContext.h>

#include "GLHeaders.h"


namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class GLRenderContext final: public RenderContext
{
public:
    GLRenderContext();
    ~GLRenderContext();

    /// From RenderContext
    //@{
    const char* Description() const override;
    uint32_t Score() const override;
    bool Validate() const override;
    void Init(void* window, uint32_t& samples, bool vsync, bool sRGB) override;
    Noesis::RenderDevice* GetDevice() const override;
    void BeginRender() override;
    void EndRender() override;
    void Resize() override;
    float GetGPUTimeMs() const override;
    void SetClearColor(float r, float g, float b, float a) override;
    void SetDefaultRenderTarget(uint32_t width, uint32_t height) override;
    Noesis::Ptr<Image> CaptureRenderTarget(Noesis::RenderTarget* surface) const override;
    void Swap() override;
    //@}

private:
    uint32_t GLVersion() const;
    void FindExtensions();
    bool HaveQueries() const;

private:
#if NS_RENDERER_USE_WGL
    HDC mSurface;
    HGLRC mContext;
#elif NS_RENDERER_USE_EGL
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
#elif NS_RENDERER_USE_EAGL
    void* mContext;
    uint32_t mRenderingAPI;
    GLint mWidth;
    GLint mHeight;
    GLuint mFBO;
    GLuint mFBOResolved;
    GLuint mColor;
    GLuint mColorAA;
    GLuint mStencil;
#elif NS_RENDERER_USE_NSGL
    void* mContext;
#elif NS_RENDERER_USE_GLX
    ::Display* mDisplay;
    GLXDrawable mSurface;
    GLXContext mContext;
#else
    #error Unknown Windowing System Interface
#endif

    float mClearColor[4];

#if NS_RENDERER_USE_WGL
    HMODULE mOGL32;

    PFNWGLGETPROCADDRESSPROC wglGetProcAddress;
    PFNWGLMAKECURRENTPROC wglMakeCurrent;
    PFNWGLCREATECONTEXTPROC wglCreateContext;
    PFNWGLDELETECONTEXTPROC wglDeleteContext;

    PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
    PFNGLGETERRORPROC glGetError;
    PFNGLVIEWPORTPROC glViewport;
    PFNGLPIXELSTOREI glPixelStorei;
    PFNGLREADPIXELSPROC glReadPixels;
    PFNGLGETINTEGERVPROC glGetIntegerv;
    PFNGLGETSTRINGPROC glGetString;
    PFNGLENABLEPROC glEnable;
    PFNGLDISABLEPROC glDisable;
    PFNGLCLEARCOLORPROC glClearColor;
    PFNGLCLEARSTENCILPROC glClearStencil;
    PFNGLCLEARPROC glClear;
    PFNGLCOLORMASKPROC glColorMask;

    PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;
    PFNGLGETQUERYOBJECTUI64VEXTPROC glGetQueryObjectui64vEXT;

    PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
#endif

#if NS_RENDERER_USE_WGL || NS_RENDERER_USE_EGL
    PFNGLGETSTRINGIPROC glGetStringi;

    PFNGLGENQUERIESPROC glGenQueries;
    PFNGLDELETEQUERIESPROC glDeleteQueries;
    PFNGLBEGINQUERYPROC glBeginQuery;
    PFNGLENDQUERYPROC glEndQuery;
    PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;

    PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
    PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
#endif

    void (GL_APIENTRYP GetQueryObjectui64v_)(GLuint id, GLenum pname, GLuint64 *params);

    GLuint mQueries[6];
    uint32_t mReadQuery;
    uint32_t mWriteQuery;
    float mGPUTime;

    Noesis::Ptr<Noesis::RenderDevice> mDevice;

    NS_DECLARE_REFLECTION(GLRenderContext, RenderContext)
};

}

#endif
