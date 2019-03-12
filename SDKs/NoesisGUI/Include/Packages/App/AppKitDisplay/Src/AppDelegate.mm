
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "AppDelegate.h"
#include "AppKitDisplay.h"

#include <AppKit/NSMenu.h>
#include <AppKit/NSMenuItem.h>
#include <NsCore/Version.h>


using namespace Noesis;
using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation AppDelegate

NoesisApp::AppKitDisplay* display;

- (id)initWithDisplay:(NoesisApp::AppKitDisplay*)display_
{
    self = [super init];
    display = display_;
    return self;
}

- (void)about:(id)sender
{
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
        @"", @"Version",
        [NSString stringWithUTF8String:GetBuildVersion()], @"ApplicationVersion",
        @"Copyright \xc2\xa9 2009-2016 Noesis Technologies", @"Copyright", nil];
    [NSApp orderFrontStandardAboutPanelWithOptions: dict];
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    NSString* appName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    NSMenu* menu = [[NSMenu alloc] initWithTitle:@""];

    [menu addItemWithTitle:
        [NSString stringWithFormat: @"About %@", appName]
        action:@selector(about:) keyEquivalent:@""];
    [menu addItem:[NSMenuItem separatorItem]];

    NSMenuItem* servicesItem = [menu addItemWithTitle:@"Services" action:nil keyEquivalent:@""];
    NSMenu*    servicesMenu = [[NSMenu alloc] initWithTitle:@""];
    [servicesItem setSubmenu: servicesMenu];
    [NSApp setServicesMenu: servicesMenu];
    [menu addItem:[NSMenuItem separatorItem]];

    [menu addItemWithTitle:[NSString stringWithFormat: @"Hide %@", appName]
        action:@selector(hide:) keyEquivalent:@"h"];
    [[menu addItemWithTitle:@"Hide Others"
        action:@selector(hideOtherApplications:)
        keyEquivalent:@"h"] setKeyEquivalentModifierMask:(NSAlternateKeyMask|NSCommandKeyMask)];
    [menu addItemWithTitle:@"Show All"
        action:@selector(unhideAllApplications:) keyEquivalent:@""];

    [menu addItem:[NSMenuItem separatorItem]];
    [menu addItemWithTitle:[NSString stringWithFormat: @"Quit %@", appName]
        action:@selector(terminate:) keyEquivalent:@"q"];

    NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:@"" action:NULL keyEquivalent:@""];
    [item setSubmenu:menu];

    NSMenu* mainMenu = [[NSMenu alloc] initWithTitle:@""];
    [mainMenu addItem:item];
    [NSApp setMainMenu:mainMenu];

    [menu release];
    [servicesMenu release];
    [item release];
    [mainMenu release];
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    [[[NSApplication sharedApplication] mainWindow] close];
    return NSTerminateCancel;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    NSUInteger mask = NSAnyEventMask;
    NSDate* date = NSDate.distantPast;
    NSRunLoopMode mode = NSDefaultRunLoopMode;
    NSEvent* event;

    while (!display->mClosed)
    {
        @autoreleasepool
        {
            do
            {
                event = [NSApp nextEventMatchingMask:mask untilDate:date inMode:mode dequeue:YES];
                [NSApp sendEvent:event];
            }
            while (event != nil);

            if (!display->mClosed)
            {
                display->OnRender();
            }
        }
    }

    [NSApp stop: nil];
}

@end
