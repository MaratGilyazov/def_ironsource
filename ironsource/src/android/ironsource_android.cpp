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

struct IronSourceWrapperClass
{
  jobject                 m_ISW_JNI;

  jmethodID               m_init;
  jmethodID               m_onPause;
  jmethodID               m_onResume;
  jmethodID               m_validateIntegration;
};

IronSourceWrapperClass g_isw;

void Ironsource_InitExtension() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;
    
    jclass cls = GetClass(env, "com/afeskov/defironsource/IronSourceWrapper");
    g_isw.m_init = env->GetMethodID(cls, "init", "(Ljava/lang/String;)V");
    g_isw.m_onPause = env->GetMethodID(cls, "onPause", "()V");
    g_isw.m_onResume = env->GetMethodID(cls, "onResume", "()V");
    g_isw.m_validateIntegration = env->GetMethodID(cls, "validateIntegration", "()V");

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

void Ironsource_Init(const char* api_key, bool gdpr_consent) {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jstring appkey = env->NewStringUTF(api_key);
    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_init, appkey);
    env->DeleteLocalRef(appkey);
    // TODO IronSource.setConsent(gdpr_consent);
}

void Ironsource_ValidateIntegration() {
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    env->CallVoidMethod(g_isw.m_ISW_JNI, g_isw.m_validateIntegration);
}

void Ironsource_LoadInterstitial() { }
bool Ironsource_IsInterstitialReady() { }
void Ironsource_ShowInterstitial() { }
bool Ironsource_IsRewardedReady() { }
void Ironsource_ShowRewarded() { }

#endif