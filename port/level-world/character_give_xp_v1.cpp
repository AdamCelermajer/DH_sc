#include "character_give_xp_v1.hpp"
#include <cstring>

namespace dh2::character_give_xp_v1 {namespace {
std::int32_t signed_word(std::uint32_t bits){std::int32_t value;std::memcpy(&value,&bits,4);return value;}
std::int32_t wrap_add(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)+std::uint32_t(b));}
std::int32_t wrap_sub(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)-std::uint32_t(b));}
std::int32_t wrap_mul(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)*std::uint32_t(b));}
std::int32_t arithmetic_shift8(std::int32_t value){
    const auto bits=std::uint32_t(value);
    return signed_word((bits>>8)|((bits&0x80000000u)?0xff000000u:0u));
}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(a&&b&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool same_view(const data::PropertyView& a,const data::PropertyView& b){
    return a.defaults==b.defaults&&a.types==b.types&&a.base==b.base&&a.saved==b.saved&&
           a.gear==b.gear&&a.resolved==b.resolved&&a.groups==b.groups&&a.group_count==b.group_count;
}
}

PlayerByCharacterStatus player_by_character_internal_id(
    const PlayerByCharacterBinding* binding,std::uintptr_t character,
    PlayerByCharacterResult* output){
    if(!binding||!binding->registry||!binding->player_services||!character||!output)
        return PlayerByCharacterStatus::invalid_argument;
    player_locality_v1::Result selected{};
    const auto status=player_locality_v1::get_player_by_character(
        binding->registry,binding->player_services,character,0,&selected);
    if(status!=player_locality_v1::Status::complete)
        return PlayerByCharacterStatus::lookup_failed;
    if(!selected.player)return PlayerByCharacterStatus::missing_player;
    PlayerByCharacterResult result{};
    result.route=selected.route;
    result.player=selected.player;
    result.internal_id=selected.player->internal_id;
    *output=result;
    return PlayerByCharacterStatus::complete;
}

Runtime::Runtime(Bindings bindings,Backend backend):bindings_(bindings),backend_(backend){
    if(bindings_.properties)captured_properties_=*bindings_.properties;
    if(bindings_.save)captured_save_character_=bindings_.save->character();
}
bool Runtime::coherent()const noexcept{
    return bindings_.properties&&bindings_.save&&same_view(*bindings_.properties,captured_properties_)&&
        bindings_.save->character()==captured_save_character_&&
        captured_save_character_==bindings_.character;
}
Status Runtime::invoke(Operation operation,const char* name,std::int32_t argument,
                       Reply& reply,Result& result,std::string& error){
    result.last_operation=operation;reply={};++result.calls;
    const Request request{operation,bindings_.character,bindings_.properties,bindings_.save,argument,name};
    try{
        if(operation==Operation::player_by_character&&backend_.player_lookup){
            PlayerByCharacterResult selected{};
            const auto selected_status=player_by_character_internal_id(
                backend_.player_lookup,bindings_.character,&selected);
            if(selected_status!=PlayerByCharacterStatus::complete){
                error="Source PlayerManager::GetPlayerByCharacter tail lookup failed";
                return Status::service_failed;
            }
            reply.value=selected.internal_id;
        }else{
        const auto provider=operation==Operation::level_up?backend_.level_up:backend_.invoke;
        if(!provider){
            if(operation==Operation::level_up){
                error="Character::LevelUp at 0x3bf724 requires a full save/UI/trophy/script provider";
                return Status::level_up_required;
            }
            error="Reached Character::_GiveXP service is unavailable";
            return Status::service_unavailable;
        }
        if(provider(backend_.context,&request,&reply,error)){
            if(error.empty())error=operation==Operation::level_up?
                "Character::LevelUp provider failed after the XP property prefix":
                "Character::_GiveXP service failed";
            return Status::service_failed;
        }
        }
    }catch(const std::exception& e){error=e.what();return Status::service_failed;}
    catch(...){error="Character::_GiveXP service threw";return Status::service_failed;}
    if(!coherent()){error="Character/Save/PropertyView owner changed during _GiveXP";return Status::projection_changed;}
    return Status::complete;
}

