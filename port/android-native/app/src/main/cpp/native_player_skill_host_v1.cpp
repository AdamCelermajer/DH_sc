#include "native_player_skill_host_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <exception>
#include <limits>

namespace dh2::native::player_skill_host_v1 {
namespace {
using Function=dh2::character_native_bindings::Function;
using Fn=Function;

int fail(char* text,std::size_t bytes,const char* name,const char* why) noexcept {
    if(text&&bytes)
        std::snprintf(text,bytes,"Player skill host %s: %s",name?name:"?",why?why:"failed");
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
int fail_detail(char* text,std::size_t bytes,const char* name,const char* why,
                const std::string& detail) noexcept {
    if(text&&bytes)
        std::snprintf(text,bytes,"Player skill host %s: %s%s%s",name?name:"?",
                      why?why:"failed",detail.empty()?"":" | ",detail.c_str());
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
void put_number(dh2_script_value& v,float n) noexcept {
    v={};v.type=DH2_SCRIPT_NUMBER;v.number=n;
}
void put_boolean(dh2_script_value& v,bool b) noexcept {
    v={};v.type=DH2_SCRIPT_BOOLEAN;v.boolean=b?1u:0u;
}
void put_identity(dh2_script_value& v,std::uintptr_t identity) noexcept {
    v={};v.type=DH2_SCRIPT_IDENTITY;v.identity=identity;
}
// sfc::script::lua::Value::getUInteger is `(unsigned)getNumber()`, ARM
// __aeabi_f2uiz: saturating, NaN and negative values become zero.
std::uint32_t f2uiz(float value) noexcept {
    if(!(value>0.f))return 0;
    if(value>=4294967296.f)return UINT32_MAX;
    return static_cast<std::uint32_t>(value);
}
// __aeabi_f2iz: truncation toward zero, saturating, NaN is zero.
std::int32_t f2iz(float value) noexcept {
    if(value!=value)return 0;
    if(value>=2147483648.f)return INT32_MAX;
    if(value<=-2147483648.f)return INT32_MIN;
    return static_cast<std::int32_t>(value);
}
bool is_number(const dh2_script_value& v) noexcept {return v.type==DH2_SCRIPT_NUMBER;}
// getUserData/getPointer accept the Pointer (2) and UserData (7) tags alike.
bool is_object_like(const dh2_script_value& v) noexcept {return v.type==DH2_SCRIPT_IDENTITY||v.type==7;}

struct Context {
    const Services& s;
    Function function;
    const char* name;
    std::uintptr_t receiver;
    const dh2_script_value* a;
    std::uint32_t count;
    dh2_script_value* out;
    std::uint32_t capacity;
    std::uint32_t* returned;
    char* text;
    std::size_t bytes;

    int failure(const char* why) const noexcept {return fail(text,bytes,name,why);}
    int need_out(std::uint32_t n) const noexcept {
        if(!out||capacity<n)return failure("missing result buffer");
        return 0;
    }
    int facts(ObjectFacts& facts_out) const noexcept {
        if(!s.object_facts)return failure("GameObject facts provider is unavailable");
        try {
            if(s.object_facts(s.context,receiver,&facts_out)!=0)
                return failure("receiver is not a live GameObject known to the renderer");
        }catch(...){return failure("GameObject facts provider threw");}
        return 0;
    }
};

int do_play_fx(Context& c) {
    // Character/GameObject::_PlayFX: no arguments or a non-number id is a
    // silent no-op.
    if(!c.count||!is_number(c.a[0]))return 0;
    if(!c.s.play_fx)return c.failure("VisualFXManager provider is unavailable");
    const auto id=f2uiz(c.a[0].number);
    // Arrays::AnimatedEffectTable::size is checked by the FX owner; an id that
    // does not fit the signed row index can never name a row.
    const std::int32_t effect=id>static_cast<std::uint32_t>(INT32_MAX)?-1:static_cast<std::int32_t>(id);
    std::string error;
    int status=0;
    try {
        if(c.count<=3) {
            // PlayAnimFXSet(id, this, 0): attached to the receiver at Vec3f_Origin.
            status=c.s.play_fx(c.s.context,effect,c.receiver,nullptr,error);
        } else {
            float position[3]{};
            if(is_number(c.a[1])&&is_number(c.a[2])&&is_number(c.a[3])) {
                position[0]=c.a[1].number;position[1]=c.a[2].number;position[2]=c.a[3].number;
            } else {
                ObjectFacts facts{};
                if(auto rc=c.facts(facts))return rc;
                std::memcpy(position,facts.position,sizeof(position));
            }
            status=c.s.play_fx(c.s.context,effect,0,position,error);
        }
    }catch(...){return c.failure("VisualFXManager provider threw");}
    return status?fail_detail(c.text,c.bytes,c.name,"PlayAnimFXSet failed",error):0;
}

int do_play_sound_3d(Context& c) {
    if(!c.count)return c.failure("requires a sound name");
    if(!c.s.play_sound_3d)return c.failure("VoxSoundManager provider is unavailable");
    const char* label="";
    std::size_t length=0;
    if(c.a[0].type==DH2_SCRIPT_STRING&&c.a[0].text){
        label=c.a[0].text;length=c.a[0].text_bytes;
    }
    float position[3]{};
    if(c.count>3&&is_number(c.a[1])&&is_number(c.a[2])&&is_number(c.a[3])) {
        position[0]=c.a[1].number;position[1]=c.a[2].number;position[2]=c.a[3].number;
    } else {
        ObjectFacts facts{};
        if(auto rc=c.facts(facts))return rc;
        std::memcpy(position,facts.position,sizeof(position));
    }
    std::string error;
    try {
        if(c.s.play_sound_3d(c.s.context,label,length,position,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"Play3D failed",error);
    }catch(...){return c.failure("VoxSoundManager provider threw");}
    return 0;
}

int do_rand(Context& c) {
    if(!c.s.random||!c.s.online||!c.s.object_seed)
        return c.failure("process Random stream is unavailable");
    if(c.count>16)return c.failure("too many arguments");
    float numbers[16]{};
    std::uint32_t tags[16]{};
    for(std::uint32_t i=0;i<c.count;++i) {
        tags[i]=c.a[i].type;
        numbers[i]=c.a[i].type==DH2_SCRIPT_NUMBER?c.a[i].number:0.f;
    }
    dh2_random_result result{};
    // Original _Rand: 1 number -> [0,n); 2 numbers -> [a,b); other arity -> [0,100);
    // wrong tags push nothing. Online draws use the object's own seed.
    const auto status=dh2_random_callback(c.s.random,c.s.object_seed,*c.s.online?1u:0u,
                                          numbers,tags,c.count,&result);
    if(status)return c.failure(status==2?"Rand bound is not representable":"invalid Rand arguments");
    if(!result.count)return 0;
    if(auto rc=c.need_out(1))return rc;
    put_number(c.out[0],static_cast<float>(result.value));
    *c.returned=1;return 0;
}

int do_position(Context& c) {
    ObjectFacts facts{};
    if(auto rc=c.facts(facts))return rc;
    if(auto rc=c.need_out(3))return rc;
    for(unsigned i=0;i<3;++i)put_number(c.out[i],facts.position[i]);
    *c.returned=3;return 0;
}

int do_name(Context& c) {
    // pushString(this+0x44). The storage has to outlive this call: the binder
    // copies the text into the Lua state right after the callback returns.
    static thread_local char storage[sizeof(ObjectFacts::name)];
    ObjectFacts facts{};
    if(auto rc=c.facts(facts))return rc;
    if(auto rc=c.need_out(1))return rc;
    std::memcpy(storage,facts.name,sizeof(storage));storage[sizeof(storage)-1]=0;
    c.out[0]={};c.out[0].type=DH2_SCRIPT_STRING;c.out[0].text=storage;
    c.out[0].text_bytes=std::strlen(storage);
    *c.returned=1;return 0;
}

int do_prop_hp(Context& c) {
    ObjectFacts facts{};
    if(auto rc=c.facts(facts))return rc;
    if(auto rc=c.need_out(3))return rc;
    // Character::_GetPropHP: three pushInteger values, current, maximum and
    // (100 * rawCurrent / (rawMax >> 8)) >> 8; an unset maximum pushes zeros.
    std::int32_t current=0,maximum=0,percent=0;
    if(facts.max_hp_raw) {
        maximum=facts.max_hp_raw>>8;
        current=facts.hp_raw>>8;
        if(maximum) {
            const std::int64_t scaled=std::int64_t(100)*facts.hp_raw/maximum;
            percent=static_cast<std::int32_t>(scaled)>>8;
        }
    }
    put_number(c.out[0],static_cast<float>(current));
    put_number(c.out[1],static_cast<float>(maximum));
    put_number(c.out[2],static_cast<float>(percent));
    *c.returned=3;return 0;
}

int do_query(Context& c) {
    ObjectFacts facts{};
    if(auto rc=c.facts(facts))return rc;
    if(auto rc=c.need_out(1))return rc;
    bool value=false;
    switch(c.function) {
    case Fn::game_object_is_player:value=facts.is_player;break;
    case Fn::game_object_is_character:value=facts.is_character;break;
    case Fn::game_object_is_dead:value=facts.is_dead;break;
    case Fn::game_object_is_door:value=facts.is_door;break;
    case Fn::game_object_is_decor:value=facts.is_decor;break;
    default:return c.failure("unexpected GameObject query");
    }
    put_boolean(c.out[0],value);*c.returned=1;return 0;
}

int do_state(Context& c) {
    ObjectFacts facts{};
    if(auto rc=c.facts(facts))return rc;
    if(!facts.is_character||facts.state<0)return c.failure("receiver has no CharStateMachine");
    if(auto rc=c.need_out(1))return rc;
    put_number(c.out[0],static_cast<float>(facts.state));*c.returned=1;return 0;
}

int do_buff(Context& c) {
    if(c.receiver==c.s.self)
        return c.failure("the Player's buffs belong to its own BuffOwner");
    BuffCommand command=BuffCommand::create;
    if(c.function==Fn::character_remove_buff)command=BuffCommand::remove;
    else if(c.function==Fn::character_apply_prop_class) {
        // ApplyBuff shares ApplyPropClass's native; the Pointer buff argument
        // selects the buff form. A property-class application on another
        // Character's sheet is not attached.
        if(c.count<2||c.a[1].type!=DH2_SCRIPT_IDENTITY)
            return c.failure("ApplyPropClass on a non-Player Character sheet is not attached");
        command=BuffCommand::apply;
    }
    if(!c.s.buff_command)return c.failure("receiver BuffOwner provider is unavailable");
    const int status=c.s.buff_command(c.s.context,command,c.receiver,c.a,c.count,c.out,c.capacity,
                                      c.returned,c.text,c.bytes);
    if(status&&c.text&&!c.text[0])return c.failure("receiver BuffOwner rejected the request");
    return status;
}

int do_get_prop(Context& c) {
    if(c.receiver==c.s.self)return c.failure("the Player's properties belong to its property owner");
    if(c.count<1||!is_number(c.a[0])||!(c.a[0].number>=0.f)||c.a[0].number>=224.f)
        return c.failure("GetProp requires a property id");
    // Only the Character's own resolved sheet exists for another receiver; the
    // shared temporary sheet and external sheet Pointers belong to the Player.
    if(c.count>1&&!(c.a[1].type==DH2_SCRIPT_BOOLEAN&&!c.a[1].boolean)&&c.a[1].type!=DH2_SCRIPT_NIL)
        return c.failure("GetProp with a sheet selector is not attached for this receiver");
    if(!c.s.get_property)return c.failure("receiver property provider is unavailable");
    std::int32_t value=0;std::string error;
    try {
        if(c.s.get_property(c.s.context,c.receiver,static_cast<std::uint32_t>(c.a[0].number),&value,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"GetProp failed",error);
    }catch(...){return c.failure("receiver property provider threw");}
    if(auto rc=c.need_out(1))return rc;
    put_number(c.out[0],static_cast<float>(value));*c.returned=1;return 0;
}

int do_ai_flags(Context& c) {
    ObjectFacts facts{};
    if(auto rc=c.facts(facts))return rc;
    if(!facts.is_character||!facts.has_ai_flags)return c.failure("receiver has no CharAI row");
    if(auto rc=c.need_out(1))return rc;
    // pushInteger(AITable[GetCharAIId()].Flags)
    put_number(c.out[0],static_cast<float>(static_cast<std::int32_t>(facts.ai_flags)));
    *c.returned=1;return 0;
}

int do_melee_range(Context& c) {
    if(!c.s.target_in_melee_range)return c.failure("CharAI::AI_IsInMeleeRange provider is unavailable");
    bool value=false;std::string error;
    try {
        if(c.s.target_in_melee_range(c.s.context,c.receiver,&value,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"AI_IsInMeleeRange failed",error);
    }catch(...){return c.failure("CharAI::AI_IsInMeleeRange provider threw");}
    if(auto rc=c.need_out(1))return rc;
    put_boolean(c.out[0],value);*c.returned=1;return 0;
}

int do_weapon(Context& c) {
    if(c.receiver!=c.s.self)
        return c.failure("only the Player's canonical ItemInventory is attached");
    if(!c.s.weapon_flags)return c.failure("Player ItemInventory provider is unavailable");
    std::uint32_t flags=0;std::string error;
    try {
        if(c.s.weapon_flags(c.s.context,&flags,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"weapon query failed",error);
    }catch(...){return c.failure("Player ItemInventory provider threw");}
    if(auto rc=c.need_out(1))return rc;
    // EquipmentWeaponFlagV1: weapon_bow=2 is ItemInventory::HasBow, weapon_ranged=128
    // is HasRangedWeapon; CanMeleeAttack = !CanRangeAttack = !HasRangedWeapon.
    const bool value=c.function==Fn::character_has_bow?(flags&2u)!=0:(flags&128u)==0;
    put_boolean(c.out[0],value);*c.returned=1;return 0;
}

int do_regen_hp(Context& c) {
    if(!c.count||!is_number(c.a[0]))return 0;
    if(c.receiver!=c.s.self)return c.failure("only the Player's HP owner is attached");
    if(!c.s.regen_hp)return c.failure("Character::RegenHP provider is unavailable");
    std::string error;
    try {
        if(c.s.regen_hp(c.s.context,f2iz(c.a[0].number),error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"RegenHP failed",error);
    }catch(...){return c.failure("Character::RegenHP provider threw");}
    return 0;
}

int do_set_targetable(Context& c) {
    // Only a boolean argument writes Character+0x415; anything else is ignored.
    if(!c.count||c.a[0].type!=DH2_SCRIPT_BOOLEAN)return 0;
    if(!c.s.set_targetable)return c.failure("Character+0x415 provider is unavailable");
    std::string error;
    try {
        if(c.s.set_targetable(c.s.context,c.receiver,c.a[0].boolean!=0,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"SetIsTargetable failed",error);
    }catch(...){return c.failure("Character+0x415 provider threw");}
    return 0;
}

int do_set_target(Context& c) {
    if(!c.count||!is_object_like(c.a[0]))return 0;
    if(!c.s.set_target)return c.failure("CharAI::AI_SetTarget provider is unavailable");
    std::string error;
    try {
        if(c.s.set_target(c.s.context,c.receiver,c.a[0].identity,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"AI_SetTarget failed",error);
    }catch(...){return c.failure("CharAI::AI_SetTarget provider threw");}
    return 0;
}

int do_command(Context& c,ObjectCommand command) {
    if(!c.s.object_command)return c.failure("GameObject command provider is unavailable");
    std::uintptr_t other=0;
    std::uint32_t duration=0;
    bool flag=true;
    switch(command) {
    case ObjectCommand::lock:case ObjectCommand::unlock:break;
    case ObjectCommand::kill:
        // _Kill forwards the killer only for a type-7 UserData argument.
        if(c.count&&c.a[0].type==7)other=c.a[0].identity;
        break;
    case ObjectCommand::stun:case ObjectCommand::scare:
        // _SetStunState/_SetScareState need a numeric duration; the optional
        // second argument is honoured only when it is a boolean (default true).
        if(!c.count||!is_number(c.a[0]))return 0;
        duration=f2uiz(c.a[0].number);
        if(c.count>1&&c.a[1].type==DH2_SCRIPT_BOOLEAN)flag=c.a[1].boolean!=0;
        break;
    case ObjectCommand::knock_back:
        // _SetKnockBackState: great knockback unless the first argument is false.
        if(c.count&&c.a[0].type==DH2_SCRIPT_BOOLEAN&&!c.a[0].boolean)flag=false;
        break;
    }
    std::string error;
    try {
        if(c.s.object_command(c.s.context,command,c.receiver,other,duration,flag,error)!=0)
            return fail_detail(c.text,c.bytes,c.name,"GameObject command failed",error);
    }catch(...){return c.failure("GameObject command provider threw");}
    return 0;
}
} // namespace

bool handles(Function function) noexcept {
    switch(function) {
    case Fn::game_object_lock:case Fn::game_object_unlock:
    case Fn::game_object_get_id:case Fn::game_object_get_name:
    case Fn::game_object_get_self:
    case Fn::game_object_is_dead:case Fn::game_object_is_character:
    case Fn::game_object_is_player:case Fn::game_object_is_door:
    case Fn::game_object_is_decor:
    case Fn::game_object_get_position:
    case Fn::game_object_play_fx:case Fn::game_object_play_sound3_d:
    case Fn::game_object_rand:
    case Fn::character_kill:
    case Fn::character_set_target:case Fn::character_set_is_targetable:
    case Fn::character_get_state:
    case Fn::character_set_scare_state:case Fn::character_set_stun_state:
    case Fn::character_set_knock_back_state:
    case Fn::character_get_prop_hp:
    case Fn::character_regen_hp:
    case Fn::character_has_bow:case Fn::character_can_attack_in_melee:
    case Fn::character_get_char_ai_flags:case Fn::character_target_in_melee_range:
    case Fn::character_create_buff:case Fn::character_remove_buff:
    case Fn::character_apply_prop_class:case Fn::character_get_prop:
        return true;
    default:
        return false;
    }
}

int invoke(const Services& services,Function function,const char* name,
           std::uintptr_t receiver,const dh2_script_value* arguments,
           std::uint32_t count,dh2_script_value* results,std::uint32_t capacity,
           std::uint32_t* returned,char* text,std::size_t bytes) noexcept {
    if(!returned||(count&&!arguments))
        return fail(text,bytes,name,"invalid native controls");
    *returned=0;
    if(!receiver||!services.self)return fail(text,bytes,name,"missing receiver GameObject");
    Context c{services,function,name,receiver,arguments,count,results,capacity,returned,text,bytes};
    try {
        switch(function) {
        case Fn::game_object_play_fx:return do_play_fx(c);
        case Fn::game_object_play_sound3_d:return do_play_sound_3d(c);
        case Fn::game_object_rand:return do_rand(c);
        case Fn::game_object_get_self:
        case Fn::game_object_get_id:
            // Pointer / UserData of the receiver. The Lua layer wraps GetSelf
            // into a method-bearing object table.
            if(auto rc=c.need_out(1))return rc;
            put_identity(results[0],receiver);*returned=1;return 0;
        case Fn::game_object_get_name:return do_name(c);
        case Fn::game_object_get_position:return do_position(c);
        case Fn::character_get_prop_hp:return do_prop_hp(c);
        case Fn::game_object_is_dead:case Fn::game_object_is_character:
        case Fn::game_object_is_player:case Fn::game_object_is_door:
        case Fn::game_object_is_decor:return do_query(c);
        case Fn::character_get_state:return do_state(c);
        case Fn::character_create_buff:case Fn::character_remove_buff:
        case Fn::character_apply_prop_class:return do_buff(c);
        case Fn::character_get_prop:return do_get_prop(c);
        case Fn::character_get_char_ai_flags:return do_ai_flags(c);
        case Fn::character_target_in_melee_range:return do_melee_range(c);
        case Fn::character_has_bow:case Fn::character_can_attack_in_melee:return do_weapon(c);
        case Fn::character_regen_hp:return do_regen_hp(c);
        case Fn::character_set_is_targetable:return do_set_targetable(c);
        case Fn::character_set_target:return do_set_target(c);
        case Fn::game_object_lock:return do_command(c,ObjectCommand::lock);
        case Fn::game_object_unlock:return do_command(c,ObjectCommand::unlock);
        case Fn::character_kill:return do_command(c,ObjectCommand::kill);
        case Fn::character_set_stun_state:return do_command(c,ObjectCommand::stun);
        case Fn::character_set_scare_state:return do_command(c,ObjectCommand::scare);
        case Fn::character_set_knock_back_state:return do_command(c,ObjectCommand::knock_back);
        default:break;
        }
    }catch(const std::exception& e){
        return fail_detail(text,bytes,name,"provider exception",e.what());
    }catch(...){
        return fail(text,bytes,name,"provider exception");
    }
    return fail(text,bytes,name,"not owned by the Player skill host");
}

} // namespace dh2::native::player_skill_host_v1
