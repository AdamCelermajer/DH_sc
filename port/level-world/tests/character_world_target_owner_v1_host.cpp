#define main existing_v3_player_fixture_main
#include "character_player_skills_v3.cpp"
#undef main
#include "../character_world_target_owner_v1.hpp"
namespace {
struct BorrowedWorld {
 target_search::Object48 objects[2]{};
 sk::SkillTargetCharacterV6 characters[2]{};
 data::PropertyState properties[2];data::CombatActorState life[2];
 scene::Scene scenes[2];std::uintptr_t nodes[2]{};std::uint8_t enabled[2]{1,1};
 float cached[2][3]{},heading[2]{},controller_heading[2]{};
 character::ControllerCommandState32 command{7,11,0,0,0,0};
 unsigned lookups=0;bool reject=false;
 const data::AiTables* ai{};
 // Explicit wrapper-boundary fixture executes the genuine cached faction
 // kernel; it does NOT represent the unrecovered original handle/kind prefix.
 static int enemy(void* p,std::uintptr_t a,std::uintptr_t b,std::uintptr_t* out){auto& w=*static_cast<BorrowedWorld*>(p);if(a!=11||b!=22)return -1;*out=data::ai_enemy(*w.ai,w.characters[0].resolved[0],w.characters[1].resolved[0],true,false);return 0;}
 static int actor(void* p,std::uintptr_t identity,sk::WorldTargetActorBorrowV1* out){auto& w=*static_cast<BorrowedWorld*>(p);++w.lookups;if(w.reject)return -1;
  auto i=identity==11?0:identity==22?1:2;if(i==2)return -1;
  *out={identity,&w.objects[i],&w.characters[i],&w.scenes[i],&w.life[i],&w.nodes[i],&w.enabled[i],w.cached[i],w.objects[i].position,&w.heading[i],&w.controller_heading[i],nullptr,nullptr};return 0;
 }
 static int controller(void* p,std::uintptr_t identity,character::ControllerCommandState32* out){auto& w=*static_cast<BorrowedWorld*>(p);if(identity!=w.command.owner)return -1;*out=w.command;return 0;}
};
}
int main(int argc,char** argv){try{
 check(argc==2);const std::string root=argv[1];Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error),error);auto d=design.borrow();BorrowedWorld world;
 const auto at=std::find(d.characters()->names.begin(),d.characters()->names.end(),"KnightPlayerBase");check(at!=d.characters()->names.end());
 data::reset_properties(*d.rules(),world.properties[0],&d.characters()->rows[at-d.characters()->names.begin()]);check(data::recalc_properties_with_class(*d.classes(),*d.rules(),world.properties[0],error),error);
 bool found=false;for(const auto& row:d.characters()->rows){data::reset_properties(*d.rules(),world.properties[1],&row);if(!data::recalc_properties_with_class(*d.classes(),*d.rules(),world.properties[1],error))continue;const auto* ai=data::ai_props(*d.ai(),world.properties[1].resolved[1]);if(ai&&ai->type==4&&data::ai_enemy(*d.ai(),world.properties[0].resolved[0],world.properties[1].resolved[0],true,false)){found=true;break;}}check(found);
 for(unsigned i=0;i<2;++i){world.objects[i].identity=i?22:11;world.objects[i].visible=1;world.characters[i]={world.objects[i].identity,world.properties[i].resolved.data(),i?"Enemy":"Player",0x2000,0,0,1,1};}
 world.objects[1].position[1]=-100;world.objects[1].position[0]=10;
 target_search::Entry16 head{},entry{};entry={&head,&world.objects[1]};head={&entry,nullptr};target_search::Room16 sentinel{},room{};room={&sentinel,&head};sentinel={&room,nullptr};target_search::Registry8 registry{&sentinel};
 world.ai=d.ai();sk::CharacterWorldTargetOwnerV1 required_owner(registry,*d.ai(),{&world,BorrowedWorld::actor,BorrowedWorld::controller,nullptr,nullptr});auto required_search=required_owner.search_services();target_search::Request24 enemy_request{target_search::is_enemy,0,11,22};target_search::Response16 enemy_response{};check(required_search.invoke(required_search.context,&enemy_request,&enemy_response)==-1,"Cached faction rows never substitute whole AI_IsEnemy wrapper");
 sk::CharacterWorldTargetOwnerV1 owner(registry,*d.ai(),{&world,BorrowedWorld::actor,BorrowedWorld::controller,nullptr,BorrowedWorld::enemy});auto native=owner.native_world(11,nullptr);check(native.character==&world.objects[0]&&native.registry==&registry);
 target_search::Target24 heap[8]{};target_search::List40 list{};auto services=owner.search_services();check(!target_search::dh2_target_list_init(&list,heap,8,native.character,1,&services));check(!target_search::dh2_target_search(&list,&registry,200,3.141592741f,&services));check(list.count==1&&list.heap[0].identity==22,"Actual AI row/hostility/radius and V6 target kernels over explicit borrowed world input");
 world.life[1].dead=1;check(target_search::dh2_target_search(&list,&registry,200,3.141592741f,&services)==2&&list.count==0,"Same actual dead owner reaches required AI_IsFriend before dead filter");world.life[1].dead=0;
 world.objects[1].visible=0;check(!target_search::dh2_target_search(&list,&registry,200,3.141592741f,&services)&&list.count==0);world.objects[1].visible=1;
 world.objects[1].zoned=1;world.objects[1].in_zone=0;check(!target_search::dh2_target_search(&list,&registry,200,3.141592741f,&services)&&list.count==0);world.objects[1].in_zone=1;
 check(!owner.look_at(11,22),owner.error());check(world.heading[0]!=0&&world.heading[0]==world.controller_heading[0],"Actual CmdLookAt/body updates SAME actor/controller heading");
 const auto ordinary=world.heading[0];world.nodes[1]=123;world.cached[1][0]=-100;world.cached[1][1]=0;check(!owner.look_at(11,22));check(world.heading[0]!=ordinary,"Exact enabled target-node cache selected");
 const auto cached_heading=world.heading[0];world.enabled[1]=0;check(!owner.look_at(11,22));check(world.heading[0]==ordinary,"Disabled target node uses actual position");
 world.command.locked=1;world.reject=true;const auto before=world.lookups;check(!owner.look_at(11,22)&&world.lookups==before,"Original blocked controller does not reach target producer");
 world.command.forced=1;check(owner.look_at(11,22)==-1&&world.heading[0]==ordinary,"Forced controller reaches missing borrow; no fabricated facing");world.reject=false;world.command.locked=world.command.forced=0;
 world.life[1].dead=1;target_search::Request24 interactive{target_search::is_interactive,0,22,11};target_search::Response16 response{};check(services.invoke(services.context,&interactive,&response)==-1,"Reached dead-friendly path requires original AI_IsFriend");
 check(cached_heading!=ordinary);std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_design_rows\":true,\"explicit_world_topology_and_source_flags_input\":true,\"same_property_life_scene_heading_borrows\":true,\"no_inventory_save_session_or_relation_owner\":true,\"production_renderer_connection\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
