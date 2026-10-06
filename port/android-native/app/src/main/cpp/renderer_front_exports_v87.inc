// Authored front visual API; no gameplay actor or world duplication.
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
    menu_background=true;
    if(menu_persona_class>=0){load_class_preview_definitions(assets);load_class_preview_actors(assets,menu_persona_class);menu_persona_epoch=std::chrono::steady_clock::now();}
    for(const auto& batch:draws)if(batch.material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&
        batch.material.gles2_technique=="L1_Vc_Al_----_----_----_----")
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original menu material alpha connected | material %s | technique %s | alpha source blue replacement | lighting pending",
        batch.material.id.c_str(),batch.material.gles2_technique.c_str());
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original menu swamp connected | original camera values | existing material renderer | avatar pending");
    return report;
  }catch(const std::exception& error){return std::string("Menu background failed: ")+error.what();}
}
bool select_menu_persona(int index,AAssetManager* assets,std::string& error){
 if(index < -1 || index > 2){error="Unsafe menu persona class";return false;}
 if(menu_persona_class==index){error.clear();return true;}
 menu_persona_class=index;
 try{if(menu_background&&active()){release_class_previews();if(index>=0){load_class_preview_definitions(assets);load_class_preview_actors(assets,index);menu_persona_epoch=std::chrono::steady_clock::now();}}}
 catch(const std::exception& failure){error=failure.what();return false;}
 error.clear();return true;
}
void draw_menu_background(int width,int height){
  if(!menu_background||!active())throw std::runtime_error("Menu swamp renderer unavailable");
  const int w=std::min(width,height*3/2),h=std::min(height,width*2/3);
  glViewport((width-w)/2,(height-h)/2,w,h);
  draw(w,h);
  glViewport(0,0,width,height);
}
