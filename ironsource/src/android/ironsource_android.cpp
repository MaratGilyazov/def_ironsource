#if defined(DM_PLATFORM_ANDROID)

void Ironsource_Init(const char* api_key, bool gdpr_consent) { }
void Ironsource_ValidateIntegration() { }
void Ironsource_LoadInterstitial() { }
bool Ironsource_IsInterstitialReady() { }
void Ironsource_ShowInterstitial() { }
bool Ironsource_IsRewardedReady() { }
void Ironsource_ShowRewarded() { }

#endif