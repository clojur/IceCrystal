#ifndef _WindowsApplication_H
#define _WindowsApplication_H

#include"Application.h"
#include <ork/core/Logger.h>
#include <ork/common/RenderContext.h>
#include <ork/render/FrameBuffer.h>
namespace Soul
{
	class WindowsApplication :public Application
	{
	public:
		HWND _hWnd;
		RenderContext _context;
		ork::ptr<ork::Mesh<ork::vec2f, unsigned int>> _mesh;
		ork::ptr<ork::Program> _program;
	public:
		WindowsApplication();
	public:
		//create window
		virtual bool createWindow(int width, int height, INSTANCE hInstance);

		//main
		virtual void main(int argc, char* argv[]);

		void creatScene();
		void render();

	LRESULT  eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		ork::ptr<ork::FrameBuffer> _fb;
	};
};

#endif // !_WindowsApplication_H
