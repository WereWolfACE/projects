//
//  NSUserDefaults+MPSecureUserDefaults.h
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

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>


enum {
	NSSecureUserDefaultsFound = 0,
	NSSecureUserDefaultsNotFound = 1,
	NSSecureUserDefaultsCorrupted = 2
};
typedef NSInteger NSSecureUserDefaultsFindResult;


@interface NSUserDefaults (MPSecureUserDefaults)

/**
 * Sets the secret. Make sure that your secret is stored in a save place, it is recommanded to write it
 * directly into your code. Required property.
 */
+ (void)setSecret:(NSData *)secret;
+ (void)setCrypt:(BOOL)cryptData;

/**
 * Read data from user defaults. If key doesn't exist, valid is YES and the function mimics
 * the return behavior of the respective non-secure method. Please note that the methods below
 * will always return the result, even if it is *NOT* secure. This is a change from previous versions
 * of Secure-NSUserDefaults. It is therefore necessary to check to figure out an appropriate consequence
 * for invalid defaults.
 */
- (BOOL)secureBoolForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (float)secureFloatForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (double)secureDoubleForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSInteger)secureIntegerForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSArray *)secureArrayForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSArray *)secureStringArrayForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSData *)secureDataForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSDictionary *)secureDictionaryForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSNumber *)secureNumberForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (NSString *)secureStringForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;
- (id)secureObjectForKey:(NSString *)key resultType:(NSSecureUserDefaultsFindResult *)result;

/**
 * Write data to user defaults. Only property list objects (NSData, NSString, NSNumber, NSDate, NSArray, NSDictionary)
 * are supported. Passing nil as either the value or key mimics the behavior of the non-secure method.
 */
- (void)setSecureBool:(BOOL)value forKey:(NSString *)key;
- (void)setSecureFloat:(float)value forKey:(NSString *)key;
- (void)setSecureDouble:(double)value forKey:(NSString *)key;
- (void)setSecureInteger:(NSInteger)value forKey:(NSString *)key;
- (void)setSecureNumber:(NSNumber *)value forKey:(NSString *)key;
- (void)setSecureObject:(id)value forKey:(NSString *)key;
- (void)removeSecureObjectForKey:(NSString *)key;

@end
