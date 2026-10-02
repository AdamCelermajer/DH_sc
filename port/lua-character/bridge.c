#include "methods.h"
#include "../character-classes/classes.h"
#include "../equipment-bonuses/equipment.h"
#include "../gear-properties/gears.h"
#include "../character-health/health.h"
#include "../character-damage/damage.h"
#include "../character-death/death.h"
#include "lua.h"
#include "lauxlib.h"
#include <math.h>
#include <string.h>
static char data_key;
static char class_key;
static char loot_key;
static char power_key;
#define STATE_TYPE "dh2.source.property-state"
struct dataset { struct dh2_property_table table;unsigned char bytes[]; };
struct class_dataset { struct dh2_class_table table;unsigned char bytes[]; };
struct loot_dataset { struct dh2_loot_tables table;unsigned char bytes[]; };
struct power_dataset { struct dh2_power_tables table;unsigned char bytes[]; };
#define GEAR_SLOTS 16
#define GEAR_POWERS 32
struct object {
    struct dh2_character_props state;struct dh2_equipment equipment;
    int32_t gear_ids[2][GEAR_SLOTS],power_ids[2][GEAR_SLOTS][GEAR_POWERS];
    uint32_t power_counts[2][GEAR_SLOTS];
    int32_t combat_state;uint16_t hit_count;
    struct dh2_death_actor death;
};
static int get_state(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);lua_pushinteger(L,obj->combat_state);return 1;
}
static int get_hit_count(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);lua_pushinteger(L,obj->hit_count);return 1;
}
static int get_name(lua_State *L) {
    luaL_checkudata(L,1,STATE_TYPE);lua_getfenv(L,1);lua_rawgeti(L,-1,5);return 1;
}
/* Authored inputs for exact recovered combat formulas; this does not execute
 * an original state machine, combo counter or Character constructor. */
static int combat_context(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=4 || lua_type(L,2)!=LUA_TNUMBER || lua_type(L,3)!=LUA_TNUMBER || lua_type(L,4)!=LUA_TSTRING)
        return luaL_error(L,"combat context requires state, hit count and name");
    lua_Number state=lua_tonumber(L,2),hit=lua_tonumber(L,3);size_t length;
    const char *name=lua_tolstring(L,4,&length);
    if(!isfinite(state) || state< -2147483648.0 || state>=2147483648.0 || (lua_Number)(int32_t)state!=state ||
       !isfinite(hit) || hit<0 || hit>65535 || (lua_Number)(uint16_t)hit!=hit || !length || length>255 || memchr(name,0,length))
        return luaL_error(L,"invalid combat context");
    lua_getfenv(L,1);lua_pushvalue(L,4);lua_rawseti(L,-2,5);
    obj->combat_state=(int32_t)state;obj->hit_count=(uint16_t)hit;return 0;
}
static struct dataset *data_for(lua_State *L,int object) {
    lua_getfenv(L,object);lua_rawgeti(L,-1,1);
    struct dataset *data=(struct dataset *)lua_touserdata(L,-1);
    if(!data)luaL_error(L,"property dataset unavailable");
    return data;
}
static uint32_t context_bool(lua_State *L,int index,const char *key) {
    lua_pushstring(L,key);lua_rawget(L,index);
    if(lua_type(L,-1)!=LUA_TBOOLEAN)luaL_error(L,"death context flags must be booleans");
    uint32_t value=(uint32_t)lua_toboolean(L,-1);lua_pop(L,1);return value;
}
static int32_t context_integer(lua_State *L,int index,const char *key) {
    lua_pushstring(L,key);lua_rawget(L,index);
    if(lua_type(L,-1)!=LUA_TNUMBER)luaL_error(L,"death context requires integers");
    lua_Number value=lua_tonumber(L,-1);
    if(!isfinite(value) || value< -2147483648.0 || value>=2147483648.0 || (lua_Number)(int32_t)value!=value)
        luaL_error(L,"invalid death context integer");
    lua_pop(L,1);return (int32_t)value;
}
/* Authored metadata and native Kill projection; actual original Character and
 * death event queue ownership remain pending. */
