#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include "com_afeskov_defironsource_IronSourceWrapper.h"
#include "../ironsource.h"
#include "../ironsource_callback.h"

static JNIEnv* Attach()
{
    JNIEnv* env;
    JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->AttachCurrentThread(&env, NULL);
    return env;
}

static bool Detach(JNIEnv* env)
{
    bool exception = (bool) env->ExceptionCheck();
    env->ExceptionClear();
    JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->DetachCurrentThread();
    return !exception;
}

namespace {
struct AttachScope
{
    JNIEnv* m_Env;
    AttachScope() : m_Env(Attach())
    {
    }
    ~AttachScope()
    {
        Detach(m_Env);
    }
};
}

static jclass GetClass(JNIEnv* env, const char* classname)
{
    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF(classname);
    jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);
    return outcls;
}

struct IronSourceWrapperClass
{
  jobject                 m_ISW_JNI;

  jmethodID               m_init;
  jmethodID               m_setConsent;
  jmethodID               m_onPause;
  jmethodID               m_onResume;
  jmethodID               m_validateIntegration;
  jmethodID               m_loadInterstitial;
  jmethodID               m_isInterstitialReady;
  jmethodID               m_showInterstitial;
  jmethodID               m_isRewardedReady;
  jmethodID               m_showRewarded;
};

IronSourceWrapperClass g_isw;

void Ironsource_InitExtension() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;
    
    jclass cls = GetClass(env, "com/afeskov/defironsource/IronSourceWrapper");
    g_isw.m_init = env->GetMethodID(cls, "init", "(Ljava/lang/String;)V");
    g_isw.m_setConsent = env->GetMethodID(cls, "setConsent", "(Z)V");
    g_isw.m_onPause = env->GetMethodID(cls, "onPause", "()V");
    g_isw.m_onResume = env->GetMethodID(cls, "onResume", "()V");
    g_isw.m_validateIntegration = env->GetMethodID(cls, "validateIntegration", "()V");
    g_isw.m_loadInterstitial = env->GetMethodID(cls, "loadInterstitial", "()V");
    g_isw.m_isInterstitialReady = env->GetMethodID(cls, "isInterstitialReady", "()Z");
    g_isw.m_showInterstitial = env->GetMethodID(cls, "showInterstitial", "(Ljava/lang/String;)V");
    g_isw.m_isRewardedReady = env->GetMethodID(cls, "isRewardedReady", "()Z");
    g_isw.m_showRewarded = env->GetMethodID(cls, "showRewarded", "(Ljava/lang/String;)V");

    jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");
    g_isw.m_ISW_JNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, dmGraphics::GetNativeAndroidActivity()));
}

void Ironsource_OnAppPause() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_onPause);
}

void Ironsource_OnAppResume() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_onResume);
}

void Ironsource_SetConsent(bool consent) {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_setConsent, consent ? JNI_TRUE : JNI_FALSE);
}

void Ironsource_Init(const char* api_key) {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jstring appkey = env->NewStringUTF(api_key);
    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_init, appkey);
    env->DeleteLocalRef(appkey);
}

void Ironsource_ValidateIntegration() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_validateIntegration);
}

void Ironsource_LoadInterstitial() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_loadInterstitial);
}

bool Ironsource_IsInterstitialReady() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jboolean return_value = (jboolean)env->CallBooleanMethod(g_isw.m_ISW_JNI, g_isw.m_isInterstitialReady);
    return JNI_TRUE == return_value;
}

void Ironsource_ShowInterstitial() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_showInterstitial, NULL);
}

bool Ironsource_IsRewardedReady() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jboolean return_value = (jboolean)env->CallBooleanMethod(g_isw.m_ISW_JNI, g_isw.m_isRewardedReady);
    return JNI_TRUE == return_value;
}

void Ironsource_ShowRewarded() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_showRewarded, NULL);
}

