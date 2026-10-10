#include "live_enemy_binding.hpp"
namespace dh::foundation::enemy_ai {
bool validate_live_enemy_borrow(const LiveEnemyBorrow& b,ActorId id,std::string& e){
    e.clear();
    if(id==invalid_actor_id||!b.actor||b.actor->id!=id||!b.receiver_lease){e="Required retained same live enemy receiver and lifetime lease";return false;}
    if(!b.properties||!b.properties->resolved||!b.target||!b.target->owner||
       b.target->owner->identity!=id||!b.state||!b.controller||!b.path_owner||
       !b.scene_clock||!b.random_channel0){e="Required same source enemy properties/target/FSM/controller/path/clock/RNG owners";return false;}
    if(b.target->target!=b.actor->target_id){e="Required coherent canonical enemy target and ActorState projection";return false;}
    return true;
}
bool dispatch_live_monster_target(const LiveEnemyProviders& p,ActorId id,
    std::uint32_t event,ActorId payload,std::string& e){
    e.clear();MonsterEvent mapped{};
    if(!source_target_event(event,mapped)){e="Character event has no original monster target callback";return false;}
    if(!p.borrow||!p.selected){e="Required retained enemy borrow and actual selected AIS providers";return false;}
    LiveEnemyBorrow b{};
    if(!p.borrow(id,b,e)||!validate_live_enemy_borrow(b,id,e))return false;
    SelectedMonsterCallback callback{};
    if(!p.selected(b,callback,e)){if(e.empty())e="Required actual selected enemy AIS callback";return false;}
    // A source external script can select monster.lua; native monster__ uses
    // AISMonster. Neither actor classification nor AI row text selects here.
    if(callback.character!=id||!callback.script||!callback.original_monster_program||
       (callback.kind!=dh2::character::script_monster&&callback.kind!=dh2::character::script_external)||!callback.invoke){
        e="Required actual selected original monster AIS program and callback";return false;
    }
    if(!callback.invoke(event,payload,e)){if(e.empty())e="Original selected monster callback service failed";return false;}
    // No rollback: original native/VM failure retains its executed prefix.
    if(b.target->target!=b.actor->target_id){e="Selected monster callback failed to publish canonical target to same ActorState";return false;}
    return true;
}
}
