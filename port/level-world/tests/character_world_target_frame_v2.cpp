#define main historical_world_registration_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_world_runtime_v1_host.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_world_target_frame_v2.hpp"
#include "../player_target_died_v2.hpp"
struct Died {
 TargetOwner16 owner{0x500000005ull,9,0,0};TargetState48 state{};
 CharacterControlServices16 control{this,body};TargetServices16 target{this,debug};
 unsigned phase{},stops{},events{},debug_calls{};bool fail=false;
 Died(){state.identity=0x600000006ull;state.owner=&owner;state.target=state.last_target=99;}
 static int body(void* raw,const CharacterControlRequest32* q,CharacterControlResponse16* out){auto& d=*static_cast<Died*>(raw);assert(q->subject==d.owner.identity);if(q->service==control_is_remotely_updated){out->word=0;return 1;}if(q->service==control_stop_object){++d.stops;return 1;}if(q->service==control_character_event){assert(q->argument==0x3f);++d.events;return 1;}return -1;}
 static int debug(void* raw,TargetState48*,const TargetRequest24* q,std::uint32_t* out){auto& d=*static_cast<Died*>(raw);assert(q->service==target_debug_load||q->service==target_debug_query);++d.debug_calls;*out=0;return d.fail?-1:0;}
 static int borrow(void* raw,PlayerTargetDiedPhaseV2 p,PlayerTargetDiedBorrowV2* out){auto& d=*static_cast<Died*>(raw);assert(unsigned(p)==d.phase++);*out={};out->controller={1,d.owner.identity,0,0,0,0};out->control=&d.control;out->target=&d.state;out->target_services=&d.target;return 0;}
};
struct Frame {
 TargetState48 state{};TargetOwner16 owner{};unsigned calls{},events{};
 std::uint32_t machine=3,event{};bool fail=false,clear=true;
 static int current(void* raw,std::uintptr_t id,std::uint32_t* out){auto& f=*static_cast<Frame*>(raw);assert(id==f.owner.identity);++f.calls;if(f.fail)return -1;*out=f.machine;return 0;}
 static int raised(void* raw,std::uintptr_t id,std::uint32_t e,std::uintptr_t){auto& f=*static_cast<Frame*>(raw);assert(id==f.owner.identity);++f.events;f.event=e;
  if(e==0xa)assert(f.state.alive==1); // source captured alive commits AFTER callback
  if(e==0xd)assert(f.state.sight==0); // likewise source sight commit
  if(f.clear)f.state.target=0;return 0;
 }
};
int main(){
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;
 ai.rows[0].view_radius=ai.rows[1].view_radius=100;ai.factions={{{0,1},{1,-1}},{{0,-1},{1,1}}};
 CharacterWorldRuntimeV1 world(ai,16);Actor player(0x100000001ull,0,0),enemy(0x200000002ull,1,1),friend_actor(0x300000003ull,0,0);
 assert(!world.add(player.registration(1))&&!world.add(enemy.registration(2))&&!world.add(friend_actor.registration(3)));
 auto* debug=dh2_character_debug_create();assert(debug);DebugFileServices24 files{};
 CharacterWorldAttackGeometryV1 geometry(world,ai,*debug,files,{});
 Frame f;f.owner.identity=player.search.identity;f.state.identity=0x400000004ull;f.state.owner=&f.owner;
 CharacterWorldTargetFrameV2 frame(world,geometry,ai,{&f,Frame::current,Frame::raised,nullptr});unsigned checks=0;
 f.machine=17;f.state.target=enemy.search.identity;assert(!frame.update(f.state)&&f.calls==1&&!f.events);++checks;
 f.calls=0;f.machine=0;assert(!frame.update(f.state)&&f.calls==2&&!f.events);++checks;
 f.calls=0;f.machine=3;f.state.target=0;assert(!frame.update(f.state)&&f.calls==2&&!f.events);++checks;
 f.state.target=enemy.search.identity;f.state.last_target=enemy.search.identity;enemy.life.dead=1;
 assert(!frame.update(f.state)&&!f.state.target&&!f.state.last_target&&f.event==0xc);++checks;
 // Dead non-monster FRIEND remains genuinely interactive in original
 // Character.IsInteractive(owner); this reaches actual alive transition.
 f.state.target=friend_actor.search.identity;f.state.alive=1;friend_actor.life.dead=1;
 assert(!frame.update(f.state)&&f.event==0xa&&!f.state.target&&!f.state.alive);++checks;
 enemy.life.dead=0;f.state.target=enemy.search.identity;f.state.alive=1;f.state.sight=0;
 assert(!frame.update(f.state)&&f.event==0xd&&!f.state.target&&f.state.sight==1);++checks;
 f.state.target=enemy.search.identity;f.fail=true;auto before=f.state;
 assert(frame.update(f.state)==2&&!std::memcmp(&before,&f.state,sizeof(before))&&frame.error().find("SM_GetState")!=std::string::npos);++checks;
 std::cout<<"source AI target frame: "<<checks<<" checks PASS; same World life/FSM classification, distinct AI identity, post-event stores; external event endpoint fixture\n";
 dh2_character_debug_destroy(debug);
 Died d;std::string error;assert(!player_target_died_v2({&d,Died::borrow},error));
 assert(d.stops==1&&d.events==1&&d.phase==3&&!d.state.target&&!d.state.last_target&&!d.owner.word14d0);
 Died failed;failed.fail=true;assert(player_target_died_v2({&failed,Died::borrow},error)<0);
 assert(failed.stops==1&&failed.events==1&&failed.phase==2&&failed.state.target==99&&failed.state.candidate==0&&!failed.owner.word14d0);
 std::cout<<"whole AISPlayer target death endpoint: 4 checks PASS; real Cmd_Stop/SetTarget/Sync kernels, explicit controllable/debug fixtures\n";
}
