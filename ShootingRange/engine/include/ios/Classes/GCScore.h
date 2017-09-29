/**
 * Счет в какой-то конкретной категории
 */
@interface GCScore : NSObject <NSCoding> {
	
	int64_t _value;
		// значение счета
	
	NSString* _category;
		// идентификатор категории, к которой относится счет

	BOOL _sent;
		// было ли успешно послано значение счета в геймцентр
	
	BOOL _sending;
		// посылается ли значение счета в геймцентр сейчас
	
}

/**
 * Создать новый счет с категорией category и значением value
 */
- (id) initWithCategory:(NSString*)category andValue:(int64_t)value;

/**
 * Обозначить, что счет нужно будет послать снова
 */
- (void)setNotSent;

/**
 * Послать счет, если он еще не был успешно послан в геймцентр
 */
- (void)sendScore;

/**
 * Значение счета
 */
@property(nonatomic, assign) int64_t value;

@end