static int death_context(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=2 || lua_type(L,2)!=LUA_TTABLE)return luaL_error(L,"death context table required");
    struct dh2_death_actor actor;
    actor.dead=context_bool(L,2,"dead");actor.network=context_bool(L,2,"network");actor.suppress_events=context_bool(L,2,"suppress_events");
    actor.property_id=context_integer(L,2,"property_id");actor.template_id=context_integer(L,2,"template_id");
    lua_pushliteral(L,"target_id");lua_rawget(L,2);
    if(lua_type(L,-1)!=LUA_TNUMBER)return luaL_error(L,"death target id required");
    lua_Number target_id=lua_tonumber(L,-1);
    if(!isfinite(target_id) || target_id<0 || target_id>=4294967296.0 || (lua_Number)(uint32_t)target_id!=target_id ||
       actor.property_id< -32768 || actor.property_id>32767 || actor.template_id< -32768 || actor.template_id>32767)
        return luaL_error(L,"invalid death metadata");
    actor.target_id=(uint32_t)target_id;obj->death=actor;return 0;
}
static int is_dead(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);lua_pushboolean(L,obj->death.dead);return 1;
}
static void result_integer(lua_State *L,const char *key,lua_Integer value) {lua_pushinteger(L,value);lua_setfield(L,-2,key);}
static void result_boolean(lua_State *L,const char *key,uint32_t value) {lua_pushboolean(L,value);lua_setfield(L,-2,key);}
static int kill_nonplayer(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=2 || lua_type(L,2)!=LUA_TTABLE)return luaL_error(L,"death policy table required");
    struct dh2_death_policy policy;
    policy.forced=context_bool(L,2,"forced");policy.loot_manager_present=context_bool(L,2,"loot_manager_present");
    const char *keys[]={"kill_enemies","clear_enemies","kill_template","clear_template"};
    for(unsigned i=0;i<4;++i)policy.objective_ids[i]=context_integer(L,2,keys[i]);
    struct dataset *data=data_for(L,1);struct dh2_character_props next=obj->state;
    struct dh2_death_actor actor=obj->death;struct dh2_death_result result;
    if(dh2_death_nonplayer(&data->table,&next,&actor,&policy,&result))return luaL_error(L,"invalid nonplayer death data");
    lua_newtable(L);result_boolean(L,"processed",result.processed);result_boolean(L,"dead",actor.dead);
    result_boolean(L,"drop_loot_requested",result.drop_loot_requested);result_integer(L,"drop_loot_id",result.drop_loot_id);
    lua_newtable(L);
    for(unsigned i=0;i<result.event_count;++i) {
        const struct dh2_death_event *event=&result.events[i];lua_newtable(L);
        result_integer(L,"kind",event->kind);lua_pushnumber(L,(lua_Number)event->target_id);lua_setfield(L,-2,"target_id");
        result_integer(L,"objective_id",event->objective_id);result_integer(L,"match_id",event->match_id);
        lua_rawseti(L,-2,(int)i+1);
    }
    lua_setfield(L,-2,"events");
    /* Commit only after constructing every return table, including allocations. */
    obj->state=next;obj->death=actor;return 1;
}
/* Authored access to the non-player HitFor projection. Required policy values
 * expose unresolved engine boundaries instead of inventing network/AI/death
 * ownership. Results: processed, death requested, damage whole, death reason.
 * Requesting death deliberately does not change an actor's dead flag. */
