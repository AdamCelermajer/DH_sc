#include "../character_animation_instance.hpp"
#include "../character_state_owner_behavior.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
unsigned checks=0,frames=0,notifications=0;
void check(bool condition,const std::string& error){++checks;if(!condition)throw std::runtime_error(error);}
std::vector<std::uint8_t> read(const std::filesystem::path& path){
 std::ifstream stream(path,std::ios::binary);if(!stream)throw std::runtime_error("Missing input "+path.string());
 return {std::istreambuf_iterator<char>(stream),{}};
}
data::Bytes bytes(const std::vector<std::uint8_t>& data){return {data.data(),data.size()};}
std::vector<float> pose(const scene::Scene& scene){
 std::vector<float> out;for(const auto& node:scene.graph)out.insert(out.end(),node.world.begin(),node.world.end());return out;
}
struct Focus {
 character::CharacterAnimationInstance* instance;
 const data::AnimationTables* tables;
 data::AnimationRandom random;
 unsigned animations=0,events=0;
};
void body(void* opaque,character::State*,const character::Request* request){
 auto& focus=*static_cast<Focus*>(opaque);std::string error;
 check(request->service==character::set_animation,"Unexpected Idle source body dependency");
 check(focus.instance->start(*focus.tables,request->argument[0],focus.random,1,error),error);++focus.animations;
}
int remaining(void* opaque,character::StateOwnerMachine40*,const character::StateOwnerRequest48* request,character::StateOwnerResponse8*){
 auto& focus=*static_cast<Focus*>(opaque);
 // Explicit observer fixture: this ownership test does not claim full CharAI.
 if(request->operation!=character::state_owner_character_event||request->event!=0x1d)return -1;
 ++focus.events;return 0;
}
void observe(void*,actor::BlendedPlayback&,const actor::BlendedPlaybackEvent&){++notifications;}
}
int main(int argc,char** argv){try{
 check(argc==5,"Usage: bank.bin model.bdae assets-root original-data-directory");
 const auto bank_bytes=read(argv[1]),model_bytes=read(argv[2]);
 resources::BresView model{};scene::Scene factory;std::string error;
 check(dh2_bres_open(&model,model_bytes.data(),model_bytes.size())==resources::BresError::ok,"Model rejected");
 check(scene::load(model,factory,error),error);
 const std::filesystem::path assets=argv[3],data_directory=argv[4];
 auto reader=[&](const data::AnimationBankResource& resource,std::vector<std::uint8_t>& output,std::string&){output=read(assets/resource.asset);return true;};
 std::shared_ptr<const character::CharacterAnimationResources> resources;
 check(character::CharacterAnimationResources::load(bytes(bank_bytes),factory,reader,resources,error),error);
 const auto resource_count=resources->metadata().resources.size(),occurrences=resources->metadata().registration_requests.size();
 auto first=character::CharacterAnimationInstance::create(resources,error);check(bool(first),error);
 auto second=character::CharacterAnimationInstance::create(resources,error);check(bool(second),error);
 check(&first->scene()!=&second->scene()&&first->scene().graph.data()!=second->scene().graph.data(),"Character poses alias");
 check(&first->resources()==&second->resources(),"Immutable resources are not shared");
 for(const auto& resource:resources->metadata().resources){
  const auto& order=resources->metadata().registration_requests;
  const auto original_first=std::find(order.begin(),order.end(),resource.clip_id)-order.begin();
  check(first->playback().engine_index(resource.clip_id)==original_first,"Registration first occurrence differs");
 }
 check(first->playback().transform_set().clip_count()==occurrences,"Repeated libraries were deduplicated");
 const auto template_start=resources->clips().at(resources->metadata().template_clip_id).start;
 check(first->playback().transform_set().clip(0)->start==template_start,"Signed template start changed");
 auto keep=resources;
 const auto empty_reader=[](const data::AnimationBankResource&,std::vector<std::uint8_t>& out,std::string&){out.clear();return true;};
 check(!character::CharacterAnimationResources::load(bytes(bank_bytes),factory,empty_reader,resources,error)&&resources==keep,"Failed load replaced pinned resources");
 auto corrupt_reader=[&](const data::AnimationBankResource& resource,std::vector<std::uint8_t>& output,std::string&){output=read(assets/resource.asset);output[0]^=1;return true;};
 check(!character::CharacterAnimationResources::load(bytes(bank_bytes),factory,corrupt_reader,resources,error)&&resources==keep,"Tampered resource replaced pinned owner");
 check(!character::CharacterAnimationInstance::create({},error),"Missing owner accepted");
 const auto names=read(data_directory/"animations_dictionary_pyarraynames.bin"),values=read(data_directory/"animations_dictionary_pyarray.bin");
 data::Dictionary dictionary;check(data::load_dictionary(bytes(names),bytes(values),dictionary,error),error);
 const auto records=read(data_directory/"animations_pyarray.bin"),table_names=read(data_directory/"animations_pyarraynames.bin"),fields=read(data_directory/"animations_pystructnames.bin");
 data::AnimationTables tables;check(data::load_animation_tables(bytes(records),bytes(table_names),bytes(fields),dictionary,tables,error),error);
 const auto field=std::find(tables.state_names.begin(),tables.state_names.end(),"Idle");check(field!=tables.state_names.end(),"Idle field missing");
 const auto index=resources->metadata().animation_table;check(index<tables.characters.size(),"Character animation row missing");
 const auto& values_idle=tables.characters[index].fields[field-tables.state_names.begin()];check(values_idle.size()==1,"Idle scalar missing");
 character::Facts facts{};facts.idle=values_idle[0];facts.stance=0;
 // Actual nonplayer stance0 preserves the source sequence. These source-body
 // facts deliberately do not claim an original full equipment/property reader.
 Focus focus{first.get(),&tables,{}};
 character::Services bodies{&focus,body};character::StateOwnerBehaviorPredicate8 predicates{};
 character::StateOwnerBehaviorContext40 context{&facts,&bodies,&predicates,{&focus,remaining}};
 character::StateOwnerServices16 services{};
 check(dh2_character_state_owner_behavior_bind(&services,&context)==1,"Native behavior binding failed");
 character::CharacterStateOwner owner(0x100000002ull);
 first->playback().observer={nullptr,observe};
 check(owner.initialize_level(3,services)==1&&owner.state().current==3&&owner.state().flags==0x2380,"Source owned Idle focus failed");
 check(focus.animations==1&&focus.events==1,"Source Idle animation/notification order missing");
 resources.reset();keep.reset();factory={};
 const auto second_pose=pose(second->scene());const auto second_timeline=second->playback().slots;
 for(unsigned i=0;i<180;++i){
  check(first->scene_phase(1000+i*17,error),error);
  check(first->animator_phase(tables,focus.random,1,error),error);++frames;
  for(float value:pose(first->scene()))check(std::isfinite(value),"Nonfinite per-character pose");
 }
 check(pose(second->scene())==second_pose,"Another character's animation modified this pose");
 check(second->playback().root_timestamp==0&&second->playback().current_timeline().current_ms==second_timeline[second->playback().blend.current].timeline.current_ms,"Another character's clock advanced");
 first.reset();check(second->scene_phase(1000,error),error);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"frames\":"<<frames<<",\"resources\":"<<resource_count<<",\"registration_occurrences\":"<<occurrences<<",\"template_start\":"<<template_start<<",\"source_Idle_Focus\":true,\"per_character_pose_and_clock_isolation\":true,\"CPU_backing_retained_after_external_owner_release\":true,\"full_AI_event_delivery\":false,\"full_physical_frame\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
