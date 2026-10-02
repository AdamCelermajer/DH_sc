#include "methods.h"
#include "../character-classes/classes.h"
#include "../equipment-bonuses/equipment.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#include <string.h>
static char data_key;
static char class_key;
static char loot_key;
#define STATE_TYPE "dh2.source.property-state"
struct dataset { struct dh2_property_table table;unsigned char bytes[]; };
struct class_dataset { struct dh2_class_table table;unsigned char bytes[]; };
struct loot_dataset { struct dh2_loot_tables table;unsigned char bytes[]; };
struct object { struct dh2_character_props state;struct dh2_equipment equipment; };
static struct dataset *data_for(lua_State *L,int object) {
    lua_getfenv(L,object);lua_rawgeti(L,-1,1);
    struct dataset *data=(struct dataset *)lua_touserdata(L,-1);
    if(!data)luaL_error(L,"property dataset unavailable");
    return data;
}
static int method(lua_State *L,uint32_t op) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    int count=lua_gettop(L)-1;if(count>32)return luaL_error(L,"property argument limit exceeded");
    float args[32]={0};uint32_t tags[32]={0};
    for(int i=0;i<count;++i) {
        int t=lua_type(L,i+2);
        if(t==LUA_TNUMBER) { tags[i]=3;args[i]=(float)lua_tonumber(L,i+2); }
        else if(t==LUA_TBOOLEAN) { tags[i]=1;args[i]=(float)lua_toboolean(L,i+2); }
        else if(t==LUA_TUSERDATA || t==LUA_TLIGHTUSERDATA)tags[i]=2;
    }
    struct dataset *data=data_for(L,1);struct dh2_property_result result;
    uint32_t status=dh2_character_property_method(&data->table,&obj->state,op,args,tags,(uint32_t)count,&result);
    if(status)return luaL_error(L,"unsupported or invalid property arguments");
    if(result.count) { lua_pushinteger(L,result.value);return 1; }
    return 0;
}
static int get_prop(lua_State *L) { return method(L,0); }
static int set_prop(lua_State *L) { return method(L,1); }
static int equipment_bonus(lua_State *L,uint32_t op) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)<2)return 0;
    uint32_t off=0;int type=lua_type(L,2);
    if(type==LUA_TBOOLEAN)off=(uint32_t)lua_toboolean(L,2);
    else if(type==LUA_TNUMBER)off=lua_tonumber(L,2)!=0;
    else if(type!=LUA_TNIL)return luaL_error(L,"unsupported equipment bonus argument");
    int32_t value;
    if(dh2_equipment_bonus(&obj->equipment,&obj->state.final,op,off,&value))return luaL_error(L,"invalid equipment snapshot");
    lua_pushinteger(L,value);return 1;
}
static int crit_bonus(lua_State *L) { return equipment_bonus(L,DH2_BONUS_CRIT); }
static int attack_bonus(lua_State *L) { return equipment_bonus(L,DH2_BONUS_ATTACK); }
static int damage_bonus(lua_State *L) { return equipment_bonus(L,DH2_BONUS_DAMAGE); }
static int has_shield(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);int32_t value;
    if(dh2_equipment_flag(&obj->equipment,DH2_HAS_SHIELD,&value))return luaL_error(L,"invalid equipment snapshot");
    lua_pushboolean(L,value);return 1;
}
static int select_equipment(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=2 || lua_type(L,2)!=LUA_TNUMBER)return luaL_error(L,"invalid equipment set");
    lua_Number set=lua_tonumber(L,2);
    if(set!=0 && set!=1)return luaL_error(L,"invalid equipment set");
    obj->equipment.current_set=(uint32_t)set;return 0;
}
static int equip_item(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=4 || lua_type(L,2)!=LUA_TNUMBER || lua_type(L,3)!=LUA_TNUMBER || lua_type(L,4)!=LUA_TNUMBER)
        return luaL_error(L,"invalid equipment arguments");
    lua_Number set=lua_tonumber(L,2),slot=lua_tonumber(L,3),id=lua_tonumber(L,4);
    if((set!=0 && set!=1) || !isfinite(slot) || slot<0 || slot>2 || (lua_Number)(uint32_t)slot!=slot)
        return luaL_error(L,"invalid equipment slot");
    lua_getfenv(L,1);lua_rawgeti(L,-1,3);struct loot_dataset *loot=lua_touserdata(L,-1);
    if(!loot)return luaL_error(L,"item data is not loaded for this object");
    if(!isfinite(id) || id< -1 || id>=loot->table.counts[DH2_ITEMS] || (lua_Number)(int32_t)id!=id)
        return luaL_error(L,"invalid item index");
    int32_t ids[6];for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i)ids[3*s+i]=obj->equipment.slots[s][i].item_id;
    ids[(uint32_t)set*3+(uint32_t)slot]=(int32_t)id;struct dh2_equipment next;
    if(dh2_equipment_load(&loot->table,ids,obj->equipment.current_set,(uint32_t)obj->state.final.values[203],&next))
        return luaL_error(L,"invalid equipment data");
    obj->equipment=next;return 0;
}
static int apply_class(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    int count=lua_gettop(L);
    if(count<2 || count>3 || lua_type(L,2)!=LUA_TNUMBER ||
       (count==3 && lua_type(L,3)!=LUA_TBOOLEAN))return luaL_error(L,"invalid class arguments");
    lua_Number id=lua_tonumber(L,2);uint32_t flag=count==3?(uint32_t)lua_toboolean(L,3):0;
    struct dataset *data=data_for(L,1);
    lua_getfenv(L,1);lua_rawgeti(L,-1,2);
    struct class_dataset *classes=lua_touserdata(L,-1);
    if(!classes)return luaL_error(L,"character class data is not loaded for this object");
    if(!isfinite(id) || id<0 || id>=classes->table.count || (lua_Number)(uint32_t)id!=id)
        return luaL_error(L,"invalid class index");
    /* Authored diagnostic method: apply to base, then recompose all final fields.
     * It does not reset/reload base or implement the original Character lifecycle. */
    struct dh2_character_props next=obj->state;
    if(dh2_class_apply(&classes->table,&data->table,&next,0,NULL,(int32_t)id,flag))
        return luaL_error(L,"invalid class data or application limit exceeded");
    struct dh2_property_inputs inputs={&next.base,&next.saved,&next.gears,NULL,0};
    for(uint32_t field=0;field<224;++field)if(dh2_property_recalc(&data->table,&inputs,field,&next.final))
        return luaL_error(L,"invalid character property data");
    obj->state=next;return 0;
}
static int create(lua_State *L) {
    if(lua_type(L,1)!=LUA_TNUMBER)return luaL_error(L,"property row must be a number");
    lua_Number row=lua_tonumber(L,1);
    lua_pushlightuserdata(L,&data_key);lua_rawget(L,LUA_REGISTRYINDEX);
    struct dataset *data=(struct dataset *)lua_touserdata(L,-1);
    if(!data)return luaL_error(L,"character property data is not loaded");
    if(!isfinite(row) || row<0 || row>=data->table.counts[0] || (lua_Number)(uint32_t)row!=row)
        return luaL_error(L,"invalid property row");
    int dataset_index=lua_gettop(L);
    struct object *obj=lua_newuserdata(L,sizeof(*obj));
    memset(&obj->equipment,0,sizeof(obj->equipment));
    for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i)obj->equipment.slots[s][i].item_id= -1;
    if(dh2_character_props_init(&data->table,&obj->state) || dh2_property_load(&data->table,(uint32_t)row,&obj->state.base))
        return luaL_error(L,"invalid character property data");
    struct dh2_property_inputs inputs={&obj->state.base,&obj->state.saved,&obj->state.gears,NULL,0};
    for(uint32_t id=0;id<224;++id)if(dh2_property_recalc(&data->table,&inputs,id,&obj->state.final))
        return luaL_error(L,"invalid character property data");
    luaL_getmetatable(L,STATE_TYPE);lua_setmetatable(L,-2);
    /* Each state retains its exact dataset generation after replacement. */
    lua_newtable(L);lua_pushvalue(L,dataset_index);lua_rawseti(L,-2,1);
    lua_pushlightuserdata(L,&class_key);lua_rawget(L,LUA_REGISTRYINDEX);lua_rawseti(L,-2,2);
    lua_pushlightuserdata(L,&loot_key);lua_rawget(L,LUA_REGISTRYINDEX);lua_rawseti(L,-2,3);
    lua_setfenv(L,-2);
    return 1;
}
void dh2_lua_register_characters(lua_State *L) {
    luaL_newmetatable(L,STATE_TYPE);lua_newtable(L);
    lua_pushcfunction(L,get_prop);lua_setfield(L,-2,"GetProp");
    lua_pushcfunction(L,set_prop);lua_setfield(L,-2,"SetProp");
    lua_pushcfunction(L,apply_class);lua_setfield(L,-2,"ApplyClass");
    lua_pushcfunction(L,crit_bonus);lua_setfield(L,-2,"GetCritRatingBonus");
    lua_pushcfunction(L,attack_bonus);lua_setfield(L,-2,"GetAttackRatingBonus");
    lua_pushcfunction(L,damage_bonus);lua_setfield(L,-2,"GetDamageBonus");
    lua_pushcfunction(L,has_shield);lua_setfield(L,-2,"HasShield");
    lua_pushcfunction(L,equip_item);lua_setfield(L,-2,"EquipItem");
    lua_pushcfunction(L,select_equipment);lua_setfield(L,-2,"SelectEquipmentSet");
    lua_setfield(L,-2,"__index");lua_pushboolean(L,0);lua_setfield(L,-2,"__metatable");lua_pop(L,1);
    lua_pushcfunction(L,create);lua_setglobal(L,"DH2CreatePropertyState");
}
static int import(lua_State *L) {
    const struct dh2_property_table *input=lua_touserdata(L,1);
    struct dataset *data=lua_newuserdata(L,sizeof(*data)+input->size);
    memcpy(data->bytes,input->bytes,input->size);
    if(dh2_property_open(&data->table,data->bytes,input->size))return luaL_error(L,"invalid property dataset");
    lua_pushlightuserdata(L,&data_key);lua_pushvalue(L,-2);lua_rawset(L,LUA_REGISTRYINDEX);
    return 0;
}
int dh2_lua_characters_load(lua_State *L,const struct dh2_property_table *view) {
    return lua_cpcall(L,import,(void *)view);
}
static int import_classes(lua_State *L) {
    const struct dh2_class_table *input=lua_touserdata(L,1);
    struct class_dataset *data=lua_newuserdata(L,sizeof(*data)+input->size);
    memcpy(data->bytes,input->bytes,input->size);
    if(dh2_class_open(&data->table,data->bytes,input->size))return luaL_error(L,"invalid class dataset");
    lua_pushlightuserdata(L,&class_key);lua_pushvalue(L,-2);lua_rawset(L,LUA_REGISTRYINDEX);
    return 0;
}
int dh2_lua_classes_load(lua_State *L,const struct dh2_class_table *view) {
    return lua_cpcall(L,import_classes,(void *)view);
}
static int import_loot(lua_State *L) {
    const struct dh2_loot_tables *input=lua_touserdata(L,1);
    struct loot_dataset *data=lua_newuserdata(L,sizeof(*data)+input->size);
    memcpy(data->bytes,input->bytes,input->size);
    if(dh2_loot_open(&data->table,data->bytes,input->size))return luaL_error(L,"invalid item dataset");
    lua_pushlightuserdata(L,&loot_key);lua_pushvalue(L,-2);lua_rawset(L,LUA_REGISTRYINDEX);return 0;
}
int dh2_lua_loot_load(lua_State *L,const struct dh2_loot_tables *view) { return lua_cpcall(L,import_loot,(void *)view); }
