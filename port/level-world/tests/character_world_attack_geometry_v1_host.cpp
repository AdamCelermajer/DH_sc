#include "../character_world_attack_geometry_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;using namespace dh2::character;using namespace dh2::character::skills;
struct Actor {
 target_search::Object48 search{};SkillTargetCharacterV6 target{};
 std::int32_t words[224]{};data::CombatActorState life{};State machine{};
 scene::Scene scene{};target_providers::Handle16 handle{};std::uintptr_t current{},node{};
 std::uint8_t enabled{};float cache[3]{};
 CharacterAttackEmptyInventoryV1 inventory{words};
 static int refresh(void* p,WorldTargetActorBorrowV1* out){auto& a=*static_cast<Actor*>(p);*out={};out->identity=a.search.identity;out->search=&a.search;out->character=&a.target;out->scene=&a.scene;out->life=&a.life;out->position=a.search.position;out->target_node=&a.node;out->target_enabled=&a.enabled;out->cached_target_position=a.cache;return 0;}
 Actor(std::uintptr_t id,int faction,int type){search.identity=id;search.visible=1;words[0]=faction;words[1]=type;words[32]=-1;target={id,words,"fixture",0,0,0,1,1};machine.flags=0x2000;}
 WorldActorRegistrationV1 registration(int key){return {search.identity,key,this,refresh,&machine,&handle,&current};}
};
struct Context {
 Actor& player;Actor& target;unsigned inventory_calls{};bool missing{},bad_identity{};int kind{};unsigned fallback{};
 static int inventory(void* p,std::uintptr_t id,WorldAttackInventoryBorrowV1* out){auto& c=*static_cast<Context*>(p);++c.inventory_calls;if(c.missing)return -1;auto* a=id==c.player.search.identity?&c.player:id==c.target.search.identity?&c.target:nullptr;if(!a)return -1;*out=a->inventory.borrow();if(c.bad_identity)out->resolved=c.player.words;return 0;}
 static int object_kind(void* p,std::uintptr_t,std::int32_t* out){*out=static_cast<Context*>(p)->kind;return 0;}
 static int interaction(void* p,std::uintptr_t,std::uintptr_t,std::int32_t* out){auto& c=*static_cast<Context*>(p);++c.fallback;*out=1;return 0;} // Named unrecovered interaction branch fixture.
 static int open(void*,const char*,std::uintptr_t* out){*out=0;return 0;} // Genuine absent debug file fixture.
 static int close(void*,std::uintptr_t){assert(false);return -1;}
};
int main(){
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;ai.rows[0].melee_radius=20;ai.rows[1].melee_radius=10;
 ai.factions={{{0,1},{1,-1}},{{0,-1},{1,1}}};
 CharacterWorldRuntimeV1 world(ai,16);Actor player(0x100000001ull,0,0),target(0x200000002ull,1,1);
 assert(!world.add(player.registration(1))&&!world.add(target.registration(2)));
 Context context{player,target};DebugFileServices24 files{nullptr,Context::open,Context::close};
 auto* switches=dh2_character_debug_create();assert(switches);
 CharacterWorldAttackGeometryV1 geometry(world,ai,*switches,files,{&context,Context::inventory,Context::object_kind,Context::interaction});
 std::string error;std::int32_t result{};const auto owner=player.search.identity,other=target.search.identity;
 assert(geometry.read(owner,other,WorldAIAttackQueryV1::InventoryCanMeleeAttack,result,error)&&result==1);
 assert(geometry.read(owner,other,WorldAIAttackQueryV1::CharacterCanRangeAttack,result,error)&&result==0);
 target.search.position[0]=29;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==1);
 target.search.position[0]=30;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==0);
 target.search.position[0]=31;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==0);
 // Exact target-node cached position selection supersedes the raw point.
 target.node=1;target.enabled=1;target.cache[0]=5;
 assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==1);
 target.enabled=0;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==0);target.node=0;
 context.bad_identity=true;assert(!geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error));context.bad_identity=false;
 context.kind=1;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==1&&context.fallback==1);context.kind=0;
 context.missing=true;const auto before=context.inventory_calls;
 player.words[32]=4;player.words[30]=10*256;player.words[31]=20*256;
 assert(geometry.read(owner,other,WorldAIAttackQueryV1::CharacterCanRangeAttack,result,error)&&result==1&&context.inventory_calls==before);
 target.search.position[0]=10;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInRange,result,error)&&result==1);
 target.search.position[0]=20;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInRange,result,error)&&result==1);
 target.search.position[0]=21;assert(geometry.read(owner,other,WorldAIAttackQueryV1::IsInRange,result,error)&&result==0);
 player.words[32]=-1;assert(!geometry.read(owner,other,WorldAIAttackQueryV1::CharacterCanRangeAttack,result,error));
 const auto null_before=context.inventory_calls;
 assert(geometry.read(owner,0,WorldAIAttackQueryV1::IsInRange,result,error)&&result==0&&context.inventory_calls==null_before);
 assert(geometry.read(owner,0,WorldAIAttackQueryV1::IsInMeleeRange,result,error)&&result==0);
 dh2_character_debug_destroy(switches);
 std::cout<<"{\"validation\":\"PASS\",\"same_world_properties_handle_positions\":true,\"constructor_inventory_and_strict_geometry\":true,\"unused_inventory_shortcut\":true,\"interaction_backend_fixture\":true}\n";
}
