#import <GameKit/GameKit.h>

@interface GCViewController
	: GKGameCenterViewController<GKGameCenterControllerDelegate> 
{
	bool __m_enableCustomCursor;
}

+ (BOOL)isSupported;

- (id)init;

- (void)showAchievements;

- (void)showLeaderboardWithCategory:(NSString*)category; 

@end