static int nonplayer_hit(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=4 || lua_type(L,2)!=LUA_TNUMBER || lua_type(L,3)!=LUA_TTABLE || lua_type(L,4)!=LUA_TNUMBER)
        return luaL_error(L,"nonplayer hit requires unsigned raw damage, policy and death reason");
    lua_Number damage=lua_tonumber(L,2),reason=lua_tonumber(L,4);
    if(!isfinite(damage) || damage<0 || damage>=4294967296.0 ||
       !isfinite(reason) || reason< -2147483648.0 || reason>=2147483648.0 || (lua_Number)(int32_t)reason!=reason)
        return luaL_error(L,"invalid damage or death reason");
    struct dh2_hit_policy p;memset(&p,0,sizeof(p));
    const char *keys[]={"target_dead","target_monster","local_player_alive","online","manager_present",
        "monster_invincible","force_kill_config","force_kill_switch","target_network"};
    uint32_t *fields[]={&p.target_dead,&p.target_monster,&p.local_player_alive,&p.online,&p.manager_present,
        &p.monster_invincible,&p.force_kill_config,&p.force_kill_switch,&p.target_network};
    for(unsigned i=0;i<9;++i) {
        lua_pushstring(L,keys[i]);lua_rawget(L,3);
        if(lua_type(L,-1)!=LUA_TBOOLEAN)return luaL_error(L,"hit policy flags must be booleans");
        *fields[i]=(uint32_t)lua_toboolean(L,-1);lua_pop(L,1);
    }
    lua_pushliteral(L,"manager_mode");lua_rawget(L,3);
    if(lua_type(L,-1)!=LUA_TNUMBER)return luaL_error(L,"hit manager mode must be an integer");
    lua_Number mode=lua_tonumber(L,-1);
    if(!isfinite(mode) || mode< -2147483648.0 || mode>=2147483648.0 || (lua_Number)(int32_t)mode!=mode)
        return luaL_error(L,"invalid hit manager mode");
    p.manager_mode=(int32_t)mode;lua_pop(L,1);
    struct dataset *data=data_for(L,1);struct dh2_hit_state hit={(int32_t)reason};struct dh2_hit_result result;
    if(dh2_hit_nonplayer(&data->table,&obj->state,&hit,&p,(uint32_t)damage,&result))return luaL_error(L,"invalid nonplayer hit data");
    lua_pushboolean(L,result.processed);lua_pushboolean(L,result.death_requested);
    lua_pushinteger(L,result.damage_whole);lua_pushinteger(L,hit.death_reason);return 4;
}
static int health_hp(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);int32_t values[3];
    if(dh2_health_script_hp(&obj->state,values))return luaL_error(L,"undefined HP percentage division");
    for(unsigned i=0;i<3;++i)lua_pushinteger(L,values[i]);
    return 3;
}
/* Original callbacks consume the first numeric argument as a raw fixed amount;
 * missing/wrong-tag first arguments produce no result. Offline normal mana
 * policy is used; no network/config/debug exemption is fabricated. */
static int health_callback(lua_State *L,uint32_t op) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);int count=lua_gettop(L)-1;
    if(count>32)return luaL_error(L,"health argument limit exceeded");
    if(!count || lua_type(L,2)!=LUA_TNUMBER)return 0;
    lua_Number number=lua_tonumber(L,2);
    if(!isfinite(number) || number< -2147483648.0 || number>=2147483648.0)
        return luaL_error(L,"invalid raw health or mana amount");
    int32_t value=(int32_t)number,result=0;struct dataset *data=data_for(L,1);uint32_t status;
    if(op<2)status=dh2_health_regen(&data->table,&obj->state,op,value);
    else if(op==2)status=dh2_health_has_mana(&obj->state,value,0,&result);
    else status=dh2_health_use_mana(&data->table,&obj->state,value,0,&result);
    if(status)return luaL_error(L,"invalid health data");
    if(op>=2) {lua_pushboolean(L,result);return 1;}return 0;
}
static int regen_hp(lua_State *L) {return health_callback(L,0);}
static int regen_mp(lua_State *L) {return health_callback(L,1);}
static int has_mana(lua_State *L) {return health_callback(L,2);}
static int use_mana(lua_State *L) {return health_callback(L,3);}
/* Explicit source access to reconstructed native methods. These additional Lua
 * method names are authored controls, not recovered registration claims. */
