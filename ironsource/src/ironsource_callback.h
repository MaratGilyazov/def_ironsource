#pragma once

#include <dmsdk/sdk.h>

enum IronsourceCallbackType
{
  INTERSTITIAL_DID_LOAD,
  INTERSTITIAL_DID_SHOW,
  INTERSTITIAL_DID_CLICK,
  INTERSTITIAL_DID_CLOSE,
  INTERSTITIAL_DID_OPEN,
  INTERSTITIAL_DID_FAIL_TO_SHOW,
  INTERSTITIAL_DID_FAIL_TO_LOAD,

  REWARDED_AVAILABLE,
  REWARDED_NOT_AVAILABLE,
  REWARDED_DID_COMPLETE,
  REWARDED_DID_FAIL,
  REWARDED_DID_OPEN,
  REWARDED_DID_CLOSE,
  REWARDED_DID_CLICK,
  REWARDED_DID_START,
  REWARDED_DID_END
};

struct IronsourceListener {
  IronsourceListener() : m_L(0), m_Callback(LUA_NOREF), m_Self(LUA_NOREF) {
  }
  lua_State* m_L;
  int m_Callback;
  int m_Self;
};

struct CallbackData
{
  int msg_type;
};

void IronsourceCallback_set_callback(lua_State* L, int pos);
void IronsourceCallback_callback_updates();
void IronsourceCallback_initialize();
void IronsourceCallback_finalize();
void IronsourceCallback_add_to_queue(int type);
