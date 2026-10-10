//Included after the existing actual driver/config/shader-pack owners, inside
//the SAME model_renderer private GPU domain. No alternate Scene is created.
Matrix transpose_batch_v113(const Matrix& m){Matrix out;for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c)out[c*4+r]=m[r*4+c];return out;}
bool inverse_batch_v113(const Matrix& m,Matrix& out,std::string& e){
 //Native uniform transport: algebra over the actual submitted matrices;
 //does not publish a source transform or claim original inverse instruction parity.
 float a[4][8]{};for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){a[r][c]=m[c*4+r];a[r][c+4]=r==c?1.f:0.f;}
 for(unsigned c=0;c<4;++c){unsigned row=c;for(unsigned r=c+1;r<4;++r)if(std::abs(a[r][c])>std::abs(a[row][c]))row=r;
  if(!std::isfinite(a[row][c])||a[row][c]==0.f){e="Actual shader matrix is singular";return false;}if(row!=c)for(unsigned k=0;k<8;++k)std::swap(a[row][k],a[c][k]);
  const float pivot=a[c][c];for(float& v:a[c])v/=pivot;for(unsigned r=0;r<4;++r)if(r!=c){const float weight=a[r][c];for(unsigned k=0;k<8;++k)a[r][k]-=weight*a[c][k];}
 }
 for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){const auto value=a[r][c+4];if(!std::isfinite(value)){e="Actual inverse shader matrix overflow";return false;}out[c*4+r]=value;}e.clear();return true;
}
void release_native_batch_shader_v113(Draw& draw,bool lost){
 if(!draw.native_shader_v113)return;for(auto name:draw.native_shader_v113->attributes)release_buffer_v41(name,lost);draw.native_shader_v113.reset();
}
bool update_native_batch_attributes_v113(const dh2::world::NativeBatchPartV111& part,Draw& draw,std::string& e)try{
 if(!draw.native_shader_v113||!draw.native_shader_v113->program){e="Actual compiled shader owner absent";return false;}auto& gpu=*draw.native_shader_v113;
 for(const auto& binding:gpu.program->attributes()){
  const auto slot=binding.source_slot;const auto& stream=part.attributes.at(slot);const auto n=part.vertices.size();
  const bool default_color=slot==2&&dh2::world::native_batch_default_color_v113(part);
  if(!default_color&&(stream.values.size()!=n||!stream.components||stream.components>4)){
   e="Actual selected shader "+part.pass_v112.vertex_file+" requires absent source attribute "+std::to_string(slot)+
    " (node="+std::to_string(part.source_node)+", primitive="+std::to_string(part.source_primitive_v111)+")";return false;
  }
  dh2::resources::CpuVectorCapacityV41 charge;std::vector<std::array<float,4>> values;
  if(!dh2::resources::reserve_cpu_vector_v41(values,charge,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,n,e))return false;values.resize(n);
  const auto width=slot==0?3u:default_color?4u:stream.components;
  for(std::size_t i=0;i<n;++i){if(default_color)std::copy_n(part.vertices[i].color,4,values[i].data());
   else{values[i]=stream.values[i];if(slot==0)std::copy_n(part.vertices[i].position,3,values[i].data());
    else if(slot==1&&!part.quantized_normal.empty()){for(unsigned k=0;k<3;++k)values[i][k]=float(part.quantized_normal.at(i)[k])/32767.f;}
    else if(slot==4&&!part.quantized_uv.empty())std::copy_n(part.vertices[i].uv,2,values[i].data());
    else if(!stream.quantized_components.empty()){for(unsigned k=0;k<width;++k)values[i][k]=float(stream.quantized_components.at(i)[k])*stream.quantized_scale[k]+stream.quantized_offset[k];}
    else if((slot==2||slot==3)&&stream.source_type==1){for(unsigned k=0;k<width;++k)values[i][k]/=255.f;}
   }
  }
  if(gpu.attributes[slot])buffer_subdata_v41(gpu.attributes[slot],GL_ARRAY_BUFFER,buffer_bytes_v41(values.size(),sizeof(values[0])),values.data(),dh2::resources::ResourceScopeV37::world);
  else buffer_storage_v41(gpu.attributes[slot],GL_ARRAY_BUFFER,buffer_bytes_v41(values.size(),sizeof(values[0])),values.data(),part.dynamic_v112?GL_DYNAMIC_DRAW:GL_STATIC_DRAW,dh2::resources::ResourceScopeV37::world);
  gpu.widths[slot]=width;
 }
 e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
bool prepare_native_batch_shader_v113(const dh2::world::NativeBatchPartV111& part,Draw& draw,std::string& e)try{
 auto& gpu=source_geometry_gpu_v64;if(!part.material_values_v113||!gpu.assets||!gpu.original){e="Actual compiled material/APK owner absent";return false;}
 if(effect_shader_pack_v4.empty()){effect_shader_pack_v4=read(gpu.assets,"shaders.pak","shaders");}
 dh2::scene::ShaderSourcePack pack;if(!pack.load(effect_shader_pack_v4.data(),effect_shader_pack_v4.size(),e))return false;
 if(!effect_shader_config_v4.config()){dh2::textures::ShaderConfigFileServicesV1 files;files.read=[&](const char* name,bool& found,std::string& value,std::string& e){const auto* member=pack.find(name);found=member!=nullptr;if(found)value.assign(reinterpret_cast<const char*>(member->bytes.data()),member->bytes.size());e.clear();return true;};if(!effect_shader_config_v4.initialize(files,e))return false;}
 const auto& pass=part.pass_v112;const auto* config=effect_shader_config_v4.config();
 //Length prefixes keep distinct authored source components/configurations
 //distinct in the shared original program cache.
 std::string key;for(const auto* text:{&pass.vertex_file,&pass.vertex_defines,&pass.fragment_file,&pass.fragment_defines})key+=std::to_string(text->size())+":"+*text;
 key+=std::to_string(effect_driver_options_v4.flags88())+":"+(config?*config:std::string("<not-produced>"));
 auto found=gpu.batch_programs_v113.find(key);if(found==gpu.batch_programs_v113.end()){auto program=std::make_shared<NativeBatchProgramV113>();if(!program->initialize(pass,pack,effect_driver_options_v4.flags88(),config,dh2::android_resources::budget_lease_v39(),e))return false;found=gpu.batch_programs_v113.emplace(std::move(key),std::move(program)).first;}
 draw.native_shader_v113=std::make_shared<NativeBatchGpuStateV113>();auto& state=*draw.native_shader_v113;state.program=found->second;state.material=part.material_values_v113;state.lights=part.material_lights_v113;
 GLint limit{};glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&limit);if(glGetError()!=GL_NO_ERROR||limit<0){e="Actual fragment sampler capability query failed";return false;}
 for(const auto& u:state.program->uniforms())if(u.gl_type==GL_SAMPLER_2D||u.gl_type==GL_SAMPLER_CUBE){
  if(u.gl_type!=GL_SAMPLER_2D){e="Actual selected cube-sampler image producer required";return false;}
  auto name=u.name;const auto bracket=name.find('[');if(bracket!=std::string::npos)name.resize(bracket);const auto binding=state.material->sampler_bindings.find(name);
  if(binding==state.material->sampler_bindings.end()){e="Actual selected pass sampler binding absent: "+name;return false;}
  const dh2::world::NativeBatchMaterialValueV113* value{};const auto instance=state.material->find(binding->second);if(instance!=state.material->end())value=&instance->second;else{const auto defaults=state.material->effect_defaults.find(binding->second);if(defaults!=state.material->effect_defaults.end())value=&defaults->second;}
  if(!value||value->source_type!=11||value->images.size()!=u.count||state.sampler_textures.size()+u.count>static_cast<std::size_t>(limit)){e="Actual sampler material images/count/capability mismatch";return false;}
  auto& units=state.sampler_units[u.name];for(const auto& image:value->images){units.push_back(static_cast<std::int32_t>(state.sampler_textures.size()));state.sampler_textures.push_back(image.empty()?0:upload(gpu.assets,image,gpu.texture_cache,gpu.textures,gpu.original.get()));}
 }
 if(!update_native_batch_attributes_v113(part,draw,e))return false;e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
bool submit_native_batch_shader_v113(const Draw& draw,const Matrix& wvp,std::string& e)try{
 if(!draw.native_shader_v113||!draw.native_shader_v113->program||!draw.source_pass_v112){e="Actual compiled shader/pass owner absent";return false;}
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_runtime_v61(actual,e)||!same_source_geometry_v64(actual)||!actual.roots)return false;
 auto& state=*draw.native_shader_v113;const auto& frame=actual.roots->frame_fields_v67();
 const Matrix identity{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
 //The compiled vertex stream is already world-space, so World is the actual
 //identity transform of this compiled node, not an invented actor pose.
 std::array<Matrix,57> matrices{};matrices[35]=draw.local_world_v124?draw.placement:identity;matrices[36]=submitted_view;matrices[37]=submitted_projection_v113;
 matrices[38]=draw.local_world_v124?dh2::scene::multiply(submitted_view,draw.placement):submitted_view;matrices[39]=wvp;
 for(auto pair:{std::pair<unsigned,unsigned>{40,35},{41,36},{42,39},{43,38},{44,39},{54,37}})if(!inverse_batch_v113(matrices[pair.second],matrices[pair.first],e))return false;
 for(auto pair:{std::pair<unsigned,unsigned>{45,40},{46,41},{47,42},{48,43},{49,44},{50,35},{51,36},{52,38},{53,39},{55,37},{56,54}})matrices[pair.first]=transpose_batch_v113(matrices[pair.second]);
 std::vector<float> float_cells;std::vector<std::int32_t> int_cells;std::array<float,4> light_cells{};std::array<float,4> fog_color;
 for(unsigned i=0;i<4;++i)fog_color[i]=float(frame.fog_color[i])/255.f;
 const auto* material=state.material.get();
 auto read_uniform=[&](const dh2::scene::ShaderUniformReflectionV4& u,NativeBatchUniformValueV113& out,std::string& e){
  out.elements=u.count;
  if(u.gl_type==GL_SAMPLER_2D||u.gl_type==GL_SAMPLER_CUBE){auto found=state.sampler_units.find(u.name);if(found==state.sampler_units.end()){e="Retired actual sampler binding";return false;}out.integers=found->second.data();return true;}
  if(u.count==1&&u.semantic>=35&&u.semantic<=56){const auto& matrix=matrices[u.semantic];if(u.gl_type==GL_FLOAT_MAT4){out.floats=matrix.data();return true;}const unsigned width=u.gl_type==GL_FLOAT_MAT3?3:u.gl_type==GL_FLOAT_MAT2?2:0;if(!width){e="Actual reflected matrix uniform shape differs";return false;}float_cells.resize(width*width);for(unsigned c=0;c<width;++c)for(unsigned r=0;r<width;++r)float_cells[c*width+r]=matrix[c*4+r];out.floats=float_cells.data();return true;}
  if(u.count==1&&u.semantic==58){if(u.gl_type==GL_FLOAT_VEC3){out.floats=submitted_eye.data();return true;}if(u.gl_type==GL_FLOAT_VEC4){light_cells={submitted_eye[0],submitted_eye[1],submitted_eye[2],1.f};out.floats=light_cells.data();return true;}e="Actual camera-position uniform shape differs";return false;}
  if(u.count==1&&u.semantic==28){out.floats=frame.ambient104.data();return true;}
  if(u.count==1&&u.semantic==30){out.floats=frame.fog_start_end.data();return true;}
  if(u.count==1&&u.semantic==31){out.floats=fog_color.data();return true;}
  if(u.count==1&&u.semantic>=19&&u.semantic<=27){
   if(!state.lights||u.sub_id>=4){e="Actual ApplySettings material light producer absent";return false;}auto light=state.lights->lights[u.sub_id];
   //Whole5b187c commitLightParameter: ignores enabled5a; NULL and semantic
   //outside19..26 return without a GL write, retaining SAME program values.
   if(!light||u.semantic==27){out.skip=true;return true;}
   if(light->native_destroyed_v113){e="Destroyed actual material CLight";return false;}
   switch(u.semantic){case 19:case 20:{std::array<float,16> m;if(!light->transformation50||!light->transformation50(m,e))return false;if(u.semantic==20||light->type58==2)light_cells={m[8],m[9],m[10],0.f};else light_cells={m[12],m[13],m[14],1.f};out.floats=light_cells.data();return true;}case 21:out.floats=light->attenuation34.data();return true;case 22:out.floats=light->ambient4.data();return true;case 23:out.floats=light->diffuse14.data();return true;case 24:out.floats=light->specular24.data();return true;case 25:out.floats=&light->exponent4c;return true;case 26:out.floats=&light->cutoff48;return true;}
  }
  if(!material){e="Actual material parameter owner retired";return false;}auto name=u.name;const auto bracket=name.find('[');if(bracket!=std::string::npos)name.resize(bracket);
  const dh2::world::NativeBatchMaterialValueV113* value{};auto local=material->find(name);if(local!=material->end())value=&local->second;
  if(!value){auto named=material->effect_defaults.find(name);if(named!=material->effect_defaults.end())value=&named->second;}
  if(!value&&u.semantic){for(const auto& entry:material->effect_defaults)if(entry.second.semantic==u.semantic){if(value){e="Ambiguous actual semantic material parameter";return false;}const auto override=material->find(entry.first);value=override==material->end()?&entry.second:&override->second;}}
  if(!value&&u.count==1){if(u.semantic==6){out.floats=draw.material.color;return true;}if(u.semantic==3){out.floats=draw.material.texture_matrix;return true;}if(u.semantic==10){out.floats=&draw.material.alpha_ref;return true;}}
  unsigned width{};switch(u.gl_type){case GL_FLOAT:case GL_INT:case GL_BOOL:width=1;break;case GL_FLOAT_VEC2:case GL_INT_VEC2:case GL_BOOL_VEC2:width=2;break;case GL_FLOAT_VEC3:case GL_INT_VEC3:case GL_BOOL_VEC3:width=3;break;case GL_FLOAT_VEC4:case GL_INT_VEC4:case GL_BOOL_VEC4:case GL_FLOAT_MAT2:width=4;break;case GL_FLOAT_MAT3:width=9;break;case GL_FLOAT_MAT4:width=16;break;}
  const auto elements=std::size_t(u.count)*width;
  if(value&&value->source_type==15){if((u.gl_type!=GL_FLOAT_VEC3&&u.gl_type!=GL_FLOAT_VEC4)||value->bytes.size()!=std::size_t(u.count)*4){e="Actual packed SColor source/uniform shape differs";return false;}float_cells.resize(elements);for(unsigned i=0;i<u.count;++i)for(unsigned c=0;c<width;++c)float_cells[i*width+c]=float(value->bytes[i*4+c])/255.f;out.floats=float_cells.data();return true;}
  if(value&&value->source_type==16){if((u.gl_type!=GL_FLOAT_VEC3&&u.gl_type!=GL_FLOAT_VEC4)||value->bytes.size()!=std::size_t(u.count)*16){e="Actual SColorf source/uniform shape differs";return false;}float_cells.resize(elements);for(unsigned i=0;i<u.count;++i)std::memcpy(float_cells.data()+i*width,value->bytes.data()+i*16,width*4);out.floats=float_cells.data();return true;}
  if(!value||!width||value->bytes.size()!=elements*4){e="Actual source uniform parameter not produced: "+u.name;return false;}
  if(u.gl_type==GL_INT||u.gl_type==GL_BOOL||u.gl_type==GL_INT_VEC2||u.gl_type==GL_INT_VEC3||u.gl_type==GL_INT_VEC4||u.gl_type==GL_BOOL_VEC2||u.gl_type==GL_BOOL_VEC3||u.gl_type==GL_BOOL_VEC4){int_cells.resize(elements);std::memcpy(int_cells.data(),value->bytes.data(),elements*4);out.integers=int_cells.data();}
  else{float_cells.resize(elements);std::memcpy(float_cells.data(),value->bytes.data(),elements*4);out.floats=float_cells.data();}return true;
 };
 if(!state.program->bind_uniforms(read_uniform,e))return false;
 const auto& pass=*draw.source_pass_v112;pass.blend?glEnable(GL_BLEND):glDisable(GL_BLEND);glBlendFunc(pass.blend_src,pass.blend_dst);glBlendEquation(pass.blend_equation);pass.depth?glEnable(GL_DEPTH_TEST):glDisable(GL_DEPTH_TEST);glDepthFunc(pass.depth_function);glDepthMask(pass.depth_write?GL_TRUE:GL_FALSE);pass.cull?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);glCullFace(pass.cull_face);glFrontFace(pass.front_face);
 for(std::size_t i=0;i<state.sampler_textures.size();++i){glActiveTexture(GL_TEXTURE0+static_cast<GLenum>(i));glBindTexture(GL_TEXTURE_2D,state.sampler_textures[i]);}
 glDisableVertexAttribArray(position);glDisableVertexAttribArray(texcoord);glDisableVertexAttribArray(color);
 for(const auto& attribute:state.program->attributes()){glBindBuffer(GL_ARRAY_BUFFER,state.attributes.at(attribute.source_slot));glEnableVertexAttribArray(attribute.location);glVertexAttribPointer(attribute.location,state.widths.at(attribute.source_slot),GL_FLOAT,GL_FALSE,sizeof(std::array<float,4>),nullptr);}
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,draw.indices);glDrawElements(GL_TRIANGLES,draw.count,GL_UNSIGNED_SHORT,nullptr);
 for(const auto& attribute:state.program->attributes())glDisableVertexAttribArray(attribute.location);
 const auto error=glGetError();if(error!=GL_NO_ERROR){e="Original batch shader submission GLES error "+std::to_string(error);return false;}e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
