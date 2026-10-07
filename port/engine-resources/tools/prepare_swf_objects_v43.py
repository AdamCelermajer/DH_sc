"""Stage SWF CPU/vertex/shader ownership; never modify shared production files."""
from pathlib import Path
import difflib,hashlib,json
root=Path(__file__).resolve().parents[3]
out=root/'port/engine-resources/reports/swf-objects-v43';out.mkdir(parents=True,exist_ok=True)
baseline={};changes={}
def change(path,fn):
 before=(root/path).read_text();after=fn(before)
 assert before!=after,path
 baseline[path]=hashlib.sha256((root/path).read_bytes()).hexdigest();changes[path]=after
def budget_h(s):
 s=s.replace('renderbuffer,cpu_request,count','renderbuffer,cpu_request,shader,count')
 s=s.replace('4096,8192,8192,256,256,256,4096}}','4096,8192,8192,256,256,256,4096,256}}')
 s=s.replace(' std::array<std::uint64_t,std::size_t(ResourceKindV37::count)> objects{};',
 ''' // Count-only program/shader storage remains explicitly unknown in bytes.
 std::array<std::uint64_t,std::size_t(ResourceKindV37::count)> unknown_gpu_storage_objects{};
 std::array<std::uint64_t,std::size_t(ResourceKindV37::count)> objects{};''')
 s=s.replace('live_by_scope{},pending_by_scope{};','live_by_scope{},pending_by_scope{},peak_by_scope{};')
 return s
change('port/engine-resources/resource_budget_v37.hpp',budget_h)
def budget_cpp(s):
 s=s.replace('u.objects[std::size_t(c.kind)]=gpu||!gpu_kind(c.kind)?1:0;',
 '''u.objects[std::size_t(c.kind)]=gpu||!gpu_kind(c.kind)?1:0;
 if(gpu&&!c.gpu_bytes&&(c.kind==ResourceKindV37::program||c.kind==ResourceKindV37::shader))u.unknown_gpu_storage_objects[std::size_t(c.kind)]=1;''')
 s=s.replace('a.objects[i]+=b.objects[i];','a.objects[i]+=b.objects[i];a.unknown_gpu_storage_objects[i]+=b.unknown_gpu_storage_objects[i];')
 s=s.replace('a.objects[i]-=b.objects[i];','a.objects[i]-=b.objects[i];a.unknown_gpu_storage_objects[i]-=b.unknown_gpu_storage_objects[i];')
 s=s.replace('u.objects[i]=positive(next.objects[i],old.objects[i]);','u.objects[i]=positive(next.objects[i],old.objects[i]);u.unknown_gpu_storage_objects[i]=positive(next.unknown_gpu_storage_objects[i],old.unknown_gpu_storage_objects[i]);')
 s=s.replace('p.objects[i]=std::max(p.objects[i],u.objects[i]);','p.objects[i]=std::max(p.objects[i],u.objects[i]);p.unknown_gpu_storage_objects[i]=std::max(p.unknown_gpu_storage_objects[i],u.unknown_gpu_storage_objects[i]);')
 old=''' auto& p=snapshot_.peak;p.gpu_bytes=std::max(p.gpu_bytes,u.gpu_bytes);p.cpu_bytes=std::max(p.cpu_bytes,u.cpu_bytes);
 p.texture_gpu_bytes=std::max(p.texture_gpu_bytes,u.texture_gpu_bytes);p.fx_buffer_gpu_bytes=std::max(p.fx_buffer_gpu_bytes,u.fx_buffer_gpu_bytes);
 for(std::size_t i=0;i<kinds;++i){p.objects[i]=std::max(p.objects[i],u.objects[i]);p.unknown_gpu_storage_objects[i]=std::max(p.unknown_gpu_storage_objects[i],u.unknown_gpu_storage_objects[i]);p.gpu_bytes_by_kind[i]=std::max(p.gpu_bytes_by_kind[i],u.gpu_bytes_by_kind[i]);p.cpu_bytes_by_kind[i]=std::max(p.cpu_bytes_by_kind[i],u.cpu_bytes_by_kind[i]);}'''
 new=''' auto peak=[](ResourceUsageV37& p,const ResourceUsageV37& value){
  p.gpu_bytes=std::max(p.gpu_bytes,value.gpu_bytes);p.cpu_bytes=std::max(p.cpu_bytes,value.cpu_bytes);
  p.texture_gpu_bytes=std::max(p.texture_gpu_bytes,value.texture_gpu_bytes);p.fx_buffer_gpu_bytes=std::max(p.fx_buffer_gpu_bytes,value.fx_buffer_gpu_bytes);
  for(std::size_t i=0;i<kinds;++i){p.objects[i]=std::max(p.objects[i],value.objects[i]);p.unknown_gpu_storage_objects[i]=std::max(p.unknown_gpu_storage_objects[i],value.unknown_gpu_storage_objects[i]);p.gpu_bytes_by_kind[i]=std::max(p.gpu_bytes_by_kind[i],value.gpu_bytes_by_kind[i]);p.cpu_bytes_by_kind[i]=std::max(p.cpu_bytes_by_kind[i],value.cpu_bytes_by_kind[i]);}
 };
 peak(snapshot_.peak,u);
 for(std::size_t i=0;i<scopes;++i){auto scoped=snapshot_.live_by_scope[i];add(scoped,snapshot_.pending_by_scope[i]);peak(snapshot_.peak_by_scope[i],scoped);}'''
 assert old in s;s=s.replace(old,new)
 # Count delta applies equally to unknown-byte objects sharing one GL name.
 s=s.replace('for(std::size_t i=0;i<kinds;++i)held.objects[i]=target.objects[i]>old_usage.objects[i]?target.objects[i]-old_usage.objects[i]:0;',
 'for(std::size_t i=0;i<kinds;++i){held.objects[i]=target.objects[i]>old_usage.objects[i]?target.objects[i]-old_usage.objects[i]:0;held.unknown_gpu_storage_objects[i]=target.unknown_gpu_storage_objects[i]>old_usage.unknown_gpu_storage_objects[i]?target.unknown_gpu_storage_objects[i]-old_usage.unknown_gpu_storage_objects[i]:0;}')
 return s
