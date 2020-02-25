#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include "../ironsource.h"

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

void Ironsource_InitExtension() {
    dmLogUserDebug("Ironsource_InitExtension");
}

void Ironsource_OnAppPause() {
    dmLogUserDebug("Ironsource_OnAppPause");
}

void Ironsource_OnAppResume() {
    dmLogUserDebug("Ironsource_OnAppResume");
}

void Ironsource_Init(const char* api_key, bool gdpr_consent) {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.ironsource.mediationsdk.IronSource");
    jmethodID method = env->GetStaticMethodID(cls, "init", "(Landroid/app/Activity;Ljava/lang/String;)V");
    jstring appkey = env->NewStringUTF(api_key);
    env->CallStaticVoidMethod(cls, method, dmGraphics::GetNativeAndroidActivity(), appkey);
    // TODO IronSource.setConsent(gdpr_consent);
    env->DeleteLocalRef(appkey);
}

void Ironsource_ValidateIntegration() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.ironsource.mediationsdk.integration.IntegrationHelper");
    jmethodID method = env->GetStaticMethodID(cls, "validateIntegration", "(Landroid/app/Activity;)V");
    env->CallStaticVoidMethod(cls, method, dmGraphics::GetNativeAndroidActivity());
}

void Ironsource_LoadInterstitial() { }
bool Ironsource_IsInterstitialReady() { }
void Ironsource_ShowInterstitial() { }
bool Ironsource_IsRewardedReady() { }
void Ironsource_ShowRewarded() { }

#endif