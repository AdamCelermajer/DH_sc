// Asset-backed receiver regression for the saved-slot and creation previews.
// The UI adapter and source PCLS/property readers are real; slot allocation,
// GL upload and the final draw recorder are declared host endpoints.
#include "../app/src/main/cpp/front_inspection_avatar_services_v1.hpp"
#include "../../game-data/class_preview_setup.hpp"
#include "../../game-data/fresh_player_profile_v1.hpp"
#include "../../game-data/menu_profile_metadata_v1.hpp"
#include "../../level-world/character_props_id_owner_v1.hpp"
#include "../../level-world/objects.hpp"
#include "../../engine-resources/resource_budget_v37.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {
using Raw=std::vector<std::uint8_t>;
using namespace dh2;
unsigned checks;
void check(bool value,const std::string& why){++checks;if(!value)throw std::runtime_error(why);}
Raw read(const std::string& root,const std::string& name){
 std::ifstream file(root+"/"+name,std::ios::binary);
 if(!file)throw std::runtime_error("Missing packaged fixture: "+name);
 return {std::istreambuf_iterator<char>(file),{}};
}
data::Bytes bytes(const Raw& raw){return {raw.data(),raw.size()};}
struct Assets {
 data::CharacterTable characters;data::ClassTables classes;data::PropertyRules rules;
 data::LootTablesV2 loot;data::AnimationTables animations;data::Dictionary clips;
 std::array<data::ClassPreviewDefinition,3> definitions;
 std::string root;
 explicit Assets(std::string path):root(std::move(path)){
  std::string error;
  auto group=[&](const char* name){std::array<Raw,3> out;unsigned i{};
   for(const char* suffix:{"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"})out[i++]=read(root,std::string("data/")+name+suffix);
   return out;};
  auto raw=group("character_properties");check(data::load_characters(bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),characters,error),error);
  raw=group("character_classes");check(data::load_classes(bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),classes,error),error);
  check(data::load_property_rules(characters,rules,error),error);
  raw=group("loot_table");check(loot.load(bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),error),error);
  auto dictionary=read(root,"data/animations_dictionary_pyarray.bin"),names=read(root,"data/animations_dictionary_pyarraynames.bin");
  check(data::load_dictionary(bytes(names),bytes(dictionary),clips,error),error);
  raw=group("animations");check(data::load_animation_tables(bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),clips,animations,error),error);
  check(data::class_preview_definitions(characters,classes,rules,loot.borrow(),animations,clips,definitions,error),error);
 }
 std::shared_ptr<objects::Resource> receiver(unsigned index){
  check(index<definitions.size(),"Class index outside authored preview array");
  const auto& definition=definitions[index];std::vector<std::string> modules{"MC_Head__naked-mesh-skin"};
  for(const auto& item:definition.starting_items)
   if(item.module.find("MC_Torso_")==0||item.module.find("MC_Feet_")==0||item.module.find("MC_Hands_")==0)modules.push_back(item.module+"-mesh-skin");
  check(modules.size()==4,"Starting armor must deliver four source controllers");
  auto model=read(root,"models/prince_modular.bdae");
  auto animation=read(root,"animations/"+definition.idle_clip.substr(definition.idle_clip.find_last_of('/')+1));
  auto result=std::make_shared<objects::Resource>();std::string error;
  check(objects::load_modular_resource(model.data(),model.size(),modules,animation.data(),animation.size(),*result,error),error);
  return result;
 }
};
struct SavedChooser {
 Assets& assets;std::array<Raw,4> profiles;std::shared_ptr<objects::Resource> drawn;
 std::int32_t drawn_slot{-1},drawn_class{-1};unsigned inspection_calls{};
 static bool destroy(void* raw,std::string&){auto& self=*static_cast<SavedChooser*>(raw);self.drawn.reset();self.drawn_slot=self.drawn_class=-1;return true;}
 static bool setup(void* raw,std::int32_t slot,std::string& error){
  auto& self=*static_cast<SavedChooser*>(raw);if(slot<0)return true;
  data::MenuProfileMetadataV1 metadata;data::MenuProfileMetadataServicesV1 services;
  services.store_selected_difficulty=[](void*,auto,std::string&){return true;};
  if(!data::load_menu_profile_metadata_v1(bytes(self.profiles.at(slot)),self.assets.characters,slot,0,services,metadata,error))return false;
  data::PlayerProfileIndexV1 profile;if(!profile.load(bytes(self.profiles.at(slot)),error))return false;
  data::PlayerSavegameV1 save;save.set_slot(slot);std::int16_t cached=-1;data::LootRandom8V2 random{};
  struct PropsRead {Assets& assets;data::PlayerProfileIndexV1& profile;data::PlayerSavegameV1& save;};
  PropsRead read{self.assets,profile,save};
  character::CharacterPropsIdServicesV1 props;props.context=&read;
  props.is_player=[](void*,bool& value,std::string&){value=true;return true;};
  props.load_save=[](void* raw,data::PlayerSavegameV1& same,std::int32_t mask,std::string& e){
   auto& read=*static_cast<PropsRead*>(raw);
   check(&same==&read.save&&mask==1,"SafeGetCharPropsId must read SAME selected PCLS");
   std::size_t used{};return same.load_class(read.profile.borrow().payload("PCLS"),read.assets.characters.names,used,e);};
  std::int32_t selected{};
  if(!character::character_safe_props_id_v1(cached,"","",self.assets.characters,&save,random,props,selected,error))return false;
  check(selected==metadata.character_row,"Menu label and Character13c8 must consume SAME PCLS");
  for(unsigned i=0;i<self.assets.definitions.size();++i)if(self.assets.definitions[i].row==selected){
   self.drawn=self.assets.receiver(i);self.drawn_slot=slot;self.drawn_class=selected;return true;}
  error="Selected PCLS has no authored class receiver";return false;
 }
 static bool camera(void*,std::string&){return true;}
 ui::MenuAvatarPreviewServicesV1 services(){return {this,destroy,setup,camera};}
};
void expected(const objects::Resource& resource,const std::array<int,4>& controllers,const char* class_name){
 check(resource.scene.instances.size()==controllers.size(),std::string(class_name)+" receiver controller count");
 for(unsigned i=0;i<controllers.size();++i)check(resource.scene.instances[i].controller==controllers[i],std::string(class_name)+" wrong authored controller");
 check(!resource.primitives.empty()&&resource.animation.track_count()>0,std::string(class_name)+" empty model/animation receiver");
}
using Matrix=std::array<float,16>;
using Vertex=objects::Vertex;
Matrix identity(){return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};}
struct RecordedDraw {unsigned vertices{},node{},class_index{};};
struct RecordedWeapon {objects::Resource resource;std::vector<RecordedDraw> draws;unsigned anchor{};};
struct RecordedActor {objects::Resource& resource;std::vector<RecordedDraw> draws;std::vector<RecordedWeapon> weapons;unsigned anchor{};float scale[3]{1,1,1};std::uint64_t elapsed{},sample_elapsed{};explicit RecordedActor(objects::Resource& actual):resource(actual){}};
std::vector<RecordedActor> class_preview_actors;
scene::Scene current_scene;
bool class_scene{},menu_background{};
int menu_persona_class=-1;
int class_selected=-1,class_clip_start{},class_clip_end{},class_clip_cursor{},sampled_ms{};
struct ClassClip {std::string name;int start{},end{};};
std::vector<ClassClip> class_clips;
std::string class_clip_name;
std::chrono::steady_clock::time_point menu_persona_epoch;
constexpr unsigned ANDROID_LOG_INFO=1;
int __android_log_print(unsigned,const char*,const char*,...){return 0;}
bool class_scene_active(){return class_scene;}
// The runner copies this one function verbatim from the shipping renderer
// include; its actual clip/index/cursor stores run against the authored BRES.
#include "menu_preview_class_transition_under_test.inc"
constexpr unsigned GL_ARRAY_BUFFER=0;
std::size_t buffer_bytes_v41(std::size_t count,std::size_t stride){return count*stride;}
void buffer_subdata_v41(unsigned,unsigned,std::size_t,const void*,resources::ResourceScopeV37){}
std::vector<unsigned> submitted_classes;
void draw_creation(){
 const auto projection=identity();
 auto submit=[](const RecordedDraw& draw,const Matrix&){submitted_classes.push_back(draw.class_index);};
#include "../app/src/main/cpp/renderer_front_draw_v87.inc"
}
}
int main(int argc,char** argv){try{
 check(argc==2,"Pass packaged assets directory");Assets assets(argv[1]);std::string error;
 SavedChooser chooser{assets,{},{}};
 for(const auto entry:{std::pair<unsigned,const char*>{0,"KnightPlayerBase"},{2,"MagePlayerBase"}}){
  dh2::data::FreshPlayerProfileV1 profile;
  check(dh2::data::fresh_player_profile_v1(assets.characters,entry.second,entry.first==2?"GALCHOU":"Warrior",1,2,profile,error),error);
  chooser.profiles[entry.first]=std::move(profile.bytes);
 }
 dh2::ui::MenuAvatarPreviewStateV1 state;auto services=chooser.services();auto process_owner=std::make_shared<int>(1);
 auto inspection=services;inspection.setup_character=[](void* raw,auto,std::string&){++static_cast<SavedChooser*>(raw)->inspection_calls;return true;};
 check(dh2::ui::change_menu_avatar_preview_v1(state,0,false,services,error),error);
 auto warrior=chooser.drawn;expected(*warrior,{123,171,42,85},"Warrior chooser");
 // Metadata reload, persisted persona marker and creation's enable-persona
 // attempt must all keep the process adapter used by the final draw owner.
 for(unsigned i=0;i<3;++i)check(!dh2::android_ui::install_front_inspection_avatar_services_v1(services,process_owner,inspection),"Inspection replaced process adapter");
 check(dh2::ui::change_menu_avatar_preview_v1(state,2,false,services,error),error);
 auto mage=chooser.drawn;expected(*mage,{123,169,40,83},"Mage chooser");
 check(state.slot==2&&chooser.drawn_slot==2&&chooser.drawn_class==290&&mage!=warrior&&!chooser.inspection_calls,"GALCHOU slot2 retained Warrior draw receiver");
 check(dh2::ui::change_menu_avatar_preview_v1(state,0,false,services,error),error);
 expected(*chooser.drawn,{123,171,42,85},"Warrior chooser return");
 // Creation keeps all three actors, selecting their authored anchor/camera;
 // selected class 2 must retain Mage armor and staff rather than actor0's gear.
 std::array<std::shared_ptr<dh2::objects::Resource>,3> created;
 for(unsigned i=0;i<created.size();++i)created[i]=assets.receiver(i);
 const auto scene_bytes=read(assets.root,"models/class_selection.bdae");resources::BresView scene_bres;
 check(dh2_bres_open(&scene_bres,scene_bytes.data(),scene_bytes.size())==resources::BresError::ok,"Packaged creation BRES");
 check(scene::load(scene_bres,current_scene,error),error);
 auto word=[](const std::uint8_t* p){return unsigned(p[0])|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);};
 for(unsigned i=0;i<dh2_bres_library_count(&scene_bres,resources::Library::animation_clip);++i){
  const auto* clip=dh2_bres_library_item(&scene_bres,resources::Library::animation_clip,i);
  const auto name=word(clip);check(name<scene_bytes.size(),"Authored class clip name offset");
  class_clips.push_back({reinterpret_cast<const char*>(scene_bytes.data()+name),int(word(clip+4)),int(word(clip+8))});
 }
 const char* anchors[]={"dummy_Warrior-node","dummy_Rogue-node","dummy_Mage-node"};
 for(unsigned i=0;i<created.size();++i){
  const auto anchor=std::find_if(current_scene.graph.begin(),current_scene.graph.end(),[&](const scene::Node& n){return n.id==anchors[i];});
  check(anchor!=current_scene.graph.end(),std::string("Missing authored creation anchor: ")+anchors[i]);
  RecordedActor actor(*created[i]);actor.anchor=unsigned(anchor-current_scene.graph.begin());
  for(const auto& primitive:actor.resource.primitives)actor.draws.push_back({0,primitive.node,i});
  class_preview_actors.push_back(std::move(actor));
 }
 class_scene=true;
 for(unsigned index:{0u,1u,2u,1u,0u,1u,2u}){
  check(select_class_scene(int(index),100000,error),error);
  check(select_class_scene(int(index),0,error),error);
  check(class_selected==int(index)&&class_clip_name=="lol_"+std::to_string(index+1)+"_idle","Creation transition retained the previous class/camera clip");
  check(created[index]!=created[(index+1)%3],"Creation selected another class's model receiver");
  if(index==0)expected(*created[index],{123,171,42,85},"Warrior creation");
  if(index==2)expected(*created[index],{123,169,40,83},"Mage creation");
  submitted_classes.clear();draw_creation();
  for(unsigned i=0;i<created.size();++i)check(std::count(submitted_classes.begin(),submitted_classes.end(),i)==static_cast<std::ptrdiff_t>(created[i]->primitives.size()),"Production creation draw substituted or omitted a class receiver");
 }
 check(assets.definitions[0].starting_items[3].module=="MC_RWeapon_Longsword_01"&&assets.definitions[2].starting_items[3].module=="MC_RWeapon_Quarterstaff_01","Mage and Warrior use the same starting weapon");
 std::cout<<"PASS saved chooser slot0->GALCHOU/Mage slot2->slot0 and creation Warrior/Mage transitions; distinct actual modular receivers/controllers and longsword/staff; checks "<<checks<<"\n";
 return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
