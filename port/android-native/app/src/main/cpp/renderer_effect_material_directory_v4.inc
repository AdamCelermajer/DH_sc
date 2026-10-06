// New-only integration helpers. Include after GLES2/gl2.h, shader_program_
// collection_v4.hpp, effect_material_directory_v4.hpp and engine-math/math.hpp.
// Caller exposes EffectDrawProgramV4's actual GLuint via a borrowed getter.
static bool reflect_effect_program_v4(GLuint program,
 std::vector<dh2::scene::ShaderUniformReflectionV4>& out,std::string& error){
 GLint linked=0,count=0,maximum=0;glGetProgramiv(program,GL_LINK_STATUS,&linked);
 if(!linked){error="Required linked source effect GL program";return false;}
 glGetProgramiv(program,GL_ACTIVE_UNIFORMS,&count);glGetProgramiv(program,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maximum);
 if(count<0||maximum<=0){error="Required actual GL uniform reflection bounds";return false;}
 std::vector<GLchar> name(std::size_t(maximum),0);
 std::vector<dh2::scene::ShaderUniformReflectionV4> pending;pending.reserve(std::size_t(count));
 for(GLint i=0;i<count;++i){GLsizei length=0;GLint size=0;GLenum type=0;
  glGetActiveUniform(program,GLuint(i),maximum,&length,&size,&type,name.data());
  if(length<0||length>=maximum||size<=0){error="Required valid reflected source GL uniform";return false;}
  dh2::scene::ShaderUniformReflectionV4 p;p.name.assign(name.data(),std::size_t(length));p.gl_type=type;p.count=std::uint32_t(size);p.location=glGetUniformLocation(program,p.name.c_str());
  if(p.location<0){error="Required active source GL uniform location";return false;}
  pending.push_back(std::move(p));
 }
 out=std::move(pending);return true;
}
struct EffectMaterialProfileBorrowV4 {
 const float* current_diffuse_color{};
 const dh2::math::Matrix4f* source_texture_matrix{};
 std::uintptr_t same_texture_owner{};
 const float* source_sampler_bias{};
};
static bool read_effect_profile_material_v4(void* context,
 const dh2::scene::ShaderUniformReflectionV4& uniform,
 dh2::scene::EffectMaterialValueBorrowV4& out,std::string& error){
 auto& m=*static_cast<EffectMaterialProfileBorrowV4*>(context);
 if(uniform.count!=1){error="Required actual single-element ProfileCOMMON material";return false;}
 if(uniform.semantic==6&&uniform.type==8&&m.current_diffuse_color){out.bytes=reinterpret_cast<const std::uint8_t*>(m.current_diffuse_color);out.byte_size=16;return true;}
 if(uniform.semantic==2&&uniform.type==12&&uniform.sub_id==0&&m.same_texture_owner){out.texture_identities=&m.same_texture_owner;out.texture_count=1;return true;}
 if(uniform.semantic==33&&uniform.type==5&&uniform.sub_id==0&&m.source_sampler_bias){out.bytes=reinterpret_cast<const std::uint8_t*>(m.source_sampler_bias);out.byte_size=4;return true;}
 // Matrix pointer-array borrow must survive the synchronous read callback.
 // The profile adapter below passes a stable member array for this branch.
 error="Required current ProfileCOMMON material value: "+uniform.name;return false;
}
struct EffectMaterialProfileContextV4 {
 EffectMaterialProfileBorrowV4 borrowed;
 const std::uint8_t* matrix_pointer{};
 static bool read(void* context,const dh2::scene::ShaderUniformReflectionV4& u,
 dh2::scene::EffectMaterialValueBorrowV4& out,std::string& error){
  auto& c=*static_cast<EffectMaterialProfileContextV4*>(context);
  if(u.semantic==3&&u.type==11&&u.count==1&&c.borrowed.source_texture_matrix){
   c.matrix_pointer=reinterpret_cast<const std::uint8_t*>(c.borrowed.source_texture_matrix);
   out.matrices68=&c.matrix_pointer;out.matrix_count=1;return true;
  }
  return read_effect_profile_material_v4(&c.borrowed,u,out,error);
 }
};
