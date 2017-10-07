#import <GameKit/GameKit.h>

/**
 * Вьюконтроллер списка ачивментов, который умеет показыватся и скрываться модально
 * относительно родителя. Использование:
 *   GCAchievementViewController* c = [[GCAchievementViewController alloc] initWithParent:viewController];
 *   [c show];
 *   [c release];
 */
@interface GCAchievementViewController
	: GKAchievementViewController<GKAchievementViewControllerDelegate>
{

	UIViewController* _parent;
		// ссылка на родительский вьюконтроллер
}

/**
 * Создать с заданным родителем
 */
- (id)init;

/**
 * Показать поверх родителя; после этого контроллер можно удалять,
 * так как им будет владеть родитель
 */
- (void)show;

@end
