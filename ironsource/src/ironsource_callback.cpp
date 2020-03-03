#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_ANDROID)
#include "ironsource_callback.h"
#include "utils/LuaUtils.h"
#include <stdlib.h>

static IronsourceListener      defUtoLua;
static dmArray<CallbackData>   m_callbacksQueue;
static dmMutex::HMutex         m_mutex;

static void RegisterCallback(lua_State* L, int index, IronsourceListener* cbk)
{
  if(cbk->m_Callback != LUA_NOREF)
  {
    dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Callback);
    dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Self);
  }

  cbk->m_L = dmScript::GetMainThread(L);

  luaL_checktype(L, index, LUA_TFUNCTION);
  lua_pushvalue(L, index);
  cbk->m_Callback = dmScript::Ref(L, LUA_REGISTRYINDEX);

  dmScript::GetInstance(L);
  cbk->m_Self = dmScript::Ref(L, LUA_REGISTRYINDEX);
}

static void UnregisterCallback(IronsourceListener* cbk)
{
  if(cbk->m_Callback != LUA_NOREF)
  {
    dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Callback);
    dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Self);
    cbk->m_Callback = LUA_NOREF;
  }
}

static void IronsourceCallback_invoke_callback(int type, IronsourceListener* cbk)
{
  if(cbk->m_Callback == LUA_NOREF)
  {
    dmLogInfo("Ironsource callback do not exist.");
    return;
  }

  lua_State* L = cbk->m_L;
  int top = lua_gettop(L);
  lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Callback);
  lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Self);
  lua_pushvalue(L, -1);
  dmScript::SetInstance(L);

  if (!dmScript::IsInstanceValid(L)) {
    UnregisterCallback(&defUtoLua);
    dmLogError("Could not run Ironsource callback because the instance has been deleted.");
    lua_pop(L, 2);
  }
  else {
    lua_pushnumber(L, type);
    int count_table_elements = 0;

    lua_createtable(L, 0, count_table_elements);
    // luaL_push_pair_str_str(L, key_1, value_1);

    int ret = lua_pcall(L, 3, 0, 0);
    if(ret != 0) {
      dmLogError("Error running callback: %s", lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  }
  assert(top == lua_gettop(L));
}

void IronsourceCallback_initialize(){
  m_mutex = dmMutex::New();
}

void IronsourceCallback_finalize(){
  dmMutex::Delete(m_mutex);
  UnregisterCallback(&defUtoLua);
}

void IronsourceCallback_set_callback(lua_State* L, int pos){
  int type = lua_type(L, pos);
  if (type == LUA_TNONE || type == LUA_TNIL) {
    UnregisterCallback(&defUtoLua);
  }
  else{
    RegisterCallback(L, pos, &defUtoLua);
  }
}

void IronsourceCallback_add_to_queue(int type){
  DM_MUTEX_SCOPED_LOCK(m_mutex);
  
  CallbackData data;
  data.msg_type = type;

  if(m_callbacksQueue.Full())
  {
    m_callbacksQueue.OffsetCapacity(4);
  }
  m_callbacksQueue.Push(data);
}

void IronsourceCallback_callback_updates(){
  if (m_callbacksQueue.Empty()) {
    return;
  }

  DM_MUTEX_SCOPED_LOCK(m_mutex);
  
  for(uint32_t i = 0; i != m_callbacksQueue.Size(); ++i)
  {
    CallbackData* data = &m_callbacksQueue[i];
    IronsourceCallback_invoke_callback(data->msg_type, &defUtoLua);
    // if(data->value_1)
    //   free(data->value_1);
    // data->value_1 = 0;
  }
  m_callbacksQueue.SetSize(0);
}
#endif
