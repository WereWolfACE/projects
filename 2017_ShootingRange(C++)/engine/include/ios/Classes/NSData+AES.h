#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonCryptor.h>


@interface NSData (AES)

- (NSData*) EncryptAES: (NSData *) key;
- (NSData *) DecryptAES: (NSData *) key;

@end