change('port/engine-resources/resource_budget_v37.cpp',budget_cpp)
def cache_h(s):
 s=s.replace('#include <cstddef>','#include "../engine-resources/cpu_vector_capacity_v41.hpp"\n#include <cstddef>')
 s=s.replace('std::vector<float> vertices;','resources::CpuVectorCapacityV41 capacity_v43;std::vector<float> vertices;')
 s=s.replace(' std::list<Entry> entries_;',' std::shared_ptr<resources::ContextResourceBudgetV37> budget_v43_;\n resources::ResourceScopeV37 scope_v43_{resources::ResourceScopeV37::other};\n std::list<Entry> entries_;')
 s=s.replace(' using Upload=', ' bool bind_budget_v43(const std::shared_ptr<resources::ContextResourceBudgetV37>&,resources::ResourceScopeV37,std::string&);\n using Upload=')
 return s
change('port/engine-ui/swf_vertex_cache_v36.hpp',cache_h)
def cache_cpp(s):
 s=s.replace('std::uint64_t SwfVertexCacheV36::fingerprint', '''bool SwfVertexCacheV36::bind_budget_v43(const std::shared_ptr<resources::ContextResourceBudgetV37>& budget,resources::ResourceScopeV37 scope,std::string& error){
 if(!budget||(scope!=resources::ResourceScopeV37::swf_front&&scope!=resources::ResourceScopeV37::swf_gameplay)||(!entries_.empty()&&(budget_v43_!=budget||scope_v43_!=scope))){error="V43 cache requires explicit same front/gameplay ledger before allocation";return false;}
 budget_v43_=budget;scope_v43_=scope;error.clear();return true;
}
std::uint64_t SwfVertexCacheV36::fingerprint''')
 s=s.replace('Entry candidate;candidate.hash=hash;candidate.mode=mode;candidate.vertices=vertices;',
 '''Entry candidate;candidate.hash=hash;candidate.mode=mode;
 if(budget_v43_&&!resources::reserve_cpu_vector_v41(candidate.vertices,candidate.capacity_v43,budget_v43_,scope_v43_,vertices.size(),error))return false;
 candidate.vertices=vertices;''')
 return s
