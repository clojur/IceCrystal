#ifndef _WindowsApplication_H
#define _WindowsApplication_H

#include"Application.h"
#include <Soul/core/Logger.h>
#include <Soul/common/RenderContext.h>
#include <Soul/render/FrameBuffer.h>
namespace Soul
{
	class WindowsApplication :public Application
	{
	public:
		HWND _hWnd;
		RenderContext _context;
		Soul::ptr<Soul::Mesh<Soul::vec2f, unsigned int>> _mesh;
		Soul::ptr<Soul::Program> _program;
	public:
		WindowsApplication();
	public:
		//create window
		virtual bool createWindow(int width, int height, INSTANCE hInstance);

		//main
		virtual void main(int argc, char* argv[]);

		void creatScene();
		void resize();
		void render();

	LRESULT  eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		Soul::ptr<Soul::FrameBuffer> _fb;
		int _width;
		int _height;
	};
};

#endif // !_WindowsApplication_H
