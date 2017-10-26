@interface GCAchievement : NSObject<NSCoding> {
	NSString* _identifier;
	double _percentComplete;
}

-(id)initWithIdentifier:(NSString*) identifier;

- (NSString*) debugInfo;

// Отправить ачивмент
- (void)sendAchievement;

// Получить объект GKAchievement
- (id) getGKObject;

@property(nonatomic, assign) double percentComplete;
@end
