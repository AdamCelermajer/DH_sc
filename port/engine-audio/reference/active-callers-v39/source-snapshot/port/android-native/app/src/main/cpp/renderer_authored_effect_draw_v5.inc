// Include at namespace scope after GLES2/gl2.h, effect_render_pass_v4.hpp and
// shader_sources.hpp. Presentation borrows the retained V2 geometry/texture.
// The caller owns source texture setup and SceneManager transparent ordering.
struct EffectDrawBorrowV5 {
 GLuint vertices{},indices{},texture{};GLsizei count{},stride{};
 std::size_t position_offset{},color_offset{},uv_offset{};
 const float* world_view_projection{};const float* diffuse_color{};
 float sampler_bias{};
 // A positive receipt from the actual texture owner is mandatory; this helper
 // neither installs guessed filter/wrap/mipmap state nor uploads another atlas.
 bool source_texture_ready{};bool source_color_missing{};
};
class EffectDrawProgramV5 {
 GLuint program_{};
 GLint wvp_{-1},matrix_{-1},diffuse_{-1},sampler_{-1},bias_{-1};
 static GLuint compile(const dh2::scene::ShaderSourcePlan& plan){
  if(plan.gl_type!=GL_VERTEX_SHADER&&plan.gl_type!=GL_FRAGMENT_SHADER)
   throw std::runtime_error("Effect source shader type unavailable");
  const GLuint shader=glCreateShader(plan.gl_type);
  std::array<const GLchar*,8> text{};std::array<GLint,8> sizes{};
  for(std::size_t i=0;i<8;++i){text[i]=plan.chunks[i].data();sizes[i]=static_cast<GLint>(plan.chunks[i].size());}
  glShaderSource(shader,8,text.data(),sizes.data());glCompileShader(shader);
  GLint good=0;glGetShaderiv(shader,GL_COMPILE_STATUS,&good);
  if(!good){std::array<GLchar,2048> log{};glGetShaderInfoLog(shader,static_cast<GLsizei>(log.size()),nullptr,log.data());
   glDeleteShader(shader);throw std::runtime_error(std::string("Effect actual source shader failed: ")+log.data());}
  return shader;
 }
 struct Guard {
  GLint program{},active{},texture{},array{},elements{},src_rgb{},dst_rgb{},src_alpha{},dst_alpha{},eq_rgb{},eq_alpha{},depth_func{},cull_face{},front_face{};
  GLboolean depth_write{};std::array<GLboolean,6> enabled{};
  struct Attribute{GLint enabled{},size{},type{},normalized{},stride{},buffer{};void* pointer{};};
  std::array<Attribute,3> attributes{};std::array<std::array<float,4>,3> current{};
  Guard(const Guard&)=delete;Guard& operator=(const Guard&)=delete;
  static constexpr std::array<GLenum,6> caps{{GL_BLEND,GL_DEPTH_TEST,GL_CULL_FACE,GL_STENCIL_TEST,GL_SAMPLE_COVERAGE,GL_POLYGON_OFFSET_FILL}};
  Guard(){
   glGetIntegerv(GL_CURRENT_PROGRAM,&program);glGetIntegerv(GL_ACTIVE_TEXTURE,&active);
   glActiveTexture(GL_TEXTURE0);glGetIntegerv(GL_TEXTURE_BINDING_2D,&texture);
   glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&array);glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING,&elements);
   glGetIntegerv(GL_BLEND_SRC_RGB,&src_rgb);glGetIntegerv(GL_BLEND_DST_RGB,&dst_rgb);
   glGetIntegerv(GL_BLEND_SRC_ALPHA,&src_alpha);glGetIntegerv(GL_BLEND_DST_ALPHA,&dst_alpha);
   glGetIntegerv(GL_BLEND_EQUATION_RGB,&eq_rgb);glGetIntegerv(GL_BLEND_EQUATION_ALPHA,&eq_alpha);
   glGetIntegerv(GL_DEPTH_FUNC,&depth_func);glGetBooleanv(GL_DEPTH_WRITEMASK,&depth_write);
   glGetIntegerv(GL_CULL_FACE_MODE,&cull_face);glGetIntegerv(GL_FRONT_FACE,&front_face);
   for(std::size_t i=0;i<caps.size();++i)enabled[i]=glIsEnabled(caps[i]);
   for(GLuint i=0;i<3;++i){glGetVertexAttribfv(i,GL_CURRENT_VERTEX_ATTRIB,current[i].data());auto& a=attributes[i];
    glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&a.enabled);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_SIZE,&a.size);
    glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_TYPE,&a.type);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,&a.normalized);
    glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_STRIDE,&a.stride);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,&a.buffer);
    glGetVertexAttribPointerv(i,GL_VERTEX_ATTRIB_ARRAY_POINTER,&a.pointer);
   }
  }
  ~Guard(){
   for(GLuint i=0;i<3;++i){glVertexAttrib4fv(i,current[i].data());const auto& a=attributes[i];glBindBuffer(GL_ARRAY_BUFFER,static_cast<GLuint>(a.buffer));
    glVertexAttribPointer(i,a.size,static_cast<GLenum>(a.type),static_cast<GLboolean>(a.normalized),a.stride,a.pointer);
    if(a.enabled)glEnableVertexAttribArray(i);else glDisableVertexAttribArray(i);}
   glBindBuffer(GL_ARRAY_BUFFER,static_cast<GLuint>(array));glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,static_cast<GLuint>(elements));
   glBlendFuncSeparate(src_rgb,dst_rgb,src_alpha,dst_alpha);glBlendEquationSeparate(eq_rgb,eq_alpha);
   glDepthFunc(depth_func);glDepthMask(depth_write);glCullFace(cull_face);glFrontFace(front_face);
   for(std::size_t i=0;i<caps.size();++i){if(enabled[i])glEnable(caps[i]);else glDisable(caps[i]);}
   glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(texture));glActiveTexture(static_cast<GLenum>(active));glUseProgram(static_cast<GLuint>(program));
  }
 };
