// Include inside model_renderer after existing GPU/equipment helpers. This is
// the modern GL submission adapter for the recovered inventory camera/pose;
// it borrows the sole player visual and current equipment, never advances AI.
namespace {
bool source_inventory_geometry_v70(const dh2::visual::Root*&,
 const std::vector<Draw>*&,std::shared_ptr<void>&,std::string&);
}
bool draw_character_inventory_preview_v4(int width,int height,std::string& error){
 const bool campaign=source_campaign_active_v55();
 if(width<=0||height<=0||!program||(!campaign&&(!world_mode||!enabled||!native_actor_ready||!player_equipment||!player_equipment->ready()||!equipment_platform))){
  error="Inventory avatar requires the same initialized live player/Gear/visual";return false;
 }
 try{
  struct State {
   GLint program{},array{},element{},active{},texture[2]{},depth_function{},front{},cull_mode{},blend_source_rgb{},blend_dest_rgb{},blend_source_alpha{},blend_dest_alpha{};
   GLboolean depth{},blend{},cull{},depth_mask{};
   struct Attribute {GLint enabled{},buffer{},size{},type{},normalized{},stride{};void* pointer{};} attributes[3];
   GLint indices[3];
   State(GLint a,GLint b,GLint c):indices{a,b,c}{
    glGetIntegerv(GL_CURRENT_PROGRAM,&program);glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&array);glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING,&element);glGetIntegerv(GL_ACTIVE_TEXTURE,&active);
    for(unsigned i=0;i<2;++i){glActiveTexture(GL_TEXTURE0+i);glGetIntegerv(GL_TEXTURE_BINDING_2D,&texture[i]);}glActiveTexture(active);
    depth=glIsEnabled(GL_DEPTH_TEST);blend=glIsEnabled(GL_BLEND);cull=glIsEnabled(GL_CULL_FACE);glGetBooleanv(GL_DEPTH_WRITEMASK,&depth_mask);
    glGetIntegerv(GL_DEPTH_FUNC,&depth_function);glGetIntegerv(GL_FRONT_FACE,&front);glGetIntegerv(GL_CULL_FACE_MODE,&cull_mode);
    glGetIntegerv(GL_BLEND_SRC_RGB,&blend_source_rgb);glGetIntegerv(GL_BLEND_DST_RGB,&blend_dest_rgb);glGetIntegerv(GL_BLEND_SRC_ALPHA,&blend_source_alpha);glGetIntegerv(GL_BLEND_DST_ALPHA,&blend_dest_alpha);
    for(unsigned i=0;i<3;++i){auto& v=attributes[i];const GLuint n=GLuint(indices[i]);glGetVertexAttribiv(n,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&v.enabled);glGetVertexAttribiv(n,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,&v.buffer);glGetVertexAttribiv(n,GL_VERTEX_ATTRIB_ARRAY_SIZE,&v.size);glGetVertexAttribiv(n,GL_VERTEX_ATTRIB_ARRAY_TYPE,&v.type);glGetVertexAttribiv(n,GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,&v.normalized);glGetVertexAttribiv(n,GL_VERTEX_ATTRIB_ARRAY_STRIDE,&v.stride);glGetVertexAttribPointerv(n,GL_VERTEX_ATTRIB_ARRAY_POINTER,&v.pointer);}
   }
   ~State(){
    for(unsigned i=0;i<3;++i){const auto& v=attributes[i];glBindBuffer(GL_ARRAY_BUFFER,GLuint(v.buffer));glVertexAttribPointer(GLuint(indices[i]),v.size,GLenum(v.type),GLboolean(v.normalized),v.stride,v.pointer);if(v.enabled)glEnableVertexAttribArray(GLuint(indices[i]));else glDisableVertexAttribArray(GLuint(indices[i]));}
    glBindBuffer(GL_ARRAY_BUFFER,GLuint(array));glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,GLuint(element));
    for(unsigned i=0;i<2;++i){glActiveTexture(GL_TEXTURE0+i);glBindTexture(GL_TEXTURE_2D,GLuint(texture[i]));}glActiveTexture(GLenum(active));
    glDepthMask(depth_mask);glDepthFunc(GLenum(depth_function));glFrontFace(GLenum(front));glCullFace(GLenum(cull_mode));glBlendFuncSeparate(GLenum(blend_source_rgb),GLenum(blend_dest_rgb),GLenum(blend_source_alpha),GLenum(blend_dest_alpha));
    if(depth)glEnable(GL_DEPTH_TEST);else glDisable(GL_DEPTH_TEST);if(blend)glEnable(GL_BLEND);else glDisable(GL_BLEND);if(cull)glEnable(GL_CULL_FACE);else glDisable(GL_CULL_FACE);glUseProgram(GLuint(program));
   }
  } restore(position,texcoord,color);
  // The pane can open before the first world scene phase. Publish the SAME
  // root/current pose into graph and instance matrices before Gear skinning;
  // otherwise rebase below removes a root transform never applied to parts.
  // This only derives transforms: no animation clock, events, RNG or AI step.
  const dh2::visual::Root* avatar_root{};
  const std::vector<Draw>* avatar_draws{};std::shared_ptr<void> avatar_owner;
  if(campaign){
   if(!source_inventory_geometry_v70(avatar_root,avatar_draws,avatar_owner,error))return false;
  }else{
   if(!prince_visual.update_world(current_scene,error))return false;
   const std::vector<dh2::skinning::VisualDrawViewV32>* parts{};
   if(!player_equipment->draw_views(parts,error))return false;
   sync_equipment_draws(*parts,equipment_platform->manager,equipment_platform->cache);
   avatar_root=&prince_visual.root;avatar_draws=&equipment_draws;
  }
  if(!avatar_root||!avatar_draws||avatar_draws->empty()){error="Same live equipment has no inventory avatar geometry";return false;}
  // Source CreateAvatarCamera452910..452aac: eye(0,-800,200),
  // target(0,0,200), up(0,0,1), near10/far1000; pane aspect is updated
  // by RenderCharacterPane452578..4525c4. Preserve exact authored FOV bits.
  std::uint32_t fov_bits=0x3f0efb1au;float fov{};std::memcpy(&fov,&fov_bits,4);
  const float cotangent=1.f/std::tan(fov*.5f),aspect=float(width)/float(height),near=10.f,far=1000.f;
  Matrix view{1,0,0,0,0,0,-1,0,0,1,0,0,0,-200,-800,1};
  Matrix perspective{cotangent/aspect,0,0,0,0,cotangent,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};
  const auto camera=dh2::scene::multiply(perspective,view);
  const auto& root=*avatar_root;
  dh2::math::Quaternion original{root.quaternion[0],root.quaternion[1],root.quaternion[2],root.quaternion[3]};dh2::math::Vector3f degrees{};
  dh2_quat_to_euler_degrees(&original,&degrees);
  // RenderCharacterPane4526b8..452728: actual root Euler X/Y converted
  // with original float degree factor3c8efa35; Z is source literal -0.5.
  std::uint32_t degree_bits=0x3c8efa35u;float degree{};std::memcpy(&degree,&degree_bits,4);
  dh2::math::Quaternion preview{};dh2_quat_from_euler(&preview,degrees.x*degree,degrees.y*degree,-.5f);
  dh2::math::Matrix4f original_matrix{},preview_matrix{};dh2_quat_matrix_transposed(&original,&original_matrix);dh2_quat_matrix_transposed(&preview,&preview_matrix);
  Matrix inverse{};inverse[15]=1;
  for(unsigned i=0;i<3;++i){if(!std::isfinite(root.scale[i])||root.scale[i]==0){error="Same live avatar root scale is singular";return false;}
   for(unsigned j=0;j<3;++j){inverse[j*4+i]=original_matrix.m[i*4+j]/root.scale[i];preview_matrix.m[i*4+j]*=root.scale[i];}
   inverse[12+i]=-(inverse[i]*root.position[0]+inverse[4+i]*root.position[1]+inverse[8+i]*root.position[2]);
  }
  Matrix preview_world{};std::copy(preview_matrix.m,preview_matrix.m+16,preview_world.begin());
  const auto rebase=dh2::scene::multiply(preview_world,inverse);
  glUseProgram(program);glUniform1i(glGetUniformLocation(program,"diffuse"),0);glUniform1i(glGetUniformLocation(program,"alpha_map"),1);
  glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glEnable(GL_BLEND);glEnableVertexAttribArray(position);glEnableVertexAttribArray(texcoord);glEnableVertexAttribArray(color);
  for(const auto& draw:*avatar_draws){
   const auto transform=dh2::scene::multiply(camera,dh2::scene::multiply(rebase,draw.placement));glUniformMatrix4fv(mvp,1,GL_FALSE,transform.data());
   draw.material.backface?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);glCullFace(GL_BACK);glFrontFace(GL_CCW);glBlendFunc(GL_SRC_ALPHA,draw.material.additive?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);glDepthMask(draw.material.additive?GL_FALSE:GL_TRUE);
   glUniformMatrix4fv(texture_matrix,1,GL_FALSE,draw.material.texture_matrix);glUniform4fv(material_color,1,draw.material.color);glUniform1f(has_alpha,draw.material.alpha_map.empty()?0:1);glUniform1f(alpha_ref,draw.material.alpha_ref);
   const bool blue=draw.material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&draw.material.gles2_technique=="L1_Vc_Al_----_----_----_----";glUniform1f(original_alpha_blue,blue?1:0);
   glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,draw.diffuse);glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,draw.alpha);glBindBuffer(GL_ARRAY_BUFFER,draw.vertices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,draw.indices);
   glVertexAttribPointer(position,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,p)));glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,uv)));glVertexAttribPointer(color,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,color)));glDrawElements(GL_TRIANGLES,draw.count,GL_UNSIGNED_SHORT,nullptr);
  }
  return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
