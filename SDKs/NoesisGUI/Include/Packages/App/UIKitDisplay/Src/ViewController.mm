////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#import "ViewController.h"
#import "View.h"
#import "UIKitDisplay.h"


using namespace Noesis;
using namespace NoesisApp;


@interface ViewController()
{
UIKitDisplay* display;
}
@end


@implementation ViewController

-(id)initWithDisplay:(UIKitDisplay*)display_
{
	self = [super init];
	self.wantsFullScreenLayout = YES;
    display = display_;
    return self;
}

- (void)loadView
{
    ::View* view = [[[::View alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    view->display = display;
    view.multipleTouchEnabled = YES;
    self.view = view;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    display->OnSizeChanged(display->GetClientWidth(), display->GetClientHeight());
}

@end
