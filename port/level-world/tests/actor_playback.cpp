#include "../actor_playback.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
using namespace dh2;
std::vector<std::uint8_t> read(const std::string& path){
 std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Missing fixture: "+path);
 return {std::istreambuf_iterator<char>(f),{}};
}
data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
template<class T>T value(const std::uint8_t* p){T out;std::memcpy(&out,p,sizeof(out));return out;}
void check(bool b,const std::string& message){if(!b)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;const std::string assets=argv[1];std::string error;
 auto a=read(assets+"/data/animations_pyarray.bin"),b=read(assets+"/data/animations_pyarraynames.bin"),c=read(assets+"/data/animations_pystructnames.bin"),d=read(assets+"/data/animations_dictionary_pyarraynames.bin"),e=read(assets+"/data/animations_dictionary_pyarray.bin");
 data::Dictionary dictionary;data::AnimationTables tables;
 check(data::load_dictionary(bytes(d),bytes(e),dictionary,error)&&data::load_animation_tables(bytes(a),bytes(b),bytes(c),dictionary,tables,error),error);
 check(data::animation_state(tables,48,"Idle")==&tables.sequences[262]&&data::animation_state(tables,48,"Walk")==&tables.sequences[280]&&data::animation_state(tables,48,"Run")==&tables.sequences[271],"Knight authored state rows differ");
 std::set<int> ids;std::function<void(int,unsigned)> collect=[&](int id,unsigned depth){
  check(depth<3,"Authored redirect exceeds source limit");for(const auto& step:tables.sequences.at(id).steps){if(step.redir==1)collect(step.anim,depth+1);else{check(step.redir==0,"Authored direct clip rejected");ids.insert(step.anim);}}
 };
 // Android SL__LIST_IPHONE=0xd2: five Idle/Walk stances, un-stanced Run.
 for(int id=262;id<267;++id)collect(id,0);for(int id=280;id<285;++id)collect(id,0);collect(271,0);
 check(ids.size()==9,"Reachable Android locomotion bank differs");
 auto raw=read(assets+"/models/prince_modular.bdae");resources::BresView view{};scene::Scene rest;
 check(dh2_bres_open(&view,raw.data(),raw.size())==resources::BresError::ok&&scene::load(view,rest,error),error);
 check(rest.graph.size()==35,"Prince graph node count differs");actor::ClipBank bank;
 for(int id:ids){const auto path=dictionary.values.at(id);const auto name=path.substr(path.find_last_of("/\\")+1);auto clip=read(assets+"/animations/"+name);animation::Player player;
  check(player.load(clip.data(),clip.size(),rest,error),error);check(!player.unbound&&!player.skipped&&player.end>player.start,"Unbound/unsupported/empty Prince clip");bank.emplace(id,std::move(player));
 }
 auto gold=read(argv[2]);check(gold.size()>=8&&!std::memcmp(gold.data(),"APG1",4),"Playback gold header rejected");
 const auto count=value<std::uint32_t>(gold.data()+4);check(gold.size()==8ull+count*108ull,"Playback gold dimensions rejected");
 unsigned comparisons=0,samples=0,switches=0,boundaries=0,replay_samples=0,idle_secondary=0;
 std::set<int> sampled_clips;
 for(unsigned orientation=0;orientation<2;++orientation){
  actor::Playback playback;data::AnimationRandom random;visual::SceneBinding binding;scene::Scene live,authored;
  unsigned active_trace=~0u,previous_timestamp=0;float previous_point[3]{},expected_root[3]{};
  for(unsigned i=0;i<count;++i){const auto* row=gold.data()+8+108*i;
   const auto trace=value<std::uint32_t>(row),op=value<std::uint32_t>(row+4),argument=value<std::uint32_t>(row+8);const auto sequence=value<std::int32_t>(row+12);const auto speed=value<float>(row+16);const auto clip_id=value<std::int32_t>(row+20);
   if(trace!=active_trace){active_trace=trace;playback=actor::Playback{};random={};live=rest;authored=rest;binding=visual::SceneBinding{};check(binding.bind(live,error),error);check(binding.animated_node()==33&&live.graph[33].name=="root_camera","Prince source root differs");const float rotation[]={0,0,orientation?1.57079632679489661923f:0.f};check(binding.set_rotation(rotation),"Source visual rotation rejected");previous_timestamp=0;std::fill(previous_point,previous_point+3,0.f);std::fill(expected_root,expected_root+3,0.f);}
   const auto previous_clip=playback.clip_id;const auto previous_restarts=playback.restarts;bool sampled=false,reset=false;
   switch(op){
    case 0:check(playback.start(tables,sequence,random,bank,binding,live,speed,error),error);switches+=previous_clip!=-1;break;
    case 1:check(playback.scene_phase(argument,bank,binding,live,error),error);sampled=true;break;
    case 2:{std::int32_t captured;std::memcpy(&captured,&argument,4);check(captured==playback.completion.extra_ms,"Animator argument differs from captured completion extra");check(playback.animator_phase(tables,random,bank,binding,live,speed,captured,error),error);break;}
    case 3:{std::int32_t end;std::memcpy(&end,&argument,4);check(dh2_timeline_jump(&playback.timeline,end)==0,"Boundary fixture jump rejected");++boundaries;break;}
    default:throw std::runtime_error("Unknown playback fixture action");
   }
   reset=playback.restarts!=previous_restarts;if(reset){sampled=true;++replay_samples;}
   const std::string label="Playback trace="+std::to_string(trace)+" row="+std::to_string(i)+" op="+std::to_string(op);
   check(playback.clip_id==clip_id,label+" clip differs");
   check(!std::memcmp(&playback.timeline,row+24,56),label+" timeline differs from original instructions");
   check(!std::memcmp(&playback.completion,row+80,8),label+" completion differs from original instructions");
   check(playback.root_timestamp==value<std::uint32_t>(row+88)&&playback.restarts==value<std::uint32_t>(row+92)&&playback.completions==value<std::uint32_t>(row+96),label+" phase counters differ");
   check(random.seed==value<std::uint32_t>(row+100)&&random.calls==value<std::uint32_t>(row+104),label+" authored selection RNG differs from original instructions");
   check(playback.displacement&&playback.scheduler.clip().move_go,label+" authored MoveGO differs");
   if(sampled){const auto& player=bank.at(clip_id);const auto expected_ms=value<std::int32_t>(row+24);const auto timestamp=value<std::uint32_t>(row+88);
    if(reset){check(player.sample(authored,player.start,error),error);std::copy(authored.graph[33].translation,authored.graph[33].translation+3,previous_point);previous_timestamp=timestamp+1;}
    check(player.sample(authored,expected_ms,error),error);const auto* point=authored.graph[33].translation;
    const float dx=timestamp==previous_timestamp?0.f:point[0]-previous_point[0],dy=timestamp==previous_timestamp?0.f:point[1]-previous_point[1];
    // Independent quarter-turn convention: authored -Y walks toward game +X.
    expected_root[0]+=orientation?-dy:dx;expected_root[1]+=orientation?dx:dy;
    for(unsigned axis=0;axis<3;++axis){check(std::fabs(binding.root.position[axis]-expected_root[axis])<.03f,label+" root displacement differs from independent authored samples");check(binding.root.animated[axis]==point[axis],label+" animated point differs");check(binding.root.helper[axis]==-point[axis],label+" helper compensation differs");}
    std::copy(point,point+3,previous_point);previous_timestamp=timestamp;++samples;sampled_clips.insert(clip_id);idle_secondary+=clip_id==1041;
    for(const auto& node:live.graph)for(float component:node.world)check(std::isfinite(component),label+" nonfinite scene matrix");
    for(const auto& instance:live.instances)check(instance.world==live.graph.at(instance.node_index).world,label+" instance matrix differs from animated graph");
   }
   ++comparisons;
  }
 }
 check(sampled_clips==std::set<int>({1040,1041,1114,1126}),"Default authored Idle variants/Walk/Run not all sampled");
 check(idle_secondary&&switches==16&&boundaries==24,"Playback branch coverage differs");
 actor::Playback invalid;check(!invalid.set_speed(-1,error),"Negative global factor accepted");check(!invalid.set_speed(std::nanf(""),error),"NaN global factor accepted");visual::SceneBinding binding;auto live=rest;check(binding.bind(live,error),error);check(!invalid.scene_phase(1,bank,binding,live,error),"Inactive scene phase accepted");data::AnimationRandom rng;actor::ClipBank empty;check(!invalid.start(tables,271,rng,empty,binding,live,1,error),"Missing clip bank accepted");
 std::cout<<"{\"comparisons\":"<<comparisons<<",\"original_phase_records\":"<<count<<",\"traces\":14,\"authored_bank_clips\":9,\"sampled_default_clips\":4,\"prince_nodes\":35,\"root_samples\":"<<samples<<",\"synchronous_replay_samples\":"<<replay_samples<<",\"state_switches\":"<<switches<<",\"strict_boundary_fixtures\":"<<boundaries<<",\"rejection_checks\":4,\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
