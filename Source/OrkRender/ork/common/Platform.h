#pragma once

#include "OrkMacro.h"
#define SOUL_PLATFORM_WIN32     1
#define SOUL_PLATFORM_LINUX     2
#define SOUL_PLATFORM_APPLE     3
#define SOUL_PLATFORM_ANDRIOD   4

#define SOUL_PATH_LENGHT        1024
#define SOUL_CALL               __stdcall


#define SOUL_PLATFORM   SOUL_PLATFORM_WIN32

#if SOUL_PLATFORM == SOUL_PLATFORM_WIN32
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <tchar.h>
#include <windows.h>


#include "GL/glew.h"
#include "GL/wglew.h"

#include <gl/GL.h>
#include <gl/GLu.h>

#ifdef SOULSOUL3D_EXPORTS
#define SOUL3D_API __declspec(dllexport)
#else
#define SOUL3D_API __declspec(dllimport)
#endif


#define  HDISPLAY    HDC
#define  HWINDOW     HWND
#define  HLIBRARY    HMODULE
#define  INSTANCE    HINSTANCE
#define  WGLRC		 HGLRC


typedef const char* STRING;


#elif SOUL_PLATFORM == SOUL_PLATFORM_LINUX || SOUL_PLATFORM == SOUL_PLATFORM_ANDRIOD

#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#   include <wchar.h>
#   include <X11/X.h>
#   include <X11/Xlib.h>
#   include "glew.h"
#   include <GL/gl.h>
#   include <GL/glx.h>
#   include <GL/glu.h>
#   include <dlfcn.h>
#   include <pthread.h>
#   include <semaphore.h>
#   include <unistd.h>
#   include <sys/time.h>

#ifdef SOULSOUL3D_EXPORTS
#define SOUL3D_API
#else
#define SOUL3D_API

#   define  HDISPLAY    void*
#   define  HWINDOW     void*
#   define  HLIBRARY    void*

#endif

#endif