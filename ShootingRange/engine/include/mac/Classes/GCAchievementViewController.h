#import <GameKit/GameKit.h>

/**
 * Вьюконтроллер списка ачивментов, который умеет показыватся и скрываться модально
 * относительно родителя. Использование:
 *   GCAchievementViewController* c = [[GCAchievementViewController alloc] initWithParent:viewController];
 *   [c show];
 *   [c release];
 */
@interface GCAchievementViewController : GKAchievementViewController<GKAchievementViewControllerDelegate>
{
	bool __m_enableCustomCursor;
}

/**
 * Показать поверх родителя; после этого контроллер можно удалять,
 * так как им будет владеть родитель
 */
- (void)show;

@end
