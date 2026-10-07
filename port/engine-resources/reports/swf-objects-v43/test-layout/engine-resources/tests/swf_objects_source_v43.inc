#include "../gpu_object_owner_v43.hpp"
#include "../../engine-ui/swf_vertex_cache_v36.hpp"
#include "../../android-native/app/src/main/cpp/authored_shader_program.hpp"
#include <limits>
#include <unordered_map>
namespace dh2::android_resources {std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39();}
namespace dh2::perf {struct TestState {std::uint64_t uploads{};};extern TestState state;}
namespace dh2::scene {
struct ShaderSourcePack {bool load(const std::uint8_t*,std::size_t,std::string&){return true;}};
struct ShaderSourcePlan {unsigned gl_type{};std::array<std::string,8> chunks;};
}
namespace dh2::android_ui {
std::string asset_bytes(AAssetManager*,const char*,const char*){return "fixture";}
scene::ShaderSourcePlan plan(const scene::ShaderSourcePack&,const char*,unsigned type){scene::ShaderSourcePlan p;p.gl_type=type;for(auto& c:p.chunks)c="x";return p;}
class SwfGpu {public:
 std::shared_ptr<resources::ContextResourceBudgetV37> budget_v39_;
 resources::ResourceScopeV37 scope_v39_;
 resources::GpuObjectOwnerV43 stream_owner_v43_;
 std::unordered_map<std::uint32_t,resources::GpuObjectOwnerV43> retained_owners_v43_;
 GLuint buffer_{};bool publication_failure_at_upload{},context_loss_at_upload{};
 SwfGpu(std::shared_ptr<resources::ContextResourceBudgetV37> b,resources::ResourceScopeV37 s):budget_v39_(b),scope_v39_(s){}
 void barrier_v37(const char*);
 void release_vertices_v43(std::uintptr_t);
 void stream_storage_v43(const std::vector<float>&);
 ui::SwfVertexCacheV36::Upload actual_upload(){
return [this](const float* data,std::size_t count,std::uintptr_t& out,std::string& error){
        barrier_v37("SWF retained vertex resource entry");
        std::uint64_t bytes;if(!resources::checked_resource_bytes_v37(count,sizeof(float),bytes,error))return false;
        if(bytes>std::uint64_t(std::numeric_limits<GLsizeiptr>::max()))throw std::runtime_error("SWF vertex GL span overflow");
        resources::GpuObjectOwnerV43 candidate;
        if(!candidate.create(budget_v39_,resources::ResourceKindV37::vertex_buffer,scope_v39_,bytes,
          [&](std::uint32_t& name){glGenBuffers(1,&name);if(!name)return;
            glBindBuffer(GL_ARRAY_BUFFER,name);glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(bytes),data,GL_STATIC_DRAW);
            barrier_v37("SWF retained vertex upload");},
          [](std::uint32_t name){glDeleteBuffers(1,&name);},error))return false;
        const auto name=candidate.name();
        const auto inserted=retained_owners_v43_.try_emplace(name,std::move(candidate));
        if(!inserted.second)throw std::runtime_error("SWF retained buffer name collision");
        out=name;dh2::perf::state.uploads+=bytes;return true;
    };
}
};
void SwfGpu::release_vertices_v43(std::uintptr_t value){
    const auto found=retained_owners_v43_.find(std::uint32_t(value));
    if(found==retained_owners_v43_.end())throw std::runtime_error("SWF retained buffer ledger owner missing");
    retained_owners_v43_.erase(found);
}
void SwfGpu::stream_storage_v43(const std::vector<float>& vertices){
    std::string error;std::uint64_t bytes;
    if(!resources::checked_resource_bytes_v37(vertices.size(),sizeof(float),bytes,error))throw std::runtime_error(error);
    if(bytes>std::uint64_t(std::numeric_limits<GLsizeiptr>::max()))throw std::runtime_error("SWF stream GL span overflow");
    barrier_v37("SWF stream vertex resource entry");
    const auto upload=[&](std::uint32_t name){
      glBindBuffer(GL_ARRAY_BUFFER,name);glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(bytes),vertices.data(),GL_STREAM_DRAW);
      barrier_v37("SWF stream vertex upload");};
    try{
      if(!stream_owner_v43_.name()){
        if(!stream_owner_v43_.create(budget_v39_,resources::ResourceKindV37::vertex_buffer,scope_v39_,bytes,
          [&](std::uint32_t& name){glGenBuffers(1,&name);if(!name)return;barrier_v37("SWF stream buffer allocation");upload(name);},
          [](std::uint32_t name){glDeleteBuffers(1,&name);},error))throw std::runtime_error(error);
      }else if(!stream_owner_v43_.replace_storage(bytes,upload,error))throw std::runtime_error(error);
    }catch(...){buffer_=stream_owner_v43_.name();throw;}
    buffer_=stream_owner_v43_.name();
}
resources::GpuObjectOwnerV43 compile(const dh2::scene::ShaderSourcePlan& source,resources::ResourceScopeV37 scope){
    resources::GpuObjectOwnerV43 shader;std::string error;
    if(!shader.create(android_resources::budget_lease_v39(),resources::ResourceKindV37::shader,scope,0,
        [&](std::uint32_t& name){name=glCreateShader(source.gl_type);},[](std::uint32_t name){glDeleteShader(name);},error))throw std::runtime_error(error);
    std::array<const char*,8> text{};std::array<GLint,8> sizes{};
    for(std::size_t i=0;i<text.size();++i){text[i]=source.chunks[i].data();sizes[i]=static_cast<GLint>(source.chunks[i].size());}
    glShaderSource(shader.name(),static_cast<GLsizei>(text.size()),text.data(),sizes.data());glCompileShader(shader.name());
    GLint ok=0;glGetShaderiv(shader.name(),GL_COMPILE_STATUS,&ok);
    if(!ok){char log[4096]{};glGetShaderInfoLog(shader.name(),sizeof(log),nullptr,log);
        throw std::runtime_error(std::string("Authored shader compile failed: ")+log);}
    return shader;
}
void check(const char* step){
    const auto code=glGetError();if(code!=GL_NO_ERROR){char value[160];std::snprintf(value,sizeof(value),"Authored UI %s GL error 0x%04x",step,code);throw std::runtime_error(value);}
}
Program create(AAssetManager* assets,bool premultiplied,resources::ResourceScopeV37 scope){
    const auto bytes=asset_bytes(assets,"shaders.pak","365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0");
    dh2::scene::ShaderSourcePack pack;std::string error;
    if(!pack.load(reinterpret_cast<const std::uint8_t*>(bytes.data()),bytes.size(),error))
        throw std::runtime_error("Authored shader pack invalid: "+error);
    const auto vs=plan(pack,"GameSWFVS.glsl",4);
    const auto fs=plan(pack,premultiplied?"GameSWFFS_blend.glsl":"GameSWFFS.glsl",5);
    resources::GpuObjectOwnerV43 vertex,fragment;Program out;
    try{
        vertex=compile(vs,scope);fragment=compile(fs,scope);
        if(!out.ownership_v43.create(android_resources::budget_lease_v39(),resources::ResourceKindV37::program,scope,0,
          [](std::uint32_t& name){name=glCreateProgram();},[](std::uint32_t name){
            GLint active=0;glGetIntegerv(GL_CURRENT_PROGRAM,&active);if(GLuint(active)==name)glUseProgram(0);glDeleteProgram(name);},error))throw std::runtime_error(error);
        out.name=out.ownership_v43.name();
        glAttachShader(out.name,vertex.name());glAttachShader(out.name,fragment.name());glLinkProgram(out.name);
        GLint ok=0;glGetProgramiv(out.name,GL_LINK_STATUS,&ok);
        if(!ok){char log[4096]{};glGetProgramInfoLog(out.name,sizeof(log),nullptr,log);throw std::runtime_error(std::string("Authored shader link failed: ")+log);}
        out.position=glGetAttribLocation(out.name,"Position");out.uv=glGetAttribLocation(out.name,"TexCoord0");
        out.color=glGetAttribLocation(out.name,"Color0");out.matrix=glGetUniformLocation(out.name,"WorldViewProjectionMatrix");
        out.diffuse=glGetUniformLocation(out.name,"DiffuseColor");out.sampler=glGetUniformLocation(out.name,"TextureSampler");
        if(out.position<0||out.uv<0||out.color<0||out.matrix<0||out.diffuse<0||out.sampler<0)
            throw std::runtime_error("Authored UI shader interface incomplete");
        glDetachShader(out.name,vertex.name());glDetachShader(out.name,fragment.name());vertex.reset();fragment.reset();check("program creation");return out;
    }catch(...){release(out);vertex.reset();fragment.reset();throw;}
}
void release(Program& value) noexcept{value={};}
}
