  if(class_scene||(menu_background&&menu_persona_class>=0))for(auto& actor:class_preview_actors){
   if(menu_background)actor.sample_elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-menu_persona_epoch).count();
   auto& resource=actor.resource;const auto& clip=resource.animation;std::string error;
   const auto ms=clip.start+int(actor.sample_elapsed%unsigned(clip.end-clip.start));
   if(!dh2::objects::sample(resource,ms,error))throw std::runtime_error("Class body animation failed: "+error);
   // Original main SetupCharacter position (0,-200,-20). The menu-only
   // body uses its authored idle and starting armor; saved equipment pending.
   // SetupCharacter quaternion::set(0,0,PI * -0.125), source PI float.
   const float menu_angle=3.1415927410125732f * -.125f;
   Matrix placement=menu_background?Matrix{std::cos(menu_angle),std::sin(menu_angle),0,0,-std::sin(menu_angle),std::cos(menu_angle),0,0,0,0,1,0,0,-200,-20,1}:current_scene.graph.at(actor.anchor).world;
   for(unsigned column=0;column<3;++column)for(unsigned row=0;row<4;++row)placement[column*4+row]*=actor.scale[column];
   const auto owner_projection=dh2::scene::multiply(projection,placement);
   for(unsigned i=0;i<actor.draws.size();++i){const auto& primitive=resource.primitives[i];auto& batch=actor.draws[i];
    buffer_subdata_v41(batch.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(primitive.vertices.size(),sizeof(Vertex)),primitive.vertices.data(),dh2::resources::ResourceScopeV37::actor);
    const auto transform=primitive.skin.nodes.empty()?dh2::scene::multiply(owner_projection,resource.scene.graph.at(primitive.node).world):owner_projection;
    submit(batch,transform);
   }
   for(const auto& weapon:actor.weapons){
    const auto attachment=dh2::scene::multiply(owner_projection,resource.scene.graph.at(weapon.anchor).world);
    for(const auto& batch:weapon.draws){
     const auto transform=dh2::scene::multiply(attachment,weapon.resource.scene.graph.at(batch.node).world);submit(batch,transform);
    }
   }
  }
