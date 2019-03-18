#include "WindowsApplication.h"
#include "Main.h"

#include "stbi/stb_image.h"
#include "ork/common/GL/glew.h"
#include <tchar.h>
Soul::WindowsApplication::WindowsApplication()
{
	_hWnd = 0;
}
bool Soul::WindowsApplication::createWindow(int width, int height, INSTANCE hInstance)
{
	//register windows class
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = _T("Soul.Earth");
	wcex.hIconSm = 0;

	RegisterClassEx(&wcex);

	//create window
	_hWnd = CreateWindow(
		_T("Soul.Earth")
		, _T("SoulApplication")
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, 0
		, CW_USEDEFAULT
		, 0
		, nullptr
		, nullptr
		, hInstance
		, this);
	if (_hWnd == 0)
		return false;

	ShowWindow(_hWnd, SW_SHOW);
	//update show
	UpdateWindow(_hWnd);

	if (!_context.initRenderContext(_hWnd))
	{
		DestroyWindow(_hWnd);
		return false;
	}

	creatScene();
	return true;
}

void Soul::WindowsApplication::main(int argc, char * argv[])
{
	MSG msg = { 0 };

	//Main msg loop
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		render();
	}

	_context.closeRenderContext();
}

void Soul::WindowsApplication::creatScene()
{
	_mesh = new ork::Mesh<ork::vec2f, unsigned int>(ork::MeshMode::TRIANGLE_STRIP,ork::MeshUsage::GPU_STATIC);
	_mesh->addAttributeType(0, 2, ork::AttributeType::A32F, false);
	_mesh->addVertex(ork::vec2f(-1,-1));
	_mesh->addVertex(ork::vec2f(+1, -1));
	_mesh->addVertex(ork::vec2f(-1, +1));
	_mesh->addVertex(ork::vec2f(+1, +1));

	unsigned char data[16] = {
		0,255,0,255,
		0,255,0,255,
		0,255,0,255,
		0,255,0,255
	};

	ork::ptr<ork::Texture2D> tex = new ork::Texture2D(
	 4
	,4
	,ork::TextureInternalFormat::R8
	,ork::TextureFormat::RGB
	,ork::PixelType::UNSIGNED_BYTE
	,ork::Texture::Parameters().mag(ork::TextureFilter::NEAREST)
	,ork::Buffer::Parameters(),ork::CPUBuffer(data));

	_program = new ork::Program(new ork::Module(330,NULL,
		"uniform sampler2D sampler;\n\
		 uniform vec2 scale;\n\
		 layout(location=0) out vec4 data;\n\
		 void main()\n\
		 {\n\
			data=texture(sampler,gl_FragCoord.xy*scale).rrrr;\n\
		 }\n"));

	_program->getUniformSampler("sampler")->set(tex);
}

void Soul::WindowsApplication::render()
{
	
	ork::ptr<ork::FrameBuffer> fb = ork::FrameBuffer::getDefault();
	fb->clear(true,true,false);
	fb->setClearColor(ork::vec4f(1,0,0,1));
	fb->draw(_program,*_mesh);

	_context.swapBuffers();
}

void Soul::WindowsApplication::reSize(int x, int y)
{
	RECT rect;
	GetWindowRect((HWND)_hWnd, &rect);
	//ork::FrameBuffer::getDefault()->setViewport(ork::vec4<GLint>(0, 0, rect.right, rect.bottom));
	//_program->getUniform2f("scale")->set(ork::vec2f(1.0f / rect.right, 1.0f / rect.bottom));
}

LRESULT Soul::WindowsApplication::eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetWindowRect((HWND)_hWnd, &rect);
		reSize(rect.right, rect.bottom);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return S_OK;
}

LRESULT Soul::WindowsApplication::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_CREATE == message)
	{
		CREATESTRUCT* pSTRUCT = (CREATESTRUCT*)lParam;
		WindowsApplication* pApp = (WindowsApplication*)pSTRUCT->lpCreateParams;

#ifndef GWL_USERDATA
#define GWL_USERDATA (-21)
#endif

		SetWindowLongPtr(hWnd,GWL_USERDATA, (LONG)pApp);
		return pApp->eventProc(hWnd,WM_CREATE,wParam,lParam);
	}
	else
	{
		WindowsApplication* pApp =(WindowsApplication*)GetWindowLongPtr(hWnd, GWL_USERDATA);
		if (pApp)
		{
			return  pApp->eventProc(hWnd,message,wParam,lParam);
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}

int RenderWin32(int argc, char* argv[])
{
	Soul::WindowsApplication app;

	if (!app.createWindow(800, 600, (HINSTANCE)GetModuleHandle(NULL)))
	{
		return 0;
	}
	app.main(argc, argv);
	return 0;
}

MainFunction renderWin32Main("renderWin32", RenderWin32);