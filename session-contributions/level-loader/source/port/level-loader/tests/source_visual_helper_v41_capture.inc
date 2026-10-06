
 auto helper_visual=containers.front()->visual();require(helper_visual&&helper_visual->ready(),"actual retained chest Visual is absent");
 require(helper_visual->source_colbox28_v40()==1&&helper_visual->marker().found==1,"actual source helper colbox/bounds flags missing");
 const auto helper_bounds_before=helper_visual->mesh_box();const auto retained_instances_before=helper_visual->scene().instances.size();
 std::uint32_t helper_node=UINT32_MAX;require(find_source_visual_helper_v40(helper_visual->scene(),helper_node,e)&&helper_node==helper_visual->marker().node_index,e);
 std::size_t helper_instance=helper_visual->scene().instances.size();
 for(std::size_t i=0;i<helper_visual->scene().instances.size();++i)if(helper_visual->scene().instances[i].node_index==helper_node)helper_instance=i;
 require(helper_instance<helper_visual->scene().instances.size()&&helper_visual->source_mesh_render_enabled_v40()[helper_instance]==0&&!(helper_visual->node_flags()[helper_node]&1u),"actual helper visibility/render fields were not suppressed by C1");
 assets::Mesh retained_helper_mesh{};require(dh2_mesh_open(&retained_helper_mesh,&helper_visual->bres(),helper_visual->scene().instances[helper_instance].geometry)==assets::Error::ok,"helper geometry was deleted instead of retained for bounds");
 const auto helper_key=containers.front()->receiver().base().shared_handle().key;
 GenericActorVisualInputV37 helper_input({bytes,&fixture.manager,roots,[&](const CanonicalObjectBorrowV1& object,ActorVisualProducerV37& out,std::string& e){
  if(object.identity!=containers.front()->receiver().base().identity()){e="Explicit host helper probe lacks another actual Visual lookup";return false;}
  return actor_visual_producer_v37(containers.front(),out,e);
 }});
 unsigned helper_draws=0;std::vector<std::string> helper_material_ids;
 auto helper_read=[&](const ActorDrawPrimitiveReadV38& draw,std::string&){
  ++helper_draws;require(draw.instance_index!=helper_instance&&draw.skin&&draw.position_space==ActorPositionSpaceV38::retained_skin_world,"suppressed helper was drawn or actual skin world space lost");
  require(draw.material_binding==ActorMaterialBindingV38::original_assigned_slot&&draw.bound_material==&draw.actor.scene.materials[draw.instance.materials[draw.primitive_index]],"actual draw did not use original material-slot assignment");
  require(&draw.actor.source_mesh_render_enabled==&helper_visual->source_mesh_render_enabled_v40()&&&draw.cached_world==&helper_visual->scene().instances[draw.instance_index].world,"draw detached actual source render field/world matrix");
  helper_material_ids.push_back(draw.bound_material->id);return true;
 };
 ActorDrawSubmissionResultV38 helper_result;require(submit_generic_actor_draw_v38(helper_input,helper_key,helper_read,helper_result,e),e);
 require(helper_result.visible_instances==1&&helper_result.hidden_instances==1&&helper_result.primitives==2&&helper_result.static_primitives==0&&helper_result.skinned_primitives==2&&helper_result.unresolved_materials==0&&helper_draws==2,"actual source chest should submit exactly two skinned primitives and no white helper");
 require(helper_material_ids==std::vector<std::string>{"Material__37","Material__38"},"actual source chest material catalog differs");
 // Prove independent source render-byte gate survives an explicit visibility
 // override; restore the actual C1 helper state afterward. No name filter.
 std::uintptr_t helper_handle{};require(helper_visual->node_from_name(helper_visual->scene().graph[helper_node].name.c_str(),helper_handle,e)&&helper_handle,e);
 require(helper_visual->set_node_local_visibility(helper_handle,true,e)&&(helper_visual->node_flags()[helper_node]&1u),e);
 require(submit_generic_actor_draw_v38(helper_input,helper_key,helper_read,helper_result,e)&&helper_result.primitives==2&&helper_result.hidden_instances==1,"render byte138 source gate was replaced with visibility-only/name filtering");
 require(helper_visual->set_node_local_visibility(helper_handle,false,e),e);
 require(helper_visual->scene().instances.size()==retained_instances_before&&helper_visual->mesh_box()==helper_bounds_before&&helper_visual->marker().found==1,"helper suppression damaged original bounds/geometry ownership");
 std::cerr<<"SOURCE_VISUAL_HELPER_V41 PASS retained_instances="<<retained_instances_before<<" visible_instances=1 hidden_helper=1 primitives=2 native_skinned=2 rigid=0 unresolved_materials=0 helper_bounds_preserved=1 independent_render_byte_guard=1 whole_level_init=0 backend_rendering=0\n";
