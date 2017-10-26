#ifdef __OBJC__

#import <Cocoa/Cocoa.h>
#import "Core/App/Mac_AppWindow.h"
#import "Core/App/Mac_OpenGLView.h"


@interface AppDelegate : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>
{
	MyOpenGLView* _glView;
    IBOutlet NSMenuItem *customCursorMenuItem;
	BOOL _exitFlag;
	BOOL _isActive;
}

+ (AppDelegate*) GetInstance;
+ (MyOpenGLView*) GetRootView;
- (IBAction) toggleFullScreen: (id)sender;
- (IBAction) toggleCustomCursor: (id)sender;
- (void) setInteractionWithMainWindowEnabled: (BOOL)enable;
- (void) initPushNotificationsWithSound: (BOOL)withSound;

@end

#endif


#ifdef __cplusplus

namespace GUI
{
class Widget;
}

// Ввод текста - заглушки
void ShowKeyboard();
void HideKeyboard();
std::string GetInputText(); // Возвращает текст в textField
void SetInputText(const std::string &text); // Устанавливает текст в textField
void SetInputWidget(GUI::Widget *w);
GUI::Widget* GetInputWidget();

#endif
