// Представляет данные геймцентра по приложению
@interface GCData : NSObject<NSCoding> {
	
	NSMutableDictionary* _players;
		// все игроки, когда-либо заходившие
	
	NSString* _currentPlayerId;
		// идентификатор игрока
	
	NSMutableDictionary* _achievements;
		// ачивменты, полученные из геймцентра
}

/**
 * Глобальный объект
 */
+ (id)sharedInstance;

/**
 * Установить прогресс ачивмента
 */
- (void)setAchievementProgress:(float)progress byID:(NSString*)achievementID;

/**
 * Сбросить данные на диск
 */
- (void)flush;

/**
 * Конструктор
 */
- (id)init;

/**
 * Показать уведомление об ачивменте пользователю
 */
- (void)notifyAchievementByIdentifier:(NSString*)achievementID;

/**
 * Отправить данные в геймцентр
 */ 
- (void)submitData;
@end
