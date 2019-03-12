////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus


#include <NsApp/Application.h>
#include <NsApp/ApplicationLauncher.h>
#include <NsApp/ApplicationLauncherApi.h>
#include <NsApp/CommandLine.h>
#include <NsApp/Display.h>
#include <NsApp/DisplayApi.h>
#include <NsApp/DisplayLauncher.h>
#include <NsApp/DisplayLauncherApi.h>
#include <NsApp/EmbeddedFontProvider.h>
#include <NsApp/EmbeddedTextureProvider.h>
#include <NsApp/EmbeddedXamlProvider.h>
#include <NsApp/FileTextureProvider.h>
#include <NsApp/Launcher.h>
#include <NsApp/LauncherApi.h>
#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/Window.h>
#include <NsCore/AssignableFromType.h>
#include <NsCore/Atomic.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/BaseObject.h>
#include <NsCore/BaseRefCounted.h>
#include <NsCore/Bitset.h>
#include <NsCore/Boxing.h>
#include <NsCore/Category.h>
#include <NsCore/CompilerSettings.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/ComponentFactory.h>
#include <NsCore/CpuProfiler.h>
#include <NsCore/Delegate.h>
#include <NsCore/Deque.h>
#include <NsCore/DynamicCast.h>
#include <NsCore/Error.h>
#include <NsCore/File.h>
#include <NsCore/FileStream.h>
#include <NsCore/Find.h>
#include <NsCore/FixedAllocator.h>
#include <NsCore/FixedString.h>
#include <NsCore/FixedVector.h>
#include <NsCore/Hash.h>
#include <NsCore/HashMap.h>
#include <NsCore/HashSet.h>
#include <NsCore/HighResTimer.h>
#include <NsCore/IdOf.h>
#include <NsCore/Interface.h>
#include <NsCore/Kernel.h>
#include <NsCore/KernelApi.h>
#include <NsCore/List.h>
#include <NsCore/Log.h>
#include <NsCore/Map.h>
#include <NsCore/MemProfiler.h>
#include <NsCore/Memory.h>
#include <NsCore/MemoryAllocator.h>
#include <NsCore/MemoryManager.h>
#include <NsCore/MemoryStream.h>
#include <NsCore/MetaData.h>
#include <NsCore/NSTLForwards.h>
#include <NsCore/NSTLMemoryAllocator.h>
#include <NsCore/NSTLPoolAllocator.h>
#include <NsCore/Noesis.h>
#include <NsCore/NsFactory.h>
#include <NsCore/Nullable.h>
#include <NsCore/NullableApi.h>
#include <NsCore/Package.h>
#include <NsCore/PlatformSettings.h>
#include <NsCore/Profiler.h>
#include <NsCore/Ptr.h>
#include <NsCore/Queue.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/ReflectionHelper.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsCore/ReflectionImplementEnum.h>
#include <NsCore/ReflectionRegistry.h>
#include <NsCore/RegisterComponent.h>
#include <NsCore/ScopeGuard.h>
#include <NsCore/Set.h>
#include <NsCore/Stack.h>
#include <NsCore/Stream.h>
#include <NsCore/String.h>
#include <NsCore/StringUtils.h>
#include <NsCore/Symbol.h>
#include <NsCore/TimelineProfiler.h>
#include <NsCore/Type.h>
#include <NsCore/TypeClass.h>
#include <NsCore/TypeClassBuilder.h>
#include <NsCore/TypeClassCreator.h>
#include <NsCore/TypeClassCreatorEmpty.h>
#include <NsCore/TypeCollection.h>
#include <NsCore/TypeCreate.h>
#include <NsCore/TypeEnum.h>
#include <NsCore/TypeEnumCreator.h>
#include <NsCore/TypeEnumHelper.h>
#include <NsCore/TypeEnumImpl.h>
#include <NsCore/TypeId.h>
#include <NsCore/TypeInfo.h>
#include <NsCore/TypeMeta.h>
#include <NsCore/TypeMetaData.h>
#include <NsCore/TypeOf.h>
#include <NsCore/TypeOfBase.h>
#include <NsCore/TypeOfBasic.h>
#include <NsCore/TypeOfForward.h>
#include <NsCore/TypeOfPointer.h>
#include <NsCore/TypePointer.h>
#include <NsCore/TypeProperty.h>
#include <NsCore/TypePropertyImpl.h>
#include <NsCore/TypePropertyUtil.h>
#include <NsCore/UTF8.h>
#include <NsCore/ValueHelper.h>
#include <NsCore/Vector.h>
#include <NsCore/Version.h>
#include <NsDrawing/Color.h>
#include <NsDrawing/CornerRadius.h>
#include <NsDrawing/Point.h>
#include <NsDrawing/Rect.h>
#include <NsDrawing/SVGPath.h>
#include <NsDrawing/Size.h>
#include <NsDrawing/Thickness.h>
#include <NsDrawing/TypesApi.h>
#include <NsGui/AdornerDecorator.h>
#include <NsGui/AmbientPropertyMetaData.h>
#include <NsGui/AmbientStack.h>
#include <NsGui/Animatable.h>
#include <NsGui/Animation.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/AnimationClock.h>
#include <NsGui/AnimationTimeline.h>
#include <NsGui/AnimationUsingKeyFrames.h>
#include <NsGui/ApplicationCommands.h>
#include <NsGui/BackEase.h>
#include <NsGui/BaseAnimation.h>
#include <NsGui/BaseBinding.h>
#include <NsGui/BaseBindingExpression.h>
#include <NsGui/BaseButton.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/BaseDefinition.h>
#include <NsGui/BaseDictionary.h>
#include <NsGui/BaseMenu.h>
#include <NsGui/BaseSetter.h>
#include <NsGui/BaseTextBox.h>
#include <NsGui/BaseTrigger.h>
#include <NsGui/BaseValueConverter.h>
#include <NsGui/BeginStoryboard.h>
#include <NsGui/Binding.h>
#include <NsGui/BindingExpression.h>
#include <NsGui/BindingListener.h>
#include <NsGui/BindingOperations.h>
#include <NsGui/BitmapImage.h>
#include <NsGui/BitmapSource.h>
#include <NsGui/Bold.h>
#include <NsGui/Border.h>
#include <NsGui/BounceEase.h>
#include <NsGui/Brush.h>
#include <NsGui/Brushes.h>
#include <NsGui/BulletDecorator.h>
#include <NsGui/Button.h>
#include <NsGui/CachedFontProvider.h>
#include <NsGui/CachedFontProviderApi.h>
#include <NsGui/Canvas.h>
#include <NsGui/ChangedHandler.h>
#include <NsGui/CheckBox.h>
#include <NsGui/CircleEase.h>
#include <NsGui/Clock.h>
#include <NsGui/ClockController.h>
#include <NsGui/ClockGroup.h>
#include <NsGui/Collection.h>
#include <NsGui/CollectionView.h>
#include <NsGui/CollectionViewSource.h>
#include <NsGui/Colors.h>
#include <NsGui/ColumnDefinition.h>
#include <NsGui/CombinedGeometry.h>
#include <NsGui/ComboBox.h>
#include <NsGui/ComboBoxItem.h>
#include <NsGui/CommandBinding.h>
#include <NsGui/CommandData.h>
#include <NsGui/CommandManager.h>
#include <NsGui/ComponentCommands.h>
#include <NsGui/CompositeTransform.h>
#include <NsGui/CompositionUnderline.h>
#include <NsGui/Condition.h>
#include <NsGui/ContentControl.h>
#include <NsGui/ContentPresenter.h>
#include <NsGui/ContentPropertyMetaData.h>
#include <NsGui/ContextMenu.h>
#include <NsGui/ContextMenuService.h>
#include <NsGui/Control.h>
#include <NsGui/ControlTemplate.h>
#include <NsGui/ControllableStoryboardAction.h>
#include <NsGui/CoreApi.h>
#include <NsGui/CubicEase.h>
#include <NsGui/DashStyle.h>
#include <NsGui/DataTemplate.h>
#include <NsGui/DataTemplateSelector.h>
#include <NsGui/DataTrigger.h>
#include <NsGui/Decorator.h>
#include <NsGui/DependencyData.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/DependencyObjectValueData.h>
#include <NsGui/DependencyProperty.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/DependsOnAttributeMetaData.h>
#include <NsGui/DictionaryKeyPropertyMetaData.h>
#include <NsGui/DictionaryMap.h>
#include <NsGui/DiscreteKeyFrame.h>
#include <NsGui/DispatcherObject.h>
#include <NsGui/DockPanel.h>
#include <NsGui/DrawingContext.h>
#include <NsGui/Duration.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/EasingKeyFrame.h>
#include <NsGui/ElasticEase.h>
#include <NsGui/Element.h>
#include <NsGui/Ellipse.h>
#include <NsGui/EllipseGeometry.h>
#include <NsGui/Enums.h>
#include <NsGui/EventTrigger.h>
#include <NsGui/Events.h>
#include <NsGui/Expander.h>
#include <NsGui/ExponentialEase.h>
#include <NsGui/Expression.h>
#include <NsGui/FocusManager.h>
#include <NsGui/FontFamily.h>
#include <NsGui/FontProperties.h>
#include <NsGui/FontProvider.h>
#include <NsGui/FormattedText.h>
#include <NsGui/FrameworkElement.h>
#include <NsGui/FrameworkElementEvents.h>
#include <NsGui/FrameworkPropertyMetadata.h>
#include <NsGui/FrameworkTemplate.h>
#include <NsGui/Freezable.h>
#include <NsGui/FreezableCollection.h>
#include <NsGui/FreezableEventReason.h>
#include <NsGui/Geometry.h>
#include <NsGui/GeometryGroup.h>
#include <NsGui/GradientBrush.h>
#include <NsGui/GradientStop.h>
#include <NsGui/GradientStopCollection.h>
#include <NsGui/Grid.h>
#include <NsGui/GridLength.h>
#include <NsGui/GroupBox.h>
#include <NsGui/HandoffBehavior.h>
#include <NsGui/HeaderedContentControl.h>
#include <NsGui/HeaderedItemsControl.h>
#include <NsGui/HierarchicalDataTemplate.h>
#include <NsGui/Hyperlink.h>
#include <NsGui/ICollectionView.h>
#include <NsGui/ICommand.h>
#include <NsGui/ICommandSource.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IDictionary.h>
#include <NsGui/IDictionaryIndexer.h>
#include <NsGui/IEasingFunction.h>
#include <NsGui/IExpression.h>
#include <NsGui/IExpressionTypes.h>
#include <NsGui/IItemContainerGenerator.h>
#include <NsGui/ILayerManager.h>
#include <NsGui/IList.h>
#include <NsGui/IListIndexer.h>
#include <NsGui/IMarkupExtension.h>
#include <NsGui/INameScope.h>
#include <NsGui/INameScopeTypes.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/INotifyDictionaryChanged.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsGui/IPropertyValueConverter.h>
#include <NsGui/IRecyclingItemContainerGenerator.h>
#include <NsGui/IRenderProxyCreator.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IResourceKey.h>
#include <NsGui/IScrollInfo.h>
#include <NsGui/ISealable.h>
#include <NsGui/IStoryboard.h>
#include <NsGui/ITimeManager.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/IUITreeNodeTypes.h>
#include <NsGui/IValueConverter.h>
#include <NsGui/IView.h>
#include <NsGui/IXamlContext.h>
#include <NsGui/IXamlContextReceiver.h>
#include <NsGui/Image.h>
#include <NsGui/ImageBrush.h>
#include <NsGui/ImageSource.h>
#include <NsGui/Inline.h>
#include <NsGui/InputBinding.h>
#include <NsGui/InputEnums.h>
#include <NsGui/InputGesture.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/Italic.h>
#include <NsGui/ItemCollection.h>
#include <NsGui/ItemContainerGenerator.h>
#include <NsGui/ItemsControl.h>
#include <NsGui/ItemsPanelTemplate.h>
#include <NsGui/ItemsPresenter.h>
#include <NsGui/KeyBinding.h>
#include <NsGui/KeyFrame.h>
#include <NsGui/KeyGesture.h>
#include <NsGui/KeySpline.h>
#include <NsGui/KeyTime.h>
#include <NsGui/Keyboard.h>
#include <NsGui/KeyboardNavigation.h>
#include <NsGui/Label.h>
#include <NsGui/LayerManager.h>
#include <NsGui/Line.h>
#include <NsGui/LineBreak.h>
#include <NsGui/LineGeometry.h>
#include <NsGui/LinearGradientBrush.h>
#include <NsGui/LinearKeyFrame.h>
#include <NsGui/ListBox.h>
#include <NsGui/ListBoxItem.h>
#include <NsGui/LogicalTreeHelper.h>
#include <NsGui/Matrix3DProjection.h>
#include <NsGui/MatrixTransform.h>
#include <NsGui/Menu.h>
#include <NsGui/MenuItem.h>
#include <NsGui/MeshGeometry.h>
#include <NsGui/Mouse.h>
#include <NsGui/MouseState.h>
#include <NsGui/MultiDataTrigger.h>
#include <NsGui/MultiTrigger.h>
#include <NsGui/NameScope.h>
#include <NsGui/NameScopePropertyMetaData.h>
#include <NsGui/ObservableCollection.h>
#include <NsGui/Page.h>
#include <NsGui/Panel.h>
#include <NsGui/ParallelTimeline.h>
#include <NsGui/PasswordBox.h>
#include <NsGui/Path.h>
#include <NsGui/PauseStoryboard.h>
#include <NsGui/Pen.h>
#include <NsGui/PlaneProjection.h>
#include <NsGui/Popup.h>
#include <NsGui/PowerEase.h>
#include <NsGui/ProgressBar.h>
#include <NsGui/Projection.h>
#include <NsGui/PropertyMetadata.h>
#include <NsGui/PropertyPath.h>
#include <NsGui/ProviderValue.h>
#include <NsGui/QuadraticEase.h>
#include <NsGui/QuarticEase.h>
#include <NsGui/QuinticEase.h>
#include <NsGui/RadialGradientBrush.h>
#include <NsGui/RadioButton.h>
#include <NsGui/RangeBase.h>
#include <NsGui/Rectangle.h>
#include <NsGui/RectangleGeometry.h>
#include <NsGui/RelativeSource.h>
#include <NsGui/RenderOptions.h>
#include <NsGui/RepeatBehavior.h>
#include <NsGui/RepeatButton.h>
#include <NsGui/ResourceDictionary.h>
#include <NsGui/ResourceKeyString.h>
#include <NsGui/ResourceKeyType.h>
#include <NsGui/ResumeStoryboard.h>
#include <NsGui/RotateTransform.h>
#include <NsGui/RoutedCommand.h>
#include <NsGui/RoutedEvent.h>
#include <NsGui/RoutedUICommand.h>
#include <NsGui/RowDefinition.h>
#include <NsGui/Run.h>
#include <NsGui/ScaleTransform.h>
#include <NsGui/ScrollBar.h>
#include <NsGui/ScrollContentPresenter.h>
#include <NsGui/ScrollViewer.h>
#include <NsGui/Selector.h>
#include <NsGui/Separator.h>
#include <NsGui/Setter.h>
#include <NsGui/Shape.h>
#include <NsGui/SineEase.h>
#include <NsGui/SizeChangedInfo.h>
#include <NsGui/SkewTransform.h>
#include <NsGui/Slider.h>
#include <NsGui/SolidColorBrush.h>
#include <NsGui/Span.h>
#include <NsGui/SplineKeyFrame.h>
#include <NsGui/StackPanel.h>
#include <NsGui/StatusBar.h>
#include <NsGui/StatusBarItem.h>
#include <NsGui/StopStoryboard.h>
#include <NsGui/Storyboard.h>
#include <NsGui/StreamGeometry.h>
#include <NsGui/StreamGeometryContext.h>
#include <NsGui/Style.h>
#include <NsGui/TabControl.h>
#include <NsGui/TabItem.h>
#include <NsGui/TabPanel.h>
#include <NsGui/TemplateBindingExpression.h>
#include <NsGui/TemplateBindingExtension.h>
#include <NsGui/TemplatePartMetaData.h>
#include <NsGui/Text.h>
#include <NsGui/TextBlock.h>
#include <NsGui/TextBox.h>
#include <NsGui/TextElement.h>
#include <NsGui/TextProperties.h>
#include <NsGui/TextSelector.h>
#include <NsGui/TextureProvider.h>
#include <NsGui/TextureSource.h>
#include <NsGui/Thumb.h>
#include <NsGui/TickBar.h>
#include <NsGui/TileBrush.h>
#include <NsGui/TimeSpan.h>
#include <NsGui/Timeline.h>
#include <NsGui/TimelineGroup.h>
#include <NsGui/ToggleButton.h>
#include <NsGui/ToolBar.h>
#include <NsGui/ToolBarOverflowPanel.h>
#include <NsGui/ToolBarPanel.h>
#include <NsGui/ToolBarTray.h>
#include <NsGui/ToolTip.h>
#include <NsGui/ToolTipService.h>
#include <NsGui/Track.h>
#include <NsGui/Transform.h>
#include <NsGui/TransformGroup.h>
#include <NsGui/TranslateTransform.h>
#include <NsGui/TreeView.h>
#include <NsGui/TreeViewItem.h>
#include <NsGui/Trigger.h>
#include <NsGui/TriggerAction.h>
#include <NsGui/TypeConverterMetaData.h>
#include <NsGui/UIElement.h>
#include <NsGui/UIElementCollection.h>
#include <NsGui/UIElementData.h>
#include <NsGui/UIElementEvents.h>
#include <NsGui/UIPropertyMetadata.h>
#include <NsGui/Underline.h>
#include <NsGui/UniformGrid.h>
#include <NsGui/UpdateSourceTrigger.h>
#include <NsGui/UserControl.h>
#include <NsGui/ValueDestination.h>
#include <NsGui/ValueStorageManager.h>
#include <NsGui/ValueStorageManagerImpl.h>
#include <NsGui/ValueTargetProvider.h>
#include <NsGui/Viewbox.h>
#include <NsGui/VirtualizationCacheLength.h>
#include <NsGui/VirtualizingPanel.h>
#include <NsGui/VirtualizingStackPanel.h>
#include <NsGui/Visual.h>
#include <NsGui/VisualBrush.h>
#include <NsGui/VisualCollection.h>
#include <NsGui/VisualState.h>
#include <NsGui/VisualStateGroup.h>
#include <NsGui/VisualStateManager.h>
#include <NsGui/VisualTransition.h>
#include <NsGui/VisualTreeHelper.h>
#include <NsGui/WrapPanel.h>
#include <NsGui/XamlProvider.h>
#include <NsGui/XamlReaderProvider.h>
#include <NsGui/mscorlibTypes.h>
#include <NsMath/AABBox.h>
#include <NsMath/AABoxForward.h>
#include <NsMath/Constants.h>
#include <NsMath/Matrix.h>
#include <NsMath/MatrixForward.h>
#include <NsMath/Transform.h>
#include <NsMath/TransformForward.h>
#include <NsMath/Utils.h>
#include <NsMath/Vector.h>
#include <NsMath/VectorForward.h>
#include <NsMath/VectorMathApi.h>

#endif