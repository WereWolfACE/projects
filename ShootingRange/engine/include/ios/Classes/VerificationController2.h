// Проверка покупок для мака и ios 7+, пока просто считает все покупки валидными

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface VerificationController2 : NSObject {
}

+ (VerificationController2 *) sharedInstance;


// Checking the results of this is not enough.
// The final verification happens in the connection:didReceiveData: callback within
// this class.  So ensure IAP feaures are unlocked from there.
- (BOOL)addPurchaseToVerificationQueue:(SKPaymentTransaction *)transaction;

- (void)verifyQueuedPurchases;

- (BOOL)hasPendingVerification;

@end
