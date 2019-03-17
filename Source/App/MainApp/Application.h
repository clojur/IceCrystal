#ifndef _SoulApplication_H
#define _SoulApplicationApp_H
#include "ork/common/Platform.h"
namespace Soul
{
	class Application
	{
	public:

		//create window
		virtual bool createWindow(int width,int height, INSTANCE hInstance)=0;

		//main
		virtual void main(int argc,char* argv[]) = 0;

	protected:

	};
};

#endif // !__CSoulApp__H
