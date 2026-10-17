#pragma once

// Included inside model_renderer's anonymous namespace, after
// model_menu_scene_v69.hpp. This owns only the GPU/model preview resource;
// save metadata and the source Character remain with their existing owners.
struct MenuPreviewActorV1 {
 // The skin owner borrows this exact scene address. Keep Resource separately
 // allocated so replacing the preview never invalidates that borrowed graph.
 std::unique_ptr<dh2::objects::Resource> resource;
 dh2::skinning::VisualSkinResourcesV6 skin_resources;
 std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> skin;
 std::vector<Draw> draws;
 std::vector<Draw> weapon_draws;
 std::vector<GLuint> images;
 std::vector<GLuint> weapon_images;
 float scale[3]{1,1,1};
 std::int32_t slot=-1,character_row=-1;
 std::chrono::steady_clock::time_point epoch{};
 bool ready=false;
};
MenuPreviewActorV1 menu_preview_actor;
AAssetManager* menu_preview_assets=nullptr;
dh2::data::CharacterTable menu_preview_characters;
bool menu_preview_characters_loaded=false;
dh2::data::LootTablesV2::Borrow menu_preview_loot;

struct MenuPreviewSavedGearV1 {
 std::array<std::array<std::int32_t,9>,2> item{{
  {{-1,-1,-1,-1,-1,-1,-1,-1,-1}},
  {{-1,-1,-1,-1,-1,-1,-1,-1,-1}}}};
 std::uint32_t selected=0;
};
bool menu_preview_gear_word_v1(dh2::data::Bytes bytes,std::size_t& at,std::uint32_t& out){
 if(!bytes.data||at>bytes.size||bytes.size-at<4)return false;
 const auto* p=bytes.data+at;out=std::uint32_t(p[0])|
  (std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
 at+=4;return true;
}
bool menu_preview_gear_string_v1(dh2::data::Bytes bytes,std::size_t& at,std::string& out){
 std::uint32_t length=0;if(!menu_preview_gear_word_v1(bytes,at,length)||
    !length||length>1048576||at>bytes.size||length>bytes.size-at||
    bytes.data[at+length-1]!=0)return false;
 out.assign(reinterpret_cast<const char*>(bytes.data+at),length-1);at+=length;return true;
}
bool menu_preview_parse_gear_v1(dh2::data::Bytes bytes,
 const dh2::data::ItemTable& items,MenuPreviewSavedGearV1& out,std::string& error){
 out={};std::size_t at=0;std::uint32_t gold=0,count=0;
 if(!menu_preview_gear_word_v1(bytes,at,gold)||
    !menu_preview_gear_word_v1(bytes,at,out.selected)||out.selected>1||
    !menu_preview_gear_word_v1(bytes,at,count)||count>65536){
  error="Saved GEAR appearance header is truncated or outside source bounds";return false;
 }
 for(std::uint32_t i=0;i<count;++i){
  std::string identifier;std::uint32_t slots[2]{},quantity=0,value=0,powers=0;
  if(!menu_preview_gear_string_v1(bytes,at,identifier)||
     !menu_preview_gear_word_v1(bytes,at,slots[0])||
     !menu_preview_gear_word_v1(bytes,at,slots[1])||
     !menu_preview_gear_word_v1(bytes,at,quantity)||
     !menu_preview_gear_word_v1(bytes,at,value)||at>=bytes.size){
   error="Saved GEAR appearance item prefix is truncated";return false;
  }
  ++at; // Identified byte.
  if(!menu_preview_gear_word_v1(bytes,at,powers)||powers>65536){
   error="Saved GEAR appearance power list is truncated or oversized";return false;
  }
  for(std::uint32_t p=0;p<powers;++p){
   std::string ignored;if(!menu_preview_gear_string_v1(bytes,at,ignored)){
    error="Saved GEAR appearance power name is truncated";return false;
   }
  }
  const auto id=dh2::data::item_id(items,identifier);
  for(unsigned set=0;set<2;++set){
   if(slots[set]==UINT32_MAX)continue;
   if(slots[set]>=9||id<0){error="Saved GEAR appearance references an invalid equipped Item/slot";return false;}
   auto& existing=out.item[set][slots[set]];
   if(existing!=-1){error="Saved GEAR appearance equips multiple Items to one source slot";return false;}
   existing=id;
  }
 }
 error.clear();return true;
}

bool load_menu_preview_loot_table_v1(AAssetManager* assets,std::string& error){
 if(menu_preview_loot)return true;
 if(!assets){error="MainMenu preview ItemTable asset manager is unavailable";return false;}
 try{
  const auto records=read(assets,"loot_table_pyarray.bin","data");
  const auto names=read(assets,"loot_table_pyarraynames.bin","data");
  const auto schema=read(assets,"loot_table_pystructnames.bin","data");
  dh2::data::LootTablesV2 table;
  if(!table.load({records.data(),records.size()},{names.data(),names.size()},
       {schema.data(),schema.size()},error))return false;
  menu_preview_loot=table.borrow();error.clear();return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}

static dh2::skinning::VisualAssetResultV6 read_menu_preview_weapon_v1(
 void* context,const char* uri,std::vector<std::uint8_t>& bytes,std::string& error){
 auto* assets=static_cast<AAssetManager*>(context);if(!assets||!uri){
  error="MainMenu saved weapon asset request is incomplete";
  return dh2::skinning::VisualAssetResultV6::failed;
 }
 try{bytes=read(assets,uri,"actors");error.clear();return dh2::skinning::VisualAssetResultV6::found;}
 catch(const std::exception& failure){const std::string text=failure.what();
  if(text.rfind("Bundled asset missing: actors/",0)==0){error.clear();return dh2::skinning::VisualAssetResultV6::missing;}
  error=text;return dh2::skinning::VisualAssetResultV6::failed;
 }
}

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
 // VisualSkinOwnerV6 holds a pointer into resource->scene. Destroy it before
 // releasing either its borrowed resources or the stable scene storage.
 menu_preview_actor.skin.reset();
 menu_preview_actor.skin_resources=dh2::skinning::VisualSkinResourcesV6{};
 if(gl_context)release(menu_preview_actor.draws,menu_preview_actor.images);
 else{menu_preview_actor.draws.clear();menu_preview_actor.images.clear();}
 if(gl_context)release(menu_preview_actor.weapon_draws,menu_preview_actor.weapon_images);
 else{menu_preview_actor.weapon_draws.clear();menu_preview_actor.weapon_images.clear();}
 menu_preview_actor.resource.reset();
 std::fill(std::begin(menu_preview_actor.scale),std::end(menu_preview_actor.scale),1.f);
 menu_preview_actor.slot=-1;menu_preview_actor.character_row=-1;
 menu_preview_actor.ready=false;
}

bool load_menu_preview_actor_v1(AAssetManager* assets,std::int32_t slot,
 std::int32_t character_row,const dh2::data::ProfileSection12V1* gear_section,
 dh2::data::Bytes gear_payload,std::string& error){
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
 std::vector<Draw> next_draws,next_weapon_draws;std::vector<GLuint> next_images,next_weapon_images;
 dh2::objects::Resource decoded_resource;
 auto next_resource=std::make_unique<dh2::objects::Resource>();
 dh2::skinning::VisualSkinResourcesV6 next_skin_resources;
 std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> next_skin;
 float next_scale[3]{1,1,1};
 try{
  // Reuse authored class properties and MenuIdle, while an indexed GEAR row
  // projects only immutable item identifiers and equipment slots. It never
  // creates a Save, Character, Item, V4 inventory or property owner.
  load_class_preview_definitions(assets);
  const auto& definition=class_preview_definitions.at(definition_index);
  if(definition.row<0||definition.character.empty()||definition.idle_clip.empty())
   throw std::runtime_error("Selected authored class preview definition is incomplete");
  MenuPreviewSavedGearV1 saved_gear{};const bool has_saved_gear=gear_section!=nullptr;
  if(has_saved_gear&&gear_section->size){
   if(!load_menu_preview_loot_table_v1(assets,error))
    throw std::runtime_error("Saved GEAR ItemTable unavailable: "+error);
   if(!menu_preview_parse_gear_v1(gear_payload,menu_preview_loot.items(),saved_gear,error))
    throw std::runtime_error(error);
  }
  const auto model=read(assets,"prince_modular.bdae","models");
  std::vector<std::string> modules;
  if(!has_saved_gear){
   modules.push_back("MC_Head__naked-mesh-skin");
   for(const auto& item:definition.starting_items)
    if(item.module.find("MC_Torso_")==0||item.module.find("MC_Feet_")==0||
       item.module.find("MC_Hands_")==0)modules.push_back(item.module+"-mesh-skin");
   if(modules.size()!=4)throw std::runtime_error("Selected class starting body modules are incomplete");
  }else{
   dh2::skinning::ModularSkinCatalog catalog;
   if(!dh2::skinning::load_modular_skin_catalog(model.data(),model.size(),catalog,error))
    throw std::runtime_error("Saved GEAR modular catalog rejected: "+error);
   static constexpr struct {unsigned slot;const char* category;} body_slots[]=
    {{0,"MC_Torso"},{3,"MC_Feet"},{4,"MC_Hands"},{8,"MC_Head"}};
   const auto* item_table=menu_preview_loot?&menu_preview_loot.items():nullptr;
   for(const auto& body:body_slots){
    const auto id=gear_section->size?saved_gear.item[0][body.slot]:-1;
    const auto* item=id>=0&&item_table?dh2::data::item(*item_table,id):nullptr;
    if(id>=0&&!item)throw std::runtime_error("Saved GEAR body item is absent from ItemTable");
    const dh2::skinning::ModularSkinModule* module=nullptr;bool placeholder=false;
    if(!dh2::skinning::resolve_modular_skin(catalog,body.category,
         item?item->name:std::string{},item!=nullptr,module,placeholder,error)||!module)
     throw std::runtime_error("Saved GEAR body module resolution failed: "+error);
    modules.push_back(module->controller);
   }
  }
  const auto separator=definition.idle_clip.find_last_of('/');
  const auto animation=read(assets,definition.idle_clip.substr(separator+1),"animations");
  if(!dh2::objects::load_modular_resource(model.data(),model.size(),modules,
       animation.data(),animation.size(),decoded_resource,error))
   throw std::runtime_error("MainMenu modular Character preview rejected: "+error);
  if(decoded_resource.animation.skipped||
     decoded_resource.animation.end<=decoded_resource.animation.start)
   throw std::runtime_error("Selected class MenuIdle animation channels are unsupported");
  *next_resource=std::move(decoded_resource);
  if(dh2_character_visual_scale(next_scale,
        definition.properties.base.data()+12))
   throw std::runtime_error("Selected class Character visual scale is invalid");
  if(!next_skin_resources.load(model,error))
   throw std::runtime_error("MainMenu modular visual owner rejected: "+error);
  next_skin=std::make_unique<dh2::skinning::VisualSkinOwnerV6>(
      next_skin_resources.borrow(),next_resource->scene,
      dh2::skinning::VisualAssetServicesV6{assets,read_menu_preview_weapon_v1});
  if(!next_skin->initialize(error))
   throw std::runtime_error("MainMenu modular visual owner initialization failed: "+error);
  for(const char* category:{"MC_Torso","MC_RWeapon","MC_LWeapon","MC_Feet","MC_Hands","MC_Head"}){
   const auto id=next_skin->category_id(category);
   if(id>=0&&!next_skin->set_modular(id,-1,error))
    throw std::runtime_error(std::string("MainMenu preview body module hide failed: ")+error);
  }
  unsigned saved_weapon_count=0,loaded_weapon_count=0;
  if(has_saved_gear){
   const auto* item_table=menu_preview_loot?&menu_preview_loot.items():nullptr;
   for(const unsigned weapon_slot:{1u,2u}){
    const auto id=gear_section->size?saved_gear.item[saved_gear.selected][weapon_slot]:-1;
    const auto* item=id>=0&&item_table?dh2::data::item(*item_table,id):nullptr;
    if(id>=0&&!item)throw std::runtime_error("Saved GEAR weapon is absent from ItemTable");
    std::string name=item?item->name:std::string{};std::int32_t mode=std::int32_t(weapon_slot);
    if(item){
     if(name.find("Shield")!=std::string::npos)mode=0;
     else if(name.find("Bow")!=std::string::npos)mode=2;
     else if(weapon_slot==2&&name.find("Claw")!=std::string::npos){
      const auto at=name.find("RWeapon");if(at!=std::string::npos)name[at]='L';mode=2;
     }
    }
    if(!next_skin->set_weapon(item?name.c_str():nullptr,
         std::int32_t(weapon_slot),mode,error))
     throw std::runtime_error("Saved GEAR weapon visual selection failed: "+error);
    if(item){
     ++saved_weapon_count;
     if(!next_skin->weapon_uri(std::int32_t(weapon_slot)).empty())++loaded_weapon_count;
    }
   }
  }
  const std::vector<dh2::skinning::VisualDrawViewV32>* weapon_views=nullptr;
  if(!next_skin->draw_views(weapon_views,error)||!weapon_views)
   throw std::runtime_error("MainMenu saved weapon view query failed: "+error);
  std::map<std::string,GLuint> weapon_texture_cache;
  for(const auto& view:*weapon_views){
   if(!view.weapon_slot)continue;
   if(!view.geometry||!view.material_table||!view.materials||!view.positions||
      view.materials->size()!=view.geometry->primitives.size()||
      view.positions->size()!=view.geometry->positions.size())
    throw std::runtime_error("MainMenu saved weapon view is incomplete");
   for(std::size_t primitive_index=0;primitive_index<view.geometry->primitives.size();++primitive_index){
    const auto& primitive=view.geometry->primitives[primitive_index];
    const auto material_index=view.materials->at(primitive_index);
    if(material_index>=view.material_table->size()||primitive.collada_type||
       primitive.indices.size()%3||primitive.indices.size()>3000000||
       view.positions->size()>65536)
     throw std::runtime_error("MainMenu saved weapon topology/material is unsupported");
    std::vector<Vertex> vertices(view.positions->size());
    const auto attribute=[&](unsigned slot)->const dh2::skinning::VisualAttributeV6*{
     const auto index=primitive.attributes.at(slot);if(index<0)return nullptr;
     if(std::size_t(index)>=view.geometry->attributes.size())
      throw std::runtime_error("MainMenu saved weapon attribute index exceeds geometry");
     return &view.geometry->attributes[std::size_t(index)];
    };
    const auto* uv=attribute(4);const auto* color=attribute(2);
    const auto vertex_count=vertices.size();
    if((uv&&uv->components>=2&&uv->values.size()<vertex_count*uv->components)||
       (color&&color->components&&color->values.size()<vertex_count*color->components))
     throw std::runtime_error("MainMenu saved weapon UV/color stream is shorter than the position stream");
    for(std::size_t vertex_index=0;vertex_index<vertices.size();++vertex_index){
     auto& vertex=vertices[vertex_index];std::copy(view.positions->at(vertex_index).begin(),
         view.positions->at(vertex_index).end(),vertex.p);
     if(uv&&uv->components>=2)std::copy_n(uv->values.data()+vertex_index*uv->components,2,vertex.uv);
     if(color)for(unsigned channel=0;channel<std::min(4u,color->components);++channel)
      vertex.color[channel]=color->values[vertex_index*color->components+channel]/(color->type==1?255.f:1.f);
    }
    std::vector<std::uint16_t> indices;indices.reserve(primitive.indices.size());
    for(const auto index:primitive.indices){if(index>=vertices.size()||index>65535)
      throw std::runtime_error("MainMenu saved weapon index exceeds GLES2 bounds");
     indices.push_back(static_cast<std::uint16_t>(index));}
    next_weapon_draws.emplace_back();auto& batch=next_weapon_draws.back();
    batch.material=view.material_table->at(material_index);
    batch.diffuse=upload(assets,batch.material.diffuse,weapon_texture_cache,next_weapon_images);
    batch.alpha=upload(assets,batch.material.alpha_map,weapon_texture_cache,next_weapon_images);
    batch.count=static_cast<GLsizei>(indices.size());batch.environment=true;batch.placement=view.world;
    glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);
    glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(std::uint16_t),indices.data(),GL_STATIC_DRAW);
    check("MainMenu saved weapon buffer upload");
   }
  }
  std::map<std::string,GLuint> cache;
  for(const auto& primitive:next_resource->primitives){
   next_draws.emplace_back();auto& batch=next_draws.back();
   batch.node=primitive.node;batch.material=next_resource->scene.materials.at(primitive.material);
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
  __android_log_print(ANDROID_LOG_INFO,"DH2Native",
      "MainMenu saved appearance projection | slot %d | source %s | GEAR bytes %u | weapon resources %u/%u | weapon draw parts %zu | full Character SG_Load(4) remains separate",
      slot,gear_section?(gear_section->size?"indexed GEAR":"empty GEAR / naked"):
          "no GEAR section / authored class body",
      gear_section?gear_section->size:0,loaded_weapon_count,saved_weapon_count,
      next_weapon_draws.size());
  // Transfer the stable scene together with the owner that borrows it. The
  // owner remains live so each frame can resolve attachments from the new pose.
  release_menu_preview_actor_v1(true);
  menu_preview_actor.resource=std::move(next_resource);
  menu_preview_actor.skin_resources=std::move(next_skin_resources);
  menu_preview_actor.skin=std::move(next_skin);
  menu_preview_actor.draws=std::move(next_draws);
  menu_preview_actor.weapon_draws=std::move(next_weapon_draws);
  menu_preview_actor.images=std::move(next_images);
  menu_preview_actor.weapon_images=std::move(next_weapon_images);
  std::copy(std::begin(next_scale),std::end(next_scale),menu_preview_actor.scale);
  menu_preview_actor.slot=slot;menu_preview_actor.character_row=character_row;
  menu_preview_actor.epoch=std::chrono::steady_clock::now();
  menu_preview_actor.ready=true;menu_preview_assets=assets;
  error.clear();return true;
 }catch(const std::exception& failure){
  release(next_draws,next_images);release(next_weapon_draws,next_weapon_images);
  error=failure.what();return false;
 }
}

