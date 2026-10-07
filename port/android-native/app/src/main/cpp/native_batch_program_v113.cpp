#include "native_batch_program_v113.hpp"
#include <algorithm>
#include <array>
#include <climits>
#include <cstring>
#include <stdexcept>
namespace model_renderer {namespace {
void errors(const char* site){const auto error=glGetError();if(error!=GL_NO_ERROR)throw std::runtime_error(std::string("Original batch GLES ")+site+": "+std::to_string(error));}
int attribute_slot(std::string name){
 for(auto& c:name)if(c>='A'&&c<='Z')c=char(c+'a'-'A');
 if(name=="vertex"||name=="position")return 0;if(name=="normal")return 1;
 if(name=="color"||name=="color0")return 2;if(name=="color1")return 3;
 if(name.rfind("texcoord",0)==0){const auto suffix=name.substr(8);if(suffix.empty())return 4;unsigned n{};for(char c:suffix){if(c<'0'||c>'9')return -1;n=n*10+unsigned(c-'0');if(n>13)return -1;}return int(n+4);}
 //Unknown shader attributes require the actual selected source attribute-map
 //producer. Never bind another stream or a constant normal by resemblance.
 return -1;
}
unsigned components(GLenum type){switch(type){case GL_FLOAT:return 1;case GL_FLOAT_VEC2:return 2;case GL_FLOAT_VEC3:return 3;case GL_FLOAT_VEC4:return 4;default:return 0;}}
}
bool NativeBatchProgramV113::initialize(const dh2::scene::EffectRenderPassV4& pass,const dh2::scene::ShaderSourcePack& pack,std::uint32_t flags,const std::string* config,std::shared_ptr<dh2::resources::ContextResourceBudgetV37> budget,std::string& e)try{
 bool complete=false;struct FailedPrefix {NativeBatchProgramV113& value;bool& complete;~FailedPrefix(){if(!complete)value.release(false);}} failed{*this,complete};
 if(program_||!budget||!budget->context_state_v41().ready)throw std::runtime_error("Actual batch program requires fresh live context owner");budget_=std::move(budget);generation_=budget_->context_state_v41().generation;
 const auto* vertex=pack.find(pass.vertex_file);const auto* fragment=pack.find(pass.fragment_file);if(!vertex||!fragment)throw std::runtime_error("Shipping batch shader files absent from SAME actual shader pack");
 dh2::scene::ShaderSourcePlan vs,fs;
 const auto text=[](const auto& row){return std::string_view(reinterpret_cast<const char*>(row.bytes.data()),row.bytes.size());};
 if(!dh2::scene::shader_source_plan(flags,4,pass.vertex_file,pass.vertex_defines,config,text(*vertex),vs,e)||!dh2::scene::shader_source_plan(flags,0,pass.fragment_file,pass.fragment_defines,config,text(*fragment),fs,e))return false;
 struct Shader {GLuint name{};dh2::resources::ResourceTokenV37 token;std::shared_ptr<dh2::resources::ContextResourceBudgetV37> budget;~Shader(){if(name)glDeleteShader(name);if(token){std::string e;if(!budget->release(token,e))std::terminate();}}};
 auto compile=[&](const auto& plan,Shader& out){dh2::resources::ResourceReservationV37 reservation;if(!budget_->reserve_create({dh2::resources::ResourceKindV37::shader,dh2::resources::ResourceScopeV37::shader,0,0},reservation,e))return false;
  out.budget=budget_;out.name=glCreateShader(plan.gl_type);errors("shader allocation");if(!out.name||!reservation.commit(out.token,e))return false;
  std::array<const GLchar*,8> strings{};std::array<GLint,8> lengths{};for(unsigned i=0;i<8;++i){if(plan.chunks[i].size()>INT_MAX)throw std::runtime_error("Actual shader source count exceeds GLES domain");strings[i]=plan.chunks[i].data();lengths[i]=static_cast<GLint>(plan.chunks[i].size());}
  glShaderSource(out.name,8,strings.data(),lengths.data());glCompileShader(out.name);GLint compiled{};glGetShaderiv(out.name,GL_COMPILE_STATUS,&compiled);errors("shader compilation");
  if(!compiled){std::array<GLchar,4096> log{};glGetShaderInfoLog(out.name,log.size(),nullptr,log.data());e=std::string("Actual shipping batch shader failed: ")+log.data();return false;}return true;
 };
 Shader v,f;if(!compile(vs,v)||!compile(fs,f))return false;
 dh2::resources::ResourceReservationV37 reservation;if(!budget_->reserve_create({dh2::resources::ResourceKindV37::program,dh2::resources::ResourceScopeV37::shader,0,0},reservation,e))return false;
 program_=glCreateProgram();errors("program allocation");if(!program_||!reservation.commit(program_token_,e))return false;
 glAttachShader(program_,v.name);glAttachShader(program_,f.name);glLinkProgram(program_);GLint linked{};glGetProgramiv(program_,GL_LINK_STATUS,&linked);errors("program link");
 if(!linked){std::array<GLchar,4096> log{};glGetProgramInfoLog(program_,log.size(),nullptr,log.data());throw std::runtime_error(std::string("Actual shipping batch link failed: ")+log.data());}
 GLint count{},maximum{};glGetProgramiv(program_,GL_ACTIVE_UNIFORMS,&count);glGetProgramiv(program_,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maximum);if(count<0||maximum<0)throw std::runtime_error("Actual shader uniform bounds invalid");
 std::vector<GLchar> name(static_cast<std::size_t>(std::max(maximum,1)));
 for(GLint i=0;i<count;++i){GLsizei length;GLint size;GLenum type;glGetActiveUniform(program_,i,name.size(),&length,&size,&type,name.data());if(length<0||std::size_t(length)>=name.size()||size<=0)throw std::runtime_error("Actual shader uniform record invalid");dh2::scene::ShaderUniformReflectionV4 u;u.name.assign(name.data(),length);u.gl_type=type;u.count=size;u.location=glGetUniformLocation(program_,u.name.c_str());dh2::scene::shader_uniform_reflect_v4(u);if(u.location<0||u.type==255)throw std::runtime_error("Actual shader uniform binding unsupported");uniforms_.push_back(std::move(u));}
 dh2::scene::shader_uniform_partition_v4(uniforms_);
 glGetProgramiv(program_,GL_ACTIVE_ATTRIBUTES,&count);glGetProgramiv(program_,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&maximum);if(count<0||maximum<0)throw std::runtime_error("Actual shader attribute bounds invalid");name.resize(static_cast<std::size_t>(std::max(maximum,1)));
 for(GLint i=0;i<count;++i){GLsizei length;GLint size;GLenum type;glGetActiveAttrib(program_,i,name.size(),&length,&size,&type,name.data());if(length<0||std::size_t(length)>=name.size()||size!=1)throw std::runtime_error("Actual shader attribute binding shape unsupported");std::string key(name.data(),length);auto slot=attribute_slot(key);auto width=components(type);const auto location=glGetAttribLocation(program_,key.c_str());if(slot<0||!width||location<0)throw std::runtime_error("Actual shader attribute-map producer required: "+key);attributes_.push_back({static_cast<std::uint32_t>(slot),location,width});}
 errors("program reflection");complete=true;e.clear();return true;
}catch(const std::exception& failure){e=failure.what();release(false);return false;}
bool NativeBatchProgramV113::bind_uniforms(const NativeBatchUniformReadV113& read,std::string& e)const try{
 if(!program_||!read||!budget_||!budget_->context_state_v41().ready||budget_->context_state_v41().generation!=generation_)throw std::runtime_error("Retired original batch shader/context");glUseProgram(program_);
 for(const auto& u:uniforms_){NativeBatchUniformValueV113 value;if(!read(u,value,e))return false;if(value.skip)continue;if(value.elements!=u.count)throw std::runtime_error("Actual shader uniform array count mismatch: "+u.name);const auto n=static_cast<GLsizei>(u.count);
  switch(u.gl_type){
   case GL_FLOAT:if(!value.floats)throw std::runtime_error("Actual float uniform cells absent");glUniform1fv(u.location,n,value.floats);break;
   case GL_FLOAT_VEC2:if(!value.floats)throw std::runtime_error("Actual float2 uniform cells absent");glUniform2fv(u.location,n,value.floats);break;
   case GL_FLOAT_VEC3:if(!value.floats)throw std::runtime_error("Actual float3 uniform cells absent");glUniform3fv(u.location,n,value.floats);break;
   case GL_FLOAT_VEC4:if(!value.floats)throw std::runtime_error("Actual float4 uniform cells absent");glUniform4fv(u.location,n,value.floats);break;
   case GL_FLOAT_MAT2:if(!value.floats)throw std::runtime_error("Actual matrix2 uniform cells absent");glUniformMatrix2fv(u.location,n,GL_FALSE,value.floats);break;
   case GL_FLOAT_MAT3:if(!value.floats)throw std::runtime_error("Actual matrix3 uniform cells absent");glUniformMatrix3fv(u.location,n,GL_FALSE,value.floats);break;
   case GL_FLOAT_MAT4:if(!value.floats)throw std::runtime_error("Actual matrix4 uniform cells absent");glUniformMatrix4fv(u.location,n,GL_FALSE,value.floats);break;
   case GL_INT:case GL_BOOL:case GL_SAMPLER_2D:case GL_SAMPLER_CUBE:if(!value.integers)throw std::runtime_error("Actual integer/sampler uniform cells absent");glUniform1iv(u.location,n,value.integers);break;
   case GL_INT_VEC2:case GL_BOOL_VEC2:if(!value.integers)throw std::runtime_error("Actual int2 uniform cells absent");glUniform2iv(u.location,n,value.integers);break;
   case GL_INT_VEC3:case GL_BOOL_VEC3:if(!value.integers)throw std::runtime_error("Actual int3 uniform cells absent");glUniform3iv(u.location,n,value.integers);break;
   case GL_INT_VEC4:case GL_BOOL_VEC4:if(!value.integers)throw std::runtime_error("Actual int4 uniform cells absent");glUniform4iv(u.location,n,value.integers);break;
   default:throw std::runtime_error("Actual shader uniform GLES type unsupported: "+u.name);
  }
 }
 errors("actual reflected uniform submission");e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
void NativeBatchProgramV113::release(bool lost)noexcept{
 if(program_&&budget_){const auto context=budget_->context_state_v41();if(!lost&&context.ready&&context.generation==generation_)glDeleteProgram(program_);}
 program_=0;if(program_token_){std::string e;if(!budget_||!budget_->release(program_token_,e))std::terminate();}program_token_={};budget_.reset();uniforms_.clear();attributes_.clear();generation_=0;
}
}
