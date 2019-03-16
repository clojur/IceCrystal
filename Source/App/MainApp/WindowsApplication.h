#ifndef _WindowsApplication_H
#define _WindowsApplication_H

#include"Application.h"
#include <windows.h>

namespace Soul
{
	class WindowsApplication :public Application
	{
	public:
		HWND _hWnd;
	public:
		WindowsApplication();
	public:
		//create window
		virtual bool createWindow(int width, int height, HINSTANCE hInstance);

		//main
		virtual void main(int argc, char* argv[]);

	protected:
		static LRESULT CALLBACK wndProc(HWND hWnd,UINT uInt,WPARAM message,LPARAM lParam);
	};
};

#endif // !_WindowsApplication_H
