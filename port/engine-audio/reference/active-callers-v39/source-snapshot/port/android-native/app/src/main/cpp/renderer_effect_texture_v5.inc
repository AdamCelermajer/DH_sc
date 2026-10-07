// Include after EffectGpuTextureV4/maps/options declarations, replacing the
// old effect_texture_v4 implementation. Dedicated adapter driver; generic
// renderer bindings are external and NEVER published as invented receivers.
dh2::textures::TextureBindingOwnerV1 effect_texture_grid_v5;
dh2::textures::TextureGlCacheFieldsV1 effect_texture_cache_v5;
std::uint32_t effect_texture_units_v5{};
dh2::textures::TextureBindingServicesV1 effect_binding_services_v5(){
 dh2::textures::TextureBindingServicesV1 s;
 s.active_texture=[](auto value,std::string&){glActiveTexture(value);return true;};
 s.bind_texture=[](auto target,auto name,std::string&){glBindTexture(target,name);return true;};
 s.update=dh2::textures::texture_clean_update_false_v1;return s;
}
dh2::textures::TextureBindingDriverBorrowV1 effect_texture_driver_v5(){return {&effect_texture_units_v5,&effect_texture_cache_v5.active_unit268};}
// Cross-domain boundary: restore actual generic GL bindings, clear the source
// borrowed grid FIRST, then synchronize the actual driver caches. A restored
// unknown generic GLuint is not a canonical CTexture receiver.
struct EffectTextureStateGuardV5 {
 dh2::textures::TextureBindingOwnerV1& effect_texture_grid_v5;
 dh2::textures::TextureGlCacheFieldsV1& effect_texture_cache_v5;
 std::uint32_t& effect_texture_units_v5;
 GLint active{},alignment{};std::array<GLint,8> textures{};
 EffectTextureStateGuardV5(dh2::textures::TextureBindingOwnerV1& grid,dh2::textures::TextureGlCacheFieldsV1& cache,std::uint32_t& units_ref):effect_texture_grid_v5(grid),effect_texture_cache_v5(cache),effect_texture_units_v5(units_ref){
  GLint units=0;glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&units);
  effect_texture_units_v5=std::min(std::uint32_t(units),8u);
  if(!effect_texture_units_v5)throw std::runtime_error("Actual source GLES texture unit count is zero");
  glGetIntegerv(GL_ACTIVE_TEXTURE,&active);glGetIntegerv(GL_UNPACK_ALIGNMENT,&alignment);
  for(std::uint32_t i=0;i<effect_texture_units_v5;++i){glActiveTexture(GL_TEXTURE0+i);glGetIntegerv(GL_TEXTURE_BINDING_2D,&textures[i]);}
  glActiveTexture(active);effect_texture_cache_v5.active_unit268=std::uint32_t(active-GL_TEXTURE0);
  effect_texture_cache_v5.unpack_alignment26c=std::uint32_t(alignment);
  if(effect_texture_cache_v5.active_unit268>=effect_texture_units_v5)throw std::runtime_error("Actual active GL texture unit outside source grid");
 }
 ~EffectTextureStateGuardV5(){
  const dh2::textures::TextureBindingDriverBorrowV1 driver{&effect_texture_units_v5,&effect_texture_cache_v5.active_unit268};const dh2::textures::TextureBindingServicesV1 services{};std::string ignored;
  for(std::uint32_t i=0;i<effect_texture_units_v5;++i){
   effect_texture_grid_v5.set_texture(i,{},0,driver,services,ignored);
   glActiveTexture(GL_TEXTURE0+i);glBindTexture(GL_TEXTURE_2D,GLuint(textures[i]));
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT,alignment);glActiveTexture(active);
  effect_texture_cache_v5.active_unit268=std::uint32_t(active-GL_TEXTURE0);
  effect_texture_cache_v5.unpack_alignment26c=std::uint32_t(alignment);
 }
};
bool release_effect_texture_v5(EffectGpuTextureV4& r,bool discard_context,bool upload_error,std::string& error){
 auto& fields=r.owner().fields();
 const dh2::textures::TextureBindingReceiverV1 receiver{&r,&fields.flags3f,&fields.dirty40,&r.texture};
 if(discard_context){
  // Lost GL context has no callable original destructor GL delivery. Discard
  // only the adapter's borrowed grid; do not issue GL calls into a new context.
  const auto driver=effect_texture_driver_v5();const auto services=effect_binding_services_v5();
  for(std::uint32_t i=0;i<effect_texture_units_v5;++i)
   if(effect_texture_grid_v5.slot(0,i)->identity==&r&&!effect_texture_grid_v5.set_texture(i,{},0,driver,services,error))return false;
  r.owner().context_lost();r.texture=0;return true;
 }
 dh2::textures::TextureUpload2dBorrowV1 data;
 if(!r.image.upload_borrow(&effect_texture_cache_v5.unpack_alignment26c,data,error))return false;
 dh2::textures::TextureUnbindServicesV1 services;
 services.delete_texture=[](auto name,std::string&){glDeleteTextures(1,&name);return true;};
 const bool result=dh2::textures::texture_unbind_2d_v1(effect_texture_grid_v5,receiver,fields,r.texture,
  effect_texture_driver_v5(),effect_binding_services_v5(),data.pending_bits,data.pending_word_count,services,upload_error,error);
 r.owner().context_lost();return result;
}
std::shared_ptr<EffectGpuTextureV4> effect_texture_v4(const std::string& name,
 dh2::android_ui::OriginalCacheAssetsV1& cache){
 if(name.empty())throw std::runtime_error("Required authored effect texture image");
 const auto found=effect_gpu_textures_v4.find(name);if(found!=effect_gpu_textures_v4.end())return found->second;
 std::vector<std::uint8_t> bytes;std::string error;bool present=false;
 if(!cache.read("data/3d/textures/"+name,present,bytes,error)||!present)throw std::runtime_error("Required actual effect texture "+name+": "+error);
 const auto* extensions=glGetString(GL_EXTENSIONS);if(!extensions)throw std::runtime_error("Required owning GLES context extensions");
 dh2::textures::Gles2TextureSamplerFeaturesV1 features;
 if(!dh2::textures::texture_gles2_sampler_features_v1(reinterpret_cast<const char*>(extensions),
  [](auto pname,float& value,std::string&){glGetFloatv(pname,&value);check("Actual source texture feature query");return true;},features,error))throw std::runtime_error(error);
 auto r=std::make_shared<EffectGpuTextureV4>();
 if(!r->image.initialize(std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes)),features,{},&effect_driver_options_v4.flags88(),
  [](auto from,auto to,std::string&){__android_log_print(ANDROID_LOG_INFO,"DH2Native","Actual effect image decoded to native RGBA | %u -> %u",from,to);return true;},error))throw std::runtime_error("Actual effect texture "+name+": "+error);
 EffectTextureStateGuardV5 restore(effect_texture_grid_v5,effect_texture_cache_v5,effect_texture_units_v5);
 // Acquire sole mutable fields BEFORE update; reacquiring would invalidate
 // its parameter receipt during virtual20.
 auto& fields=r->owner().fields();
 const dh2::textures::TextureBindingReceiverV1 receiver{r.get(),&fields.flags3f,&fields.dirty40,&r->texture};
 try{
  dh2::textures::TextureInitialBindServicesV1 initial;
  initial.generate_texture=[](auto& name,std::string&){glGenTextures(1,&name);return true;};
  const auto binding=effect_binding_services_v5();initial.active_texture=binding.active_texture;initial.bind_texture=binding.bind_texture;
  if(!effect_texture_grid_v5.initial_bind_impl_prefix(receiver,fields,r->texture,effect_texture_driver_v5(),initial,error))throw std::runtime_error(error);
  dh2::textures::TextureParameterServicesV1 parameters;
  parameters.integer=[](auto target,auto pname,auto value,std::string&){glTexParameteri(target,pname,value);return true;};
  parameters.scalar=[](auto target,auto pname,float value,std::string&){glTexParameterf(target,pname,value);return true;};
  parameters.format_flags=dh2::textures::general_fx_texture_format_flags_v2;
  parameters.compressed_filter_diagnostic=[](auto format,std::string&){__android_log_print(ANDROID_LOG_WARN,"DH2Native","Source compressed filter downgrade | %u",format);return true;};
  if(!r->owner().update(r->image.driver(),parameters,error))throw std::runtime_error(error);
  dh2::textures::TextureUpload2dBorrowV1 payload;
  if(!r->image.upload_borrow(&effect_texture_cache_v5.unpack_alignment26c,payload,error))throw std::runtime_error(error);
  dh2::textures::TextureUpload2dServicesV1 upload;
  upload.image=[](auto target,auto mip,auto width,auto height,auto internal,const std::uint8_t* data,std::size_t size,bool compressed,auto format,auto type,std::string&){
   if(compressed)glCompressedTexImage2D(target,mip,internal,width,height,0,size,data);else glTexImage2D(target,mip,internal,width,height,0,format,type,data);return true;};
  upload.gl_error=[](std::uint32_t& value,std::string&){value=glGetError();return true;};
  upload.pixel_store=[](auto name,auto value,std::string&){glPixelStorei(name,value);return true;};
  upload.compressed_mipmap_diagnostic=[](std::string&){__android_log_print(ANDROID_LOG_WARN,"DH2Native","Source compressed effect skips mip generation");return true;};
  upload.generate_mipmaps=[&](std::string& e){
   dh2::textures::TextureMipmapServicesV1 s;
   s.bind=[&](auto unit,const void* identity,auto kind,std::string& why){if(identity!=r.get()){why="Changed virtual20 receiver";return false;}return effect_texture_grid_v5.set_texture(unit,receiver,kind,effect_texture_driver_v5(),binding,why);};
   s.active_texture=binding.active_texture;
   s.generate=[](auto target,std::string&){glGenerateMipmap(target);return true;};s.integer=parameters.integer;
   return r->owner().generate_mipmaps(r.get(),{&effect_texture_units_v5,&effect_texture_cache_v5.active_unit268},s,e);
  };
  if(!r->owner().upload(payload,upload,error)||!r->owner().ready())throw std::runtime_error("Source texture updateData: "+error);
 }catch(...){
  const auto exception=std::current_exception();std::string cleanup;
  // Source bindImpl only republish10 if updateData produced error10. Adapter
  // missing-provider/exception still releases genuine allocated GL lifetime.
  const bool source_upload_error=(fields.flags3f&0x10)!=0;
  if(r->texture&&!release_effect_texture_v5(*r,false,source_upload_error,cleanup))
   throw std::runtime_error("Source texture cleanup failed: "+cleanup);
  std::rethrow_exception(exception);
 }
 effect_gpu_textures_v4.emplace(name,r);return r;
}
