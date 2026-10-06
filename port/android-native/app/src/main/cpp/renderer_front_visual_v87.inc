// Frozen front-v87 visual owners; shared root renderer and GPU lifetime.
bool menu_background=false,class_scene=false;
int class_camera_node=-1,class_selected=-1,class_clip_start=0,class_clip_end=0,class_clip_cursor=0;
struct ClassClip {std::string name;int start,end;};std::vector<ClassClip> class_clips;
std::string class_clip_name;
unsigned class_pane_frames=0;
struct ClassPreviewWeapon {
 dh2::objects::Resource resource;
 std::vector<Draw> draws;std::vector<GLuint> images;
 unsigned anchor=0;std::string module;
};
struct ClassPreviewActor {
 dh2::objects::Resource resource;
 std::vector<Draw> draws;
 std::vector<GLuint> images;
 unsigned anchor=0;
 float scale[3]{1,1,1};
 std::uint64_t elapsed=0,sample_elapsed=0;
 std::vector<ClassPreviewWeapon> weapons;
};
std::vector<ClassPreviewActor> class_preview_actors;
void release_class_previews(){
 for(auto& actor:class_preview_actors){release(actor.draws,actor.images);for(auto& weapon:actor.weapons)release(weapon.draws,weapon.images);}
 class_preview_actors.clear();
}
std::array<dh2::data::ClassPreviewDefinition,3> class_preview_definitions;
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
// VisualObject::SetWeaponSkin(0x473cd8) constructs the original weapon
// scene and parents it under s_slotMapping[attachment] (table0x956ad8).
// For starting longsword/staff use attachment1; duplicate Rogue daggers
// occupy attachments1/2. No hand-authored position/rotation corrections.
void load_class_preview_weapons(AAssetManager* assets,const dh2::data::ClassPreviewDefinition& definition,ClassPreviewActor& actor){
 const char* anchors[]={"anchor_weapon_right_offset-node","anchor_weapon_left_offset-node"};
 unsigned count=0;
 for(const auto& item:definition.starting_items){
  if(item.module.find("MC_RWeapon_")!=0)continue;
  if(count>=2)throw std::runtime_error("Starting weapon count exceeds two hands");
  const auto anchor=std::find_if(actor.resource.scene.graph.begin(),actor.resource.scene.graph.end(),[&](const dh2::scene::Node& n){return n.id==anchors[count];});
  if(anchor==actor.resource.scene.graph.end())throw std::runtime_error("Original weapon attachment anchor absent");
  actor.weapons.emplace_back();auto& weapon=actor.weapons.back();weapon.anchor=unsigned(anchor-actor.resource.scene.graph.begin());weapon.module=item.module;
  auto name=item.module+".bdae";std::transform(name.begin(),name.end(),name.begin(),[](unsigned char c){return char(std::tolower(c));});
  const auto raw=read(assets,name,"models");std::string error;
  if(!dh2::objects::load_resource(raw.data(),raw.size(),nullptr,0,weapon.resource,error))throw std::runtime_error("Original weapon scene rejected: "+error);
  std::map<std::string,GLuint> cache;
  for(const auto& primitive:weapon.resource.primitives){
   if(!primitive.skin.nodes.empty())throw std::runtime_error("Starting weapon requires unsupported skinned attachment");
   weapon.draws.emplace_back();auto& batch=weapon.draws.back();batch.node=primitive.node;batch.material=weapon.resource.scene.materials.at(primitive.material);
   batch.diffuse=upload(assets,batch.material.diffuse,cache,weapon.images,nullptr,dh2::resources::ResourceScopeV37::actor);batch.alpha=upload(assets,batch.material.alpha_map,cache,weapon.images,nullptr,dh2::resources::ResourceScopeV37::actor);batch.count=primitive.indices.size();
   glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferData(GL_ARRAY_BUFFER,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data(),GL_STATIC_DRAW);
   glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,primitive.indices.size()*sizeof(std::uint16_t),primitive.indices.data(),GL_STATIC_DRAW);check("Weapon preview upload");
  }
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original preview weapon attached | class %s | item %s | module %s | anchor %s | triangles %u",definition.character.c_str(),item.identifier.c_str(),item.module.c_str(),anchors[count],weapon.resource.triangles);
  ++count;
 }
 if(count!=(definition.character=="RoguePlayerBase"?2u:1u))throw std::runtime_error("Original starting weapon definitions incomplete");
}
int menu_persona_class=-1;
std::chrono::steady_clock::time_point menu_persona_epoch;
void load_class_preview_actors(AAssetManager* assets,int menu_class=-1){
 std::vector<ClassPreviewActor> next;
 try{
  const auto model=read(assets,"prince_modular.bdae","models");
  const char* anchors[]={"dummy_Warrior-node","dummy_Rogue-node","dummy_Mage-node"};
  for(unsigned i=0;i<class_preview_definitions.size();++i){
   if(menu_class>=0&&i!=unsigned(menu_class))continue;
   const auto& definition=class_preview_definitions[i];next.emplace_back();auto& actor=next.back();
   const auto anchor=std::find_if(current_scene.graph.begin(),current_scene.graph.end(),[&](const dh2::scene::Node& n){return n.id==anchors[i];});
   if(menu_class<0&&anchor==current_scene.graph.end())throw std::runtime_error(std::string("Original class anchor absent: ")+anchors[i]);
   actor.anchor=menu_class>=0?0:unsigned(anchor-current_scene.graph.begin());
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
    batch.diffuse=upload(assets,batch.material.diffuse,cache,actor.images,nullptr,dh2::resources::ResourceScopeV37::actor);batch.alpha=upload(assets,batch.material.alpha_map,cache,actor.images,nullptr,dh2::resources::ResourceScopeV37::actor);
    batch.count=primitive.indices.size();
    glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);
    glBufferData(GL_ARRAY_BUFFER,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data(),GL_DYNAMIC_DRAW);
    glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,primitive.indices.size()*sizeof(std::uint16_t),primitive.indices.data(),GL_STATIC_DRAW);
    check("Class preview buffer upload");
   }
   load_class_preview_weapons(assets,definition,actor);
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original class body preview connected | %s | anchor %s | modules %zu | draws %zu | idle tracks %u | unbound %u | clip %s | inventory and lighting pending",
    definition.character.c_str(),anchors[i],modules.size(),actor.draws.size(),actor.resource.animation.track_count(),actor.resource.animation.unbound,definition.idle_clip.c_str());
  }
  release_class_previews();class_preview_actors=std::move(next);
 }catch(...){for(auto& actor:next){release(actor.draws,actor.images);for(auto& weapon:actor.weapons)release(weapon.draws,weapon.images);}throw;}
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
Matrix menu_camera(){
  // MenuMainMenu::CreateAvatarCamera, ARM 0x42bf68..0x42c0bc.
  // Position (0,-900,150), target (0,0,225), Z up; original camera
  // setters specify aspect bits 0x3fd578e9 and FOV bits 0x3f3579c8.
  const std::array<float,3> eye{0,-900,150};
  std::array<float,3> f{0,900,75};normalize(f);
  auto s=cross(f,{0,0,1});normalize(s);auto u=cross(s,f);
  Matrix view{s[0],u[0],-f[0],0,s[1],u[1],-f[1],0,s[2],u[2],-f[2],0,-dot(s,eye),-dot(u,eye),dot(f,eye),1};
  float aspect,fov;const std::uint32_t aspect_bits=0x3fd578e9,fov_bits=0x3f3579c8;
  std::memcpy(&aspect,&aspect_bits,4);std::memcpy(&fov,&fov_bits,4);
  const float tangent=std::tan(fov*.5f),near=10,far=2000;
  const Matrix projection{1/(tangent*aspect),0,0,0,0,1/tangent,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};
  return dh2::scene::multiply(projection,view);
}
