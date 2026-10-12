#include "../character_npc_body.hpp"
#include "../physical_world.hpp"
#include "../../game-data/animation_bank.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <cstring>
#include <iomanip>
#include <dlfcn.h>
using namespace dh2::physical;
using Raw=std::vector<std::uint8_t>;
static unsigned checks=0,guards=0;
static void require(bool v,unsigned line){++checks;if(!v)throw std::runtime_error("NPC body line "+std::to_string(line)+" check "+std::to_string(checks));}
#define check(value) require((value),__LINE__)
static Raw file(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
static dh2::data::Bytes bytes(const Raw& v){return {v.data(),v.size()};}
template<class T>static void write(std::ostream& s,const T& v){s.write(reinterpret_cast<const char*>(&v),sizeof(v));}
static void raw(std::ostream& s,const void* p,std::size_t n){s.write(static_cast<const char*>(p),n);}
static std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
static std::string origin(void* p){Dl_info d{};check(dladdr(p,&d));return d.dli_fname;}
struct Placement {unsigned room;std::string name,row;float position[3],rotation[3];};
static std::vector<Placement> placements(const Raw& b){check(b.size()>=16&&!std::memcmp(b.data(),"DACT\1\0\0\0",8)&&b.size()==16+word(b.data()+8)*256);std::vector<Placement> out;
 for(unsigned i=0;i<word(b.data()+8);++i){auto* p=b.data()+16+256*i;if(word(p)!=1)continue;Placement v;v.room=word(p+4);v.name=reinterpret_cast<const char*>(p+8);v.row=reinterpret_cast<const char*>(p+72);std::memcpy(v.position,p+200,12);std::memcpy(v.rotation,p+212,12);out.push_back(v);}return out;}
int main(int argc,char** argv){try{
 check(argc==4);std::string root=argv[1],error;std::ofstream corpus(argv[2],std::ios::binary);check(bool(corpus));std::ofstream observations(argv[3]);check(bool(observations));
 const auto data=root+"/data/";auto cr=file(data+"character_properties_pyarray.bin"),cn=file(data+"character_properties_pyarraynames.bin"),cs=file(data+"character_properties_pystructnames.bin");
 dh2::data::CharacterTable characters;check(dh2::data::load_characters(bytes(cr),bytes(cn),bytes(cs),characters,error));dh2::data::PropertyRules rules;check(dh2::data::load_property_rules(characters,rules,error));
 auto ar=file(data+"ai_pyarray.bin"),an=file(data+"ai_pyarraynames.bin"),as=file(data+"ai_pystructnames.bin"),fr=file(data+"ai_factions_pyarray.bin"),fn=file(data+"ai_factions_pyarraynames.bin"),fs=file(data+"ai_factions_pystructnames.bin");dh2::data::AiTables ai;check(dh2::data::load_ai(bytes(ar),bytes(an),bytes(as),bytes(fr),bytes(fn),bytes(fs),ai,error));
 auto lr=file(data+"character_classes_pyarray.bin"),ln=file(data+"character_classes_pyarraynames.bin"),ls=file(data+"character_classes_pystructnames.bin");dh2::data::ClassTables classes;check(dh2::data::load_classes(bytes(lr),bytes(ln),bytes(ls),classes,error));
 std::array<CharacterNpcBodyModel,3> models;const char* names[]={"skeleton.bdae","slime_green_v2.bdae","ghost.bdae"};unsigned markers=0,skins=0;std::array<unsigned,3> nodes{},instances{},controllers{};
 for(unsigned i=0;i<3;++i){auto source=file(root+"/actors/"+names[i]);check(models[i].initialize(bytes(source),nullptr,error));auto* scene=models[i].complete_scene();nodes[i]=scene->graph.size();instances[i]=scene->instances.size();for(const auto& instance:scene->instances)controllers[i]+=instance.controller>=0;
  check(models[i].marker()->found==(i!=1));if(i==1){check(models[i].entries()->size()==1&&models[i].entries()->at(0).skinned==1);++skins;}else{check(models[i].entries()->empty());++markers;}
  auto* retained=scene;check(!models[i].initialize({nullptr,5},nullptr,error)&&models[i].complete_scene()==retained);++guards;
  std::fill(source.begin(),source.end(),0);check(models[i].ready());
 }
 auto actorbytes=file(root+"/worlds/crypt01.dact");auto placed=placements(actorbytes);check(placed.size()==11);write(corpus,std::uint32_t(0x3142504e));write(corpus,std::uint32_t(placed.size()));
 NativeWorld world;const float world_bounds[]={-300,-300,300,300};world.load(world_bounds);std::vector<b2Body*> bodies;std::array<WorldObject,11> world_owners{};
 unsigned marker_cases=0,skin_cases=0,full_config_checks=0;observations<<"{\"records\":[";
 for(unsigned i=0;i<placed.size();++i){auto& p=placed[i];auto row=std::find(characters.names.begin(),characters.names.end(),p.row);check(row!=characters.names.end());auto idx=unsigned(row-characters.names.begin());const unsigned model=p.row=="Crypt_Skeleton"?0:p.row=="Crypt_Ghost"?2:1;
  dh2::data::PropertyState props;dh2::data::reset_properties(rules,props,&characters.rows[idx]);check(dh2::data::recalc_properties_with_class(classes,rules,props,error));auto view=dh2::data::property_view(rules,props);
  unsigned identity=0,physical_identity=0;NpcBodyRequest req;req.owner=&identity;req.new_physical=&physical_identity;req.properties=&view;req.ai=&ai;std::copy(p.position,p.position+3,req.position);std::copy(p.rotation,p.rotation+3,req.rotation_degrees);
  NpcBodyProjection out{};check(models[model].project(req,out,error));check(out.character_type==4&&out.is_player==0&&out.ai_id==(model==2?68:40)&&out.collision_scale==85);check(out.base_scale[0]==100&&out.base_scale[1]==100&&out.base_scale[2]==100);check(out.visual.effective_scale[0]==.9f&&out.visual.effective_scale[1]==.9f&&out.visual.effective_scale[2]==1);check(out.body.enabled&&out.body.po_character&&out.body.pinned&&!out.body.body.bullet);check(out.body.shape.group_index==2&&out.body.shape.category_bits==0x10&&out.body.shape.mask_bits==0xd3f&&out.body.shape.kind==0&&out.body.radius==out.body.shape.radius&&out.body.radius>0&&out.body.radius!=.36f);++full_config_checks;
  marker_cases+=out.marker;skin_cases+=!out.marker;
  auto* scene=models[model].complete_scene();auto* marker=models[model].marker();auto* entries=models[model].entries();
  write(corpus,std::uint32_t(i));write(corpus,std::int32_t(view.resolved[1]));write(corpus,out.collision_scale);raw(corpus,out.base_scale,12);write(corpus,out.marker);raw(corpus,p.position,12);raw(corpus,p.rotation,12);raw(corpus,marker->bounds,24);raw(corpus,marker->parent_scale,12);write(corpus,std::uint32_t(entries->size()));if(!entries->empty())raw(corpus,entries->data(),entries->size()*sizeof(CharacterMeshEntry));
  // Actual skin matrices/boxes are retained as explicit source skin providers.
  const auto skin_count=std::count_if(scene->instances.begin(),scene->instances.end(),[](const auto& v){return v.controller>=0;});write(corpus,std::uint32_t(out.marker?0:skin_count));
  if(!out.marker)for(const auto& inst:scene->instances)if(inst.controller>=0){auto source=file(root+"/actors/"+names[model]);dh2::resources::BresView resource{};check(dh2_bres_open(&resource,source.data(),source.size())==dh2::resources::BresError::ok);dh2::skinning::Skin skin;check(dh2::skinning::load(resource,inst.controller,*scene,skin,error));auto* item=dh2_bres_library_item(&resource,dh2::resources::Library::controller,inst.controller);check(item);auto offset=word(item+8);auto box_count=word(source.data()+offset+140),box_offset=word(source.data()+offset+144);write(corpus,std::uint32_t(skin.nodes.size()));write(corpus,box_count);for(auto n:skin.nodes){dh2::math::Matrix4f m{};std::copy(scene->graph[n].world.begin(),scene->graph[n].world.end(),m.m);write(corpus,m);}if(box_count)raw(corpus,source.data()+box_offset,24*box_count);}
  write(corpus,reinterpret_cast<std::uintptr_t>(req.owner));write(corpus,reinterpret_cast<std::uintptr_t>(req.new_physical));write(corpus,out.visual);write(corpus,out.bounds);write(corpus,out.body);
  auto& wo=world_owners[i];auto* body=world.create_character(out.body,&wo);check(body&&body->GetMass()==0&&body->IsStatic()&&body->GetShapeList());auto* shape=body->GetShapeList();auto filter=shape->GetFilterData();check(filter.groupIndex==2&&filter.categoryBits==0x10&&filter.maskBits==0xd3f);check(body->GetPosition().x==out.body.body.position[0]&&body->GetPosition().y==out.body.body.position[1]);check(static_cast<b2CircleShape*>(shape)->GetRadius()==out.body.radius);bodies.push_back(body);
  // Output guard and actual mutable cache/input lifetime.
  auto prior=out;req.reserved=1;check(!models[model].project(req,out,error)&&!std::memcmp(&prior,&out,sizeof out));++guards;req.reserved=0;req.ai=nullptr;check(!models[model].project(req,out,error)&&!std::memcmp(&prior,&out,sizeof out));++guards;req.ai=&ai;
  const auto old=props.resolved[1];props.resolved[1]=44;check(!models[model].project(req,out,error)&&!std::memcmp(&prior,&out,sizeof out));++guards;props.resolved[1]=old;
  if(i)observations<<',';observations<<"{\"room\":"<<p.room<<",\"name\":\""<<p.name<<"\",\"character\":\""<<p.row<<"\",\"model\":\""<<names[model]<<"\",\"marker\":"<<out.marker<<",\"ai\":"<<out.ai_id<<",\"radius\":"<<std::setprecision(9)<<out.body.radius<<",\"relative_bounds\":[";for(unsigned k=0;k<6;++k){if(k)observations<<',';observations<<out.bounds.relative_box[k];}observations<<"]}";
 }
 observations<<"],\"models\":[";for(unsigned i=0;i<3;++i){if(i)observations<<',';observations<<"{\"name\":\""<<names[i]<<"\",\"nodes\":"<<nodes[i]<<",\"instances\":"<<instances[i]<<",\"controllers\":"<<controllers[i]<<"}";}observations<<"]}\n";
 // Pinned source startup bodies remain static; no collision/test providers are
 // invoked or asserted successful. Movement/unpin is a separate service.
 world.update(16);for(auto*& body:bodies)world.destroy(body);check(world.backend()->GetBodyCount()==1);check(marker_cases==6&&skin_cases==5&&markers==2&&skins==1&&full_config_checks==11);
 corpus.close();observations.close();check(bool(corpus)&&bool(observations));
 std::cout<<"{\"validation\":\"PASS\",\"placements\":11,\"character_kinds\":4,\"model_resources\":3,\"marker_cases\":"<<marker_cases<<",\"skin_cases\":"<<skin_cases<<",\"source_body_configs\":"<<full_config_checks<<",\"genuine_Box2D_bodies_created_and_destroyed\":11,\"pinned_world_steps\":1,\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0,\"world_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_mesh_box))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(&dh2_property_resolve))<<"\"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
