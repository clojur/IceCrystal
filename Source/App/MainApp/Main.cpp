#include <vector>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include<windows.h>
#include "Main.h"

#include "WindowsApplication.h"

using namespace std;

class MainFunctions
{
public:
    vector<const char*> names;

    vector<MainFunction::mainFunction> functions;
};

static MainFunctions *mainFunctions = NULL;

MainFunction::MainFunction(const char* name, mainFunction f)
{
    if (mainFunctions == NULL) {
        mainFunctions = new MainFunctions();
    }
    mainFunctions->names.push_back(name);
    mainFunctions->functions.push_back(f);
}

int mainFunction(int argc, char* argv[])
{
    assert(mainFunctions != NULL);

    char funcName[50];

    if (argc > 1) 
	{
        sprintf(funcName, "%s", argv[1]);
    } 
	else 
	{
        sprintf(funcName, "test");
    }

    for (unsigned int i = 0; i < mainFunctions->names.size(); ++i) 
	{
        if (strcmp(funcName, mainFunctions->names[i]) == 0) {
            return mainFunctions->functions[i](argc, argv);
        }
    }
    printf("Unknown command line argument '%s'\n", argv[1]);
    printf("Must be one of:\n");

    for (unsigned int i = 0; i < mainFunctions->names.size(); ++i) 
	{
        printf("%s\n", mainFunctions->names[i]);
    }
    return 0;
}

int main(int argc, char* argv[])
{

#ifdef WIN32
	/*set current work path*/
	char strPath[MAX_PATH] = {};

	GetModuleFileName(NULL, strPath, MAX_PATH);
	std::string workDir(strPath);
	int n = workDir.rfind('\\');
	workDir.erase(n, workDir.size() - n);
	SetCurrentDirectory(workDir.c_str());


	//强行指定运行程序名和资源目录
	argc += 2;
	char exeName[] = "renderglfw";
	//char exeName[] = "renderWin32";
	argv[1] = exeName;
	char resPath[] = "../../Source/App/ResourceFiles";
	argv[2] = resPath;
#endif // WIN32

return mainFunction(argc, argv);
}
