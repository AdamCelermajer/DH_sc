#include "quests.h"
#include "../quest-kill/quest.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#include <string.h>
void dh2_lua_push_kill_objective(lua_State *,const struct dh2_kill_objective *,uint32_t);
static char data_key;
struct dataset {struct dh2_quest_table table;unsigned char bytes[];};
static int32_t integer(lua_State *L,int index) {
    if(lua_type(L,index)!=LUA_TNUMBER)luaL_error(L,"quest arguments must be integers");
    lua_Number n=lua_tonumber(L,index);
    if(!isfinite(n) || n< -2147483648.0 || n>=2147483648.0 || (lua_Number)(int32_t)n!=n)luaL_error(L,"invalid quest integer");
    return (int32_t)n;
}
static struct dataset *current(lua_State *L) {
    lua_pushlightuserdata(L,&data_key);lua_rawget(L,LUA_REGISTRYINDEX);
    struct dataset *data=lua_touserdata(L,-1);if(!data)luaL_error(L,"quest dataset unavailable");return data;
}
static void number(lua_State *L,const char *key,int32_t n) {lua_pushinteger(L,n);lua_setfield(L,-2,key);}
static void numbers(lua_State *L,const int32_t *values,uint32_t count) {
    lua_createtable(L,(int)count,0);for(uint32_t i=0;i<count;++i) {lua_pushinteger(L,values[i]);lua_rawseti(L,-2,(int)i+1);}
}
static void text(lua_State *L,const struct dataset *data,struct dh2_quest_span span) {
    lua_pushlstring(L,(const char *)data->bytes+span.offset,span.size);
}
static void objective(lua_State *L,const struct dataset *data,const struct dh2_quest_objective *o) {
    lua_newtable(L);numbers(L,o->common,3);lua_setfield(L,-2,"common");numbers(L,o->args,3);lua_setfield(L,-2,"args");
    lua_createtable(L,2,0);for(unsigned i=0;i<2;++i) {text(L,data,o->strings[i]);lua_rawseti(L,-2,(int)i+1);}lua_setfield(L,-2,"strings");
}
static int count(lua_State *L) {
    if(lua_gettop(L))return luaL_error(L,"quest count takes no arguments");
    struct dataset *data=current(L);lua_pushinteger(L,(lua_Integer)data->table.count);return 1;
}
static int record(lua_State *L) {
    if(lua_gettop(L)!=1)return luaL_error(L,"quest record requires one row");
    int32_t row=integer(L,1);struct dataset *data=current(L);struct dh2_quest_record r;
    if(row<0 || dh2_quests_record(&data->table,(uint32_t)row,&r))return luaL_error(L,"invalid quest row");
    lua_newtable(L);numbers(L,r.ids,4);lua_setfield(L,-2,"ids");
    const char *keys[]={"conditions","objectives","rewards","rewards_hard","rewards_very_hard"};
    for(uint32_t group=0;group<5;++group) {
        lua_createtable(L,(int)r.lists[group].count,0);
        for(uint32_t i=0;i<r.lists[group].count;++i) {
            if(group==1) {
                struct dh2_quest_objective o;if(dh2_quests_objective(&data->table,(uint32_t)row,i,&o))return luaL_error(L,"invalid quest objective");
                objective(L,data,&o);
            } else {
                struct dh2_quest_span s;if(dh2_quests_list_record(&data->table,(uint32_t)row,group,i,&s))return luaL_error(L,"invalid quest list");
                int32_t values[3];for(uint32_t j=0;j<3;++j) {
                    const unsigned char *p=data->bytes+s.offset+j*4;
                    uint32_t bits=(uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;memcpy(&values[j],&bits,4);
                }
                numbers(L,values,3);
            }
            lua_rawseti(L,-2,(int)i+1);
        }
        lua_setfield(L,-2,keys[group]);
    }
    objective(L,data,&r.accept);lua_setfield(L,-2,"accept");objective(L,data,&r.end);lua_setfield(L,-2,"end");
    number(L,"target_level",r.target_level);number(L,"repeatable_byte",(int32_t)r.repeatable);number(L,"state",r.state);
    number(L,"priority",r.priority);number(L,"act",r.act);
    lua_createtable(L,14,0);for(unsigned i=0;i<14;++i) {text(L,data,r.scripts[i]);lua_rawseti(L,-2,(int)i+1);}lua_setfield(L,-2,"scripts");return 1;
}
static int create(lua_State *L) {
    if(lua_gettop(L)!=4 || lua_type(L,4)!=LUA_TBOOLEAN)return luaL_error(L,"quest kill objective requires row, objective, current and completed");
    int32_t row=integer(L,1),index=integer(L,2),progress=integer(L,3);uint32_t completed=(uint32_t)lua_toboolean(L,4);
    struct dataset *data=current(L);int dataset_index=lua_gettop(L);struct dh2_quest_objective o;
    if(row<0 || index<0 || dh2_quests_objective(&data->table,(uint32_t)row,(uint32_t)index,&o))return luaL_error(L,"invalid quest objective index");
    /* Native kill types 0/10 use property/template ID at +0x20, required count
     * +0x28. Clear types need compiled live-world counts, still pending. */
    if((o.common[0]!=0 && o.common[0]!=10) || o.args[2]<=0)return luaL_error(L,"quest objective is not a supported counted kill");
    struct dh2_kill_objective state={o.args[0],progress,o.args[2],completed};
    dh2_lua_push_kill_objective(L,&state,o.common[0]==0?0:2);
    lua_newtable(L);lua_pushvalue(L,dataset_index);lua_rawseti(L,-2,1);lua_setfenv(L,-2);return 1;
}
void dh2_lua_register_quest_data(lua_State *L) {
    lua_pushcfunction(L,count);lua_setglobal(L,"DH2GetQuestCount");lua_pushcfunction(L,record);lua_setglobal(L,"DH2GetQuestRecord");
    lua_pushcfunction(L,create);lua_setglobal(L,"DH2CreateQuestKillObjective");
}
static int import(lua_State *L) {
    const struct dh2_quest_table *input=lua_touserdata(L,1);
    struct dataset *data=lua_newuserdata(L,sizeof(*data)+input->size);memcpy(data->bytes,input->bytes,input->size);
    if(dh2_quests_open(&data->table,data->bytes,input->size))return luaL_error(L,"invalid quest dataset");
    lua_pushlightuserdata(L,&data_key);lua_pushvalue(L,-2);lua_rawset(L,LUA_REGISTRYINDEX);return 0;
}
int dh2_lua_quests_load(lua_State *L,const struct dh2_quest_table *table) {return lua_cpcall(L,import,(void *)table);}
