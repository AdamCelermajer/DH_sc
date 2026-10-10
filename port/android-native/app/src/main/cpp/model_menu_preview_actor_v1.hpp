#pragma once

// Included inside model_renderer's anonymous namespace, after
// model_menu_scene_v69.hpp. This owns only the GPU/model preview resource;
// save metadata and the source Character remain with their existing owners.
struct MenuPreviewActorV1 {
 dh2::objects::Resource resource;
 std::vector<Draw> draws;
 std::vector<GLuint> images;
 float scale[3]{1,1,1};
 std::int32_t slot=-1,character_row=-1;
 bool ready=false;
};
MenuPreviewActorV1 menu_preview_actor;
AAssetManager* menu_preview_assets=nullptr;
dh2::data::CharacterTable menu_preview_characters;
bool menu_preview_characters_loaded=false;

bool load_menu_preview_character_table_v1(AAssetManager* assets,std::string& error){
 if(menu_preview_characters_loaded)return true;
 if(!assets){error="MainMenu preview CharacterTable asset manager is unavailable";return false;}
 try{
  const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
  std::array<std::vector<std::uint8_t>,3> raw;
  for(unsigned i=0;i<raw.size();++i)
   raw[i]=read(assets,std::string("character_properties")+suffixes[i],"data");
  if(!dh2::data::load_characters(
       {raw[0].data(),raw[0].size()},
       {raw[1].data(),raw[1].size()},
       {raw[2].data(),raw[2].size()},menu_preview_characters,error))
   return false;
  menu_preview_characters_loaded=true;error.clear();return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}

void release_menu_preview_actor_v1(bool gl_context=true){
 if(gl_context)release(menu_preview_actor.draws,menu_preview_actor.images);
 else{menu_preview_actor.draws.clear();menu_preview_actor.images.clear();}
 menu_preview_actor.resource={};
 std::fill(std::begin(menu_preview_actor.scale),std::end(menu_preview_actor.scale),1.f);
 menu_preview_actor.slot=-1;menu_preview_actor.character_row=-1;
 menu_preview_actor.ready=false;
}

bool load_menu_preview_actor_v1(AAssetManager* assets,std::int32_t slot,
 std::int32_t character_row,std::string& error){
 if(slot<0){error="MainMenu preview requires a selected nonnegative slot";return false;}
 if(!assets){error="MainMenu preview asset manager is unavailable";return false;}
 dh2::player_manager_friendly_v1::SinglePlayerClassProjection class_projection{};
 if(dh2::player_manager_friendly_v1::project_single_player_class(
      character_row,&class_projection)!=
      dh2::player_manager_friendly_v1::Status::complete){
  error="Selected save class row is not a supported single-player CharacterTable row";
  return false;
 }
 unsigned definition_index=0;
 switch(class_projection.visual_class){
 case dh2::player_manager_friendly_v1::SinglePlayerVisualClass::warrior:definition_index=0;break;
 case dh2::player_manager_friendly_v1::SinglePlayerVisualClass::rogue:definition_index=1;break;
 case dh2::player_manager_friendly_v1::SinglePlayerVisualClass::mage:definition_index=2;break;
 }
 std::vector<Draw> next_draws;std::vector<GLuint> next_images;
 dh2::objects::Resource next_resource;
 float next_scale[3]{1,1,1};
 try{
  // Reuse the same authored class properties, starting loot and MenuIdle
  // sequence used by MenuCharacterSelect. MainMenu's mask-1 metadata Save
  // does not contain GEAR; this actor therefore uses that class's authored
  // starting body modules until the actual Character SG_Load(4) visual owner
  // is available.
  load_class_preview_definitions(assets);
  const auto& definition=class_preview_definitions.at(definition_index);
  if(definition.row<0||definition.character.empty()||definition.idle_clip.empty())
   throw std::runtime_error("Selected authored class preview definition is incomplete");
  std::vector<std::string> modules{"MC_Head__naked-mesh-skin"};
  for(const auto& item:definition.starting_items)
   if(item.module.find("MC_Torso_")==0||item.module.find("MC_Feet_")==0||
      item.module.find("MC_Hands_")==0)
    modules.push_back(item.module+"-mesh-skin");
  if(modules.size()!=4)throw std::runtime_error("Selected class starting body modules are incomplete");
  const auto model=read(assets,"prince_modular.bdae","models");
  const auto separator=definition.idle_clip.find_last_of('/');
  const auto animation=read(assets,definition.idle_clip.substr(separator+1),"animations");
  if(!dh2::objects::load_modular_resource(model.data(),model.size(),modules,
       animation.data(),animation.size(),next_resource,error))
   throw std::runtime_error("MainMenu modular Character preview rejected: "+error);
  if(next_resource.animation.skipped||
     next_resource.animation.end<=next_resource.animation.start)
   throw std::runtime_error("Selected class MenuIdle animation channels are unsupported");
  if(dh2_character_visual_scale(next_scale,
        definition.properties.base.data()+12))
   throw std::runtime_error("Selected class Character visual scale is invalid");
  std::map<std::string,GLuint> cache;
  for(const auto& primitive:next_resource.primitives){
   next_draws.emplace_back();auto& batch=next_draws.back();
   batch.node=primitive.node;batch.material=next_resource.scene.materials.at(primitive.material);
   batch.diffuse=upload(assets,batch.material.diffuse,cache,next_images);
   batch.alpha=upload(assets,batch.material.alpha_map,cache,next_images);
   batch.count=GLsizei(primitive.indices.size());
   glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);
   glBufferData(GL_ARRAY_BUFFER,primitive.vertices.size()*sizeof(Vertex),
                primitive.vertices.data(),GL_DYNAMIC_DRAW);
   glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,primitive.indices.size()*sizeof(std::uint16_t),
                primitive.indices.data(),GL_STATIC_DRAW);
   check("MainMenu Character preview buffer upload");
  }
  if(next_draws.empty())throw std::runtime_error("Selected class preview has no renderable body primitives");
  release_menu_preview_actor_v1(true);
  menu_preview_actor.resource=std::move(next_resource);
  menu_preview_actor.draws=std::move(next_draws);
  menu_preview_actor.images=std::move(next_images);
  std::copy(std::begin(next_scale),std::end(next_scale),menu_preview_actor.scale);
  menu_preview_actor.slot=slot;menu_preview_actor.character_row=character_row;
  menu_preview_actor.ready=true;menu_preview_assets=assets;
  error.clear();return true;
 }catch(const std::exception& failure){
  release(next_draws,next_images);error=failure.what();return false;
 }
}

