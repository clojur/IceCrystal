#include "RenderContext.h"

Soul::RenderContext::RenderContext()
{
	_hGlrc = nullptr;
	_hDisplay = nullptr;
}

bool Soul::RenderContext::initRenderContext(HWINDOW hWnd)
{
	_hWnd = hWnd;
#ifdef WIN32
	_hDisplay = GetDC(_hWnd);
	//1.init glew

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = _T("SoulInit");
	wcex.hIconSm = 0;

	RegisterClassEx(&wcex);

	//create window
	HWND tempHwnd = CreateWindow(
		_T("SoulInit")
		, _T("SoulInitOpengl")
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, 0
		, CW_USEDEFAULT
		, 0
		, nullptr
		, nullptr
		, (HINSTANCE)GetModuleHandle(NULL)
		, nullptr);

	HDC tempHdc = GetDC(tempHwnd);
	PIXELFORMATDESCRIPTOR pfd;
	//test setup pixel format
	if (!SetPixelFormat(tempHdc, 1, &pfd))
		return false;
	//Create a temporary environment to initialize glew. 
	//After initialization, 
	//you can use wglChoosePixelFormatARB, wglCreateContextAttribsARB function etc.
	HGLRC tempHrc = wglCreateContext(tempHdc);

	//Only by setting up the current OpenGL environment 
	//can the glew library be initialized
	wglMakeCurrent(tempHdc,tempHrc);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		ork::Logger::ERROR_LOGGER->log("SoulInit","glewInit Failed!");
		ork::Logger::ERROR_LOGGER->flush();
		return false;
	}
	if (!WGLEW_ARB_create_context || !WGLEW_ARB_pixel_format)
	{
		return false;
	}
	wglMakeCurrent(nullptr,nullptr);
	wglDeleteContext(tempHrc);
	DestroyWindow(tempHwnd);

	//2.init opengl
	PIXELFORMATDESCRIPTOR pfd2;
	int nPixelFormat = -1;
	int nPixCount = 0;
	float fPixAttribs[] = { 0, 0 };
	int iPixAttribs[] = {
		WGL_SUPPORT_OPENGL_ARB, 1,//֧��OPENGL��Ⱦ
		WGL_DRAW_TO_WINDOW_ARB, 1,//���ظ�ʽ�������е�����
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,//��HW����
		WGL_COLOR_BITS_ARB, 32,//R,G,Bÿ����ɫ8bit
		WGL_DEPTH_BITS_ARB, 24,//24λ���
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,//˫����������
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,//pf should be RGBA type
		WGL_STENCIL_BITS_ARB, 8,//ģ�滺����8bit
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,//�������ز���
		WGL_SAMPLES_ARB, 4,//���ز�������Ϊ4
		0
	};
	//�µĲ�ѯ���ظ�ʽ�ĺ���
	wglChoosePixelFormatARB(_hDisplay, iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	//���ز���ʱ�����Ӳ����֧�־�ʹ������Ĵ���رն��ز���
	if (nPixelFormat == -1)
	{
		//try again without MSAA
		iPixAttribs[18] = 1;
		wglChoosePixelFormatARB(_hDisplay, iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	}
	//��ȡһ�����ظ�ʽ������������Ϊ��ǰ��������
	if (!SetPixelFormat(_hDisplay, nPixelFormat, &pfd2))
	{
		ork::Logger::ERROR_LOGGER->log("SoulInit", "setup pixel format Failed!");
		ork::Logger::ERROR_LOGGER->flush();
		return false;
	}
	GLint attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,//���汾3
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,//�ΰ汾3
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//Ҫ�󷵻ؼ���ģʽ����,�����ָ����ָ��ΪWGL_CONTEXT_CORE_PROFILE_BIT_ARB�᷵��ֻ�������Ĺ��ܵĻ���
		0
	};

	_hGlrc = wglCreateContextAttribsARB(_hDisplay, 0, attribs);
	if (_hGlrc == NULL)
	{
		ork::Logger::ERROR_LOGGER->log("SoulInit", "create opengl3.3 failed!\nattempt create opengl3.2");
		ork::Logger::ERROR_LOGGER->flush();
		attribs[3] = 2;
		_hGlrc = wglCreateContextAttribsARB(_hDisplay, 0, attribs);
		if (_hGlrc == NULL)
		{
			ork::Logger::ERROR_LOGGER->log("SoulInit", "create opengl3.2 failed!");
			ork::Logger::ERROR_LOGGER->flush();
			return false;
		}
	}
	wglMakeCurrent(_hDisplay, _hGlrc);
#endif // WIN32

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	
	return true;
}

void Soul::RenderContext::closeRenderContext()
{
#ifdef WIN32
	if (_hGlrc)
	{
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(_hGlrc);
		_hGlrc = nullptr;
	}
	if (_hDisplay)
	{
		ReleaseDC(_hWnd, _hDisplay);
		_hDisplay = nullptr;
	}
#endif

}

void Soul::RenderContext::swapBuffers()
{
#ifdef WIN32
	SwapBuffers(_hDisplay);
#endif
}
