#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView
{
	BOOL isAnimating;
	BOOL isDrawingEnabled;
	BOOL isInteractionEnabled;
	NSEventModifierFlags keyModifierFlags;
}

+ (NSOpenGLPixelFormat*) defaultPixelFormat;
- (void) setDrawingEnabled: (BOOL)enable;
- (void) setInteractionWithMainWindowEnabled: (BOOL)enable;

@end

@interface MyOpenGLView(AnimationMethods)
- (BOOL)isAnimating;
- (void)startAnimation;
- (void)stopAnimation;
- (void)toggleAnimation;
@end
