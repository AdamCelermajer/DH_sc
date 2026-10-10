// Runs shipping GPU admission and topology synchronization against authored
// resources. Canonical Character allocation and GL calls are explicit host
// endpoints; PCLS, module selection, skinning, production draw/cache logic,
// uploaded vertices/materials, and creation draw routing are actual code.
#define main class_receiver_regression_main
#define buffer_subdata_v41 class_receiver_buffer_subdata_v41
#include "menu_preview_class_receivers_v1.cpp"
#undef buffer_subdata_v41
#undef main
#include "../../engine-skinning/visual_skin_owner_v6.hpp"
#include "../../engine-resources/cpu_vector_capacity_v41.hpp"
#include <cctype>
#include <map>
#include <set>

namespace dh2::android_resources {
std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39(){
 static auto budget=std::make_shared<resources::ContextResourceBudgetV37>();return budget;
}
}
namespace dh2::perf {struct State{std::size_t uploads{};};State state;}
struct AAssetManager {};
namespace dh2::android_ui {struct OriginalCacheAssetsV1 {explicit OriginalCacheAssetsV1(AAssetManager*){}};}
namespace {
struct HostVisual {dh2::scene::Scene scene;bool ready()const{return true;}std::uintptr_t root_identity()const{return reinterpret_cast<std::uintptr_t>(this);}unsigned scene_flags()const{return 1;}struct Bres{std::size_t size=3207072;};Bres bres()const{return {};}};
struct HostGear {std::shared_ptr<HostVisual> visual;std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> owner;bool ready()const{return bool(owner);}bool draw_views(const std::vector<dh2::skinning::VisualDrawViewV32>*& out,std::string& e){return owner->draw_views(out,e);}};
struct HostActor {struct Object{std::uintptr_t identity{};};std::shared_ptr<Object> object;std::shared_ptr<HostVisual> visual;std::uintptr_t source_visual()const{return reinterpret_cast<std::uintptr_t>(visual.get());}const std::string* source_string(unsigned)const{static const std::string uri="data/3D/characters/prince/prince_modular.bdae";return &uri;}};
struct HostVisualLink {std::shared_ptr<HostVisual> value;std::shared_ptr<HostVisual> visual(){return value;}};
struct HostDesign {const dh2::data::CharacterTable* value{};const dh2::data::CharacterTable* characters(){return value;}};
}
namespace dh2::world {
struct CanonicalCharacterCandidateRecordV60 {
 bool failed{},menu_preview_fresh_v122{};std::int32_t menu_preview_selected_slot_v122{-1};
 std::shared_ptr<HostActor> actor;std::shared_ptr<HostVisualLink> visual;std::shared_ptr<HostGear> equipment;HostGear* prepared_equipment_v60{};
 std::shared_ptr<data::PlayerSavegameV1> save;std::shared_ptr<data::PropertyState> properties;HostDesign design;
 struct Fields{std::int16_t* properties_id13c8{};}init_fields;std::int16_t property{};
};
struct RetainedVisualDrawV27 {bool bind(std::shared_ptr<HostVisual>,std::string&){throw std::runtime_error("Expected actual modular menu receiver");}};
}
namespace dh2::application {
struct ApplicationServicesOwnerV5 {
 struct Objects {std::vector<std::uintptr_t> ids;std::vector<std::uintptr_t> characters(){return ids;}};
 struct Process {std::shared_ptr<Objects> objects=std::make_shared<Objects>();std::shared_ptr<Objects> manager(){return objects;}};
 std::shared_ptr<Process> process=std::make_shared<Process>();std::uintptr_t identity(){return reinterpret_cast<std::uintptr_t>(this);}std::shared_ptr<Process> source_objects_v121(){return process;}
};
}
namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
using GLuint=unsigned;using GLsizei=int;
constexpr unsigned GL_ELEMENT_ARRAY_BUFFER=1,GL_DYNAMIC_DRAW=2,GL_STATIC_DRAW=3;
struct Draw {unsigned node{},vertices{},indices{},diffuse{},alpha{};int count{};bool environment{};Matrix placement{};dh2::scene::Material material;dh2::resources::CpuGeometryStorageV41<Vertex> cpu_geometry_v41;};
struct ModelPublicationScopeV50 {};
struct SourceActorGpuV64 {std::weak_ptr<Record> record;std::uintptr_t visual_identity{},root_identity{};bool modular{},menu_preview_trace_logged_v123{};std::unique_ptr<dh2::world::RetainedVisualDrawV27> retained;std::vector<Draw> batches;std::vector<unsigned> private_images;std::vector<dh2::skinning::VisualDrawViewV32> parts;std::vector<std::size_t> mapping;std::vector<std::uint8_t> visible;};
struct SourceGeometryGpuV64 {std::weak_ptr<void> world;AAssetManager* assets{};std::unique_ptr<dh2::android_ui::OriginalCacheAssetsV1> original;std::map<std::uintptr_t,SourceActorGpuV64> actors;std::map<std::string,unsigned> texture_cache;std::vector<unsigned> textures;};
struct Domain {AAssetManager* assets{};bool current(std::string&){return true;}};
std::map<std::uintptr_t,std::weak_ptr<Domain>> menu_renderer_domains_v121;
std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> host_application;
std::map<std::uintptr_t,std::shared_ptr<Record>> records;
SourceGeometryGpuV64 menu_preview_gpu_v123;
bool borrow_actual_application_services_v5(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& out,std::string&){out=host_application;return true;}
bool borrow_native_menu_preview_character_v122(const std::shared_ptr<Domain>&,std::uintptr_t id,std::shared_ptr<Record>& out,bool& matched,std::string&){auto at=records.find(id);matched=at!=records.end();out=matched?at->second:nullptr;return true;}
unsigned topology_uploads{},releases{},next_buffer=1;
unsigned pose_uploads{};
std::map<unsigned,std::vector<std::uint8_t>> uploaded;
void buffer_storage_v41(unsigned& id,unsigned,std::size_t n,const void* bytes,unsigned,dh2::resources::ResourceScopeV37){id=next_buffer++;auto begin=static_cast<const std::uint8_t*>(bytes);uploaded[id]={begin,begin+n};++topology_uploads;}
void buffer_subdata_v41(unsigned id,unsigned,std::size_t n,const void* bytes,dh2::resources::ResourceScopeV37){
 auto at=uploaded.find(id);check(at!=uploaded.end()&&at->second.size()==n,"Animated GPU update changed or lost its retained vertex buffer");
 auto begin=static_cast<const std::uint8_t*>(bytes);at->second.assign(begin,begin+n);++pose_uploads;
}
void release(std::vector<Draw>& draws,std::vector<unsigned>& images){for(const auto& draw:draws){uploaded.erase(draw.vertices);uploaded.erase(draw.indices);}draws.clear();images.clear();}
void release_source_actor_gpu_v64(SourceActorGpuV64& actor,bool){release(actor.batches,actor.private_images);actor={};++releases;}
void release_native_menu_geometry_v123(bool){for(auto& p:menu_preview_gpu_v123.actors)release_source_actor_gpu_v64(p.second,false);menu_preview_gpu_v123={};}
unsigned upload(AAssetManager*,const std::string& name,std::map<std::string,unsigned>& cache,std::vector<unsigned>& images,dh2::android_ui::OriginalCacheAssetsV1*,dh2::resources::ResourceScopeV37){auto at=cache.emplace(name,unsigned(cache.size()+1));if(at.second)images.push_back(at.first->second);return at.first->second;}
void check(const char*){}
#include "menu_preview_gpu_sync_under_test.inc"
std::vector<std::string> submitted_materials;
std::uint64_t submitted_geometry;
void submit_geometry_batch_v64(const Draw& draw,const Matrix&,const Draw*& previous){
 previous=&draw;submitted_materials.push_back(draw.material.diffuse);
 check(uploaded.at(draw.vertices).size()==draw.cpu_geometry_v41.vertices.size()*sizeof(Vertex),"Shipping GPU used a stale vertex buffer");
 for(auto byte:uploaded.at(draw.vertices)){submitted_geometry^=byte;submitted_geometry*=1099511628211ULL;}
 for(auto byte:uploaded.at(draw.indices)){submitted_geometry^=byte;submitted_geometry*=1099511628211ULL;}
}
void sync_source_retained_frames_v68(SourceGeometryGpuV64&,SourceActorGpuV64&,std::shared_ptr<HostVisual>){throw std::runtime_error("Unexpected non-modular menu path");}
#include "menu_preview_gpu_draw_under_test.inc"
dh2::skinning::VisualAssetResultV6 asset(void* raw,const char* uri,Raw& bytes,std::string&){
 auto& root=*static_cast<std::string*>(raw);std::string file(uri);file=file.substr(file.find_last_of('/')+1);for(auto& c:file)c=char(std::tolower(static_cast<unsigned char>(c)));bytes=read(root,"models/"+file);return dh2::skinning::VisualAssetResultV6::found;
}
std::shared_ptr<Record> receiver(Assets& assets,unsigned index,std::uintptr_t identity_value,bool fresh=false){
 std::string error;auto result=std::make_shared<Record>();const auto& definition=assets.definitions[index];
 result->menu_preview_selected_slot_v122=int(index);result->menu_preview_fresh_v122=fresh;
 result->save=std::make_shared<dh2::data::PlayerSavegameV1>();result->save->set_slot(fresh?-1:int(index));
 dh2::data::FreshPlayerProfileV1 profile;check(dh2::data::fresh_player_profile_v1(assets.characters,definition.character.c_str(),"Selection",1,2,profile,error),error);
 dh2::data::PlayerProfileIndexV1 sections;check(sections.load(bytes(profile.bytes),error),error);std::size_t used{};
 check(result->save->load_class(sections.borrow().payload("PCLS"),assets.characters.names,used,error),error);
 result->property=std::int16_t(result->save->class_id());result->init_fields.properties_id13c8=&result->property;
 result->properties=std::make_shared<dh2::data::PropertyState>(definition.properties);result->design.value=&assets.characters;
 result->actor=std::make_shared<HostActor>();result->actor->object=std::make_shared<HostActor::Object>();result->actor->object->identity=identity_value;
 dh2::skinning::VisualSkinResourcesV6 resource;check(resource.load(read(assets.root,"models/prince_modular.bdae"),error),error);
 auto visual=std::make_shared<HostVisual>();visual->scene=resource.borrow().factory_scene();result->actor->visual=visual;
 result->visual=std::make_shared<HostVisualLink>();result->visual->value=visual;
 result->equipment=std::make_shared<HostGear>();result->equipment->visual=visual;
 auto& owner=result->equipment->owner;owner=std::make_unique<dh2::skinning::VisualSkinOwnerV6>(resource.borrow(),visual->scene,dh2::skinning::VisualAssetServicesV6{&assets.root,asset});check(owner->initialize(error),error);
 for(const auto& item:definition.starting_items){const auto separator=item.module.find('_',3);const auto category=separator==std::string::npos?-1:owner->category_id(item.module.substr(0,separator).c_str());
  if(category>=0){const auto module=owner->module_id(category,item.module.c_str());check(module>=0&&owner->set_modular(category,module,error),error);}
  else if(item.module.find("MC_RWeapon_")==0)check(owner->set_weapon(item.module.c_str(),1,1,error),error);
 }
 return result;
}
std::uint64_t render(std::shared_ptr<Record> record){
 const auto id=record->actor->object->identity;records.clear();records[id]=record;host_application->process->objects->ids={id};submitted_materials.clear();submitted_geometry=14695981039346656037ULL;draw_native_menu_character_v123(identity());return submitted_geometry;
}
}
int main(int argc,char** argv){try{
 check(class_receiver_regression_main(argc,argv)==0,"Existing saved/creation authored receiver regression failed");
 Assets assets(argv[1]);host_application=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();AAssetManager manager;auto domain=std::make_shared<Domain>();domain->assets=&manager;menu_renderer_domains_v121[host_application->identity()]=domain;
 auto warrior=receiver(assets,0,0x100000001ULL),mage=receiver(assets,2,0x100000002ULL);
 const auto warrior_digest=render(warrior);check(!submitted_materials.empty(),"Warrior GPU draw absent");auto warrior_materials=submitted_materials;
 const auto before=topology_uploads;const auto mage_digest=render(mage);
 check(mage_digest!=warrior_digest&&submitted_materials!=warrior_materials&&topology_uploads>before,"Production GPU submitted unchanged Warrior geometry/materials for Mage");
 check(std::any_of(submitted_materials.begin(),submitted_materials.end(),[](const auto& uri){return uri.find("mage")!=std::string::npos;}),"Mage GPU draw did not submit authored Mage atlas");
 check(menu_preview_gpu_v123.actors.size()==1&&menu_preview_gpu_v123.actors.count(0x100000002ULL),"Previous saved Warrior cache survived Mage switch");
 const auto stable_uploads=topology_uploads;check(render(mage)==mage_digest&&topology_uploads==stable_uploads,"Unchanged Mage needlessly rebuilt or changed topology");
 // Object addresses may be reused after Flush; the record and visual identities
 // must invalidate GPU topology even if the numeric Character address matches.
 auto reused=receiver(assets,0,0x100000002ULL);check(render(reused)==warrior_digest&&topology_uploads>stable_uploads,"Reused Character identity retained Mage GPU topology");
 auto empty=receiver(assets,0,0x100000003ULL,true);render(empty);check(submitted_materials.empty()&&menu_preview_gpu_v123.actors.empty(),"Empty slot retained saved/default avatar GPU draw");
 check(render(mage)==mage_digest&&menu_preview_gpu_v123.actors.size()==1,"Mage not restored after Empty slot");
 // Advance each actual authored menu idle clip on the SAME live modular
 // scene, then execute shipping pose synchronization and final GPU draw.
 // The process Timer/CharAnimator remains a device integration boundary.
 for(const auto& entry:{std::pair<unsigned,std::shared_ptr<Record>>{0,warrior},{2,mage}}){
  const auto& definition=assets.definitions[entry.first];auto& scene=entry.second->actor->visual->scene;
  const auto raw=read(assets.root,"animations/"+definition.idle_clip.substr(definition.idle_clip.find_last_of('/')+1));
  dh2::animation::Player idle;std::string error;
  check(idle.load(raw.data(),raw.size(),scene,error),error);
  check(idle.track_count()>0&&idle.end>idle.start,"Authored menu breathing clip lacks a moving timeline");
  check(idle.sample(scene,idle.start,error),error);const auto first_pose=render(entry.second);
  const auto topology_before=topology_uploads,poses_before=pose_uploads;
  check(idle.sample(scene,idle.start+(idle.end-idle.start)/3,error),error);const auto next_pose=render(entry.second);
  check(next_pose!=first_pose&&pose_uploads>poses_before,"Authored menu breathing did not change final submitted GPU vertices");
  check(topology_uploads==topology_before,"Breathing needlessly rebuilt menu GPU topology");
  const auto stable_poses=pose_uploads;
  check(render(entry.second)==next_pose&&pose_uploads==stable_poses,"Unchanged breathing sample changed or reuploaded GPU vertices");
 }
 release_native_menu_geometry_v123(false);records.clear();warrior.reset();mage.reset();reused.reset();empty.reset();check(uploaded.empty(),"Retired menu GPU buffers survived cleanup");
 std::cout<<"PASS production final GPU selection Warrior->Mage->reused Warrior->Empty(no avatar)->Mage; authored vertices/materials, stale-cache retirement, creation class routing and Warrior/Mage breathing vertex updates; checks "<<checks<<"\n";
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
