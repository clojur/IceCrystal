////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/StringUtils.h>
#include <NsCore/UTF8.h>


#if defined(NS_PLATFORM_WINDOWS)
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <dirent.h>
    #include <errno.h>
#endif


namespace Noesis
{
////////////////////////////////////////////////////////////////////////////////////////////////////
bool FindFirst(const char* directory, const char* extension, FindData& findData)
{
#if defined(NS_PLATFORM_WINDOWS)
    char fullPath[PATH_MAX];
    String::Copy(fullPath, sizeof(fullPath), directory);
    String::Append(fullPath, sizeof(fullPath), "/*");
    String::Append(fullPath, sizeof(fullPath), extension);

    uint16_t u16str[PATH_MAX];
    uint32_t numChars = UTF8::UTF8To16(fullPath, u16str, PATH_MAX);
    NS_ASSERT(numChars <= PATH_MAX);

    WIN32_FIND_DATAW w32FindData;
    HANDLE h = FindFirstFileExW((LPCWSTR)u16str, FindExInfoBasic, &w32FindData,
        FindExSearchNameMatch, 0, 0);
    if (h != INVALID_HANDLE_VALUE)
    {
        numChars = UTF8::UTF16To8((uint16_t*)w32FindData.cFileName, findData.filename, PATH_MAX);
        NS_ASSERT(numChars <= PATH_MAX);
        String::Copy(findData.extension, sizeof(extension), extension);
        findData.handle = h;
        return true;
    }

    return false;

#else
    DIR* dir = opendir(directory);

    if (dir != 0)
    {
       String::Copy(findData.extension, sizeof(extension), extension);
       findData.handle = dir;

        if (FindNext(findData))
        {
            return true;
        }
        else
        {
            FindClose(findData);
            return false;
        }
    }

    return false;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool FindNext(FindData& findData)
{
#if defined(NS_PLATFORM_WINDOWS)
    WIN32_FIND_DATAW w32FindData;
    BOOL res = FindNextFileW(findData.handle, &w32FindData);
    
    if (res)
    {
        uint32_t n = UTF8::UTF16To8((uint16_t*)w32FindData.cFileName, findData.filename, PATH_MAX);
        NS_ASSERT(n <= PATH_MAX);
        return true;
    }

    return false;

#else
    DIR* dir = (DIR*)findData.handle;

    while (true)
    {
        dirent* entry = readdir(dir);

        if (entry != 0)
        {
            if (String::EndsWith(entry->d_name, findData.extension, IgnoreCase_True))
            {
                String::Copy(findData.filename, sizeof(findData.filename), entry->d_name);
                return true;
            }
        }
        else
        {
            return false;
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FindClose(FindData& findData)
{
#if defined(NS_PLATFORM_WINDOWS)
    BOOL r = ::FindClose(findData.handle);
    NS_ASSERT(r != 0);

#else
    DIR* dir = (DIR*)findData.handle;
    int r = closedir(dir);
    NS_ASSERT(r == 0);
#endif
}

}
