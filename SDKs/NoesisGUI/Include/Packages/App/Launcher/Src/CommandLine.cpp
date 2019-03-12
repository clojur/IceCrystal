////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/CommandLine.h>
#include <NsCore/StringUtils.h>
#include <NsCore/Error.h>
#include <NsCore/String.h>


using namespace Noesis;
using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
CommandLine::CommandLine(): mNumArgs(0), mArgs(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CommandLine::CommandLine(uint32_t argc, char const* const* argv): mNumArgs(argc), mArgs(argv)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CommandLine::GetNumArgs() const
{
    return mNumArgs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
char const* const* CommandLine::GetArgs() const
{
    return mArgs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* CommandLine::GetArg(uint32_t index) const
{
    NS_ASSERT(index < mNumArgs);
    return mArgs[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* CommandLine::FindOption(const char* option, const char* default_) const
{
    for (uint32_t i = 0; i < mNumArgs; i++)
    {
        const char* arg = mArgs[i];
        if (String::StartsWith(arg, "--") && String::Equals(option, arg + 2, IgnoreCase_True)
            &&  i + 1 < mNumArgs && *mArgs[i+1] != '-')
        {
            return mArgs[i + 1];
        }
    }

    return default_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool CommandLine::HasOption(const char* option) const
{
    for (uint32_t i = 0; i < mNumArgs; i++)
    {
        const char* arg = GetArg(i);
        if (String::StartsWith(arg, "--") && String::Equals(option, arg + 2, IgnoreCase_True))
        {
            return true;
        }
    }

    return false;
}
