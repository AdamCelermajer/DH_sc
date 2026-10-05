#include "../character_animation_event_owner_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;using namespace dh2::character;using namespace dh2::character::skills;
struct Actor {
 target_search::Object48 search{};SkillTargetCharacterV6 target{};
 std::int32_t words[224]{};data::CombatActorState life{};State machine{};
 scene::Scene scene{};target_providers::Handle16 handle{};std::uintptr_t current{},node{};
 std::uint8_t enabled{};float cache[3]{},angle{},controller_angle{};unsigned died_calls{};
 static int refresh(void* p,WorldTargetActorBorrowV1* out){auto& a=*static_cast<Actor*>(p);*out={a.search.identity,&a.search,&a.target,&a.scene,&a.life,&a.node,&a.enabled,a.cache,a.search.position,&a.angle,&a.controller_angle,nullptr,nullptr};return 0;}
 static int command(void* p,ControllerCommandState32* out){auto& a=*static_cast<Actor*>(p);*out={1,a.search.identity,0,0,0,0};return 0;}
 static int died(void* p,std::uintptr_t id){auto& a=*static_cast<Actor*>(p);assert(a.current==id);++a.died_calls;a.current=0;return 0;}
 WorldActorRegistrationV1 registration(int key){return {search.identity,key,this,refresh,&machine,&handle,&current,command,nullptr,nullptr,died};}
 Actor(std::uintptr_t id,int faction,int type){search.identity=id;search.visible=1;words[0]=faction;words[1]=type;target={id,words,"fixture",0,0,0,1,1};machine.flags=0x2000;}
};