public:
 // One actual state snapshot for a complete source-ordered effect batch.
 // Each draw still installs its own actual program/material/pass state.
 using BatchScope=Guard;
 GLuint native_program()const noexcept{return program_;}
 // Call on the owning GL context. Plans must be made by shader_source_plan
 // from actual driver flags/config and bundled ProfileCOMMON source bytes.
 void initialize(const dh2::scene::EffectRenderPassV4& pass,const dh2::scene::ShaderSourcePlan& vs,const dh2::scene::ShaderSourcePlan& fs){
  if(program_)throw std::runtime_error("Effect source program already initialized");
  if(pass.vertex_file!="ProfileCOMMON_emul_VS.glsl"||pass.fragment_file!="ProfileCOMMON_emul_FS.glsl"||
     pass.vertex_defines!="#define TEXTURED\n"||
     (pass.fragment_defines!="#define TEXTURED\n"&&pass.fragment_defines!="#define TEXTURED\n#define ADDITIVEBLEND\n "&&
      pass.fragment_defines!="#define TEXTURED\n#define ALPHABLEND\n")||
     vs.chunks[5]!=pass.vertex_defines||fs.chunks[5]!=pass.fragment_defines||vs.chunks[7].empty()||fs.chunks[7].empty())
   throw std::runtime_error("Effect source default technique/plan binding unavailable");
  GLuint v=0,f=0,p=0;
  try{v=compile(vs);f=compile(fs);p=glCreateProgram();glAttachShader(p,v);glAttachShader(p,f);
   glBindAttribLocation(p,0,"Position");glBindAttribLocation(p,1,"Color0");glBindAttribLocation(p,2,"TexCoord0");glLinkProgram(p);
   GLint good=0;glGetProgramiv(p,GL_LINK_STATUS,&good);if(!good)throw std::runtime_error("Effect actual source program link failed");
   wvp_=glGetUniformLocation(p,"WorldViewProjectionMatrix");matrix_=glGetUniformLocation(p,"TextureMatrix0");
   diffuse_=glGetUniformLocation(p,"DiffuseColor");sampler_=glGetUniformLocation(p,"Sampler0");bias_=glGetUniformLocation(p,"sampler0_bias");
   if(wvp_<0||matrix_<0||diffuse_<0||sampler_<0)throw std::runtime_error("Effect actual source uniforms unavailable");
   program_=p;p=0;
  }catch(...){if(v)glDeleteShader(v);if(f)glDeleteShader(f);if(p)glDeleteProgram(p);throw;}
  glDeleteShader(v);glDeleteShader(f);
 }
 void release(){if(program_)glDeleteProgram(program_);program_=0;}
 // Supply batches in the source transparent queue order. Single-cloud ordering
 // is already source-sorted in V2; multiple clouds require the scene queue.
