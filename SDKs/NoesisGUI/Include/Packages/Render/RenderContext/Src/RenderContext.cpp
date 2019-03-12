////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsRender/RenderContext.h>
#include <NsCore/Error.h>
#include <NsCore/Ptr.h>
#include <NsCore/Kernel.h>
#include <NsCore/Symbol.h>
#include <NsCore/NsFactory.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <EASTL/sort.h>


using namespace Noesis;
using namespace NoesisApp;


NS_DECLARE_SYMBOL(RenderContext)


////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderContext> NoesisApp::FindBestRenderContext()
{
    ComponentFactory::Vector v;
    ComponentFactory* factory = NsGetKernel()->GetComponentFactory();
    factory->EnumComponents(NSS(RenderContext), v);

    eastl::fixed_vector<Ptr<RenderContext>, 32> contexts;

    for (uint32_t i = 0; i < v.size(); i++)
    {
        contexts.push_back(NsCreateComponent<RenderContext>(v[i]));
    }

    eastl::sort(contexts.begin(), contexts.end(),
        [](const Ptr<RenderContext>& c0, const Ptr<RenderContext>& c1)
        {
            return c1->Score() < c0->Score();
        }
    );

    for (uint32_t i = 0; i < contexts.size(); i++)
    {
        if (contexts[i]->Validate())
        {
            return contexts[i];
        }
    }

    NS_FATAL("No valid render context found");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderContext> NoesisApp::CreateRenderContext(const char* name)
{
    ComponentFactory* factory = NsGetKernel()->GetComponentFactory();

    char id[256];
    String::FormatBuffer(id, 256, "%sRenderContext", name);

    return NsDynamicCast< Ptr<RenderContext> >(factory->CreateComponent(NsSymbol(id)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION_(RenderContext)
