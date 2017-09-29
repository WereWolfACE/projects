@interface GCPlayer : NSObject<NSCoding> {
	
	NSString* _identifier;
		// идентификатор

	NSString* _alias;
		// видимое имя

	NSMutableDictionary* _achievements;
		// набор ачивментов
	
	NSMutableDictionary* _scores;
		// набор счетов
}

- (id) initWithId:(NSString*)identifier alias:alias;

+ (id) playerWithId:(NSString*)identifier alias:alias;

- (void) setAchievementProgress:(float)progress byID:(NSString*)achievementID;

/**
 * Установить счет score для категории categoryID
 */
- (void) setScore:(int64_t)score forCategory:(NSString*)categoryID;

- (NSString*) debugInfo;

/**
 * Отправить сохраненные неотправленный данные в геймцентр
 */
- (void) submitData;

/**
 * Загрузить прогресс из геймцентра и синхронизировать с ним локальные данные
 */
- (void) loadData;

@property (nonatomic, retain) NSString* playerID;

@property (nonatomic, retain) NSString* alias;

@end
