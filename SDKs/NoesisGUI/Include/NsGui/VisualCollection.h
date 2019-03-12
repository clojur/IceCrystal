////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALCOLLECTION_H__
#define __GUI_VISUALCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Collection.h>


namespace Noesis
{

class Visual;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// VisualCollection. A collection of Visual objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API VisualCollection: public Collection
{
public:
    /// Default constructor
    VisualCollection();

    /// Constructor
    VisualCollection(Visual* visualParent);

    /// Destructor
    ~VisualCollection();

    /// Gets or sets visual parent
    //@{
    Visual* GetVisualParent() const;
    void SetVisualParent(Visual* visualParent);
    //@}

protected:
    /// From Collection
    //@{
    void OnItemAdded(BaseComponent* item, uint32_t index) override;
    void OnItemRemoved(BaseComponent* item, uint32_t index) override;
    bool CheckItem(BaseComponent* item) const override;
    //@}

private:
    void ResetChildren();

private:
    Visual* mVisualParent;

    NS_DECLARE_REFLECTION(VisualCollection, Collection)
};

}

#endif