template<class Submit>
void draw_menu_preview_actor_v1(const Matrix& projection,Submit&& submit){
 if(!menu_preview_actor.ready)return;
 auto& actor=menu_preview_actor;std::string error;
 auto& resource=actor.resource;
 if(!dh2::objects::sample(resource,resource.animation.start,error))
  throw std::runtime_error("MainMenu Character preview pose failed: "+error);
 // MenuMainMenu::SetupCharacter sets position (0,-200,-20), applies the
 // source -pi/8 Z quaternion, then sets visible true (0x42bde4..0x42be50).
 const float position[3]{0,-200,-20};
 const float angle=-3.14159265358979323846f/8.f;
 const float quaternion[4]{0,0,std::sin(angle*.5f),std::cos(angle*.5f)};
 Matrix placement{};dh2_node_matrix(placement.data(),position,quaternion,actor.scale);
 const auto owner_projection=dh2::scene::multiply(projection,placement);
 for(std::size_t i=0;i<resource.primitives.size();++i){
  const auto& primitive=resource.primitives[i];const auto& batch=actor.draws.at(i);
  glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);
  glBufferSubData(GL_ARRAY_BUFFER,0,primitive.vertices.size()*sizeof(Vertex),
                  primitive.vertices.data());
  const auto transform=primitive.skin.nodes.empty()?
      dh2::scene::multiply(owner_projection,resource.scene.graph.at(primitive.node).world):
      owner_projection;
  submit(batch,transform);
 }
}