change('port/engine-ui/swf_vertex_cache_v36.cpp',cache_cpp)
def shader_h(s):
 s=s.replace('#include <array>','#include <array>\n#include "../../../../../engine-resources/gpu_object_owner_v43.hpp"')
 s=s.replace('    GLuint name=0;', '''    resources::GpuObjectOwnerV43 ownership_v43;
    GLuint name=0;
    Program()=default;
    Program(const Program&)=delete;
    Program& operator=(const Program&)=delete;
    Program(Program&& other)noexcept{*this=std::move(other);}
    Program& operator=(Program&& other)noexcept{
      if(this!=&other){ownership_v43=std::move(other.ownership_v43);name=std::exchange(other.name,0);
       position=other.position;uv=other.uv;color=other.color;matrix=other.matrix;diffuse=other.diffuse;sampler=other.sampler;}
      return *this;
    }''')
 s=s.replace('Program create(AAssetManager*,bool premultiplied);','Program create(AAssetManager*,bool premultiplied,resources::ResourceScopeV37 scope=resources::ResourceScopeV37::shader);')
 return s
change('port/android-native/app/src/main/cpp/authored_shader_program.hpp',shader_h)
def shader_cpp(s):
 s=s.replace('#include "authored_shader_program.hpp"','#include "authored_shader_program.hpp"\n#include "native_resource_budget_v38.hpp"')
 a=s.index('GLuint compile(');b=s.index('void check(',a)
 s=s[:a]+'''resources::GpuObjectOwnerV43 compile(const dh2::scene::ShaderSourcePlan& source,resources::ResourceScopeV37 scope){
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
''' +s[b:]
 s=s.replace('Program create(AAssetManager* assets,bool premultiplied){','Program create(AAssetManager* assets,bool premultiplied,resources::ResourceScopeV37 scope){')
 s=s.replace('GLuint vertex=0,fragment=0;Program out;','resources::GpuObjectOwnerV43 vertex,fragment;Program out;')
 s=s.replace('vertex=compile(vs);fragment=compile(fs);','vertex=compile(vs,scope);fragment=compile(fs,scope);')
 s=s.replace('out.name=glCreateProgram();if(!out.name)throw std::runtime_error("Cannot allocate authored shader program");',
 '''if(!out.ownership_v43.create(android_resources::budget_lease_v39(),resources::ResourceKindV37::program,scope,0,
          [](std::uint32_t& name){name=glCreateProgram();},[](std::uint32_t name){
            GLint active=0;glGetIntegerv(GL_CURRENT_PROGRAM,&active);if(GLuint(active)==name)glUseProgram(0);glDeleteProgram(name);},error))throw std::runtime_error(error);
        out.name=out.ownership_v43.name();''')
 s=s.replace('glAttachShader(out.name,vertex);glAttachShader(out.name,fragment);','glAttachShader(out.name,vertex.name());glAttachShader(out.name,fragment.name());')
 s=s.replace('glDeleteShader(vertex);glDeleteShader(fragment);vertex=fragment=0;',
 'glDetachShader(out.name,vertex.name());glDetachShader(out.name,fragment.name());vertex.reset();fragment.reset();')
 s=s.replace('}catch(...){if(vertex)glDeleteShader(vertex);if(fragment)glDeleteShader(fragment);release(out);throw;}','}catch(...){release(out);vertex.reset();fragment.reset();throw;}')
 s=s.replace('void release(Program& value) noexcept{if(value.name)glDeleteProgram(value.name);value={};}','void release(Program& value) noexcept{value={};}')
 return s
change('port/android-native/app/src/main/cpp/authored_shader_program.cpp',shader_cpp)
def swf_h(s):
 s=s.replace('    std::vector<float> vertices_v36_;','''    resources::CpuVectorCapacityV41 vertices_capacity_v43_;
    std::vector<float> vertices_v36_;
    resources::GpuObjectOwnerV43 stream_owner_v43_;
    std::unordered_map<std::uint32_t,resources::GpuObjectOwnerV43> retained_owners_v43_;
    void release_vertices_v43(std::uintptr_t);
    void stream_storage_v43(const std::vector<float>&);''')
 return s