//-----------------------------------------------------------------------------

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdOpened(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_OPEN);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdClosed(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_CLOSE);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAvailabilityChanged(JNIEnv *env, jclass jcls, jboolean jbool) {
    IronsourceCallback_add_to_queue((bool)jbool ? (int)REWARDED_AVAILABLE : (int)REWARDED_NOT_AVAILABLE);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdStarted(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_START);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdEnded(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)REWARDED_DID_END);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdRewarded(JNIEnv *env, jclass jcls, jint jnum, jstring jstr) {
    const char* ch = env->GetStringUTFChars(jstr, 0);
    dmLogUserDebug("onRewardedVideoAdRewarded placementId: %i placementName: %s", (int)jnum, ch);
    IronsourceCallback_add_to_queue((int)REWARDED_DID_COMPLETE);
    env->ReleaseStringUTFChars(jstr, ch);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdShowFailed(JNIEnv *env, jclass jcls, jint jnum, jstring jstr) {
    const char* ch = env->GetStringUTFChars(jstr, 0);
    dmLogUserDebug("onRewardedVideoAdShowFailed errorCode: %i errorMessage: %s", (int)jnum, ch);
    IronsourceCallback_add_to_queue((int)REWARDED_DID_FAIL);
    env->ReleaseStringUTFChars(jstr, ch);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onRewardedVideoAdClicked(JNIEnv *env, jclass jcls, jint jnum, jstring jstr) {
    const char* ch = env->GetStringUTFChars(jstr, 0);
    dmLogUserDebug("onRewardedVideoAdClicked placementId: %i placementName: %s", (int)jnum, ch);
    IronsourceCallback_add_to_queue((int)REWARDED_DID_CLICK);
    env->ReleaseStringUTFChars(jstr, ch);
}

//-----------------------------------------------------------------------------

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdReady(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_LOAD);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdLoadFailed(JNIEnv *env, jclass jcls, jint jnum, jstring jstr) {
    const char* ch = env->GetStringUTFChars(jstr, 0);
    dmLogUserDebug("onInterstitialAdLoadFailed errorCode: %i errorMessage: %s", (int)jnum, ch);
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_FAIL_TO_LOAD);
    env->ReleaseStringUTFChars(jstr, ch);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdOpened(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_OPEN);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdClosed(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_CLOSE);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdShowSucceeded(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_SHOW);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdShowFailed(JNIEnv *env, jclass jcls, jint jnum, jstring jstr) {
    const char* ch = env->GetStringUTFChars(jstr, 0);
    dmLogUserDebug("onInterstitialAdShowFailed errorCode: %i errorMessage: %s", (int)jnum, ch);
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_FAIL_TO_SHOW);
    env->ReleaseStringUTFChars(jstr, ch);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onInterstitialAdClicked(JNIEnv *env, jclass jcls) {
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_CLICK);
}

//-----------------------------------------------------------------------------

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onBannerAdLoaded(JNIEnv *env, jclass jcls) {
    dmLogUserDebug("onBannerAdLoaded");
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onBannerAdLoadFailed(JNIEnv *env, jclass jcls, jint jnum, jstring jstr) {
    const char* ch = env->GetStringUTFChars(jstr, 0);
    dmLogUserDebug("onBannerAdLoadFailed errorCode: %i errorMessage: %s", (int)jnum, ch);
    IronsourceCallback_add_to_queue((int)INTERSTITIAL_DID_FAIL_TO_LOAD);
    env->ReleaseStringUTFChars(jstr, ch);
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onBannerAdClicked(JNIEnv *env, jclass jcls) {
    dmLogUserDebug("onBannerAdClicked");
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onBannerAdScreenPresented(JNIEnv *env, jclass jcls) {
    dmLogUserDebug("onBannerAdScreenPresented");
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onBannerAdScreenDismissed(JNIEnv *env, jclass jcls) {
    dmLogUserDebug("onBannerAdScreenDismissed");
}

JNIEXPORT void JNICALL Java_com_afeskov_defironsource_IronSourceWrapper_onBannerAdLeftApplication(JNIEnv *env, jclass jcls) {
    dmLogUserDebug("onBannerAdLeftApplication");
}


#endif