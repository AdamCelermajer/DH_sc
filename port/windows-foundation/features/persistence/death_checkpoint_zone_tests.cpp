#include "death_checkpoint_zone_v1.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::persistence::death_flow;
namespace dh::foundation::persistence::death_flow {
bool capture_death_checkpoint_snapshot_v1(const GameSave& game,const std::string& level,
    const std::string& name,const std::array<float,3>& key,const std::array<float,3>& revive,
    const DeathCheckpointCaptureServicesV1&,DeathCheckpointSnapshotV1& out,std::string& error){
    if(game.level_uri!=level){error="stub level mismatch";return false;}
    out={};out.level_uri=level;out.checkpoint_name=name;out.checkpoint_key_position=key;out.revive_position=revive;
    error.clear();return true;
}
}
namespace {
void check(bool v,const char* m){if(!v)throw std::runtime_error(m);}
struct Trace {
    std::vector<std::string> calls;
    std::string fail;
    std::array<float,3> key{},revive{};
};
bool record_positions(void* p,const std::string&,const std::array<float,3>& key,
                      const std::array<float,3>& revive,std::string& e){
    auto& t=*static_cast<Trace*>(p);t.calls.push_back("record");t.key=key;t.revive=revive;
    if(t.fail=="record"){e="record failed";return false;}return true;
}
bool save_qest(void* p,std::string& e){auto& t=*static_cast<Trace*>(p);t.calls.push_back("qest");if(t.fail=="qest"){e="qest failed";return false;}return true;}
bool capture_game(void* p,GameSave& game,std::string& e){auto& t=*static_cast<Trace*>(p);t.calls.push_back("capture");game.level_uri="data/scene/003_darkwood.mlx";if(t.fail=="capture"){e="capture failed";return false;}return true;}
bool actor_facts(void*,ActorId,SourceCheckpointObjectFamilyV1& family,bool& skip,std::string&){family=SourceCheckpointObjectFamilyV1::character;skip=false;return true;}
bool actor_state(void*,ActorId,std::uint32_t& state,bool& dead,std::string&){state=3;dead=false;return true;}
bool object_facts(void*,ObjectId,SourceCheckpointObjectFamilyV1& family,bool& skip,std::string&){family=SourceCheckpointObjectFamilyV1::container;skip=false;return true;}
bool persist_level(void* p,const GameSave&,const DeathCheckpointSnapshotV1& s,std::string& e){
    auto& t=*static_cast<Trace*>(p);t.calls.push_back("level-save");
    check(s.checkpoint_key_position==t.key&&s.revive_position==t.revive,"snapshot did not preserve exact zone/revive positions");
    if(t.fail=="level-save"){e="level save failed";return false;}return true;
}
DeathCheckpointZoneServicesV1 services(Trace& t){
    DeathCheckpointZoneServicesV1 s;s.context=&t;s.record_checkpoint_positions=record_positions;
    s.save_player_qest_checkpoint=save_qest;s.capture_current_game_save=capture_game;
    s.capture_services={&t,actor_facts,actor_state,object_facts};s.save_level_checkpoint=persist_level;return s;
}
DeathCheckpointZoneContactV1 contact(){
    DeathCheckpointZoneContactV1 c;c.source_level_state=38;c.contact_object_zonable=true;
    c.contact_character_is_player=true;c.contact_is_local_player_zero=true;c.local_character_present=true;
    c.level_savegame_present=true;c.level_uri="data/scene/003_darkwood.mlx";c.zone_name="Checkpoint_A";
    c.zone_position={12,34,56};c.current_checkpoint_key={1,2,3};c.current_character_position={7,8,9};return c;
}
void test_source_character_restore_state_table(){
    std::string error;SourceCharacterRestorePlanV1 p;
    check(!source_character_restore_plan_v1(0,false,false,false,std::nullopt,false,p,error),
          "Limbus-state CanRespawn group permission remains explicit when source gates permit the call");
    check(source_character_restore_plan_v1(0,false,false,false,false,false,p,error)&&
          p.requires_can_respawn&&p.resulting_dead&&
          p.ordered_operations.front()==SourceCharacterRestoreOpV1::restore_serialized_dead_bit&&
          p.ordered_operations.back()==SourceCharacterRestoreOpV1::clear_target&&
          std::find(p.ordered_operations.begin(),p.ordered_operations.end(),SourceCharacterRestoreOpV1::set_limbus_if_not_already)==p.ordered_operations.end(),
          "Limbus source branch records dead bit first and does not force Limbus when CanRespawn is false");
    check(source_character_restore_plan_v1(12,false,false,false,true,false,p,error)&&p.resulting_dead&&
          p.ordered_operations[1]==SourceCharacterRestoreOpV1::can_respawn_gate&&
          p.ordered_operations[2]==SourceCharacterRestoreOpV1::set_limbus_if_not_already,
          "Limbus state branch attempts CanRespawn before setting Limbus and dead state");
    check(source_character_restore_plan_v1(2,false,true,false,std::nullopt,false,p,error)&&p.resulting_dead&&
          !p.requires_can_respawn,
          "already-Limbus state skips CanRespawn and remains dead");
    check(source_character_restore_plan_v1(17,true,false,false,std::nullopt,false,p,error)&&p.resulting_dead&&
          !p.requires_can_respawn&&
          std::find(p.ordered_operations.begin(),p.ordered_operations.end(),SourceCharacterRestoreOpV1::set_pre_spawn_if_not_already)!=p.ordered_operations.end(),
          "PreSpawn state retains saved dead bit without CanRespawn");
    check(source_character_restore_plan_v1(3,true,false,false,false,false,p,error)&&p.resulting_dead&&
          std::find(p.ordered_operations.begin(),p.ordered_operations.end(),SourceCharacterRestoreOpV1::set_hp_zero)!=p.ordered_operations.end()&&
          std::find(p.ordered_operations.begin(),p.ordered_operations.end(),SourceCharacterRestoreOpV1::set_ai_dead)!=p.ordered_operations.end(),
          "ordinary saved-dead branch sets HP zero and AI dead, with the known false CanRespawn result");
    check(source_character_restore_plan_v1(3,false,false,false,std::nullopt,false,p,error)&&!p.resulting_dead&&
          std::find(p.ordered_operations.begin(),p.ordered_operations.end(),SourceCharacterRestoreOpV1::initialize_physical_if_missing)!=p.ordered_operations.end()&&
          std::find(p.ordered_operations.begin(),p.ordered_operations.end(),SourceCharacterRestoreOpV1::set_idle_zero)!=p.ordered_operations.end(),
          "saved-alive branch initializes missing physical body and enters Idle without CanRespawn");
    check(source_character_restore_plan_v1(3,false,false,false,std::nullopt,std::nullopt,p,error)==false,
          "saved-alive branch refuses to guess whether its source physical body is absent");
    check(source_character_restore_plan_v1(0xffffffffu,false,false,false,std::nullopt,true,p,error)&&!p.resulting_dead,
          "out-of-range serialized state avoids undefined bit shifts and follows saved-alive branch");
}
void test_source_checkpoint_object_family_table(){
    std::string error;bool eligible=false;
    for(const auto family:{SourceCheckpointObjectFamilyV1::character,SourceCheckpointObjectFamilyV1::container,
            SourceCheckpointObjectFamilyV1::door,SourceCheckpointObjectFamilyV1::module,
            SourceCheckpointObjectFamilyV1::native_trigger}){
        check(source_checkpoint_object_eligible_v1(family,false,eligible,error)&&eligible,
              "source checkpoint-enabled constructor family is eligible by default");
        check(source_checkpoint_object_eligible_v1(family,true,eligible,error)&&!eligible,
              "authored skip_checkpoint clears source eligibility");
    }
    for(const auto family:{SourceCheckpointObjectFamilyV1::object_base,SourceCheckpointObjectFamilyV1::area_zone})
        check(source_checkpoint_object_eligible_v1(family,false,eligible,error)&&!eligible,
              "ObjectBase and area Zone do not inherit checkpoint eligibility");
    check(!source_checkpoint_object_eligible_v1(SourceCheckpointObjectFamilyV1::unknown,false,eligible,error),
          "unknown source class family fails closed");
}
void test_checkpoint_zone_source_gates_and_order(){
    std::string error;DeathCheckpointZoneReceiptV1 receipt;
    auto c=contact();Trace t;
    c.contact_object_zonable=false;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_not_zonable&&t.calls.empty(),"IsZonable gate");
    c=contact();c.contact_character_is_player=false;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_not_local_player&&t.calls.empty(),"IsPlayer gate");
    c=contact();c.contact_is_local_player_zero=false;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_not_local_player&&t.calls.empty(),"same local-player pointer gate");
    c=contact();c.zone_position=c.current_checkpoint_key;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_same_checkpoint_key&&t.calls.empty(),"same checkpoint key suppresses repeat save");
    c=contact();c.source_level_state=33;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_level_or_character_gate&&t.calls.empty(),"Level+304 must exceed 33");
    c=contact();c.local_character_is_dead=true;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_level_or_character_gate&&t.calls.empty(),"source IsDead gate");
    c=contact();c.level_savegame_present=false;
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::skipped_level_or_character_gate&&t.calls.empty(),"LevelSavegame gate");
    c=contact();
    check(run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error),error.c_str());
    check(receipt.complete&&receipt.result==DeathCheckpointZoneResultV1::saved&&
          t.calls==std::vector<std::string>{"record","qest","capture","level-save"},"source checkpoint producer order");
    check(t.key==c.zone_position&&t.revive==c.current_character_position,"arg3=false stores zone key but current Character revive position");
}
void test_checkpoint_zone_failures_stop_order(){
    for(const std::string failed:{"record","qest","capture","level-save"}){
        auto c=contact();Trace t;t.fail=failed;DeathCheckpointZoneReceiptV1 receipt;std::string error;
        check(!run_death_checkpoint_zone_contact_v1(c,services(t),receipt,error)&&receipt.result==DeathCheckpointZoneResultV1::failed,"provider failure reported");
        const std::vector<std::string> expected=failed=="record"?std::vector<std::string>{"record"}:
            failed=="qest"?std::vector<std::string>{"record","qest"}:
            failed=="capture"?std::vector<std::string>{"record","qest","capture"}:
            std::vector<std::string>{"record","qest","capture","level-save"};
        check(t.calls==expected,"failed checkpoint producer must not run later source effects");
    }
}
}
int main(){try{test_checkpoint_zone_source_gates_and_order();test_checkpoint_zone_failures_stop_order();test_source_character_restore_state_table();test_source_checkpoint_object_family_table();
    std::cout<<"PASS: CheckpointZone IsZonable/local-player/key/Level+304/savegame/IsDead gates, exact positions, source save order and failure prefixes\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return EXIT_FAILURE;}return EXIT_SUCCESS;}
