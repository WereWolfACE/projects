#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>


@class AppDelegate;

@interface EAGLView : UIView
{    
@private
    IBOutlet AppDelegate* appDelegate;

    id displayLink;

    UIPinchGestureRecognizer *scalePinch; // Распознаватель жеста масштабирования
}

- (void)startAnimation;
- (void)stopAnimation;
- (void)resetAnimationFrameInterval;
- (void)drawView:(id)sender;

@end
