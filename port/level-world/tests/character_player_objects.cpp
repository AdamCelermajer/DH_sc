// Reuse immutable cache/Lua/debug helpers; retain their historical test intact.
#define main historical_scene_objects_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_objects.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../../game-data/vitals.hpp"
int main(int argc,char** argv){try{
 check(argc==5);Inputs raw(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);
 CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));
 auto tables=design.borrow();Host host(tables);Debug debug(argv[4]);
 CharacterScriptObjects objects(design.borrow(),debug.owner,&debug.files);
 auto character=[&](const char* name){
  const auto& names=tables.characters()->names;auto found=std::find(names.begin(),names.end(),name);
  check(found!=names.end());auto p=std::make_shared<dh2::data::PropertyState>();
  dh2::data::reset_properties(*tables.rules(),*p,&tables.characters()->rows[found-names.begin()]);
  check(dh2::data::recalc_properties_with_class(*tables.classes(),*tables.rules(),*p,error));
  dh2::data::SpawnVitals vitals;check(dh2::data::initialize_spawn_vitals(*tables.rules(),*p,vitals,error));return p;
 };
 auto player_properties=character("KnightPlayerBase"),monster_properties=character("Crypt_Skeleton");
 auto player_life=std::make_shared<dh2::data::CombatActorState>(),monster_life=std::make_shared<dh2::data::CombatActorState>();
 auto player=objects.add(UINT64_C(0x100000001),"PlayerCharacterPrince",player_properties,player_life,{10,20,0});
 auto owner=objects.add(UINT64_C(0x100000002),"Crypt_Skeleton",monster_properties,monster_life,{10,20,0});
 const auto* player_ai=dh2::data::ai_props(*tables.ai(),player_properties->resolved[1]);
 check(player_ai&&player_ai->type==1&&player_properties->resolved[36]>0);
 CharacterScriptSessionInput input;input.identity=owner->identity;input.name=owner->name;
 input.properties=monster_properties;input.combat=monster_life;input.position=owner->position;input.source_is_character=1;
 input.common={common.data(),common.size()};input.external={monster.data(),monster.size()};
 input.host=&host.host;input.level=&debug.level;input.target=&owner->binding;input.objects=&objects.services();
 auto session=CharacterScriptSession::create(design.borrow(),input,error);check(session&&error.empty());
 check(!session->start());ScriptSessionView view;check(session->owner().active(view));
 source(view.vm,"function BindPlayer(player) player_id=player:GetID(); player_proxy=player end");
 dh2_script_value argument{};argument.type=DH2_SCRIPT_SOURCE_OBJECT;argument.identity=player->identity;
 check(!dh2_script_vm_call_discard_source_objects(view.vm,"BindPlayer",&argument,1));
 source(view.vm,"SetTarget(player_proxy); assert(HasTarget()); saved_player=GetTarget(); assert(saved_player:GetID()==player_id and player_proxy:GetID()==player_id and saved_player~=player_proxy); assert(saved_player:GetTarget()==nil)");
 check(owner->target.target==player->identity&&owner->target.alive==1&&owner->target.sight==1);
 // The target provider must reread actual player life and position backing.
 const auto identity=player->identity,ai_identity=player->target.identity;
 player_life->dead=1;player->position={1e8f,1e8f,1e8f};source(view.vm,"SetTarget(saved_player)");
 check(!owner->target.alive&&!owner->target.sight&&player->life->dead==1);
 player_life->dead=0;player->position=owner->position;source(view.vm,"SetTarget(saved_player)");
 check(owner->target.alive==1&&owner->target.sight==1);
 auto property_view=dh2::data::property_view(*tables.rules(),*player_properties);
 check(!dh2_property_add(&property_view,36,-256)&&player->properties->resolved[36]==player_properties->resolved[36]);
 auto* properties_pointer=player_properties.get();auto* life_pointer=player_life.get();
 player_properties.reset();player_life.reset();player.reset();
 auto retained=objects.find(identity);check(retained&&retained->properties.get()==properties_pointer&&retained->life.get()==life_pointer&&retained->target.identity==ai_identity);
 source(view.vm,"assert(saved_player:GetID()==player_id); proxy=newproxy(true); getmetatable(proxy).__gc=function() assert(GetTarget():GetID()==player_id); ClearTarget() end");
 session.reset();check(!owner->target.target&&!owner->target.last_target&&!owner->binding.scope);
 check(objects.find(identity)==retained&&retained->properties->resolved[36]>0&&debug.opens==1);
 Dl_info origin{};check(dladdr(reinterpret_cast<void*>(dh2_character_ai_set_target),&origin));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_KnightPlayerBase\":true,\"actual_monster_Init\":1,\"original_Lua_player_target\":true,\"live_player_dead_and_sight_reads\":true,\"retained_player_record\":true,\"finalizer_target_clear\":true,\"world_library\":\""<<origin.dli_fname<<"\",\"full_enemy_AI\":false}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
