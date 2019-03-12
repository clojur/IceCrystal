////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


//#undef NS_MINIMUM_LOG_LEVEL
//#define NS_MINIMUM_LOG_LEVEL 0


#include "AndroidDisplay.h"

#include <NsCore/String.h>
#include <NsCore/TypeId.h>
#include <NsCore/Category.h>
#include <NsCore/Log.h>
#include <NsCore/HashMap.h>
#include <NsCore/ReflectionImplement.h>
#include <NsGui/UIElement.h>
#include <NsGui/IntegrationAPI.h>
#include <NsDrawing/Point.h>
#include <NsDrawing/Size.h>
#include <NsDrawing/Rect.h>

#include <jni.h>
#include <android/input.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>


using namespace Noesis;
using namespace NoesisApp;


namespace
{
android_app* gApp;

////////////////////////////////////////////////////////////////////////////////////////////////////
JNIEnv* Attach(JavaVM* javaVM)
{
    JNIEnv* env;
    switch (javaVM->GetEnv((void**)&env, JNI_VERSION_1_6))
    {
        case JNI_OK: return 0;
        case JNI_EDETACHED: return javaVM->AttachCurrentThread(&env, NULL) == 0 ? env : 0;
        case JNI_EVERSION: return 0;
        default: NS_ASSERT_UNREACHABLE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayKeyboard(bool show)
{
    // Attach current thread to the JavaVM
    JNIEnv* env;
    JavaVM* vm = gApp->activity->vm;
    if ((env = Attach(vm)) == 0)
    {
        return;
    }

    // Retrieves NativeActivity
    jobject nativeActivity = gApp->activity->clazz;
    jclass nativeActivityClass = env->GetObjectClass(nativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID inputMethodServiceField = env->GetStaticFieldID(contextClass, "INPUT_METHOD_SERVICE",
        "Ljava/lang/String;");
    jobject inputMethodService = env->GetStaticObjectField(contextClass, inputMethodServiceField);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE)
    jclass inputMethodManagerClass = env->FindClass("android/view/inputmethod/InputMethodManager");
    jmethodID getSystemServiceFn = env->GetMethodID(nativeActivityClass, "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject inputMethodManager = env->CallObjectMethod(nativeActivity, getSystemServiceFn,
        inputMethodService);

    // Runs getWindow().getDecorView()
    jmethodID getWindowFn = env->GetMethodID(nativeActivityClass, "getWindow",
        "()Landroid/view/Window;");
    jobject window = env->CallObjectMethod(nativeActivity, getWindowFn);
    jclass windowClass = env->FindClass("android/view/Window");
    jmethodID getDecorViewFn = env->GetMethodID(windowClass, "getDecorView",
        "()Landroid/view/View;");
    jobject decorView = env->CallObjectMethod(window, getDecorViewFn);

    if (show)
    {
        // Runs inputMethodManager.showSoftInput(...)
        jmethodID showSoftInputFn = env->GetMethodID(inputMethodManagerClass, "showSoftInput",
            "(Landroid/view/View;I)Z");
        jint flags = 0;
        env->CallBooleanMethod(inputMethodManager, showSoftInputFn, decorView, flags);
    }
    else
    {
        // Runs window.getViewToken()
        jclass viewClass = env->FindClass("android/view/View");
        jmethodID getWindowFnToken = env->GetMethodID(viewClass, "getWindowToken",
            "()Landroid/os/IBinder;");
        jobject binder = env->CallObjectMethod(decorView, getWindowFnToken);

        // inputMethodManager.hideSoftInput(...)
        jmethodID hideSoftInputFn = env->GetMethodID(inputMethodManagerClass,
            "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
        jint flags = 0;
        env->CallBooleanMethod(inputMethodManager, hideSoftInputFn, binder, flags);
    }

    // Detach current thread from Java VM
    vm->DetachCurrentThread();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool ShowKeyboard(void* user, UIElement* focused)
{
    NS_LOG_TRACE("Show SoftInput %p (%s)", focused, focused->GetClassType()->GetName());

    // NDK bugged!? This is not working!!!
    //ANativeActivity_showSoftInput(gApp->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);

    DisplayKeyboard(true);

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void HideKeyboard(void* user)
{
    NS_LOG_TRACE("Hide SoftInput");

    //ANativeActivity_hideSoftInput(gApp->activity, ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);

    DisplayKeyboard(false);
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
AndroidDisplay::AndroidDisplay(): mWidth(0), mHeight(0)
{
    gApp = (android_app*)Display::GetPrivateData();

    NS_ASSERT(gApp != 0);
    NS_ASSERT(gApp->window != 0);
    ANativeWindow_acquire(gApp->window);

    FillKeyTable();

    GUI::SetSoftwareKeyboardCallbacks(0, ShowKeyboard, HideKeyboard);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
AndroidDisplay::~AndroidDisplay()
{
    GUI::SetSoftwareKeyboardCallbacks(0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetTitle(const char* title)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetLocation(int x, int y)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetSize(uint32_t width, uint32_t height)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetWindowStyle(WindowStyle windowStyle)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetWindowState(WindowState windowState)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetResizeMode(ResizeMode resizeMode)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetShowInTaskbar(bool showInTaskbar)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetTopmost(bool topmost)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetAllowFileDrop(bool allowFileDrop)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::SetWindowStartupLocation(WindowStartupLocation location)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::Show()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::EnterMessageLoop()
{
    while (ProcessMessages())
    {
        mRender(this);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::Close()
{
    ANativeActivity_finish(gApp->activity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* AndroidDisplay::GetNativeHandle() const
{
    NS_ASSERT(gApp != 0);
    return gApp->window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t AndroidDisplay::GetClientWidth() const
{
    NS_ASSERT(gApp != 0);
    return ANativeWindow_getWidth(gApp->window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t AndroidDisplay::GetClientHeight() const
{
    NS_ASSERT(gApp != 0);
    return ANativeWindow_getHeight(gApp->window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool AndroidDisplay::ProcessMessages()
{
    NS_ASSERT(gApp != 0);

    // NOTE: Because of a bug in Android code (see: http://bit.ly/2kGadTu), we are not receiving the
    // messages for surface size changes, so we need to poll window size every frame to detect it
    UpdateSize();

    gApp->onAppCmd = OnAppCmd;
    gApp->onInputEvent = OnInputEvent;
    gApp->userData = this;

    int events;
    android_poll_source* source;
    while (ALooper_pollAll(0, 0, &events, (void**)&source) >= 0)
    {
        // Process this event
        if (source != 0)
        {
            source->process(gApp, source);
        }

        // Check if we are exiting
        if (gApp->destroyRequested != 0)
        {
            NS_LOG_TRACE("APP_CMD_DESTROY in Main Loop");
            return false;
        }
    }

    gApp->onAppCmd = 0;
    gApp->onInputEvent = 0;
    gApp->userData = 0;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::OnAppCmd(android_app* app, int cmd)
{
    AndroidDisplay* display = static_cast<AndroidDisplay*>(app->userData);
    display->DispatchAppEvent(cmd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::DispatchAppEvent(int eventId)
{
    switch (eventId)
    {
        case APP_CMD_INPUT_CHANGED:
        {
            NS_LOG_TRACE("APP_CMD_INPUT_CHANGED");
            break;
        }
        case APP_CMD_INIT_WINDOW:
        {
            NS_LOG_TRACE("APP_CMD_INIT_WINDOW");
            break;
        }
        case APP_CMD_TERM_WINDOW:
        {
            NS_LOG_TRACE("APP_CMD_TERM_WINDOW");

            NS_ASSERT(gApp != 0);
            NS_ASSERT(gApp->window != 0);
            ANativeWindow_release(gApp->window);

            gApp->destroyRequested = 1;

            break;
        }
        case APP_CMD_WINDOW_RESIZED:
        {
            NS_LOG_TRACE("APP_CMD_WINDOW_RESIZED");
            UpdateSize();
            break;
        }
        case APP_CMD_WINDOW_REDRAW_NEEDED:
        {
            NS_LOG_TRACE("APP_CMD_WINDOW_REDRAW_NEEDED");
            break;
        }
        case APP_CMD_CONTENT_RECT_CHANGED:
        {
            NS_LOG_TRACE("APP_CMD_CONTENT_RECT_CHANGED");
            UpdateSize();
            break;
        }
        case APP_CMD_GAINED_FOCUS:
        {
            NS_LOG_TRACE("APP_CMD_GAINED_FOCUS");
            mActivated(this);
            break;
        }
        case APP_CMD_LOST_FOCUS:
        {
            NS_LOG_TRACE("APP_CMD_LOST_FOCUS");
            mDeactivated(this);
            break;
        }
        case APP_CMD_CONFIG_CHANGED:
        {
            NS_LOG_TRACE("APP_CMD_CONFIG_CHANGED");
            break;
        }
        case APP_CMD_LOW_MEMORY:
        {
            NS_LOG_TRACE("APP_CMD_LOW_MEMORY");
            break;
        }
        case APP_CMD_START:
        {
            NS_LOG_TRACE("APP_CMD_START");
            break;
        }
        case APP_CMD_RESUME:
        {
            NS_LOG_TRACE("APP_CMD_RESUME");
            break;
        }
        case APP_CMD_SAVE_STATE:
        {
            NS_LOG_TRACE("APP_CMD_SAVE_STATE");
            break;
        }
        case APP_CMD_PAUSE:
        {
            NS_LOG_TRACE("APP_CMD_PAUSE");
            break;
        }
        case APP_CMD_STOP:
        {
            NS_LOG_TRACE("APP_CMD_STOP");
            break;
        }
        case APP_CMD_DESTROY:
        {
            NS_LOG_TRACE("APP_CMD_DESTROY");
            break;
        }
        default: NS_ASSERT_UNREACHABLE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int AndroidDisplay::OnInputEvent(android_app* app, AInputEvent* event)
{
    AndroidDisplay* display = static_cast<AndroidDisplay*>(app->userData);
    return display->DispatchInputEvent(event);

}

namespace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
int GetUnicodeChar(android_app* app, int eventType, int keyCode, int metaState)
{
    // Attach current thread to the JavaVM
    JNIEnv* env;
    JavaVM* vm = gApp->activity->vm;
    if ((env = Attach(vm)) == 0)
    {
        return 0;
    }

    jclass keyEventClass = env->FindClass("android/view/KeyEvent");
    int unicodeKey;

    jmethodID eventConstructor = env->GetMethodID(keyEventClass, "<init>", "(II)V");
    jobject eventObj = env->NewObject(keyEventClass, eventConstructor, eventType, keyCode);

    if (metaState == 0)
    {
        jmethodID getUnicodeCharFn = env->GetMethodID(keyEventClass, "getUnicodeChar", "()I");
        unicodeKey = env->CallIntMethod(eventObj, getUnicodeCharFn);
    }

    else
    {
        jmethodID getUnicodeCharFn = env->GetMethodID(keyEventClass, "getUnicodeChar", "(I)I");
        unicodeKey = env->CallIntMethod(eventObj, getUnicodeCharFn, metaState);
    }

    // Detach current thread from Java VM
    vm->DetachCurrentThread();

    return unicodeKey;
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int AndroidDisplay::DispatchInputEvent(AInputEvent* event)
{
    switch (AInputEvent_getType(event))
    {
        case AINPUT_EVENT_TYPE_KEY:
        {
            int keyCode = AKeyEvent_getKeyCode(event);
            int metaState = AKeyEvent_getMetaState(event);

            int action = AKeyEvent_getAction(event);
            switch (action)
            {
                case AKEY_EVENT_ACTION_DOWN:
                {
                    Key key = (Key)mKeyTable[keyCode];
                    if (key != Key_None)
                    {
                        NS_LOG_TRACE("KEY_EVENT %d [dn]", key);
                        mKeyDown(this, key);
                    }

                    int c = GetUnicodeChar(gApp, action, keyCode, metaState);
                    if (c != 0)
                    {
                        NS_LOG_TRACE("KEY_EVENT %d [ch]", c);
                        mChar(this, c);
                    }

                    break;
                }
                case AKEY_EVENT_ACTION_UP:
                {
                    Key key = (Key)mKeyTable[keyCode];
                    if (key != Key_None)
                    {
                        NS_LOG_TRACE("KEY_EVENT %d [up]", key);
                        mKeyUp(this, key);
                    }

                    break;
                }
                case AKEY_EVENT_ACTION_MULTIPLE:
                {
                    NS_LOG_TRACE("KEY_EVENT %d %d [mx]", keyCode, AKeyEvent_getRepeatCount(event));
                    break;
                }
                default: NS_ASSERT_UNREACHABLE;
            }

            break;
        }
        case AINPUT_EVENT_TYPE_MOTION:
        {
            int action = AMotionEvent_getAction(event);
            int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            uint64_t pointerId = (uint64_t)AMotionEvent_getPointerId(event, index);

            switch (action & AMOTION_EVENT_ACTION_MASK)
            {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                {
                    int x = (int)AMotionEvent_getX(event, index);
                    int y = (int)AMotionEvent_getY(event, index);
                    NS_LOG_TRACE("MOTION_EVENT TouchDown %d;%d (%llu)", x, y, pointerId);
                    mTouchDown(this, x, y, pointerId);
                    break;
                }
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                {
                    int x = (int)AMotionEvent_getX(event, index);
                    int y = (int)AMotionEvent_getY(event, index);
                    NS_LOG_TRACE("MOTION_EVENT TouchUp %d;%d (%llu)", x, y, pointerId);
                    mTouchUp(this, x, y, pointerId);
                    break;
                }
                case AMOTION_EVENT_ACTION_MOVE:
                {
                    int numPointers = AMotionEvent_getPointerCount(event);
                    for (index = 0; index < numPointers; ++index)
                    {
                        pointerId = (uint64_t)AMotionEvent_getPointerId(event, index);
                        int x = (int)AMotionEvent_getX(event, index);
                        int y = (int)AMotionEvent_getY(event, index);
                        NS_LOG_TRACE("MOTION_EVENT TouchMove %d;%d (%llu)", x, y, pointerId);
                        mTouchMove(this, x, y, pointerId);
                    }
                    break;
                }
                case AMOTION_EVENT_ACTION_CANCEL:
                case AMOTION_EVENT_ACTION_OUTSIDE:
                case AMOTION_EVENT_ACTION_HOVER_MOVE:
                case AMOTION_EVENT_ACTION_HOVER_ENTER:
                case AMOTION_EVENT_ACTION_HOVER_EXIT:
                {
                    break;
                }
                default: NS_ASSERT_UNREACHABLE;
            }

            break;
        }
        default: NS_ASSERT_UNREACHABLE;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::UpdateSize()
{
    uint32_t w = GetClientWidth();
    uint32_t h = GetClientHeight();

    if (mWidth != w || mHeight != h)
    {
        mWidth = w;
        mHeight = h;

        mSizeChanged(this, w, h);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AndroidDisplay::FillKeyTable()
{
    memset(mKeyTable, 0, sizeof(mKeyTable));

    mKeyTable[AKEYCODE_UNKNOWN] = Key_None;
    mKeyTable[AKEYCODE_SOFT_LEFT] = Key_None;
    mKeyTable[AKEYCODE_SOFT_RIGHT] = Key_None;
    mKeyTable[AKEYCODE_HOME] = Key_Home;
    mKeyTable[AKEYCODE_BACK] = Key_None;
    mKeyTable[AKEYCODE_CALL] = Key_None;
    mKeyTable[AKEYCODE_ENDCALL] = Key_None;
    mKeyTable[AKEYCODE_0] = Key_D0;
    mKeyTable[AKEYCODE_1] = Key_D1;
    mKeyTable[AKEYCODE_2] = Key_D2;
    mKeyTable[AKEYCODE_3] = Key_D3;
    mKeyTable[AKEYCODE_4] = Key_D4;
    mKeyTable[AKEYCODE_5] = Key_D5;
    mKeyTable[AKEYCODE_6] = Key_D6;
    mKeyTable[AKEYCODE_7] = Key_D7;
    mKeyTable[AKEYCODE_8] = Key_D8;
    mKeyTable[AKEYCODE_9] = Key_D9;
    mKeyTable[AKEYCODE_STAR] = Key_Multiply;
    mKeyTable[AKEYCODE_POUND] = Key_None;
    mKeyTable[AKEYCODE_DPAD_UP] = Key_Up;
    mKeyTable[AKEYCODE_DPAD_DOWN] = Key_Down;
    mKeyTable[AKEYCODE_DPAD_LEFT] = Key_Left ;
    mKeyTable[AKEYCODE_DPAD_RIGHT] = Key_Right;
    mKeyTable[AKEYCODE_DPAD_CENTER] = Key_Return;
    mKeyTable[AKEYCODE_VOLUME_UP] = Key_None;
    mKeyTable[AKEYCODE_VOLUME_DOWN] = Key_None;
    mKeyTable[AKEYCODE_POWER] = Key_None;
    mKeyTable[AKEYCODE_CAMERA] = Key_None;
    mKeyTable[AKEYCODE_CLEAR] = Key_None;
    mKeyTable[AKEYCODE_A] = Key_A;
    mKeyTable[AKEYCODE_B] = Key_B;
    mKeyTable[AKEYCODE_C] = Key_C;
    mKeyTable[AKEYCODE_D] = Key_D;
    mKeyTable[AKEYCODE_E] = Key_E;
    mKeyTable[AKEYCODE_F] = Key_F;
    mKeyTable[AKEYCODE_G] = Key_G;
    mKeyTable[AKEYCODE_H] = Key_H;
    mKeyTable[AKEYCODE_I] = Key_I;
    mKeyTable[AKEYCODE_J] = Key_J;
    mKeyTable[AKEYCODE_K] = Key_K;
    mKeyTable[AKEYCODE_L] = Key_L;
    mKeyTable[AKEYCODE_M] = Key_M;
    mKeyTable[AKEYCODE_N] = Key_N;
    mKeyTable[AKEYCODE_O] = Key_O;
    mKeyTable[AKEYCODE_P] = Key_P;
    mKeyTable[AKEYCODE_Q] = Key_Q;
    mKeyTable[AKEYCODE_R] = Key_R;
    mKeyTable[AKEYCODE_S] = Key_S;
    mKeyTable[AKEYCODE_T] = Key_T;
    mKeyTable[AKEYCODE_U] = Key_U;
    mKeyTable[AKEYCODE_V] = Key_V;
    mKeyTable[AKEYCODE_W] = Key_W;
    mKeyTable[AKEYCODE_X] = Key_X;
    mKeyTable[AKEYCODE_Y] = Key_Y;
    mKeyTable[AKEYCODE_Z] = Key_Z;
    mKeyTable[AKEYCODE_COMMA] = Key_OemComma;
    mKeyTable[AKEYCODE_PERIOD] = Key_OemPeriod;
    mKeyTable[AKEYCODE_ALT_LEFT] = Key_LeftAlt;
    mKeyTable[AKEYCODE_ALT_RIGHT] = Key_RightAlt;
    mKeyTable[AKEYCODE_SHIFT_LEFT] = Key_LeftShift;
    mKeyTable[AKEYCODE_SHIFT_RIGHT] = Key_RightShift;
    mKeyTable[AKEYCODE_TAB] = Key_Tab;
    mKeyTable[AKEYCODE_SPACE] = Key_Space;
    mKeyTable[AKEYCODE_SYM] = Key_RightAlt;
    mKeyTable[AKEYCODE_EXPLORER] = Key_None;
    mKeyTable[AKEYCODE_ENVELOPE] = Key_None;
    mKeyTable[AKEYCODE_ENTER] = Key_Return;
    mKeyTable[AKEYCODE_DEL] = Key_Back;
    mKeyTable[AKEYCODE_GRAVE] = Key_Oem3;
    mKeyTable[AKEYCODE_MINUS] = Key_Subtract;
    mKeyTable[AKEYCODE_EQUALS] = Key_OemPlus;
    mKeyTable[AKEYCODE_LEFT_BRACKET] = Key_Oem4;
    mKeyTable[AKEYCODE_RIGHT_BRACKET] = Key_Oem6;
    mKeyTable[AKEYCODE_BACKSLASH] = Key_Oem5;
    mKeyTable[AKEYCODE_SEMICOLON] = Key_Oem1;
    mKeyTable[AKEYCODE_APOSTROPHE] = Key_Oem7;
    mKeyTable[AKEYCODE_SLASH] = Key_Oem2;
    mKeyTable[AKEYCODE_AT] = Key_None;
    mKeyTable[AKEYCODE_NUM] = Key_LeftAlt;
    mKeyTable[AKEYCODE_HEADSETHOOK] = Key_None;
    mKeyTable[AKEYCODE_FOCUS] = Key_None;   // *Camera* focus
    mKeyTable[AKEYCODE_PLUS] = Key_OemPlus;
    mKeyTable[AKEYCODE_MENU] = Key_LeftAlt;
    mKeyTable[AKEYCODE_NOTIFICATION] = Key_None;
    mKeyTable[AKEYCODE_SEARCH] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_PLAY_PAUSE] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_STOP] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_NEXT] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_PREVIOUS] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_REWIND] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_FAST_FORWARD] = Key_None;
    mKeyTable[AKEYCODE_MUTE] = Key_None;
    mKeyTable[AKEYCODE_PAGE_UP] = Key_Prior;
    mKeyTable[AKEYCODE_PAGE_DOWN] = Key_Next;
    mKeyTable[AKEYCODE_PICTSYMBOLS] = Key_None;
    mKeyTable[AKEYCODE_SWITCH_CHARSET] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_A] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_B] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_C] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_X] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_Y] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_Z] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_L1] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_R1] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_L2] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_R2] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_THUMBL] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_THUMBR] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_START] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_SELECT] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_MODE] = Key_None;
    mKeyTable[AKEYCODE_ESCAPE] = Key_Escape;
    mKeyTable[AKEYCODE_FORWARD_DEL] = Key_Delete;
    mKeyTable[AKEYCODE_CTRL_LEFT] = Key_LeftCtrl;
    mKeyTable[AKEYCODE_CTRL_RIGHT] = Key_RightCtrl;
    mKeyTable[AKEYCODE_CAPS_LOCK] = Key_CapsLock;
    mKeyTable[AKEYCODE_SCROLL_LOCK] = Key_Scroll;
    mKeyTable[AKEYCODE_META_LEFT] = Key_None;
    mKeyTable[AKEYCODE_META_RIGHT] = Key_None;
    mKeyTable[AKEYCODE_FUNCTION] = Key_None;
    mKeyTable[AKEYCODE_SYSRQ] = Key_None;
    mKeyTable[AKEYCODE_BREAK] = Key_Pause;
    mKeyTable[AKEYCODE_MOVE_HOME] = Key_Home;
    mKeyTable[AKEYCODE_MOVE_END] = Key_End;
    mKeyTable[AKEYCODE_INSERT] = Key_Insert;
    mKeyTable[AKEYCODE_FORWARD] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_PLAY] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_PAUSE] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_CLOSE] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_EJECT] = Key_None;
    mKeyTable[AKEYCODE_MEDIA_RECORD] = Key_None;
    mKeyTable[AKEYCODE_F1] = Key_F1;
    mKeyTable[AKEYCODE_F2] = Key_F2;
    mKeyTable[AKEYCODE_F3] = Key_F3;
    mKeyTable[AKEYCODE_F4] = Key_F4;
    mKeyTable[AKEYCODE_F5] = Key_F5;
    mKeyTable[AKEYCODE_F6] = Key_F6;
    mKeyTable[AKEYCODE_F7] = Key_F7;
    mKeyTable[AKEYCODE_F8] = Key_F8;
    mKeyTable[AKEYCODE_F9] = Key_F9;
    mKeyTable[AKEYCODE_F10] = Key_F10;
    mKeyTable[AKEYCODE_F11] = Key_F11;
    mKeyTable[AKEYCODE_F12] = Key_F12;
    mKeyTable[AKEYCODE_NUM_LOCK] = Key_NumLock;
    mKeyTable[AKEYCODE_NUMPAD_0] = Key_NumPad0;
    mKeyTable[AKEYCODE_NUMPAD_1] = Key_NumPad1;
    mKeyTable[AKEYCODE_NUMPAD_2] = Key_NumPad2;
    mKeyTable[AKEYCODE_NUMPAD_3] = Key_NumPad3;
    mKeyTable[AKEYCODE_NUMPAD_4] = Key_NumPad4;
    mKeyTable[AKEYCODE_NUMPAD_5] = Key_NumPad5;
    mKeyTable[AKEYCODE_NUMPAD_6] = Key_NumPad6;
    mKeyTable[AKEYCODE_NUMPAD_7] = Key_NumPad7;
    mKeyTable[AKEYCODE_NUMPAD_8] = Key_NumPad8;
    mKeyTable[AKEYCODE_NUMPAD_9] = Key_NumPad9;
    mKeyTable[AKEYCODE_NUMPAD_DIVIDE] = Key_Divide;
    mKeyTable[AKEYCODE_NUMPAD_MULTIPLY] = Key_Multiply;
    mKeyTable[AKEYCODE_NUMPAD_SUBTRACT] = Key_Subtract;
    mKeyTable[AKEYCODE_NUMPAD_ADD] = Key_Add;
    mKeyTable[AKEYCODE_NUMPAD_DOT] = Key_Decimal;
    mKeyTable[AKEYCODE_NUMPAD_COMMA] = Key_None;
    mKeyTable[AKEYCODE_NUMPAD_ENTER] = Key_Enter;
    mKeyTable[AKEYCODE_NUMPAD_EQUALS] = Key_None;
    mKeyTable[AKEYCODE_NUMPAD_LEFT_PAREN] = Key_None;
    mKeyTable[AKEYCODE_NUMPAD_RIGHT_PAREN] = Key_None;
    mKeyTable[AKEYCODE_VOLUME_MUTE] = Key_None;
    mKeyTable[AKEYCODE_INFO] = Key_None;
    mKeyTable[AKEYCODE_CHANNEL_UP] = Key_None;
    mKeyTable[AKEYCODE_CHANNEL_DOWN] = Key_None;
    mKeyTable[AKEYCODE_ZOOM_IN] = Key_None;
    mKeyTable[AKEYCODE_ZOOM_OUT] = Key_None;
    mKeyTable[AKEYCODE_TV] = Key_None;
    mKeyTable[AKEYCODE_WINDOW] = Key_None;
    mKeyTable[AKEYCODE_GUIDE] = Key_None;
    mKeyTable[AKEYCODE_DVR] = Key_None;
    mKeyTable[AKEYCODE_BOOKMARK] = Key_None;
    mKeyTable[AKEYCODE_CAPTIONS] = Key_None;
    mKeyTable[AKEYCODE_SETTINGS] = Key_None;
    mKeyTable[AKEYCODE_TV_POWER] = Key_None;
    mKeyTable[AKEYCODE_TV_INPUT] = Key_None;
    mKeyTable[AKEYCODE_STB_POWER] = Key_None;
    mKeyTable[AKEYCODE_STB_INPUT] = Key_None;
    mKeyTable[AKEYCODE_AVR_POWER] = Key_None;
    mKeyTable[AKEYCODE_AVR_INPUT] = Key_None;
    mKeyTable[AKEYCODE_PROG_RED] = Key_None;
    mKeyTable[AKEYCODE_PROG_GREEN] = Key_None;
    mKeyTable[AKEYCODE_PROG_YELLOW] = Key_None;
    mKeyTable[AKEYCODE_PROG_BLUE] = Key_None;
    mKeyTable[AKEYCODE_APP_SWITCH] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_1] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_2] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_3] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_4] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_5] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_6] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_7] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_8] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_9] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_10] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_11] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_12] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_13] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_14] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_15] = Key_None;
    mKeyTable[AKEYCODE_BUTTON_16] = Key_None;
    mKeyTable[AKEYCODE_LANGUAGE_SWITCH] = Key_None;
    mKeyTable[AKEYCODE_MANNER_MODE] = Key_None;
    mKeyTable[AKEYCODE_3D_MODE] = Key_None;
    mKeyTable[AKEYCODE_CONTACTS] = Key_None;
    mKeyTable[AKEYCODE_CALENDAR] = Key_None;
    mKeyTable[AKEYCODE_MUSIC] = Key_None;
    mKeyTable[AKEYCODE_CALCULATOR] = Key_None;
    mKeyTable[AKEYCODE_ZENKAKU_HANKAKU] = Key_None;
    mKeyTable[AKEYCODE_EISU] = Key_None;
    mKeyTable[AKEYCODE_MUHENKAN] = Key_None;
    mKeyTable[AKEYCODE_HENKAN] = Key_None;
    mKeyTable[AKEYCODE_KATAKANA_HIRAGANA] = Key_None;
    mKeyTable[AKEYCODE_YEN] = Key_None;
    mKeyTable[AKEYCODE_RO] = Key_None;
    mKeyTable[AKEYCODE_KANA] = Key_None;
    mKeyTable[AKEYCODE_ASSIST] = Key_None;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(AndroidDisplay)
{
    NsMeta<TypeId>("AndroidDisplay");
    NsMeta<Category>("Display");
}
