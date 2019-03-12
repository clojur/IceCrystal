////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASESETTER_H__
#define __GUI_BASESETTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class BaseSetter;
template<class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::BaseSetter> BaseSetterCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for value setters.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.setterbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseSetter: public BaseComponent, public IUITreeNode
{
public:
    NS_DISABLE_COPY(BaseSetter)

    BaseSetter();
    virtual ~BaseSetter() = 0;

    static void SealSetters(BaseSetterCollection* setters);

    // Seals the setters
    virtual void Seal();

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(IResourceKey* key,
        bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    bool mIsSealed;

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(BaseSetter, BaseComponent)
};

NS_WARNING_POP

}

#endif
