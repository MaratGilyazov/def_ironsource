#pragma once
#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_ANDROID)

extern void Ironsource_InitExtension();
extern void Ironsource_OnAppPause();
extern void Ironsource_OnAppResume();

extern void Ironsource_SetConsent(bool consent);
extern void Ironsource_Init(const char* api_key);
extern void Ironsource_ValidateIntegration();

extern void Ironsource_LoadInterstitial();
extern bool Ironsource_IsInterstitialReady();
extern void Ironsource_ShowInterstitial();
extern bool Ironsource_IsRewardedReady();
extern void Ironsource_ShowRewarded();

#endif