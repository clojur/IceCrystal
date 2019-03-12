////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DISCRETEKEYFRAME_H__
#define __GUI_DISCRETEKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/KeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

template<class T> const char* DiscreteKeyFrameIdOf();

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the T value of the previous key frame to its own Value using discrete 
/// interpolation. Existing types are:
///
/// DiscreteBooleanKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretebooleankeyframe.aspx
///
///
/// DiscreteDoubleKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretedoublekeyframe.aspx
///
///
/// DiscreteInt16KeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discreteint16keyframe.aspx
///
///
/// DiscreteInt32KeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discreteint32keyframe.aspx
///
///
/// DiscreteColorKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretecolorkeyframe.aspx
///
///
/// DiscretePointKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretepointkeyframe.aspx
///
///
/// DiscreteRectKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discreterectkeyframe.aspx
///
///
/// DiscreteSizeKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretesizekeyframe.aspx
///
///
/// DiscreteThicknessKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretethicknesskeyframe.aspx
///
///
/// DiscreteObjectKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discreteobjectkeyframe.aspx
///
///
/// DiscreteStringKeyFrame:
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretestringkeyframe.aspx
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class NS_GUI_ANIMATION_API DiscreteKeyFrame: public KeyFrame<T>
{
public:
    DiscreteKeyFrame();
    ~DiscreteKeyFrame();

    /// From Freezable
    //@{
    Ptr<DiscreteKeyFrame<T>> Clone() const;
    Ptr<DiscreteKeyFrame<T>> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

    /// From KeyFrame
    //@{
    typedef typename Noesis::Param<T>::Type ParamType;
    T InterpolateValueCore(ParamType baseValue, float keyFrameProgress);
    //@}

    NS_DECLARE_REFLECTION(DiscreteKeyFrame, KeyFrame<T>)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef DiscreteKeyFrame<bool> DiscreteBooleanKeyFrame;
typedef DiscreteKeyFrame<float> DiscreteDoubleKeyFrame;
typedef DiscreteKeyFrame<int16_t> DiscreteInt16KeyFrame;
typedef DiscreteKeyFrame<int32_t> DiscreteInt32KeyFrame;
typedef DiscreteKeyFrame<Noesis::Color> DiscreteColorKeyFrame;
typedef DiscreteKeyFrame<Noesis::Point> DiscretePointKeyFrame;
typedef DiscreteKeyFrame<Noesis::Rect> DiscreteRectKeyFrame;
typedef DiscreteKeyFrame<Noesis::Size> DiscreteSizeKeyFrame;
typedef DiscreteKeyFrame<Noesis::Thickness> DiscreteThicknessKeyFrame;
typedef DiscreteKeyFrame<Ptr<Noesis::BaseComponent> > DiscreteObjectKeyFrame;
typedef DiscreteKeyFrame<NsString> DiscreteStringKeyFrame;

////////////////////////////////////////////////////////////////////////////////////////////////////
template<> inline const char* DiscreteKeyFrameIdOf<BooleanKeyFrame>() 
{ return "DiscreteBooleanKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<DoubleKeyFrame>() 
{ return "DiscreteDoubleKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<Int16KeyFrame>() 
{ return "DiscreteInt16KeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<Int32KeyFrame>() 
{ return "DiscreteInt32KeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<ColorKeyFrame>() 
{ return "DiscreteColorKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<PointKeyFrame>() 
{ return "DiscretePointKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<RectKeyFrame>() 
{ return "DiscreteRectKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<SizeKeyFrame>() 
{ return "DiscreteSizeKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<ThicknessKeyFrame>() 
{ return "DiscreteThicknessKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<ObjectKeyFrame>() 
{ return "DiscreteObjectKeyFrame"; }
template<> inline const char* DiscreteKeyFrameIdOf<StringKeyFrame>() 
{ return "DiscreteStringKeyFrame"; }

NS_WARNING_POP

}

#endif
