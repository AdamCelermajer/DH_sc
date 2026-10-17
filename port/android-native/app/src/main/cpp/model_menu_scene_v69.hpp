#pragma once

// Adam791e961 verified-v69 scene rendering, included once inside the existing
// renderer namespace. It shares that renderer's GPU/material/scene authority.
bool menu_background=false,class_scene=false;
int class_camera_node=-1,class_selected=-1,class_clip_start=0,class_clip_end=0,class_clip_cursor=0;
struct ClassClip {std::string name;int start,end;};std::vector<ClassClip> class_clips;
std::string class_clip_name;
unsigned class_pane_frames=0;
struct ClassPreviewActor {
 dh2::objects::Resource resource;
 std::vector<Draw> draws;
 std::vector<GLuint> images;
 unsigned anchor=0;
 float scale[3]{1,1,1};
 std::uint64_t elapsed=0,sample_elapsed=0;
};
std::vector<ClassPreviewActor> class_preview_actors;
void release_class_previews(){
 for(auto& actor:class_preview_actors)release(actor.draws,actor.images);
 class_preview_actors.clear();
}
std::array<dh2::data::ClassPreviewDefinition,3> class_preview_definitions;
extern AAssetManager* menu_preview_assets;
void load_class_preview_definitions(AAssetManager* assets){
 const char* groups[]={"character_properties","character_classes","loot_table","animations"};
 const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
 std::array<std::array<std::vector<std::uint8_t>,3>,4> raw;
 for(unsigned i=0;i<4;++i)for(unsigned j=0;j<3;++j)raw[i][j]=read(assets,std::string(groups[i])+suffixes[j],"data");
 auto bytes=[&](unsigned i,unsigned j){return dh2::data::Bytes{raw[i][j].data(),raw[i][j].size()};};
 dh2::data::CharacterTable characters;dh2::data::ClassTables classes;dh2::data::PropertyRules rules;
 dh2::data::LootTablesV2 loot;dh2::data::AnimationTables animations;dh2::data::Dictionary clips;std::string error;
 auto dictionary=read(assets,"animations_dictionary_pyarray.bin","data"),names=read(assets,"animations_dictionary_pyarraynames.bin","data");
 if(!dh2::data::load_characters(bytes(0,0),bytes(0,1),bytes(0,2),characters,error)||
    !dh2::data::load_classes(bytes(1,0),bytes(1,1),bytes(1,2),classes,error)||
    !dh2::data::load_property_rules(characters,rules,error)||
    !loot.load(bytes(2,0),bytes(2,1),bytes(2,2),error)||
    !dh2::data::load_dictionary({names.data(),names.size()},{dictionary.data(),dictionary.size()},clips,error)||
    !dh2::data::load_animation_tables(bytes(3,0),bytes(3,1),bytes(3,2),clips,animations,error)||
    !dh2::data::class_preview_definitions(characters,classes,rules,loot.borrow(),animations,clips,class_preview_definitions,error))
  throw std::runtime_error("Original class preview definitions rejected: "+error);
 for(const auto& value:class_preview_definitions)
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original class preview definition | %s | row %d | loot %d | anim table %d | starting entries %zu | idle %s | inventory and rendering pending",
   value.character.c_str(),value.row,value.loot,value.animation_table,value.starting_items.size(),value.idle_clip.c_str());
}
void load_class_preview_actors(AAssetManager* assets){
 std::vector<ClassPreviewActor> next;
 try{
  const auto model=read(assets,"prince_modular.bdae","models");
  const char* anchors[]={"dummy_Warrior-node","dummy_Rogue-node","dummy_Mage-node"};
  for(unsigned i=0;i<class_preview_definitions.size();++i){
   const auto& definition=class_preview_definitions[i];next.emplace_back();auto& actor=next.back();
   const auto anchor=std::find_if(current_scene.graph.begin(),current_scene.graph.end(),[&](const dh2::scene::Node& n){return n.id==anchors[i];});
   if(anchor==current_scene.graph.end())throw std::runtime_error(std::string("Original class anchor absent: ")+anchors[i]);
   actor.anchor=unsigned(anchor-current_scene.graph.begin());
   if(dh2_character_visual_scale(actor.scale,definition.properties.base.data()+12))throw std::runtime_error("Class preview visual scale rejected");
   // INV_UpdateSkin resolves empty head as category + __naked, with
   // __placeholder only if that exact module is absent. All three starting
   // loots have no helmet. Other armor names come from the original items.
   std::vector<std::string> modules{"MC_Head__naked-mesh-skin"};
   for(const auto& item:definition.starting_items)
    if(item.module.find("MC_Torso_")==0||item.module.find("MC_Feet_")==0||item.module.find("MC_Hands_")==0)
     modules.push_back(item.module+"-mesh-skin");
   if(modules.size()!=4)throw std::runtime_error("Original class starting armor definition incomplete");
   const auto split=definition.idle_clip.find_last_of('/');
   const auto animation=read(assets,definition.idle_clip.substr(split+1),"animations");std::string error;
   if(!dh2::objects::load_modular_resource(model.data(),model.size(),modules,animation.data(),animation.size(),actor.resource,error))
    throw std::runtime_error("Original class modular preview rejected: "+error);
   if(actor.resource.animation.skipped||actor.resource.animation.end<=actor.resource.animation.start)
    throw std::runtime_error("Class preview idle animation channels unsupported");
   std::map<std::string,GLuint> cache;
   for(const auto& primitive:actor.resource.primitives){
    actor.draws.emplace_back();auto& batch=actor.draws.back();batch.node=primitive.node;
    batch.material=actor.resource.scene.materials.at(primitive.material);
    batch.diffuse=upload(assets,batch.material.diffuse,cache,actor.images);batch.alpha=upload(assets,batch.material.alpha_map,cache,actor.images);
    batch.count=primitive.indices.size();
    glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);
    glBufferData(GL_ARRAY_BUFFER,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data(),GL_DYNAMIC_DRAW);
    glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,primitive.indices.size()*sizeof(std::uint16_t),primitive.indices.data(),GL_STATIC_DRAW);
    check("Class preview buffer upload");
   }
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original class body preview connected | %s | anchor %s | modules %zu | draws %zu | idle tracks %u | unbound %u | clip %s | weapons, inventory and lighting pending",
    definition.character.c_str(),anchors[i],modules.size(),actor.draws.size(),actor.resource.animation.track_count(),actor.resource.animation.unbound,definition.idle_clip.c_str());
  }
  release_class_previews();class_preview_actors=std::move(next);
 }catch(...){for(auto& actor:next)release(actor.draws,actor.images);throw;}
}
Matrix class_camera(int width,int height){
 const auto& m=current_scene.graph.at(class_camera_node).world;
 const std::array<float,3> eye{m[12],m[13],m[14]};
 // Collada CCameraSceneNode::onRegisterSceneNode 0x6e5338 transforms
 // local (0,0,-100) as its target through the animated world matrix.
 std::array<float,3> f{-m[8],-m[9],-m[10]};normalize(f);
 auto s=cross(f,{0,0,1});normalize(s);auto u=cross(s,f);
 Matrix view{s[0],u[0],-f[0],0,s[1],u[1],-f[1],0,s[2],u[2],-f[2],0,-dot(s,eye),-dot(u,eye),dot(f,eye),1};
 // Serialized horizontal FOV is converted by the original Collada camera
 // constructor to vertical FOV using its authored aspect (1.5).
 const float tangent=std::tan(74.08049774169922f*.01745329238474369f*.5f)/1.5f;
 const float aspect=float(width)/height,near=50,far=50000;
 Matrix projection{1/(tangent*aspect),0,0,0,0,1/tangent,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};
 return dh2::scene::multiply(projection,view);
}
Matrix menu_camera(int width,int height){
  // MenuMainMenu::CreateAvatarCamera, ARM 0x42bf68..0x42c0bc.
  // Position (0,-900,150), target (0,0,225), Z up; original camera
  // source camera specifies FOV bits 0x3f3579c8. Keep its vertical field of
  // view fixed and derive horizontal projection from the live surface, so a
  // 16:9 display reveals more of the 3D backdrop without stretching it.
  const std::array<float,3> eye{0,-900,150};
  std::array<float,3> f{0,900,75};normalize(f);
  auto s=cross(f,{0,0,1});normalize(s);auto u=cross(s,f);
  Matrix view{s[0],u[0],-f[0],0,s[1],u[1],-f[1],0,s[2],u[2],-f[2],0,-dot(s,eye),-dot(u,eye),dot(f,eye),1};
  float fov;const std::uint32_t fov_bits=0x3f3579c8;
  std::memcpy(&fov,&fov_bits,4);
  const float aspect=dh2::ui::original_menu_viewport_v1::surface_aspect(width,height);
  const float tangent=std::tan(fov*.5f),near=10,far=2000;
  const Matrix projection{1/(tangent*aspect),0,0,0,0,1/tangent,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};
  return dh2::scene::multiply(projection,view);
}
} // model_renderer unnamed namespace
bool class_scene_active(){return active()&&class_scene;}
bool class_scene_input_enabled(){
 return class_scene_active()&&(class_clip_name.find("_to_")==std::string::npos||class_clip_cursor>=class_clip_end);
}
std::string load_class_scene(AAssetManager* assets){
  try{auto bytes=read(assets,"class_selection.bdae","models");
   load_class_preview_definitions(assets);
  const auto report=load(bytes.data(),bytes.size(),assets);if(report.find("3D upload OK")!=0)return report;
  if(player.skipped)throw std::runtime_error("Class scene has unsupported animation channels");
  class_camera_node=-1;for(unsigned i=0;i<current_scene.graph.size();++i)
   if(current_scene.graph[i].id=="Camera01-node")class_camera_node=int(i);
   if(class_camera_node<0)throw std::runtime_error("Authored class camera absent");
   load_class_preview_actors(assets);
  dh2::resources::BresView view{};dh2_bres_open(&view,bytes.data(),bytes.size());class_clips.clear();
  auto word=[](const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;};
  for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::animation_clip);++i){
   const auto* p=dh2_bres_library_item(&view,dh2::resources::Library::animation_clip,i);
   const auto name=word(p);if(name>=bytes.size()||!std::memchr(bytes.data()+name,0,bytes.size()-name))throw std::runtime_error("Class clip name rejected");
   class_clips.push_back({reinterpret_cast<const char*>(bytes.data()+name),int(word(p+4)),int(word(p+8))});
  }
  class_scene=true;class_selected=-1;class_clip_name.clear();class_pane_frames=0;frozen=true;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original class scene connected | clips %zu | tracks %u | body previews %zu | weapons and lighting pending",class_clips.size(),player.track_count(),class_preview_actors.size());
  return report;
 }catch(const std::exception& e){class_scene=false;return std::string("Class scene failed: ")+e.what();}
}
bool select_class_scene(int index,int dt_ms,std::string& error){
 if(!class_scene_active()||index<0||index>2||dt_ms<0){error="Class scene update owner unavailable";return false;}
 auto set=[&](const std::string& name){for(const auto& clip:class_clips)if(clip.name==name){
  class_clip_start=clip.start;class_clip_end=clip.end;class_clip_cursor=clip.start;
  if(class_clip_name!=name)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Original class scene animation | %s | %d %d",name.c_str(),clip.start,clip.end);
  class_clip_name=name;return true;}
  error="Authored class scene clip absent: "+name;return false;};
 if(index!=class_selected){std::string name="lol_"+std::to_string(index+1)+"_idle";
  if(class_selected>=0)name="lol_"+std::to_string(class_selected+1)+"_to_"+std::to_string(index+1);
  if(!set(name))return false;class_selected=index;
 }else if(class_clip_cursor>=class_clip_end){if(!set("lol_"+std::to_string(index+1)+"_idle"))return false;}
 // UpdateAnim samples the current cursor before adding dt and clamping.
  sampled_ms=class_clip_cursor;class_clip_cursor=std::min(class_clip_end,class_clip_cursor+dt_ms);
  for(auto& actor:class_preview_actors){actor.sample_elapsed=actor.elapsed;actor.elapsed+=unsigned(dt_ms);}
 return true;
}
void draw_class_scene(int width,int height){
 if(!class_scene_active())throw std::runtime_error("Class scene renderer unavailable");draw(width,height);
 if(!class_pane_frames++){
  GLint viewport[4];glGetIntegerv(GL_VIEWPORT,viewport);const auto& camera=current_scene.graph.at(class_camera_node).world;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original class pane draw | viewport %d %d %d %d | sample ms %d | camera %.6g %.6g %.6g",viewport[0],viewport[1],viewport[2],viewport[3],sampled_ms,camera[12],camera[13],camera[14]);
 }
}
std::string load_menu_background(AAssetManager* assets){
  try{
    const auto bytes=read(assets,"main_menu_charactere_swamp.bdae","models");
    const auto report=load(bytes.data(),bytes.size(),assets);
    if(report.find("3D upload OK")!=0)return report;
    menu_background=true;menu_preview_assets=assets;
    // EGL/context recreation releases the preview's GPU resources but keeps
    // the source-selected save slot. Rebuild that same visual projection once
    // with the newly loaded scene/asset owner; doing it here avoids retrying a
    // failed or intentionally absent profile on every rendered frame.
    if(menu_preview_selection.slot>=0){
      std::string preview_error;
      if(!restore_menu_preview(preview_error))
        throw std::runtime_error("MainMenu selected Character preview restore failed: "+preview_error);
    }
    for(const auto& batch:draws)if(batch.material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&
        batch.material.gles2_technique=="L1_Vc_Al_----_----_----_----")
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original menu material alpha connected | material %s | technique %s | alpha source blue replacement | lighting pending",
        batch.material.id.c_str(),batch.material.gles2_technique.c_str());
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original menu swamp connected | original camera values | existing material renderer | avatar pending");
    return report;
  }catch(const std::exception& error){return std::string("Menu background failed: ")+error.what();}
}
void draw_menu_background(int width,int height){
  if(!menu_background||!active())throw std::runtime_error("Menu swamp renderer unavailable");
  const auto viewport=dh2::ui::original_menu_viewport_v1::background_viewport(width,height);
  if(viewport[2]<=0||viewport[3]<=0)throw std::runtime_error("Menu background surface unavailable");
  glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
  draw(viewport[2],viewport[3]);
  glViewport(0,0,width,height);
}
namespace { // continue the renderer's unnamed namespace
