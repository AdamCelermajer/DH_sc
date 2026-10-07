#include "../character_combat_sound_tables_v2.hpp"
#include "../vox_play3d_owner_v2.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;static unsigned checks{};
static void check(bool b){++checks;if(!b)throw std::runtime_error("sound runtime assertion "+std::to_string(checks));}
struct Fixture {std::vector<sound::VoxPlay3DOperationV2> trace;int disabled{},phase{},online{},mute{},route{},type{},fail{-1};
 static int call(void* p,const sound::VoxPlay3DRequestV2& q,sound::VoxPlay3DResponseV2& r){auto& f=*static_cast<Fixture*>(p);f.trace.push_back(q.operation);if(int(q.operation)==f.fail)return -1;using O=sound::VoxPlay3DOperationV2;
  if(q.operation==O::disabled)r.value=f.disabled;else if(q.operation==O::current_level){r.identity=9;r.value=f.phase;}else if(q.operation==O::online)r.value=f.online;else if(q.operation==O::network_muted)r.value=f.mute;else if(q.operation==O::platform_route)r.value=f.route;else if(q.operation==O::sound_row){r.value=77;r.type=f.type;}else if(q.operation==O::bank_info){for(unsigned i=0;i<5;++i)r.bank_fields[i]=int(i+100);}
  else if(q.operation==O::emit){check(q.selected_sound==77);for(unsigned i=0;i<5;++i)check(r.bank_fields[i]==int(i+100));}
  else if(q.operation==O::native_play){check(q.play->sound_id==77&&q.play->source_float0==-1&&q.play->source_float1==-1);}
  return 0;}
};
int main(){try{std::ifstream in("port/android-native/app/src/main/assets/data/sounds_pyarray.bin",std::ios::binary);std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(in),{}};character::CharacterCombatSoundTablesV2 tables;std::string error;check(tables.load(bytes,error)&&tables.size()==3);
 const auto* armored=tables.get(1);check(armored&&armored->death.count==1&&armored->death.ids[0]==28);check(armored->hit.count==3&&armored->hit.ids[0]==29&&armored->hit.ids[2]==31);check(armored->impact1.count==3&&armored->impact1.ids[0]==120);check(armored->impact2.count==3&&armored->impact2.ids[0]==117);check(!armored->use_impact1&&armored->use_impact2);
 const auto* fallback=tables.get(-1);check(fallback==tables.get(2)&&fallback==tables.get(100));check(!fallback->death.count&&!fallback->hit.count&&!fallback->impact1.count&&!fallback->impact2.count&&fallback->use_impact1&&!fallback->use_impact2);
 Fixture f;sound::VoxPlay3DOwnerV2 owner({&f,Fixture::call});character::CombatSoundPlayV1 play{1,2,30,{},false,1,-1,-1};
 check(owner.play(play)==0&&f.trace.size()==2);f.disabled=1;f.trace.clear();check(owner.play(play)==0&&f.trace.size()==1);
 f.disabled=0;f.phase=38;f.online=1;f.mute=1;f.trace.clear();check(owner.play(play)==0&&f.trace.size()==4);
 f.online=0;f.type=1;f.trace.clear();check(owner.play(play)==0&&f.trace.back()==sound::VoxPlay3DOperationV2::native_play);
 f.type=0;f.trace.clear();check(owner.play(play)==0&&f.trace.back()==sound::VoxPlay3DOperationV2::emit);
 f.route=1;f.trace.clear();check(owner.play(play)==0&&f.trace.back()==sound::VoxPlay3DOperationV2::platform_play);
 f.route=0;f.fail=int(sound::VoxPlay3DOperationV2::bank_info);f.trace.clear();check(owner.play(play)==-2&&f.trace.back()==sound::VoxPlay3DOperationV2::bank_info);check(!owner.error().empty());
 auto truncated=bytes;truncated.resize(20);check(!tables.load(truncated,error));check(tables.get(1)->death.ids[0]==28);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"audio_playback\":false}"<<std::endl;
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