template<class Submit>
void draw_menu_preview_actor_v1(const Matrix& projection,Submit&& submit){
 if(!menu_preview_actor.ready)return;
 auto& actor=menu_preview_actor;std::string error;
 if(!actor.resource||!actor.skin)
  throw std::runtime_error("MainMenu Character preview retained owners are incomplete");
 auto& resource=*actor.resource;
 const auto duration=resource.animation.end-resource.animation.start;
 if(duration<=0)throw std::runtime_error("MainMenu Character preview MenuIdle duration is invalid");
 const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(
     std::chrono::steady_clock::now()-actor.epoch).count();
 const auto time=resource.animation.start+std::int32_t(elapsed%duration);
 if(!dh2::objects::sample(resource,time,error))
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
 const std::vector<dh2::skinning::VisualDrawViewV32>* weapon_views=nullptr;
 if(!actor.skin->draw_views(weapon_views,error)||!weapon_views)
  throw std::runtime_error("MainMenu saved weapon attachment update failed: "+error);
 auto batch=actor.weapon_draws.begin();
 for(const auto& view:*weapon_views){
  if(!view.weapon_slot)continue;
  if(batch==actor.weapon_draws.end())
   throw std::runtime_error("MainMenu saved weapon attachment count increased after initialization");
  batch->placement=view.world;
  submit(*batch,dh2::scene::multiply(owner_projection,batch->placement));
  ++batch;
 }
 if(batch!=actor.weapon_draws.end())
  throw std::runtime_error("MainMenu saved weapon attachment count decreased after initialization");
}
