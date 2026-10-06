#include "openable_container_owner_v1.hpp"
#include "openable_container_interaction_v2.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
int main(){
 OpenableContainerFieldsV1 f;OpenableContainerServicesV1 s;std::string error;
 assert(f.source_type_f4==7&&f.state394==2&&f.key_id710==-1&&f.key_qty==1&&f.key_consume);
 auto lease=std::make_shared<int>(0);s.owner=lease;
 std::vector<std::string> trace;unsigned flags=0;bool visual=true,script=true;
 s.spawn_roll_and_probability=[](auto& roll,auto& prob,auto&){roll=0;prob=100;return true;};
 s.resolve_row=[](auto const&,auto& id,auto& r,auto&){id=4;r.loot=12;r.sound=9;r.keep_physics=false;return true;};
 s.game_object_init_post=[&](auto&){trace.push_back("base_post");return true;};
 s.meet_condition=[](auto& v,auto&){v=true;return true;};
 s.has_visual=[&](auto& v,auto&){v=visual;return true;};
 s.bind_timeline_callbacks=[&](auto&){trace.push_back("callbacks");return true;};
 s.play_animation=[&](const char* name,bool& played,auto&){trace.push_back(name);played=std::string(name)=="idle";return true;};
 s.scene_flags=[&](auto clear,auto set,auto&){flags=(flags&~clear)|set;return true;};
 s.apply_mesh_box=[&](auto&){trace.push_back("meshbox");return true;};
 s.has_sound_manager=[](auto& v,auto&){v=false;return true;};
 s.load_object_script=[&](auto const&,auto,auto&){trace.push_back("scriptload");return true;};
 s.detach_physical=[&](auto&){trace.push_back("detach");return true;};
 s.play_sound_3d=[&](auto sound,auto&){assert(sound==9);trace.push_back("sound");return true;};
 s.source_on_interact=[&](auto&){trace.push_back("update");return true;};
 s.drop_loot_table=[&](auto table,auto opener,auto powers,auto b,auto&){assert(table==12&&opener==0x123&&powers==-1&&!b);trace.push_back("drop");return true;};
 s.has_script=[&](auto& v,auto&){v=script;return true;};
 s.script_call=[&](auto name,auto actor,auto event,auto&){assert(std::string(name)=="OnOpen"&&actor==0x123&&!event);trace.push_back(name);return true;};
 OpenableContainerOwnerV1 owner(f,s);assert(owner.init_post(error));
 assert(f.state394==2&&flags==0x400&&owner.is_interactive(false)&&!owner.is_interactive(true));
 assert((trace==std::vector<std::string>{"base_post","callbacks","prespawn","spawn","idle","meshbox","scriptload"}));
 trace.clear();assert(owner.interact_base(0x123,error));assert(f.state394==3&&flags==0);
 assert((trace==std::vector<std::string>{"detach","activate","sound","update"}));
 assert(owner.animation_event("opened",error));assert(trace[4]=="drop"&&trace[5]=="OnOpen");
 assert(owner.animation_finished(false,error));assert(f.state394==4&&flags==0x400&&trace.back()=="idleactive");
 auto n=trace.size();assert(owner.interact_base(0x123,error));assert(trace.size()==n);
 // Missing real world DropLoot receiver fails at the reached boundary; no fake inventory mutation.
 s.drop_loot_table={};OpenableContainerOwnerV1 missing(f,s);assert(!missing.do_open(error));assert(error.find("DropLootTable")!=std::string::npos);
 // Original key_consume=false deliberately returns false, even with sufficient key quantity.
 f.state394=2;f.key_id710=7;f.key_consume=false;
 s.is_character=[](auto,bool& v,auto&){v=true;return true;};
 s.find_key=[](auto,auto,bool& found,std::int16_t& qty,auto&){found=true;qty=2;return true;};
 bool removed=false;s.remove_key=[&](auto,auto,bool& result,auto&){removed=true;result=true;return true;};
 OpenableContainerOwnerV1 keys(f,s);bool unlocked=true;assert(keys.try_unlock(1,unlocked,error)&&!unlocked&&!removed);
 f.key_consume=true;assert(keys.try_unlock(1,unlocked,error)&&unlocked&&removed);
 // Whole derived source Interact: real hosting event precedes inherited opening;
 // source achievement mutation happens even if the inherited state guard skips opening.
 f.key_id710=-1;f.state394=2;trace.clear();OpenableContainerInteractionServicesV2 outer;
 outer.current_level=[](auto& level,auto&){level=0x400;return true;};
 outer.local_player_hosting=[](auto& v,auto&){v=true;return true;};
 outer.room64=[](auto& v,auto&){v=5;return true;};
 outer.constant=[](auto first,auto second,auto& v,auto&){assert(std::string(first)=="v2QuestObjectiveType"&&std::string(second)=="OpenGameObject");v=13;return true;};
 outer.raise_async=[&](auto level,auto const& event,auto&){assert(level==0x400&&event.actor==0x123&&event.room==5&&event.objective_type==13&&event.source_index==-1&&!event.byte18&&!event.byte19);trace.push_back("quest");return true;};
 outer.handle_as_character=[](auto id,auto& v,auto&){v=id;return true;};
 outer.is_player=[](auto,auto& v,auto&){v=true;return true;};
 outer.props_add_int=[&](auto id,auto prop,auto amount,auto&){assert(id==0x123&&prop==218&&amount==1);trace.push_back("count_add");return true;};
 outer.props_get_int=[](auto,auto prop,auto flag,auto& v,auto&){assert(prop==218&&!flag);v=100;return true;};
 outer.is_local_player=[](auto,auto& v,auto&){v=true;return true;};
 outer.trophy_name_index=[](auto name,auto& v,auto&){assert(std::string(name)=="open_100_chests");v=22;return true;};
 outer.unlock_trophy=[&](auto id,auto&){assert(id==22);trace.push_back("trophy");return true;};
 assert(openable_container_interact_v2(keys,f,outer,0x123,error));
 assert(trace.front()=="quest"&&trace[1]=="detach"&&trace[trace.size()-2]=="count_add"&&trace.back()=="trophy");
 outer.raise_async={};assert(!openable_container_interact_v2(keys,f,outer,0x123,error));
 assert(error.find("RaiseAsync")!=std::string::npos);
 std::cout<<"OpenableContainer source state/event/unlock composition PASS (declared service fixtures)\n";
}
