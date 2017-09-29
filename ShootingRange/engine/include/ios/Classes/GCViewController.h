#import <GameKit/GameKit.h>

@interface GCViewController
	: GKGameCenterViewController<GKGameCenterControllerDelegate>
{
	UIViewController* _parent;
}

+ (BOOL)isSupported;

- (id)init;

- (void)showAchievements;

- (void)showLeaderboardWithCategory:(NSString*)category;

@end
