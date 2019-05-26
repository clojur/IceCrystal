#ifndef _Soul_MAIN_
#define _Soul_MAIN_

class MainFunction
{
public:
    typedef int (*mainFunction)(int argc, char* argv[]);

    MainFunction(const char* name, mainFunction f);
};

int mainFunction(int argc, char* argv[]);

#endif