private:
 void draw_impl(const dh2::scene::EffectRenderPassV4& pass,const EffectDrawBorrowV5* ordered,std::size_t count,bool actual_render_target_flip,bool source_queue_order_ready){
  if(!count)return;
  if(!program_||!source_queue_order_ready)throw std::runtime_error("Effect source program/transparent queue unavailable");
  if(pass.stencil||pass.sample_coverage||pass.polygon_offset)throw std::runtime_error("Effect nondefault auxiliary render state requires source owner");
  for(std::size_t i=0;i<count;++i){const auto& b=ordered[i];if(!b.source_texture_ready||!b.vertices||!b.indices||!b.texture||b.count<=0||!b.world_view_projection||!b.diffuse_color||b.stride<=0)
   throw std::runtime_error("Effect retained geometry/material/texture borrow unavailable");
  }
  auto enable=[](GLenum cap,bool value){if(value)glEnable(cap);else glDisable(cap);};
  enable(GL_BLEND,pass.blend);enable(GL_DEPTH_TEST,pass.depth);enable(GL_CULL_FACE,pass.cull);
  glDisable(GL_STENCIL_TEST);glDisable(GL_SAMPLE_COVERAGE);glDisable(GL_POLYGON_OFFSET_FILL);
  glBlendFunc(pass.blend_src,pass.blend_dst);glBlendEquation(pass.blend_equation);
  glDepthFunc(pass.depth_function);glDepthMask(pass.depth_write?GL_TRUE:GL_FALSE);glCullFace(pass.cull_face);
  glFrontFace(actual_render_target_flip?(pass.front_face==GL_CCW?GL_CW:GL_CCW):pass.front_face);
  glUseProgram(program_);glUniform1i(sampler_,0);
  const std::array<float,16> identity{{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}};
  // V2 snapshots already contain source TextureMatrix0*vec4(uv,1,0).
  glUniformMatrix4fv(matrix_,1,GL_FALSE,identity.data());
  for(std::size_t i=0;i<count;++i){const auto& b=ordered[i];glBindTexture(GL_TEXTURE_2D,b.texture);
   glUniformMatrix4fv(wvp_,1,GL_FALSE,b.world_view_projection);glUniform4fv(diffuse_,1,b.diffuse_color);
   if(bias_>=0)glUniform1f(bias_,b.sampler_bias);
   glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,b.indices);
   glEnableVertexAttribArray(0);glEnableVertexAttribArray(1);glEnableVertexAttribArray(2);
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,b.stride,reinterpret_cast<const void*>(b.position_offset));
   if(b.source_color_missing){glDisableVertexAttribArray(1);glVertexAttrib4f(1,1.f,1.f,1.f,1.f);}else glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,b.stride,reinterpret_cast<const void*>(b.color_offset));
   glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,b.stride,reinterpret_cast<const void*>(b.uv_offset));
   glDrawElements(GL_TRIANGLES,b.count,GL_UNSIGNED_SHORT,nullptr);
  }
  // No color-mask, scissor, viewport or texture-parameter calls: source driver
  // state and shared texture ownership are preserved exactly at this boundary.
 }
public:
 void draw(const dh2::scene::EffectRenderPassV4& pass,const std::vector<EffectDrawBorrowV5>& ordered,bool flip,bool receipt){
  if(ordered.empty())return;BatchScope scope;draw_impl(pass,ordered.data(),ordered.size(),flip,receipt);
 }
 void draw_in_batch(const BatchScope&,const dh2::scene::EffectRenderPassV4& pass,
  const EffectDrawBorrowV5& borrow,bool flip,bool receipt){draw_impl(pass,&borrow,1,flip,receipt);}
};
