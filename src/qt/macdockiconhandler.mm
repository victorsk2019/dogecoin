// Copyright (c) 2011-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "macdockiconhandler.h"

#include <AppKit/AppKit.h>
#include <objc/runtime.h>

<<<<<<< HEAD
#include <Cocoa/Cocoa.h>
#include <AppKit/AppKit.h>
#include <objc/runtime.h>
=======
static MacDockIconHandler *s_instance = nullptr;
>>>>>>> 1.21-dev

bool dockClickHandler(id self, SEL _cmd, ...) {
    Q_UNUSED(self)
    Q_UNUSED(_cmd)

<<<<<<< HEAD
    s_instance->handleDockIconClickEvent();

    // Return NO (false) to suppress the default OS X actions
=======
    Q_EMIT s_instance->dockIconClicked();

    // Return NO (false) to suppress the default macOS actions
>>>>>>> 1.21-dev
    return false;
}

void setupDockClickHandler() {
    Class delClass = (Class)[[[NSApplication sharedApplication] delegate] class];
    SEL shouldHandle = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
<<<<<<< HEAD
    if (class_getInstanceMethod(delClass, shouldHandle))
        class_replaceMethod(delClass, shouldHandle, (IMP)dockClickHandler, "B@:");
    else
        class_addMethod(delClass, shouldHandle, (IMP)dockClickHandler,"B@:");
=======
    class_replaceMethod(delClass, shouldHandle, (IMP)dockClickHandler, "B@:");
>>>>>>> 1.21-dev
}

MacDockIconHandler::MacDockIconHandler() : QObject()
{
    setupDockClickHandler();
}

MacDockIconHandler *MacDockIconHandler::instance()
{
    if (!s_instance)
        s_instance = new MacDockIconHandler();
    return s_instance;
}

void MacDockIconHandler::cleanup()
{
    delete s_instance;
}

/**
 * Force application activation on macOS. With Qt 5.5.1 this is required when
 * an action in the Dock menu is triggered.
 * TODO: Define a Qt version where it's no-longer necessary.
 */
void ForceActivation()
{
    [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
}

/**
 * Force application activation on macOS. With Qt 5.5.1 this is required when
 * an action in the Dock menu is triggered.
 * TODO: Define a Qt version where it's no-longer necessary.
 */
void ForceActivation()
{
    [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
}

