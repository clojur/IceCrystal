////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UIELEMENTCOLLECTION_H__
#define __GUI_UIELEMENTCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/VisualCollection.h>
#include <NsGui/UIElement.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class FrameworkElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// UIElementCollection. Represents a collection of UIElements.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UIElementCollection: public VisualCollection
{
public:
    /// Default constructor
    UIElementCollection();

    /// Constructor
    UIElementCollection(Visual* visualParent, FrameworkElement* logicalParent);

    /// Destructor
    ~UIElementCollection();

    /// Gets or sets logical parent
    //@{
    FrameworkElement* GetLogicalParent() const;
    void SetLogicalParent(FrameworkElement* logicalParent);
    //@}

protected:
    /// From Collection
    //@{
    void OnItemAdded(BaseComponent* item, uint32_t index) override;
    void OnItemRemoved(BaseComponent* item, uint32_t index) override;
    bool CheckItem(BaseComponent* item) const override;
    //@}

private:
    void InvalidateVisualParentMeasure();
    void ResetChildren();

private:
    FrameworkElement* mLogicalParent;

    NS_DECLARE_REFLECTION(UIElementCollection, VisualCollection)
};

}

#endif
