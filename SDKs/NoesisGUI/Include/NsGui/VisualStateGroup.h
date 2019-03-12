////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALSTATEGROUP_H__
#define __GUI_VISUALSTATEGROUP_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/AnimationApi.h>
#include <NsCore/String.h>
#include <NsCore/Map.h>


namespace Noesis
{

struct VisualStateChangedEventArgs;
class FrameworkElement;
class Storyboard;
class VisualStateManager;
class VisualState;
class VisualTransition;
template<class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::VisualState> VisualStateCollection;
typedef Noesis::TypedCollection<Noesis::VisualTransition> VisualTransitionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains mutually exclusive VisualState objects and VisualTransition objects that are used
/// to go from one state to another.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.visualstategroup.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API VisualStateGroup: public DependencyObject, public IUITreeNode
{
public:
    VisualStateGroup();
    ~VisualStateGroup();

    /// Gets the name of the VisualStateGroup.
    const char* GetName() const;

    /// Gets the collection of mutually exclusive VisualState objects
    VisualStateCollection* GetStates() const;

    /// Gets the collection of VisualTransition objects
    VisualTransitionCollection* GetTransitions() const;

    /// Gets or the state that is currently active
    //@{
    VisualState* GetCurrentState(FrameworkElement* fe) const;
    void SetCurrentState(FrameworkElement* fe, VisualState* state);
    //@}

    /// Finds a matching state with the same name.
    /// \note name can be null or empty
    VisualState* FindState(NsSymbol name) const;

    VisualTransition* FindTransition(VisualState* from, VisualState* to) const;

    /// Occurs after a control transitions into a different state
    //event EventHandler<VisualStateChangedEventArgs> CurrentStateChanged;
    
    /// Occurs when a control begins transitioning into a different state
    //event EventHandler<VisualStateChangedEventArgs> CurrentStateChanging;

    void UpdateAnimations(FrameworkElement* fe, Storyboard* storyboard1,
        Storyboard* storyboard2 = 0);
    
    Ptr<Storyboard> CreateTransitionStoryboard(FrameworkElement* root, 
        VisualState* newState, VisualTransition* transition);

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
    /// From DependencyObject
    //@{
    void OnInit() override;
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

private:
    void EnsureStates() const;
    void EnsureTransitions() const;

    void OnObjectDestroyed(DependencyObject* object);

private:
    NsString mName;
    mutable Ptr<VisualStateCollection> mStates;
    mutable Ptr<VisualTransitionCollection> mTransitions;

    struct ElementState
    {
        Ptr<VisualState> state;
        Ptr<Storyboard> storyboards[2];
    };

    typedef NsMap<FrameworkElement*, ElementState> CurrentStates;
    CurrentStates mCurrentStates;

    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(VisualStateGroup, DependencyObject)
};

NS_WARNING_POP

}

#endif
