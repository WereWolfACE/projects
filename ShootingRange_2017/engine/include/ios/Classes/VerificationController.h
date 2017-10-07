// Проверка покупок для ios, с ios 7 - deprecated

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface VerificationController : NSObject {
    NSMutableDictionary *transactionsReceiptStorageDictionary;
}

+ (VerificationController *) sharedInstance;


// Checking the results of this is not enough.
// The final verification happens in the connection:didReceiveData: callback within
// this class.  So ensure IAP feaures are unlocked from there.
- (BOOL)verifyPurchase:(SKPaymentTransaction *)transaction;

- (BOOL)hasPendingVerification;

@end