#include <fstream>
#include <cstring>
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
static int script_service(void*,ScriptOwnerV2&,const ScriptOwnerRequest& q,ScriptOwnerResponse& out){
 static const char lua[]="function OnInit()end;function OnAnimEvent(e,l)last_event=e;last_lag=l;calls=(calls or 0)+1;if e=='error' then error('source-animation-error') end end";
 if(q.service==owner_cached_file&&!std::strcmp(q.filename,"data/scripts/ai/_commons.luac")){out.word=1;out.bytes=lua;out.size=sizeof(lua)-1;}
 if(q.service==owner_is_character)out.word=0;
 return 0; // Named registration/cache/tick fixture; no gameplay callbacks used.
}
int main(int argc,char** argv){
 assert(argc==2);std::string root=argv[1],error;
 const auto dir=root+"/port/android-native/app/src/main/assets/data/";
 auto a=read(dir+"effects_pyarray.bin"),b=read(dir+"effects_pyarraynames.bin"),c=read(dir+"effects_pystructnames.bin"),d=read(dir+"effects_dictionary_pyarraynames.bin"),e=read(dir+"effects_dictionary_pyarray.bin");
 data::EffectsTables tables;assert(tables.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},{d.data(),d.size()},{e.data(),e.size()},error));
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;
 CharacterWorldRuntimeV1 world(ai,16);Actor player(0x100000001ull,0,0);assert(!world.add(player.registration(1)));
 ScriptOwnerV2 owner(player.search.identity);for(int i=0;i<7;++i)assert(!owner.advance({7,0,"fixture","Other"},{nullptr,script_service}));
 ScriptSessionView view{};assert(owner.active(view));
 AIEventOwner48 event_owner{player.search.identity,0,0,0,0,0,0,0};std::uintptr_t virtuals[51]{};virtuals[0x94/4]=0x3dca50;
 AIEventState64 events{};events.owner=&event_owner;events.active=view.identity;events.ais_virtuals=virtuals;
 std::int32_t lag=37;std::uintptr_t visual=1;std::uint32_t visual_root=0,cached_floor=0;
 player.scene.graph.resize(3);player.scene.graph[0].name="root";player.scene.graph[1].name="Bip01_L_Foot";player.scene.graph[1].parent=0;player.scene.graph[1].world[12]=11;
 player.scene.graph[2].name="Bip01_R_Foot";player.scene.graph[2].parent=0;player.scene.graph[2].world[12]=22;
 floors::World floors;floors.records.emplace_back(std::make_unique<floors::Record>());
 AnimationFloorBorrowV1 floor{&floors,&cached_floor};data::PropertyView props{};props.resolved=player.words;
 AnimationEventBorrowV1 borrow{player.search.identity,&owner,&view.identity,&lag,&props,&world,&visual,&player.scene,&visual_root,&floor,animation_floor_type_v1};
 CharacterAnimationEventOwnerV1 relay(events,borrow,tables.borrow(),nullptr);
 assert(relay.relay("step_left")&&relay.source_lua_error()==0);
 dh2_script_value value{};assert(!dh2_script_vm_get_global(view.vm,"last_event",&value)&&std::string(value.text,value.text_bytes)=="step_left");
 assert(!dh2_script_vm_get_global(view.vm,"last_lag",&value)&&value.number==37);
 lag=-7;assert(relay.relay("unrelated"));assert(!dh2_script_vm_get_global(view.vm,"last_lag",&value)&&value.number==-7);
 assert(relay.relay("error")&&relay.source_lua_error()>0&&owner.error().find("source-animation-error")!=std::string::npos);
 assert(relay.relay("step_right")&&relay.relay("unrelated"));
 props.resolved[7]=-1;assert(relay.relay("step_left"));props.resolved[7]=999;assert(relay.relay("step_left"));
 visual=0;assert(relay.relay("step_right"));visual=1;
 cached_floor=UINT32_MAX;assert(relay.relay("step_left"));cached_floor=99;assert(!relay.relay("step_left")&&!relay.error().empty());cached_floor=0;
 events.active=0;assert(relay.relay("step_left"));events.active=view.identity;virtuals[0x94/4]=1;assert(!relay.relay("step_left"));virtuals[0x94/4]=0x3dca50;
 std::int32_t found{};assert(!animation_specific_node_v1(player.scene,0,"Bip01_R_Foot",found)&&found==2);
 assert(!animation_specific_node_v1(player.scene,1,"Bip01_R_Foot",found)&&found==-1);
 assert(animation_specific_node_v1(player.scene,99,"Bip01_R_Foot",found)==-1);
 player.scene.graph[2].parent=-1;visual_root=UINT32_MAX;
 assert(!animation_specific_node_v1(player.scene,UINT32_MAX,"Bip01_R_Foot",found)&&found==2);
 assert(relay.relay("step_right")); // Actual scene-container forest, not guessed root0.
 // Equipment/GL reload replaces the level's owning World while the event
 // owner and callback context survive. Rebind the borrowed World before
 // destroying its predecessor; sanitizer then exercises both foot events.
 auto old_floor=std::make_unique<floors::World>();
 old_floor->records.emplace_back(std::make_unique<floors::Record>());
 floor.world=old_floor.get();assert(relay.relay("step_left"));
 auto new_floor=std::make_unique<floors::World>();
 new_floor->records.emplace_back(std::make_unique<floors::Record>());
 floor.world=new_floor.get();old_floor.reset();
 assert(relay.relay("step_left")&&relay.relay("step_right"));
 // Valid water effect275 reaches genuine unavailable FX instead of accepted no-op.
 auto water=[](void*,const char** out){*out="water";return 0;};borrow.floor_type=water;
 CharacterAnimationEventOwnerV1 missing_fx(events,borrow,tables.borrow(),nullptr);props.resolved[7]=0;
 assert(!missing_fx.relay("step_left")&&missing_fx.error().find("VisualFXManager")!=std::string::npos);
 std::cout<<"{\"validation\":\"PASS\",\"actual_effect_tables_and_owned_lua\":true,\"source_default_floor_noop\":true,\"reached_water_fx_rejected\":true,\"same_world_scene\":true}\n";
}
