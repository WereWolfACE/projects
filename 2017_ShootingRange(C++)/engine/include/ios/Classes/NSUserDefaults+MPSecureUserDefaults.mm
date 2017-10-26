//
//  NSUserDefaults+MPSecureUserDefaults.m
//  Secure-NSUserDefaults
//
//  Copyright (c) 2011 Matthias Plappert <matthiasplappert@me.com>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//  to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import <ios/Classes/NSUserDefaults+MPSecureUserDefaults.h>
#import <ios/Classes/NSData+AES.h>
#import <ios/Classes/NSData+Base64.h>


@interface NSUserDefaults (MPSecureUserDefaultsPrivate)

- (BOOL)_isValidPropertyListObject:(id)object;
- (id)_objectForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;

@end


@implementation NSUserDefaults (MPSecureUserDefaults)

#pragma mark - Static

static NSData *_secretData = nil;
static BOOL _cryptData = NO;

+ (void)setSecret:(NSData *)secret
{
	if (_secretData == nil) {
		_secretData = secret;
	}
	else {
		Assert2(false, "The secret has already been set.");
	}
}

+ (void)setCrypt:(BOOL)cryptData
{
	_cryptData = cryptData;
}

#pragma mark - Read Accessors

- (BOOL)secureBoolForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = [self secureObjectForKey:key resultType:result];
	if (object && [object respondsToSelector:@selector(boolValue)]) {
		return [object boolValue];
	}
	return NO;
}

- (float)secureFloatForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = [self secureObjectForKey:key resultType:result];
	if (object && [object respondsToSelector:@selector(floatValue)]) {
		return [object floatValue];
	}
	return 0.0f;
}

- (double)secureDoubleForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = [self secureObjectForKey:key resultType:result];
	if (object && [object respondsToSelector:@selector(doubleValue)]) {
		return [object doubleValue];
	}
	return 0.0;
}

- (NSInteger)secureIntegerForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = [self secureObjectForKey:key resultType:result];
	if (object && [object respondsToSelector:@selector(intValue)]) {
		return [object intValue];
	}
	return 0;
}

- (NSArray *)secureArrayForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = (NSArray*)[self secureObjectForKey:key resultType:result];
	if (object && [object isKindOfClass:[NSArray class]]) {
		return object;
	}
	return nil;
}

- (NSArray *)secureStringArrayForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = (NSArray*) [self secureObjectForKey:key resultType:result];
	if (object && [object isKindOfClass:[NSArray class]]) {
		for (id child in object) {
			if (![child isKindOfClass:[NSString class]]) {
				return nil;
			}
		}
		return object;
	}
	return nil;
}

- (NSData *)secureDataForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = (NSData*) [self secureObjectForKey:key resultType:result];
	if (object && [object isKindOfClass:[NSData class]]) {
		return object;
	}
	return nil;
}

- (NSDictionary *)secureDictionaryForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = (NSDictionary*) [self secureObjectForKey:key resultType:result];
	if (object && [object isKindOfClass:[NSDictionary class]]) {
		return object;
	}
	return nil;
}

- (NSNumber *)secureNumberForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = (NSNumber*) [self secureObjectForKey:key resultType:result];
	if (object && [object isKindOfClass:[NSNumber class]]) {
		return object;
	}
	return 0;
}

- (NSString *)secureStringForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = (NSString*) [self secureObjectForKey:key resultType:result];
	if (object && [object isKindOfClass:[NSString class]]) {
		return object;
	}
	else if (object && [object respondsToSelector:@selector(stringValue)]) {
		return [object stringValue];
	}
	else {
		return nil;
	}
}

- (id)secureObjectForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	id object = [self _objectForKey:key resultType:result];
	return object;
}

#pragma mark - Write Accessors

- (void)setSecureBool:(BOOL)value forKey:(NSString *)key
{
	[self setSecureObject:[NSNumber numberWithBool:value] forKey:key];
}

- (void)setSecureFloat:(float)value forKey:(NSString *)key
{
	[self setSecureObject:[NSNumber numberWithFloat:value] forKey:key];
}