change('port/android-native/app/src/main/cpp/swf_gpu.hpp',swf_h)
def swf_cpp(s):
 s=s.replace('    scope_v39_=scope;','''    scope_v39_=scope;
    std::string cache_error_v43;
    if(!vertices_capacity_v43_.same_owner(budget_v39_,scope))throw std::runtime_error("V43 SWF stream CPU scope cannot change");
    if(!vertex_cache_v36_.bind_budget_v43(budget_v39_,scope,cache_error_v43))throw std::runtime_error(cache_error_v43);
    // Strong owners release stale ledger records without deleting reused names.
    if(stream_owner_v43_.live())throw std::runtime_error("V43 live SWF stream context cannot be abandoned");
    for(const auto& owner:retained_owners_v43_)if(owner.second.live())throw std::runtime_error("V43 live SWF retained context cannot be abandoned");
    retained_owners_v43_.clear();stream_owner_v43_.reset();vertex_cache_v36_.abandon_context();''')
 s=s.replace('    vertex_cache_v36_.abandon_context();\n','')
 s=s.replace('normal_=create(assets,false);premultiplied_=create(assets,true);',
 'auto normal_candidate_v43=create(assets,false,scope_v39_);auto premultiplied_candidate_v43=create(assets,true,scope_v39_);\n    normal_=std::move(normal_candidate_v43);premultiplied_=std::move(premultiplied_candidate_v43);')
 s=s.replace('    glGenBuffers(1,&buffer_);if(!buffer_)throw std::runtime_error("SWF vertex buffer unavailable");\n    barrier_v37("SWF stream buffer allocation");','    buffer_=0; // Stream object is admitted lazily on actual fallback.')
 s=s.replace('if(vertices.capacity()<xy_size*2){vertices.reserve(xy_size*2);++scratch_growths_v36_;}',
 '''if(xy_size>std::numeric_limits<std::size_t>::max()/2)throw std::runtime_error("SWF packed vertex span overflow");
    if(vertices.capacity()<xy_size*2){std::string error;
        if(!resources::reserve_cpu_vector_v41(vertices,vertices_capacity_v43_,budget_v39_,scope_v39_,xy_size*2,error))throw std::runtime_error(error);
        ++scratch_growths_v36_;}''')
 a=s.index('    const auto upload=[this]');b=s.index('    if(!vertex_cache_v36_.acquire',a)
 s=s[:a]+'''    const auto upload=[this](const float* data,std::size_t count,std::uintptr_t& out,std::string& error){
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
    const auto release=[this](std::uintptr_t value){release_vertices_v43(value);};
''' +s[b:]
 a=s.index('    glBindBuffer(GL_ARRAY_BUFFER,retained?');b=s.index('    glEnableVertexAttribArray(program->position)',a)
 s=s[:a]+'''    if(!retained){stream_storage_v43(vertices);
        ++stream_uploads_v36_;stream_bytes_v36_+=vertices.size()*sizeof(float);dh2::perf::state.uploads+=vertices.size()*sizeof(float);}
    glBindBuffer(GL_ARRAY_BUFFER,retained?GLuint(cached_buffer):buffer_);
''' +s[b:]
 s=s.replace('vertex_cache_v36_.clear([](std::uintptr_t value){const GLuint name=GLuint(value);glDeleteBuffers(1,&name);});',
 'vertex_cache_v36_.clear([this](std::uintptr_t value){release_vertices_v43(value);});')
 a=s.index('void SwfGpu::mask_rectangle()')
 s=s[:a]+'''void SwfGpu::release_vertices_v43(std::uintptr_t value){
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
''' +s[a:]
 return s
change('port/android-native/app/src/main/cpp/swf_gpu.cpp',swf_cpp)
patch=''
for path,text in changes.items():
 (out/Path(path).name).write_text(text)
 patch+=''.join(difflib.unified_diff((root/path).read_text().splitlines(True),text.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
(out/'integration.patch').write_bytes(patch.encode())
(out/'baseline-source-sha256.json').write_text(json.dumps(baseline,indent=2)+'\n')
print(len(changes),'staged shared files')
