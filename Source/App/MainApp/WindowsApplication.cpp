#include "WindowsApplication.h"
#include <tchar.h>
Soul::WindowsApplication::WindowsApplication()
{
	_hWnd = 0;
}
bool Soul::WindowsApplication::createWindow(int width, int height, HINSTANCE hInstance)
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
		, nullptr);
	if (_hWnd == 0)
		return false;

	ShowWindow(_hWnd, SW_SHOW);
	//update show
	UpdateWindow(_hWnd);

	return true;
}

void Soul::WindowsApplication::main(int argc, char * argv[])
{
	MSG msg = { 0 };

	//Main msg loop
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Soul::WindowsApplication::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd,&ps);
		EndPaint(hWnd,&ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return S_OK;
}
