#include "../gameplay_camera_animator_v10.hpp"
#include "../gameplay_camera_device_v9.hpp"
#include <fstream>
#include <iterator>
#include <cassert>
#include <iostream>
using namespace dh2;
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Required actual cache "+p);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{if(argc!=2)return 2;std::string path=argv[1],e;std::vector<std::vector<std::uint8_t>> owned;owned.reserve(5);auto bytes=[&](const char* n){owned.push_back(read(path+"/"+n));auto& b=owned.back();return data::Bytes{b.data(),b.size()};};data::Dictionary dict;assert(data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),dict,e));data::AnimationTables tables;assert(data::load_animation_tables(bytes("animations_pyarray.bin"),bytes("animations_pyarraynames.bin"),bytes("animations_pystructnames.bin"),dict,tables,e));
 auto graph=std::make_shared<camera::GameplayCameraSceneV3>();if(!graph->load(read(path+"/cameratests.bdae"),e))throw std::runtime_error(e);
 unsigned debug=0;camera::CameraAnimationManagerServicesV10 services;services.application_lease=graph;services.dictionary=&dict;
 // Application lease/Android manufacturer/Debug endpoints are fixtures; all
 // manager state, canonical resources, camera graph and pose/timelines are real.
 services.actual_lg_devices=[&](auto& lg,auto& err){return camera::source_lg_devices_v9("Google",lg,err);};services.debug_after_add=[&](auto&){++debug;return true;};services.read=[&](const std::string& resource,auto& out,auto&){auto slash=resource.find_last_of("/\\");out=read(path+"/"+resource.substr(slash+1));return true;};
 camera::GameplayCameraAnimationManagerV10 manager(services);std::int32_t key=77;assert(manager.create(key,e)&&key==-2);const auto& row=tables.cameras.at(0);assert(tables.camera_names[0]=="Default");if(!manager.register_camera(key,row,graph->graph(),e))throw std::runtime_error(e);auto set=manager.find(key);assert(set&&set->registration.occurrences().size()==4&&set->registration.lookup(57)==0&&debug==3);
 camera::GameplayCameraAnimatorV10 animator(graph,set);if(!animator.initialize(e))throw std::runtime_error(e);unsigned completed=0;animator.set_source_completion([&](){++completed;});animator.byte10=0;animator.clip_indexc=0;bool played=false;if(!animator.play(row.idle,false,played,e))throw std::runtime_error(e);assert(played);std::cout<<"Actual idle range "<<animator.timeline.start_ms<<' '<<animator.timeline.end_ms<<" callback "<<completed<<'\n';
 for(unsigned tick=0;tick<20;++tick)if(!animator.scene_phase(tick*17,e))throw std::runtime_error(e);
 std::uint32_t selected;assert(graph->select("PlayerCamera_Default",selected,e));float eye[3],target[3];assert(graph->eye_and_target(selected,eye,target,e));std::cout<<"Actual idle eye "<<eye[0]<<' '<<eye[1]<<' '<<eye[2]<<" target "<<target[0]<<' '<<target[1]<<' '<<target[2]<<'\n';
 const auto before=completed;const auto before_graph=graph->graph().graph;
 if(!animator.play(row.crit,false,played,e))throw std::runtime_error(e);assert(played&&completed==before);
 // Original NewAnim(false) only disables displacement: no immediate pose,
 // scene update or callback. Verify every source transform stays unchanged.
 const auto& after_graph=graph->graph().graph;assert(before_graph.size()==after_graph.size());
 for(unsigned i=0;i<before_graph.size();++i){for(unsigned n=0;n<3;++n){assert(before_graph[i].translation[n]==after_graph[i].translation[n]);assert(before_graph[i].scale[n]==after_graph[i].scale[n]);}for(unsigned n=0;n<4;++n)assert(before_graph[i].quaternion[n]==after_graph[i].quaternion[n]);}
 for(unsigned tick=20;tick<200;++tick)if(!animator.scene_phase(tick*17,e))throw std::runtime_error(e);assert(completed>before);const auto once=completed;assert(animator.scene_phase(5000,e)&&completed==once);assert(animator.play(-1,false,played,e)&&!played);animator.release_after_unregistration();assert(!animator.ready());
 camera::CameraAnimationManagerServicesV10 missing;camera::GameplayCameraAnimationManagerV10 failed(missing);key=77;assert(!failed.create(key,e)&&key==77&&failed.current_source_key()==-2&&failed.find(-2)->failed);
 std::cout<<"Camera real resource/registration/same-scene/finite callback PASS; canonical set-2, four authored libraries, no Character FSM or synthetic AI event\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
