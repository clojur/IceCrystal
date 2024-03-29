#ifndef _SoulInit_H
#define _SoulInit_H

#include"Platform.h"
#include "Soul/core/Logger.h"

namespace Soul
{
	class Soul_API RenderContext
	{
	public:
		RenderContext();
		bool initRenderContext(HWINDOW hWnd);
		void closeRenderContext();
		void swapBuffers();

	private:
		HDISPLAY _hDisplay;
		HWINDOW  _hWnd;
		WGLRC	 _hGlrc;

	};
};


#endif // !_SoulInit_H
