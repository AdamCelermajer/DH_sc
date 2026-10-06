
 ActorDrawSubmissionResultV38 draw_result;unsigned draw_reads=0;std::vector<std::string> draw_material_symbols;
 auto actual_draw_read=[&](const ActorDrawPrimitiveReadV38& draw,std::string&){
  ++draw_reads;
  require(&draw.cached_world==&actual_chest_visual->scene().instances[draw.instance_index].world,"draw transport detached/recomputed actual world/scale");
  require(&draw.instance==&actual_chest_visual->scene().instances[draw.instance_index],"draw transport detached actual scene instance");
  require(draw.mesh.image.bytes==actual_chest_visual->bres().bytes,"draw transport detached actual primitive BRES");
  assets::Primitive original{};require(dh2_mesh_primitive(&draw.mesh,draw.primitive_index,&original)==assets::Error::ok,"actual source primitive unavailable");
  require(draw.primitive.indices==original.indices&&draw.primitive.index_width==original.index_width&&draw.primitive.minimum_index==original.minimum_index&&draw.primitive.maximum_index==original.maximum_index&&draw.primitive.engine_type==original.engine_type,"draw transport changed topology/index/range");
  if(draw.skin){require(draw.position_space==ActorPositionSpaceV38::retained_skin_world&&draw.skin->positions.size()==draw.mesh.vertices,"draw transport applied local matrix to actual world skin");}
  else require(draw.position_space==ActorPositionSpaceV38::raw_attribute_local,"draw transport changed rigid vertex transform semantics");
  if(draw.exact_bound_material){require(draw.material_binding==ActorMaterialBindingV38::exact_bound_symbol&&draw.exact_bound_material->id==draw.primitive.material,"draw transport invented material resolution");}
  draw_material_symbols.emplace_back(draw.primitive.material?draw.primitive.material:"");return true;
 };
 require(submit_generic_actor_draw_v38(actor_input,chest_key,actual_draw_read,draw_result,e)&&e.empty(),e);
 const auto actual_draw_primitives=draw_result.primitives;const auto actual_draw_unresolved=draw_result.unresolved_materials;
 const auto actual_draw_rigid=draw_result.static_primitives;const auto actual_draw_skinned=draw_result.skinned_primitives;
 require(draw_result.visible_instances==2&&draw_result.primitives>0&&draw_reads==draw_result.primitives,"actual chest prefix draw transport missing instances/primitives");
 const auto saved_draw_result=draw_result;
 require(!submit_generic_actor_draw_v38(actor_input,chest_key,{},draw_result,e)&&draw_result.primitives==saved_draw_result.primitives,"draw missing endpoint corrupted output");
 require(!submit_generic_actor_draw_v38(actor_input,chest_key,[](const ActorDrawPrimitiveReadV38&,std::string& e){e="Required actual backend/material pass resolver";return false;},draw_result,e)&&draw_result.primitives==saved_draw_result.primitives,"draw consumer failure corrupted output");
 require(actual_chest_visual->set_root_local_visibility_v3(false,e),e);
 const auto reads_before_hidden=draw_reads;
 require(submit_generic_actor_draw_v38(actor_input,chest_key,actual_draw_read,draw_result,e)&&draw_result.visible_instances==0&&draw_result.hidden_instances==2&&draw_result.primitives==0&&draw_reads==reads_before_hidden,"draw ignored actual root visibility");
 require(actual_chest_visual->set_root_local_visibility_v3(true,e),e);
 require(actual_chest_visual->commit_root_visibility_v3([roots](std::string&){roots->notify_visibility_changed_v3();return true;},e),e);
 const auto first_node=actual_chest_visual->scene().instances.front().node_index;
 const auto& first_node_name=actual_chest_visual->scene().graph[first_node].name;
 std::uintptr_t first_node_handle{};require(actual_chest_visual->node_from_name(first_node_name.c_str(),first_node_handle,e)&&first_node_handle,e);
 require(actual_chest_visual->set_node_local_visibility(first_node_handle,false,e),e);
 require(submit_generic_actor_draw_v38(actor_input,chest_key,actual_draw_read,draw_result,e)&&draw_result.hidden_instances>0&&draw_result.visible_instances<2,"draw ignored actual mesh-node visibility");
 require(actual_chest_visual->set_node_local_visibility(first_node_handle,true,e),e);
 require(submit_generic_actor_draw_v38(actor_input,npc_key,actual_draw_read,draw_result,e)&&draw_result.visual==ActorVisualConsumptionV37::no_visual_published&&draw_result.primitives==0,"draw fabricated fresh Character Visual");
 std::cerr<<"GENERIC_ACTOR_DRAW_V38 PASS chest_instances=2 primitives="<<actual_draw_primitives<<" rigid="<<actual_draw_rigid<<" skinned="<<actual_draw_skinned<<" unresolved_materials="<<actual_draw_unresolved<<" authored_cached_world=1 source_ranges=1 root_visibility=1 node_visibility=1 NULL_visual=1 backend_failure_preserves_out=1 gameplay_ready=0\n";
