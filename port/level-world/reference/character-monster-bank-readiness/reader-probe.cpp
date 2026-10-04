#include "../../../engine-animation/animation_registration.hpp"
#include "../../../game-data/animation_bank.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
using namespace dh2;
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(path);return {std::istreambuf_iterator<char>(f),{}};}
static void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
int main(int argc,char**argv){try{
 if(argc!=4)return 2;std::string error;auto metadata=read(argv[1]),model=read(argv[2]);data::AnimationBank bank;
 require(data::load_animation_bank({metadata.data(),metadata.size()},bank,error),error);
 resources::BresView image{};scene::Scene graph;
 require(dh2_bres_open(&image,model.data(),model.size())==resources::BresError::ok,"model Bres");require(scene::load(image,graph,error),error);
 std::map<std::int32_t,animation::Player> players;unsigned skipped=0,unbound=0;std::cout<<"{\"resources\":[";bool first=true;
 for(const auto&r:bank.resources){auto raw=read(std::string(argv[3])+"/"+r.asset);require(raw.size()==r.bytes,"resource bytes");auto&player=players[r.clip_id];require(player.load(raw.data(),raw.size(),graph,error,animation::MissingTargets::ignore),error);
  if(!first)std::cout<<',';first=false;std::cout<<"{\"clip_id\":"<<r.clip_id<<",\"tracks\":"<<player.track_count()<<",\"segments\":"<<player.segment_count()<<",\"skipped\":"<<player.skipped<<",\"unbound\":"<<player.unbound<<",\"start\":"<<player.start<<",\"end\":"<<player.end<<"}";skipped+=player.skipped;unbound+=player.unbound;
 }
 animation::RegistrationSet registration;for(auto id:bank.registration_requests)require(registration.append(id,data::animation_resource_identity(bank,id),&players.at(id),error),error);
 require(registration.set_default(data::animation_resource_identity(bank,bank.template_clip_id),&players.at(bank.template_clip_id),error),error);registration.refresh_indices();
 animation::TransformSet compiled;require(compiled.compile_dynamic(registration.compiled_inputs(),graph,error,registration.default_player()),error);
 require(compiled.clip_count()==bank.registration_requests.size(),"occurrences");require(skipped==0,"unsupported reader tracks");
 std::size_t samples=0,defaults=0,retained=0,bound=0;for(const auto&t:compiled.targets())bound+=t.node!=UINT32_MAX;
 for(std::size_t c=0;c<compiled.clip_count();++c){const auto*clip=compiled.clip(c);for(std::size_t t=0;t<compiled.targets().size();++t){const auto*binding=compiled.clip_target(c,t);defaults+=binding->mode==1&&binding->has_default;retained+=binding->mode!=2&&!binding->has_default;
   std::int32_t cursor=0;for(auto time:{clip->start,0,clip->end}){float out[4]={41,42,43,44};require(compiled.sample(c,t,time,out,4,&cursor,error),error);for(std::uint32_t n=0;n<compiled.targets()[t].components;++n)require(std::isfinite(out[n]),"nonfinite sample");++samples;}
 }}
 std::cout<<"],\"validation\":\"PASS\",\"scene_nodes\":"<<graph.graph.size()<<",\"occurrences\":"<<compiled.clip_count()<<",\"unique_resources\":"<<players.size()<<",\"targets\":"<<compiled.targets().size()<<",\"bound_targets\":"<<bound<<",\"samples\":"<<samples<<",\"defaults\":"<<defaults<<",\"retained_bindings\":"<<retained<<",\"skipped\":"<<skipped<<",\"unbound_tracks\":"<<unbound<<",\"constructor_library0_start\":"<<compiled.clip(0)->start<<",\"constructor_library0_end\":"<<compiled.clip(0)->end<<",\"lookup\":[";
 first=true;for(const auto&e:registration.entries()){if(!first)std::cout<<',';first=false;std::cout<<"["<<e.dictionary_id<<','<<e.engine_index<<']';}std::cout<<"]}\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