Status Runtime::give_xp(Result* out,std::string& error){
    if(busy_)return Status::busy;
    if(!out||!aligned(out)||overlap(out,sizeof(*out),this,sizeof(*this))||
       overlap(out,sizeof(*out),&error,sizeof(error))||
       !bindings_.character||!bindings_.properties||!bindings_.save||!bindings_.constants||
       !aligned(bindings_.properties)||!aligned(bindings_.save)||!aligned(bindings_.constants)||
       !coherent()||
       dh2_property_validate(bindings_.properties)||!bindings_.constants->bytes||
       bindings_.constants->size>16u*1024u*1024u){return Status::invalid_argument;}
    const auto separate=[&](const void* p,std::size_t n){return !overlap(out,sizeof(*out),p,n)&&!overlap(&error,sizeof(error),p,n);};
    if(!separate(bindings_.properties,sizeof(*bindings_.properties))||
       !separate(bindings_.save,sizeof(*bindings_.save))||
       !separate(bindings_.constants,sizeof(*bindings_.constants))||
       !separate(bindings_.constants->bytes,bindings_.constants->size))return Status::invalid_argument;
    const auto& v=*bindings_.properties;
    for(const auto* sheet:{v.defaults,v.types,v.base,static_cast<const std::int32_t*>(v.saved),
                           v.gear,static_cast<const std::int32_t*>(v.resolved)})
        if(!separate(sheet,224*sizeof(std::int32_t)))return Status::invalid_argument;
    if(v.group_count&&!separate(v.groups,v.group_count*sizeof(*v.groups)))return Status::invalid_argument;
    for(std::uint32_t i=0;i<v.group_count;++i){
        if(v.groups[i].count&&!separate(v.groups[i].sheets,v.groups[i].count*sizeof(*v.groups[i].sheets)))return Status::invalid_argument;
        for(std::uint32_t j=0;j<v.groups[i].count;++j)
            if(!separate(v.groups[i].sheets[j],224*sizeof(std::int32_t)))return Status::invalid_argument;
    }
    busy_=true;struct Guard{bool& value;~Guard(){value=false;}} guard{busy_};
    *out={};error.clear();auto& properties=*bindings_.properties;
    const auto get_max_level=[&](const char* key)->bool{
        dh2_pycst_result constant{};
        const auto failed=backend_.get_constant?
            backend_.get_constant(backend_.context,bindings_.constants,
                                  "CharacterDesign",key,&constant,error):
            dh2_pycst_get(bindings_.constants,"CharacterDesign",15,key,
                          std::uint32_t(std::strlen(key)),&constant);
        if(failed){
            out->status=Status::property_failed;
            if(error.empty())error="Character::_GiveXP max-level constant view is invalid";
            return false;
        }
        // PyDataConstants::getConstant returns 0 when either the section or
        // key is absent. A missing parsed-view entry is therefore source value
        // zero, not a failed property lookup.
        out->max_level=constant.found?constant.value:0;
        return true;
    };
    const auto read_unlocked_difficulty=[&](std::int32_t& difficulty)->bool{
        if(backend_.get_unlocked_difficulty){
            if(backend_.get_unlocked_difficulty(backend_.context,bindings_.character,
                                                bindings_.save,&difficulty,error)){
                out->status=Status::service_failed;
                if(error.empty())error="Character::SG_GetGameDifficultyUnlocked service failed";
                return false;
            }
        }else difficulty=bindings_.save->unlocked_difficulty();
        return true;
    };
    // Pinned IDA order: load the normal design value first, then make a fresh
    // SG_GetGameDifficultyUnlocked query. The else-if performs its own second
    // query only when the first result is not Hard.
    if(!get_max_level("MaxLevelBNormal"))return out->status;
    std::int32_t max_level_difficulty=0;
    if(!read_unlocked_difficulty(max_level_difficulty))return out->status;
    if(max_level_difficulty==1){
        if(!get_max_level("MaxLevelCHard"))return out->status;
    }else{
        if(!read_unlocked_difficulty(max_level_difficulty))return out->status;
        if(max_level_difficulty==2&&!get_max_level("MaxLevelDVeryHard"))return out->status;
    }
    out->level=arithmetic_shift8(properties.resolved[19]);
    // The original short-circuits the virtual and Level queries at max level.
    if(out->max_level<=out->level){out->status=Status::complete;return out->status;}

    Reply reply{};
    auto status=invoke(Operation::character_virtual_40,nullptr,40,reply,*out,error);
    if(status!=Status::complete){out->status=status;return status;}
    if(!reply.value){out->status=Status::complete;return out->status;}
    status=invoke(Operation::character_virtual_84,nullptr,84,reply,*out,error);
    if(status!=Status::complete){out->status=status;return status;}
    if(reply.value){out->status=Status::complete;return out->status;}
    status=invoke(Operation::current_level_suppression,nullptr,0,reply,*out,error);
    if(status!=Status::complete){out->status=status;return status;}
    if(reply.word){out->status=Status::complete;return out->status;}

    status=invoke(Operation::one_kill_level_up,"OneKillLevelUp",0,reply,*out,error);
    if(status!=Status::complete){out->status=status;return status;}
    auto raw_amount=bindings_.amount_fixed;
    if(reply.word)raw_amount=wrap_sub(properties.resolved[34],properties.resolved[33]);
    // `_GiveXP` calls Character::SG_GetGameDifficultyUnlocked again here,
    // after OneKillLevelUp, rather than reusing the Save snapshot read for the
    // max-level branch. This is an observable source reread and can differ if
    // the debug callback changed the active Save/profile projection.
    std::int32_t difficulty=0;
    if(!read_unlocked_difficulty(difficulty))return out->status;
    status=invoke(Operation::current_level_difficulty,nullptr,0,reply,*out,error);
    if(status!=Status::complete){out->status=status;return status;}
    const auto current_level_difficulty=reply.value;
    if(difficulty<current_level_difficulty)raw_amount=256;
    out->raw_amount_fixed=raw_amount;

    // PROPS_GetModifiedXP reads Special_Bonus_To_XP (property 200) directly
    // and applies signed 32-bit arithmetic before the arithmetic >>8.
    const auto factor=wrap_add(properties.resolved[200],25600)/100;
    const auto modified=arithmetic_shift8(wrap_mul(factor,raw_amount));
    out->modified_amount_fixed=modified;
    // Property 33 is a saved integer in the original CharacterProperties
    // table. Refuse an accidental view whose type makes AddInt a silent no-op.
    const auto xp_type=std::uint32_t(properties.types[33]);
    if(!(xp_type&8u)&&!(xp_type&32u)){
        out->status=Status::property_failed;error="Canonical XP property 33 is not writable by source AddInt";return out->status;
    }
    if(dh2_property_add(&properties,33,modified)){
        out->status=Status::property_failed;error="Canonical XP property 33 rejected source add";return out->status;
    }
    out->xp_added=1;
    // The source loads DebugSwitches and reads this switch after Add, even
    // though the resulting tracing flag does not change the XP calculation.
    status=invoke(Operation::trace_character_stats,"isTracingChar_Stats",0,reply,*out,error);
    if(status!=Status::complete){out->status=status;return status;}
    out->xp_after=properties.resolved[33];out->max_xp=properties.resolved[34];
    if(out->xp_after>=out->max_xp){
        out->level_up_overage=arithmetic_shift8(wrap_sub(out->xp_after,out->max_xp));
        out->level_up_called=1;
        status=invoke(Operation::level_up,nullptr,out->level_up_overage,reply,*out,error);
        if(status!=Status::complete){out->status=status;return status;}
        // Character::_GiveXP re-reads both cached properties after LevelUp and
        // clamps excess XP with the same canonical PROPS_Set path.
        out->xp_after=properties.resolved[33];
        out->max_xp=properties.resolved[34];
        if(out->xp_after>out->max_xp){
            if(dh2_property_set(&properties,33,out->max_xp)){
                out->status=Status::property_failed;error="Canonical XP property 33 rejected source post-LevelUp clamp";return out->status;
            }
            out->xp_after=properties.resolved[33];out->xp_clamped=1;
        }
    }
    if(bindings_.update_player_stat){
        status=invoke(Operation::player_by_character,nullptr,0,reply,*out,error);
        if(status!=Status::complete){out->status=status;return status;}
        out->player_internal_id=reply.value;++out->stat_player_lookups;
        // PlayerStatManager::IncreaseStat(6, raw_amount>>8, internal_id) is bx lr
        // in the pinned game binary; only its reached PlayerManager lookup has work.
    }
    out->source_return=1;out->status=Status::complete;return out->status;
}
}
