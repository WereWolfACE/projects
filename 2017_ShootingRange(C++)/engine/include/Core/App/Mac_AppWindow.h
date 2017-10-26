#import <AppKit/AppKit.h>

@interface AppWindow : NSWindow
{}

+ (AppWindow*) getInstance;
+ (void) resizeGameLayer;

@end


@class MyOpenGLView;

@interface WinDelegate : NSObject <NSWindowDelegate>
{
	MyOpenGLView *_glView;
	BOOL _isMiniaturized;
}

- (id)initWithWindow:(AppWindow*)window;
- (BOOL)windowIsMiniaturized;

@end