- (void)setSecureDouble:(double)value forKey:(NSString *)key
{
	[self setSecureObject:[NSNumber numberWithDouble:value] forKey:key];
}

- (void)setSecureInteger:(NSInteger)value forKey:(NSString *)key
{
	[self setSecureObject:[NSNumber numberWithInteger:value] forKey:key];
}

- (void)setSecureNumber:(NSNumber *)value forKey:(NSString *)key
{
	[self setSecureObject:value forKey:key];
}


- (void)setSecureObject:(id)value forKey:(NSString *)key
{
	if (value == nil || key == nil || !_cryptData) {
		// Use non-secure method
		[self setObject:value forKey:key];
	}
	else if ([self _isValidPropertyListObject:value]) {
		NSData *codeData = [NSKeyedArchiver archivedDataWithRootObject:value];
		codeData = [codeData EncryptAES:_secretData];

		NSData *codeKey = [NSKeyedArchiver archivedDataWithRootObject:key];
		codeKey = [codeKey EncryptAES:_secretData];
		NSString* codeKeyStr = [codeKey base64EncodedString];

		[self setObject:codeData forKey:codeKeyStr];
	}
}

- (void)removeSecureObjectForKey:(NSString *)key
{
	if (!_cryptData) {
		[self removeObjectForKey:key];
	}

	NSData *codeKey = [NSKeyedArchiver archivedDataWithRootObject:key];
	codeKey = [codeKey EncryptAES:_secretData];
	NSString* codeKeyStr = [codeKey base64EncodedString];
	[self removeObjectForKey:codeKeyStr];
}

#pragma mark - Private Methods

- (BOOL)_isValidPropertyListObject:(id)object
{
	if ([object isKindOfClass:[NSData class]] || [object isKindOfClass:[NSString class]] ||
		[object isKindOfClass:[NSNumber class]] || [object isKindOfClass:[NSDate class]]) {
		return YES;

	} else if ([object isKindOfClass:[NSDictionary class]]) {
		for (NSString *key in object) {
			if (![self _isValidPropertyListObject:key]) {
				// Abort
				return NO;
			} else {
				id value = [object objectForKey:key];
				if (![self _isValidPropertyListObject:value]) {
					// Abort
					return NO;
				}
			}
		}
		return YES;

	} else if ([object isKindOfClass:[NSArray class]]) {
		for (id value in object) {
			if (![self _isValidPropertyListObject:value]) {
				// Abort
				return NO;
			}
		}
		return YES;

	} else {
		static NSString *format = @"*** -[NSUserDefaults setSecureObject:forKey:]: Attempt to insert non-property value '%@' of class '%@'.";
		NSString *msg = [NSString stringWithFormat:format, object, NSStringFromClass([object class])];
		if (msg) {
			Log::Error([msg UTF8String]);
		}
		Assert(false);
		return NO;
	}
}

- (id)_objectForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result
{
	if (!_cryptData) {
		id obj = [self objectForKey:key];
		if (result) { *result = ((obj != nil) ? NSSecureUserDefaultsFound : NSSecureUserDefaultsNotFound); }
		return [self objectForKey:key];
	}

	NSData *codeKey = [NSKeyedArchiver archivedDataWithRootObject:key];
	codeKey = [codeKey EncryptAES:_secretData];
	NSString* codeKeyStr = [codeKey base64EncodedString];

	NSData *codeData = [self objectForKey:codeKeyStr];
	if (!codeData) {
		if (result) { *result = NSSecureUserDefaultsNotFound; }
		return nil;
	}

	if (result) { *result = NSSecureUserDefaultsCorrupted; }

	NSData *codeDataDecrypt = [codeData DecryptAES:_secretData];
	id object = codeDataDecrypt;

	if (codeData && codeDataDecrypt && object) {
		@try {
			id unpackedObj = [NSKeyedUnarchiver unarchiveObjectWithData:object];
			if (result) { *result = NSSecureUserDefaultsFound; }
			return unpackedObj;
		}
		@catch (NSException *ex) {}
	}

	return nil;
}

@end
