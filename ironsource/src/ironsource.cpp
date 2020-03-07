#define EXTENSION_NAME Ironsource
#define LIB_NAME "Ironsource"
#define MODULE_NAME "ironsource"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>


#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_ANDROID)
#include "ironsource.h"
#include "ironsource_callback.h"

static int init(lua_State* L) {
    const char* api_key = luaL_checkstring(L, 1);

    if (lua_type(L, 2) != LUA_TNONE) {
        bool consent = lua_toboolean(L, 2);
        Ironsource_SetConsent(consent); 
    } 
    Ironsource_Init(api_key);
    
    return 0;
}

static int set_consent(lua_State* L) {
    bool consent = lua_toboolean(L, 1);
    Ironsource_SetConsent(consent); 

    return 0;
}

static int validate_integration(lua_State* L) {
    Ironsource_ValidateIntegration();
    return 0;
}

static int load_interstitial(lua_State* L) {
    Ironsource_LoadInterstitial();
    return 0;
}

static int is_interstitial_ready(lua_State* L) {
    bool status =  Ironsource_IsInterstitialReady();
    lua_pushboolean(L, status);
    return 1;
}

static int show_interstitial(lua_State* L) {
    Ironsource_ShowInterstitial();
    return 0;
}

static int is_rewarded_ready(lua_State* L) {
    bool status =  Ironsource_IsRewardedReady();
    lua_pushboolean(L, status);
    return 1;
}

static int show_rewarded(lua_State* L) {
    Ironsource_ShowRewarded();
    return 0;
}

static int set_callback(lua_State* L) {
    IronsourceCallback_set_callback(L, 1);
    return 0;
}

static const luaL_reg Module_methods[] =
{
    {"init", init},
    {"set_consent", set_consent},
    {"validate_integration", validate_integration},
    {"set_callback", set_callback},
    {"load_interstitial", load_interstitial},
    {"is_interstitial_ready", is_interstitial_ready},
    {"show_interstitial", show_interstitial},
    {"is_rewarded_ready", is_rewarded_ready},
    {"show_rewarded", show_rewarded},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);
    luaL_register(L, MODULE_NAME, Module_methods);

    #define SETCONSTANT(name) \
    lua_pushnumber(L, (lua_Number) name); \
    lua_setfield(L, -2, #name); \

    SETCONSTANT(INTERSTITIAL_DID_LOAD)
    SETCONSTANT(INTERSTITIAL_DID_SHOW)
    SETCONSTANT(INTERSTITIAL_DID_CLICK)
    SETCONSTANT(INTERSTITIAL_DID_CLOSE)
    SETCONSTANT(INTERSTITIAL_DID_OPEN)
    SETCONSTANT(INTERSTITIAL_DID_FAIL_TO_SHOW)
    SETCONSTANT(INTERSTITIAL_DID_FAIL_TO_LOAD)

    SETCONSTANT(REWARDED_AVAILABLE)
    SETCONSTANT(REWARDED_NOT_AVAILABLE)
    SETCONSTANT(REWARDED_DID_COMPLETE)
    SETCONSTANT(REWARDED_DID_FAIL)
    SETCONSTANT(REWARDED_DID_OPEN)
    SETCONSTANT(REWARDED_DID_CLOSE)
    SETCONSTANT(REWARDED_DID_CLICK)
    SETCONSTANT(REWARDED_DID_START)
    SETCONSTANT(REWARDED_DID_END)

    #undef SETCONSTANT
    
    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeIronsource(dmExtension::AppParams* params)
{
    dmLogWarning("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeIronsource(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    Ironsource_InitExtension();
    IronsourceCallback_initialize();
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeIronsource(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeIronsource(dmExtension::Params* params)
{
    IronsourceCallback_finalize();
    return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateIronsource(dmExtension::Params* params)
{
    IronsourceCallback_callback_updates();
    return dmExtension::RESULT_OK;
}

static void OnEventIronsource(dmExtension::Params* params, const dmExtension::Event* event)
{
    if( event->m_Event == dmExtension::EVENT_ID_ACTIVATEAPP )
        Ironsource_OnAppResume();
    else if( event->m_Event == dmExtension::EVENT_ID_DEACTIVATEAPP )
        Ironsource_OnAppPause();
}

#else // unsupported platforms

dmExtension::Result AppInitializeIronsource(dmExtension::AppParams* params)
{
    dmLogWarning("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeIronsource(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeIronsource(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeIronsource(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateIronsource(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static void OnEventIronsource(dmExtension::Params* params, const dmExtension::Event* event)
{
    return;
}

#endif


DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeIronsource, AppFinalizeIronsource, InitializeIronsource, UpdateIronsource, OnEventIronsource, FinalizeIronsource)
