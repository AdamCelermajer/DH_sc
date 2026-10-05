// Include inside the renderer private namespace after ObjectActor/retained
// animation helpers. Translation unit includes both portable headers first:
// character_world_npc_physical_lifecycle_v2.hpp and
// character_world_npc_properties_v1.hpp.
// Caller constructs every borrow from the same retained MonsterScriptHandle,
// canonical World, actual NativeWorld/floor registry, and common Random owner.
void initialize_renderer_npc_physical_v2(ObjectActor& actor,AAssetManager* assets,
 dh2::character::CharacterWorldNpcPropertiesV1& property_inputs,
 dh2::character::WorldNpcPhysicalGraphBorrowV2 b,
 dh2::character::WorldNpcPhysicalGraphServicesV2 services,
 std::unique_ptr<dh2::character::CharacterWorldNpcPhysicalLifecycleV2>& retained){
 auto& m=*actor.script;
 if(retained||!m.animation||&b.object!=m.object.get()||&b.script!=m.session.get()||
    &b.machine!=m.machine.get()||&b.runtime!=&m.runtime||
    &b.visual!=&m.animation->visual()||&b.scene!=&m.animation->scene())
  throw std::runtime_error("NPC physical composition requires the same retained actor graph");
 std::string error;
 if(!b.object_fields.visible_written&&!property_inputs.initialize(actor.room,actor.name,actor.character,b.object_fields))
  throw std::runtime_error(property_inputs.error());
 if(!b.model.ready()){
  auto bytes=read(assets,actor.model,"actors");
  if(!b.model.initialize({bytes.data(),bytes.size()},&m.animation->resources().factory(),error))
   throw std::runtime_error("Required actual NPC model-space physical resource: "+error);
 }
 // Project source PFObject constructor before its first physical/PF use. A
 // previously live PFObject is rejected rather than reset during GL reload.
 if(b.runtime.object.user||dh2_nav_object_defaults(&b.runtime.object))
  throw std::runtime_error("Required fresh source NPC PFObject lifetime");
 retained=std::make_unique<dh2::character::CharacterWorldNpcPhysicalLifecycleV2>(b,services);
 dh2::physical::NpcBodyRequest request{};request.properties=&m.session->property_view();request.ai=&b.ai_tables;
 std::copy(actor.rotation_degrees.begin(),actor.rotation_degrees.end(),request.rotation_degrees);
 // Retain a failed reached prefix for the caller's explicit close/diagnostic.
 if(!retained->initialize(request,actor.name.c_str(),"MonsterLight"))
  throw std::runtime_error("Required source NPC physical lifecycle: "+retained->error());
}
