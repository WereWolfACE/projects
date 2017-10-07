#import <Gamekit/GameKit.h>

@interface GCLeaderboardViewController : 
	GKLeaderboardViewController<GKLeaderboardViewControllerDelegate>
{
	UIViewController *_parent;
}

/**
 * Создать с заданным родителем
 */
- (id)initWithScoreCategory:(NSString*)cat timeScope:(GKLeaderboardTimeScope)scope;

/**
 * Показать поверх родителя; после этого контроллер можно удалять,
 * так как им будет владеть родитель
 */
- (void)show;

@end
