////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1209]
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>
#include <NsCore/KernelApi.h>
#include <NsCore/ScopeGuard.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// NodeEnterLeave. RAII class for automatic enter/leave profiler nodes
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class NodeEnterLeave
{
public:
    NodeEnterLeave(NsSymbol sym): mPrevNode(T::GetCurrentNode())
    {
        NS_UNUSED(sym);

#if NS_INSTRUMENTED_ENABLED
        // If T::GetCurrentNode() returned 0 that means that the profiling is not active for
        //  the current thread. Instead of giving an error here we silently continue with the 
        //  execution to be friendly with non-profiled threads
        if (mPrevNode != 0 && mPrevNode->GetId() != sym)
        {
            T* childNode = &mPrevNode->AddChild(sym);
            T::SetCurrentNode(childNode);
            childNode->Enter();
            
            mRestore = true;
        }
        else
        {
            mRestore = false;
        }
#endif
    }
    
    ~NodeEnterLeave()
    {
#if NS_INSTRUMENTED_ENABLED
        if (mRestore)
        {
            T::GetCurrentNode()->Leave();
            T::SetCurrentNode(mPrevNode);
        }
#endif
    }

private:
    NodeEnterLeave(const NodeEnterLeave&);
    NodeEnterLeave& operator=(const NodeEnterLeave&);

    T* mPrevNode;
    bool mRestore;
};

}
