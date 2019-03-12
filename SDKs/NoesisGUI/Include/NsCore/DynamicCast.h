////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_DYNAMICCAST_H__
#define __CORE_DYNAMICCAST_H__


#include <NsCore/Noesis.h>


namespace Noesis { template<class T> class Ptr; }


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Functions for dynamic casting between pointers:
///
///  FrameworkElement* element = view->GetContent();
///  Button* button = NsDynamicCast<Button*>(element);
///
/// There are equivalent funtions for Ptr<>, taking and returning Ptr<>:
///
///  Ptr<FrameworkElement> xaml = GUI::LoadXaml<FrameworkElement>("Settings.xaml");
///  Ptr<Grid> grid = NsDynamicCast<Ptr<Grid>>(xaml);
///
/// NsStaticCast performs a NsDynamicCast in 'Debug' and just a static_cast in 'Release'
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class CastT, class SourceT> CastT NsDynamicCast(SourceT ptr);
template<class CastT, class SourceT> CastT NsStaticCast(SourceT ptr);
template<class CastT, class SourceT> CastT NsDynamicCast(const Noesis::Ptr<SourceT>& ptr);
template<class CastT, class SourceT> CastT NsStaticCast(const Noesis::Ptr<SourceT>& ptr);


#include <NsCore/DynamicCast.inl>

#endif
