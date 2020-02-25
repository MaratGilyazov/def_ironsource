#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS)
#include "ironsource.h"
#include "ironsource_callback.h"
#include "IronSource/IronSource.h"

#include <UIKit/UIKit.h>

UIViewController *uiViewController;

@interface IrionsourceInterstitialDelegate : NSObject<ISInterstitialDelegate>
@end

@implementation IrionsourceInterstitialDelegate

-(void)interstitialDidLoad {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_LOAD);
}

-(void)interstitialDidShow {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_SHOW);
}

-(void)didClickInterstitial {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_CLICK);
}

-(void)interstitialDidClose {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_CLOSE);
}

-(void)interstitialDidOpen {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_OPEN);
}

-(void)interstitialDidFailToShowWithError:(NSError *)error {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_FAIL_TO_SHOW);
}

-(void)interstitialDidFailToLoadWithError:(NSError *)error {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_FAIL_TO_LOAD);
}

@end

@interface IrionsourceRewardedDelegate : NSObject<ISRewardedVideoDelegate>
@end

@implementation IrionsourceRewardedDelegate

- (void)rewardedVideoHasChangedAvailability:(BOOL)available {
    if (available == YES) {
        IronsourceCallback_add_to_queue((int)REWARDED_AVAILABLE);
    } else {
        IronsourceCallback_add_to_queue((int)REWARDED_NOT_AVAILABLE);
    }
}

- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_COMPLETE);
}

- (void)rewardedVideoDidFailToShowWithError:(NSError *)error {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_FAIL);
}

- (void)rewardedVideoDidOpen {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_OPEN);
}

- (void)rewardedVideoDidClose {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_CLOSE);
}

- (void)didClickRewardedVideo:(ISPlacementInfo *)placementInfo {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_CLICK);
}

- (void)rewardedVideoDidStart {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_START);
}

- (void)rewardedVideoDidEnd {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_END);
}

@end

void Ironsource_InitExtension() {

}

void Ironsource_OnAppPause() {

}

void Ironsource_OnAppResume() {

}

void Ironsource_Init(const char* api_key, bool gdpr_consent) {
    if (gdpr_consent) {
        [IronSource setConsent:YES];
        [IronSource setMetaDataWithKey:@"do_not_sell" value:@"NO"];
    } else {
        [IronSource setConsent:NO];
        [IronSource setMetaDataWithKey:@"do_not_sell" value:@"YES"];
    }
    
    [IronSource initWithAppKey:[NSString stringWithUTF8String:api_key]];

    UIWindow* window = dmGraphics::GetNativeiOSUIWindow();
    uiViewController = window.rootViewController;

    IrionsourceInterstitialDelegate* interstitialDelegate = [[IrionsourceInterstitialDelegate alloc] init];
    [IronSource setInterstitialDelegate:interstitialDelegate];

    IrionsourceRewardedDelegate* rewardedDelegate = [[IrionsourceRewardedDelegate alloc] init];
    [IronSource setRewardedVideoDelegate:rewardedDelegate];
}

void Ironsource_ValidateIntegration() {
    [ISIntegrationHelper validateIntegration];
}

void Ironsource_LoadInterstitial() {
    [IronSource loadInterstitial];
}

bool Ironsource_IsInterstitialReady() {
    BOOL status = [IronSource hasInterstitial];
    return status == YES;
}

void Ironsource_ShowInterstitial() {
    [IronSource showInterstitialWithViewController:uiViewController];
}

bool Ironsource_IsRewardedReady() {
    BOOL status = [IronSource hasRewardedVideo];
    return status == YES;
}

void Ironsource_ShowRewarded() {
    [IronSource showRewardedVideoWithViewController:uiViewController];
}

#endif