static int health_set(lua_State *L,uint32_t channel) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=2 || lua_type(L,2)!=LUA_TNUMBER)return luaL_error(L,"whole health value required");
    lua_Number number=lua_tonumber(L,2);
    if(!isfinite(number) || number< -2147483648.0 || number>=2147483648.0)return luaL_error(L,"invalid whole health value");
    struct dataset *data=data_for(L,1);
    if(dh2_health_set(&data->table,&obj->state,channel,(int32_t)number))return luaL_error(L,"invalid health data");
    return 0;
}
static int set_hp(lua_State *L) {return health_set(L,0);}
static int set_mp(lua_State *L) {return health_set(L,1);}
static int health_validate(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=1)return luaL_error(L,"health validation takes no arguments");
    struct dataset *data=data_for(L,1);
    if(dh2_health_validate(&data->table,&obj->state))return luaL_error(L,"invalid health data");
    return 0;
}
static int health_get(lua_State *L,uint32_t channel,uint32_t total) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);int32_t value;
    if(dh2_health_get(&obj->state,channel,total,&value))return luaL_error(L,"invalid health state");
    lua_pushinteger(L,value);return 1;
}
static int get_mp(lua_State *L) {return health_get(L,1,0);}
static int get_total_hp(lua_State *L) {return health_get(L,0,1);}
static int get_total_mp(lua_State *L) {return health_get(L,1,1);}
static int health_fraction(lua_State *L,uint32_t channel) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);float value;
    if(dh2_health_fraction(&obj->state,channel,&value))return luaL_error(L,"invalid health state");
    lua_pushnumber(L,value);return 1;
}
static int hp_fraction(lua_State *L) {return health_fraction(L,0);}
static int mp_fraction(lua_State *L) {return health_fraction(L,1);}
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
static int equip(lua_State *L,uint32_t limit) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);
    if(lua_gettop(L)!=4 || lua_type(L,2)!=LUA_TNUMBER || lua_type(L,3)!=LUA_TNUMBER || lua_type(L,4)!=LUA_TNUMBER)
        return luaL_error(L,"invalid equipment arguments");
    lua_Number set=lua_tonumber(L,2),slot=lua_tonumber(L,3),id=lua_tonumber(L,4);
    if((set!=0 && set!=1) || !isfinite(slot) || slot<0 || slot>=limit || (lua_Number)(uint32_t)slot!=slot)
        return luaL_error(L,"invalid equipment slot");
    lua_getfenv(L,1);lua_rawgeti(L,-1,3);struct loot_dataset *loot=lua_touserdata(L,-1);
    if(!loot)return luaL_error(L,"item data is not loaded for this object");
    if(!isfinite(id) || id< -1 || id>=loot->table.counts[DH2_ITEMS] || (lua_Number)(int32_t)id!=id)
        return luaL_error(L,"invalid item index");
    int32_t ids[6];for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i)ids[3*s+i]=obj->equipment.slots[s][i].item_id;
    if(slot<3)ids[(uint32_t)set*3+(uint32_t)slot]=(int32_t)id;
    struct dh2_equipment next;
    if(dh2_equipment_load(&loot->table,ids,obj->equipment.current_set,(uint32_t)obj->state.final.values[203],&next))
        return luaL_error(L,"invalid equipment data");
    obj->equipment=next;obj->gear_ids[(uint32_t)set][(uint32_t)slot]=(int32_t)id;
    obj->power_counts[(uint32_t)set][(uint32_t)slot]=0;return 0;
}
static int equip_item(lua_State *L) {return equip(L,3);}
static int equip_gear(lua_State *L) {return equip(L,GEAR_SLOTS);}
static int item_powers(lua_State *L) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);int count=lua_gettop(L);
    if(count<3 || count>3+GEAR_POWERS || lua_type(L,2)!=LUA_TNUMBER || lua_type(L,3)!=LUA_TNUMBER)
        return luaL_error(L,"invalid gear power arguments");
    lua_Number set=lua_tonumber(L,2),slot=lua_tonumber(L,3);
    if((set!=0 && set!=1) || !isfinite(slot) || slot<0 || slot>=GEAR_SLOTS || (lua_Number)(uint32_t)slot!=slot)
        return luaL_error(L,"invalid gear power slot");
    lua_getfenv(L,1);lua_rawgeti(L,-1,4);struct power_dataset *powers=lua_touserdata(L,-1);
    if(!powers)return luaL_error(L,"item power data is not loaded for this object");
    int32_t ids[GEAR_POWERS];
    for(int i=0;i<count-3;++i) {
        if(lua_type(L,i+4)!=LUA_TNUMBER)return luaL_error(L,"power index must be a number");
        lua_Number id=lua_tonumber(L,i+4);
        if(!isfinite(id) || id<0 || id>=powers->table.counts[1] || (lua_Number)(uint32_t)id!=id)
            return luaL_error(L,"invalid power index");
        ids[i]=(int32_t)id;
    }
    memcpy(obj->power_ids[(uint32_t)set][(uint32_t)slot],ids,(size_t)(count-3)*sizeof(ids[0]));
    obj->power_counts[(uint32_t)set][(uint32_t)slot]=(uint32_t)(count-3);return 0;
}
static int lifecycle(lua_State *L,uint32_t operation) {
    struct object *obj=luaL_checkudata(L,1,STATE_TYPE);int count=lua_gettop(L);int32_t row=0;uint32_t flag=1;
    if(operation==0) {
        if(count!=2 || lua_type(L,2)!=LUA_TNUMBER)return luaL_error(L,"invalid base row argument");
        lua_Number id=lua_tonumber(L,2);
        if(!isfinite(id) || id< -2147483648.0 || id>=2147483648.0 || (lua_Number)(int32_t)id!=id)
            return luaL_error(L,"invalid base row");
        row=(int32_t)id;
    } else if(operation==1) {
        if(count!=1)return luaL_error(L,"gear update takes no arguments");
    } else {
        if(count>2 || (count==2 && lua_type(L,2)!=LUA_TBOOLEAN))return luaL_error(L,"recalculation requires an optional boolean");
        if(count==2)flag=(uint32_t)lua_toboolean(L,2);
    }
    struct dataset *data=data_for(L,1);lua_getfenv(L,1);lua_rawgeti(L,-1,2);struct class_dataset *classes=lua_touserdata(L,-1);
    if(!classes)return luaL_error(L,"character class data is not loaded for this object");
    struct dh2_character_props next=obj->state;uint32_t status;
    if(operation==0)status=dh2_character_update_base(&data->table,&classes->table,&next,row);
    else if(operation==2)status=dh2_character_recalc(&data->table,&classes->table,&next,flag);
    else {
        lua_getfenv(L,1);lua_rawgeti(L,-1,3);struct loot_dataset *loot=lua_touserdata(L,-1);
        lua_getfenv(L,1);lua_rawgeti(L,-1,4);struct power_dataset *powers=lua_touserdata(L,-1);
        if(!loot || !powers)return luaL_error(L,"item and power data are required for gear updates");
        struct dh2_gear_entry entries[GEAR_SLOTS];
        for(uint32_t i=0;i<GEAR_SLOTS;++i) {
            uint32_t set=(i==1 || i==2)?obj->equipment.current_set:0;
            entries[i]=(struct dh2_gear_entry){obj->gear_ids[set][i],obj->power_counts[set][i],obj->power_ids[set][i]};
        }
        status=dh2_character_update_gears(&data->table,&classes->table,&loot->table,&powers->table,&next,entries,GEAR_SLOTS);
    }
    if(status)return luaL_error(L,"invalid gear/property/class data or application limit exceeded");
    obj->state=next;obj->equipment.owner_hand_rule=(uint32_t)next.final.values[203];return 0;
}
static int update_base(lua_State *L) {return lifecycle(L,0);}
static int update_gears(lua_State *L) {return lifecycle(L,1);}
static int recalculate(lua_State *L) {return lifecycle(L,2);}
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
    memset(obj,0,sizeof(*obj));
    obj->combat_state=-1;
    obj->death.template_id=-1;
    for(unsigned s=0;s<2;++s)for(unsigned i=0;i<3;++i)obj->equipment.slots[s][i].item_id= -1;
    for(unsigned s=0;s<2;++s)for(unsigned i=0;i<GEAR_SLOTS;++i)obj->gear_ids[s][i]=-1;
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
    lua_pushlightuserdata(L,&power_key);lua_rawget(L,LUA_REGISTRYINDEX);lua_rawseti(L,-2,4);
    lua_pushliteral(L,"source character");lua_rawseti(L,-2,5);
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
    lua_pushcfunction(L,equip_gear);lua_setfield(L,-2,"EquipGear");
    lua_pushcfunction(L,item_powers);lua_setfield(L,-2,"SetItemPowers");
    lua_pushcfunction(L,update_base);lua_setfield(L,-2,"UpdateBaseProperties");
    lua_pushcfunction(L,update_gears);lua_setfield(L,-2,"UpdateGearsProperties");
    lua_pushcfunction(L,recalculate);lua_setfield(L,-2,"RecalculateProperties");
    lua_pushcfunction(L,get_state);lua_setfield(L,-2,"GetState");
    lua_pushcfunction(L,get_hit_count);lua_setfield(L,-2,"GetHitCount");
    lua_pushcfunction(L,get_name);lua_setfield(L,-2,"GetName");
    lua_pushcfunction(L,combat_context);lua_setfield(L,-2,"SetCombatContext");
    lua_pushcfunction(L,nonplayer_hit);lua_setfield(L,-2,"ApplyNonplayerHit");
    lua_pushcfunction(L,death_context);lua_setfield(L,-2,"SetDeathContext");
    lua_pushcfunction(L,is_dead);lua_setfield(L,-2,"IsDead");
    lua_pushcfunction(L,kill_nonplayer);lua_setfield(L,-2,"KillNonplayer");
    lua_pushcfunction(L,health_hp);lua_setfield(L,-2,"GetHP");
    lua_pushcfunction(L,regen_hp);lua_setfield(L,-2,"RegenHP");
    lua_pushcfunction(L,regen_mp);lua_setfield(L,-2,"RegenMP");
    lua_pushcfunction(L,has_mana);lua_setfield(L,-2,"HasMana");
    lua_pushcfunction(L,use_mana);lua_setfield(L,-2,"UseMana");
    lua_pushcfunction(L,set_hp);lua_setfield(L,-2,"SetHP");
    lua_pushcfunction(L,set_mp);lua_setfield(L,-2,"SetMP");
    lua_pushcfunction(L,health_validate);lua_setfield(L,-2,"ValidateHPMP");
    lua_pushcfunction(L,get_mp);lua_setfield(L,-2,"GetMP");
    lua_pushcfunction(L,get_total_hp);lua_setfield(L,-2,"GetTotalHP");
    lua_pushcfunction(L,get_total_mp);lua_setfield(L,-2,"GetTotalMP");
    lua_pushcfunction(L,hp_fraction);lua_setfield(L,-2,"GetHPFraction");
    lua_pushcfunction(L,mp_fraction);lua_setfield(L,-2,"GetMPFraction");
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
static int import_powers(lua_State *L) {
    const struct dh2_power_tables *input=lua_touserdata(L,1);
    struct power_dataset *data=lua_newuserdata(L,sizeof(*data)+input->size);
    memcpy(data->bytes,input->bytes,input->size);
    if(dh2_power_open(&data->table,data->bytes,input->size))return luaL_error(L,"invalid power dataset");
    lua_pushlightuserdata(L,&power_key);lua_pushvalue(L,-2);lua_rawset(L,LUA_REGISTRYINDEX);return 0;
}
int dh2_lua_powers_load(lua_State *L,const struct dh2_power_tables *view) {return lua_cpcall(L,import_powers,(void *)view);}
