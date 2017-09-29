#ifdef __OBJC__

#import <UIKit/UIKit.h>

@class EAGLView;
@class MainViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
    MainViewController *viewController;
}

+ (AppDelegate*) GetInstance;
+ (UIViewController*) GetRootViewController;
+ (UIView*) GetRootView;
+ (UIImage*) GetBestMatchedFullscreenImageWithName:(NSString*)name AndExtension:(NSString*)ext;
- (void) initPushNotificationsWithSound: (BOOL)withSound;
- (void) ResumeLoad;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;
@property (nonatomic, retain) IBOutlet MainViewController *viewController;

@end

// имена извещений о событиях приложения
extern NSString * const DidFinishLaunchingNotification;
extern NSString * const OpenURLNotification;

#endif

#ifdef __cplusplus

namespace GUI
{
class Widget;
}

// Ввод текста
void ShowKeyboard();
void ShowNumericKeyboard();
void HideKeyboard();
std::string GetInputText(); // Возвращает текст в textField
void SetInputText(const std::string &text); // Устанавливает текст в textField
void SelectInputText(int start, int length); // выделяет область текста, или устанавливает курсор при length = 0
void SetInputWidget(GUI::Widget *w);
GUI::Widget* GetInputWidget();
void SetInputCallback(std::function<int(int loc, int len, const std::string & data)> fun);

void ResetAnimationFrameInterval();

void SaveScreenshot(const std::string &pathToSave);

#endif
