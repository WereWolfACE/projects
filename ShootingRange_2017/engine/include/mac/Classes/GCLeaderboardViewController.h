#import <GameKit/GameKit.h>

/**
 * Вьюконтроллер списка ачивментов, который умеет показыватся и скрываться модально
 * относительно родителя. Использование:
 *   GCAchievementViewController* c = [[GCAchievementViewController alloc] initWithParent:viewController];
 *   [c show];
 *   [c release];
 */
@interface GCLeaderboardViewController : GKLeaderboardViewController<GKLeaderboardViewControllerDelegate>
{
	bool __m_enableCustomCursor;
}

/**
 * Создать с заданными параметрами
 */
- (id)initWithScoreCategory:(NSString*)cat timeScope:(GKLeaderboardTimeScope)scope;

/**
 * Показать поверх родителя; после этого контроллер можно удалять,
 * так как им будет владеть родитель
 */
- (void)show;

@end

