// Included after load_monster_resources. This is retained ownership migration
// of the existing DACT pipeline; whole XML Character::InitPost is separate.
std::shared_ptr<MonsterScriptHandle> construct_retained_monster_actor_v1(
 AAssetManager* assets,const std::shared_ptr<WorldScriptContext>& context,
 const ObjectActor& actor,dh2::data::Bytes common,
 const std::vector<std::uint8_t>& monster_bytes,dh2::floors::World* candidate_floor){
 const auto& catalog=dh2::world::canonical_factories_v1();
 const auto entry=std::find_if(catalog.begin(),catalog.end(),[](const auto& item){return std::string(item.name)=="Character";});
 if(entry==catalog.end()||entry->original_address!=0x340800)
  throw std::runtime_error("Original Character catalog constructor unavailable");
 auto handle=std::make_shared<MonsterScriptHandle>(actor.identity,context,entry->name);
 std::string error;
 // The existing DACT world already owns this object/name. Adopt that real
 // record without pretending that this path executed canonical XML Add.
 auto object=context->objects->find(actor.identity);
 if(!object||object->properties!=actor.property_owner||object->life!=actor.combat_owner||object->name!=actor.name)
  throw std::runtime_error("Retained Character adopted different World authority");
 auto receiver=handle->canonical(handle);
 if(!receiver.set_name(receiver.context,object->name.c_str(),error))throw std::runtime_error(error);
 handle->diagnostics=std::make_unique<dh2::character::StateOwnerDebugDiagnostics>(*context->debug,context->debug_files);
 const int table=dh2_character_animation_table_id(actor.properties().resolved[2],int(actor_animation_tables.characters.size()));
 const auto* idle=dh2::data::animation_state(actor_animation_tables,table,"Idle");
 if(!idle)throw std::runtime_error("Source NPC Idle animation table unavailable");
 handle->facts.idle=int(idle-actor_animation_tables.sequences.data());
 dh2::character::WorldNpcStateServicesV1 services{};
 services.remaining_methods={handle.get(),MonsterScriptHandle::state_method};
 services.outer={handle.get(),MonsterScriptHandle::native_frame};
 services.diagnostics=handle->diagnostics.get();services.diagnostics_required=1;
 if(!handle->construct_graph(object,load_monster_resources(assets,*context,actor),services))
  throw std::runtime_error("Retained Character graph construction failed: "+handle->error());
 dh2::character::CharacterScriptSessionInput input{};
 input.common=common;input.external={monster_bytes.data(),monster_bytes.size()};
 if(!context->script_files.session_files("data/scripts/ai/monster.luac",input.include_files,error))
  throw std::runtime_error("Monster include resource setup failed: "+error);
 input.host=&context->host;input.level=&context->level_services;
 input.target=&object->binding;input.objects=&context->objects->services();
 RendererNpcCommandInputsV1 command_inputs{};command_inputs.context=handle.get();
 command_inputs.position=npc_command_position_v1;command_inputs.raise=npc_command_raise_v1;
 handle->script_commands=std::make_shared<RendererNpcCommandsV1>(*handle,command_inputs,candidate_floor);
 handle->script_commands->attach(input);
 if(!handle->construct_script(context->design->borrow(),input))
  throw std::runtime_error("Retained Monster script allocation failed: "+handle->error());
 if(handle->session->script_name()!="monster")throw std::runtime_error("Unbundled original character script requested");
 // Preserve the existing development pipeline's loading point. The canonical
 // InitPost path instead invokes this at its exact3cf1f0 service delivery.
 if(!handle->load_script())throw std::runtime_error("Monster script Init failed: "+handle->error());
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Retained Character graph | %s | identity %016llx | same World/props/life/target/FSM/controller/CPU animation/VM | canonical XML InitPost services pending",actor.name.c_str(),static_cast<unsigned long long>(actor.identity));
 return handle;
}
