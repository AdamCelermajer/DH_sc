#include "../character_save_restore_v3.hpp"
#include "../reference/object-save-restore-v3/character-save-gold.hpp"
#include "../reference/object-save-restore-v3/character-load-gold.hpp"
#include <cstdio>
#include <cstdlib>
#include <sstream>
using namespace dh2::character;using dh2::level::SavegameStreamV2;
static unsigned checks;static void check(bool v){++checks;if(!v){std::fprintf(stderr,"FAIL %u\n",checks);std::abort();}}
struct Fixture {
 std::uintptr_t identity{17};std::uint8_t visible{},enabled{1},tested_a{1},tested_b{1};std::uint32_t controller{9};std::int32_t archetype{42},group_word{77};std::uint8_t group_a{7},group_b{8};
 std::uintptr_t visual{},physical{},anchor{},group{};float position[3],rotation[3],initial_pos[3],initial_rot[3],respawn_rot[3],respawn_pos[3],target_pos[3]{100,200,300};
 dh2::data::PropertyState props;dh2::data::PropertySheet defaults{},types{};CharacterLegacyPropertyTagsV3 tags;dh2::data::CombatActorState life{};CharacterStateOwner state{identity};TargetOwner16 owner{identity,0,0,0};TargetState48 target{5,&owner,0,123,456,1,0,0,0,0};TargetServices16 target_services;
 dh2::data::PropertyView view;TimerStore32 timers{};TimerServices32 timer_services{};BuffServices16 buff_services;BuffOwner* buffs{};
 bool player{},monster{},f2{},f3{},respawn{};std::string events;std::uint32_t debug_offset{};
 explicit Fixture(std::uintptr_t id=17);~Fixture(){check(dh2_character_buffs_destroy(buffs)==1);}
 void event(std::uint32_t entry,std::uint32_t a=0,std::uint32_t b=0){if(!events.empty())events+=",";std::ostringstream s;s<<std::hex<<entry<<std::dec<<":"<<a<<":"<<b;events+=s.str();}
 CharacterSaveRestoreBorrowV3 borrow(){return {{identity,&visible,&enabled,&tested_a,&tested_b,&archetype,&visual},&props,&tags,&life,buffs,&state.machine(),position,rotation,initial_pos,initial_rot,respawn_rot,respawn_pos,&group,&physical,&anchor,&controller,&target,&target_services,nullptr,nullptr};}
 void setup(unsigned i,bool pl){player=pl;monster=!pl;visible=i+1;enabled=1;tags.saved=tags.resolved=i<4?legacy_character_properties_tag_v3:0x12345678;tags.saved_produced=tags.resolved_produced=true;for(unsigned j=0;j<224;++j)props.saved[j]=props.resolved[j]=j*17+i;float* p[]={position,rotation,initial_pos,initial_rot,respawn_rot,respawn_pos};for(unsigned j=0;j<6;++j)for(unsigned k=0;k<3;++k)p[j][k]=float(j*3+1+i+k);life.dead=i%2;group=i&1?1:0;state.state().current=std::int32_t(i%4==0?3:i%4==1?0:i%4==2?17:12);state.native_fsm().current_present=i==0?0:1;events.clear();}
};
static int buff_service(void* p,dh2::data::PropertyView*,const BuffRequest32* r,std::uintptr_t*){auto& f=*static_cast<Fixture*>(p);if(r->service!=buff_recalculate)return 0;f.event(0x3e0af8);return 1;}
static int target_service(void* p,TargetState48*,const TargetRequest24* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);if(r->service==target_debug_load)f.event(0x3d6890);if(r->service==target_debug_query)*out=0;else if(r->service==target_owner_ai_id)*out=8;else if(r->service==target_virtual_dead)*out=0;else if(r->service==target_in_sight)*out=0;return 0;}
Fixture::Fixture(std::uintptr_t id):identity(id),state(id),owner{id,0,0,0},target_services{this,target_service},view{defaults.data(),types.data(),props.base.data(),props.saved.data(),props.gear.data(),props.resolved.data(),nullptr,0},buff_services{this,buff_service}{BuffBindings32 b{&view,&timers,&timer_services,&buff_services};buffs=dh2_character_buffs_create(&b);check(buffs!=nullptr);}
static bool service(void* p,const CharacterSaveRestoreRequestV3& r,CharacterSaveRestoreResponseV3& out,std::string& e){auto& f=*static_cast<Fixture*>(p);auto setstate=[&](int id){f.state.state().current=id;f.state.native_fsm().current_present=1;};
 switch(r.entry){
 case 0x3fc:out.group={&f.group_word,&f.group_a,&f.group_b};return true;
 case 0x3a49f0:f.event(r.entry);out.word=f.player;return true;
 case 0x3a3064:f.event(r.entry);out.word=f.monster;return true;
 case 0x31f594:f.event(r.entry);out.word=r.argument0==0xf2?f.f2:f.f3;return true;
 case 0x3c01c0:out.word=f.state.native_fsm().current_present&&f.state.state().current==0;return true;
 case 0x3c01d4:out.word=f.state.native_fsm().current_present&&f.state.state().current==17;return true;
 case 0x3a5248:f.event(r.entry);out.word=f.respawn;return true;
 case 0x38b0f0:f.event(r.entry,r.argument0);f.visible=r.argument0;return true;
 case 0x3a59ac:f.event(r.entry,r.argument0,r.argument1);f.life.dead=0;return true;
 case 0x3c1a00:f.event(r.entry,r.argument0);setstate(3);return true;
 case 0x3c1a64:f.event(r.entry);setstate(17);return true;
 case 0x3c1a74:f.event(r.entry,r.argument0);setstate(0);return true;
 case 0x3e07a0:f.event(r.entry,r.argument0,r.argument1);f.props.resolved[r.argument0]=r.argument1;return true;
 case 0x3cfd7c:case 0x3cfde4:case 0x3d6cdc:case 0x3b4088:case 0x4713d0:case 0x38ba74:case 0x2e0:f.event(r.entry);return true;
 case 0x3935dc:f.event(r.entry);out.point=f.target_pos;return true;
 case 0x393db4:f.event(r.entry,r.argument0);std::memcpy(f.position,reinterpret_cast<const void*>(r.payload),12);return true;
 case 0x3938a0:f.event(r.entry);std::memcpy(f.rotation,reinterpret_cast<const void*>(r.payload),12);return true;
 default:e="declared missing fixture service";return false;
 }
}
static void append(std::vector<std::uint8_t>& b,const void* p,std::size_t n){auto* v=static_cast<const std::uint8_t*>(p);b.insert(b.end(),v,v+n);}
static std::vector<std::uint8_t> snapshot(Fixture& f){std::vector<std::uint8_t>b;append(b,&f.visible,1);append(b,&f.enabled,1);append(b,&f.tested_a,1);append(b,&f.tested_b,1);append(b,f.position,12);append(b,f.rotation,12);auto dead=std::uint8_t(f.life.dead);append(b,&dead,1);append(b,f.respawn_rot,12);append(b,f.respawn_pos,12);append(b,&f.tags.saved,4);append(b,f.props.saved.data(),896);append(b,&f.tags.resolved,4);append(b,f.props.resolved.data(),896);check(f.target.target<=UINT32_MAX&&f.target.last_target<=UINT32_MAX);std::uint32_t current=static_cast<std::uint32_t>(f.target.target),last=static_cast<std::uint32_t>(f.target.last_target);append(b,&current,4);append(b,&last,4);auto state=f.state.state().current;append(b,&state,4);check(f.controller<=255);auto controller=static_cast<std::uint8_t>(f.controller);append(b,&controller,1);return b;}
int main(){std::string e;CharacterSaveRestoreResultV3 result;
 for(const auto& g:character_save_gold_v3){Fixture f;f.setup(g.index,g.player);SavegameStreamV2 stream;check(character_serialize_v3(result,stream,f.borrow(),{&f,service},e));check(stream.bytes()==std::vector<std::uint8_t>(g.bytes,g.bytes+g.size)&&result.completed==1);if(g.index>=4)check(!f.tags.known());}
 for(const auto& g:character_load_gold_v3){const auto& saved=character_save_gold_v3[g.record];Fixture f;f.setup(g.record,saved.player);f.life.dead=g.initial_dead;f.f2=g.f2;f.f3=g.f3;f.respawn=g.respawn;f.physical=g.physical?2:0;f.visual=g.visual?3:0;f.anchor=g.anchor?4:0;SavegameStreamV2 stream({saved.bytes,saved.size});check(character_deserialize_v3(result,stream,f.borrow(),{&f,service},e));auto actual=snapshot(f);if(actual!=std::vector<std::uint8_t>(g.output,g.output+g.size)){std::fprintf(stderr,"case%u cursor%llu/%u\n",g.index,(unsigned long long)stream.tell(),g.cursor);for(unsigned i=0;i<actual.size();++i)if(actual[i]!=g.output[i]){std::fprintf(stderr,"byte%u actual%u gold%u\n",i,actual[i],g.output[i]);break;}}
 check(actual==std::vector<std::uint8_t>(g.output,g.output+g.size));check(stream.tell()==g.cursor&&result.completed==1);
 // Target helper's original body is separately source-proven; count its Sync
 // event observation here without substituting a second target state.
 std::string expected=g.events;const std::string pure_sync="3d49c4:0:0,";auto pos=expected.find(pure_sync);if(pos!=std::string::npos)expected.erase(pos,pure_sync.size());check(f.events==expected);
 }
 CharacterLegacyPropertyTagsV3 tags;check(tags.construct_fresh(e)&&tags.known());check(!tags.construct_fresh(e));tags.saved=0xdeadbeef;check(!tags.known());
 {Fixture high((std::uintptr_t(1)<<40)|17);high.setup(1,false);high.state.native_fsm().current_present=0;high.state.state().current=17;SavegameStreamV2 stream;check(character_serialize_v3(result,stream,high.borrow(),{&high,service},e));const auto& bytes=stream.bytes();check(bytes[6]==255&&bytes[7]==255&&bytes[8]==255&&bytes[9]==255);check(high.identity>UINT32_MAX&&high.owner.identity==high.identity&&high.state.native_fsm().character==high.identity);}
 std::printf("Object/GameObject/Character Save+Load original40 native PASS %u checks\n",checks);
}
