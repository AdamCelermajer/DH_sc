"""Stage generic buffer + precise CPU vector admission, no live shared edits."""
from pathlib import Path
import difflib,hashlib,json,re
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/buffer-resource-v41';out.mkdir(parents=True,exist_ok=True)
changes={};baseline={}
def change(path,fn):
 before=(root/path).read_text();after=fn(before);assert before!=after,path
 baseline[path]=hashlib.sha256((root/path).read_bytes()).hexdigest();changes[path]=after
change('port/engine-resources/resource_budget_v37.hpp',lambda s:s.replace('struct ResourceBudgetSnapshotV37 {','struct ResourceContextStateV41 {std::uint64_t generation{};bool ready{};};\nstruct ResourceBudgetSnapshotV37 {').replace(' ResourceBudgetSnapshotV37 snapshot()const;',' ResourceBudgetSnapshotV37 snapshot()const;\n ResourceContextStateV41 context_state_v41()const;'))
change('port/engine-resources/resource_budget_v37.cpp',lambda s:s.replace('ResourceBudgetSnapshotV37 ContextResourceBudgetV37::snapshot()const{','ResourceContextStateV41 ContextResourceBudgetV37::context_state_v41()const{std::lock_guard<std::mutex> lock(mutex_);return {snapshot_.context_generation,snapshot_.context_ready};}\nResourceBudgetSnapshotV37 ContextResourceBudgetV37::snapshot()const{'))
def gpu_calls(s,scope,storage=True):
 def byte_count(text):
  m=re.fullmatch(r'(.+)\*sizeof\((.+)\)',text)
  if m:return 'buffer_bytes_v41('+m[1]+',sizeof('+m[2]+'))'
  m=re.fullmatch(r'(.+)\*2',text)
  if m:return 'buffer_bytes_v41('+m[1]+',2)'
  raise RuntimeError('Unrecognized exact buffer byte producer: '+text)
 pattern=r'glGenBuffers\(1,&([\w.]+)\);glBindBuffer\((GL_ARRAY_BUFFER|GL_ELEMENT_ARRAY_BUFFER),\1\);\s*glBufferData\(\2,([^,;]+),([^,;]+),([^;]+)\);'
 s,n=re.subn(pattern,lambda m:'buffer_storage_v41('+','.join([m[1],m[2],byte_count(m[3]),m[4],m[5],'dh2::resources::ResourceScopeV37::'+scope])+');',s)
 assert n>0 or not storage
 pattern=r'glBindBuffer\((GL_ARRAY_BUFFER|GL_ELEMENT_ARRAY_BUFFER),([\w.]+)\);\s*glBufferSubData\(\1,0,([^,;]+),([^;]+)\);'
 s=re.sub(pattern,lambda m:'buffer_subdata_v41('+','.join([m[2],m[1],byte_count(m[3]),m[4],'dh2::resources::ResourceScopeV37::'+scope])+');',s)
 return s
def model(s):
 s=s.replace('#include "world_visibility_v35.hpp"','#include "../../../../../engine-resources/cpu_vector_capacity_v41.hpp"\n#include "world_visibility_v35.hpp"')
 s=s.replace('std::vector<Vertex> cpu_vertices;','dh2::resources::CpuGeometryStorageV41<Vertex> cpu_geometry_v41;')
 s=s.replace('.cpu_vertices','.cpu_geometry_v41.vertices')
 s=s.replace('const auto& batch=group.draws[i];','auto& batch=group.draws[i];')
 s=s.replace('#include "renderer_model_texture_budget_v40.inc"','#include "renderer_buffer_budget_v41.inc"\n#include "renderer_model_texture_budget_v40.inc"')
 s=gpu_calls(s,'world')
 # Equipment all generic GPU writes are the same explicit equipment scope.
 start=s.index('void sync_equipment_draws(');end=s.index('void normalize(',start)
 section=s[start:end].replace('ResourceScopeV37::world','ResourceScopeV37::equipment')
 section=section.replace('std::vector<Vertex> vertices(geometry.positions.size());','''dh2::resources::CpuVectorCapacityV41 vertex_capacity_v41;
          std::vector<Vertex> vertices;
          if(!dh2::resources::reserve_cpu_vector_v41(vertices,vertex_capacity_v41,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::equipment,geometry.positions.size(),error))throw std::runtime_error(error);
          vertices.resize(geometry.positions.size());''')
 # Scope-local error is present at function beginning? Supply explicit one.
 section=section.replace('  bool rebuild=','  std::string error;\n  bool rebuild=',1)
 section=section.replace('std::vector<std::uint16_t> indices;indices.reserve(primitive.indices.size());','''dh2::resources::CpuVectorCapacityV41 index_capacity_v41;
          std::vector<std::uint16_t> indices;
          if(!dh2::resources::reserve_cpu_vector_v41(indices,index_capacity_v41,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::equipment,primitive.indices.size(),error))throw std::runtime_error(error);''')
 section=section.replace('draw.cpu_geometry_v41.vertices=std::move(vertices);','draw.cpu_geometry_v41.vertices=std::move(vertices);draw.cpu_geometry_v41.capacity=std::move(vertex_capacity_v41);')
 s=s[:start]+section+s[end:]
 # Model and world staging allocations admit exact numeric capacity first.
 s=s.replace('std::vector<Vertex> vertices(mesh.vertices);','''dh2::resources::CpuVectorCapacityV41 vertex_capacity_v41;std::vector<Vertex> vertices;
        if(!dh2::resources::reserve_cpu_vector_v41(vertices,vertex_capacity_v41,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,mesh.vertices,error))throw std::runtime_error(error);
        vertices.resize(mesh.vertices);''')
 s=s.replace('std::vector<std::uint16_t> indices(p.index_count);','''dh2::resources::CpuVectorCapacityV41 index_capacity_v41;std::vector<std::uint16_t> indices;
        if(!dh2::resources::reserve_cpu_vector_v41(indices,index_capacity_v41,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,p.index_count,error))throw std::runtime_error(error);
        indices.resize(p.index_count);''')
 s=s.replace('batch.cpu_geometry_v41.vertices=vertices;','''if(!dh2::resources::reserve_cpu_vector_v41(batch.cpu_geometry_v41.vertices,batch.cpu_geometry_v41.capacity,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,vertices.size(),error))throw std::runtime_error(error);
          batch.cpu_geometry_v41.vertices=vertices;''')
 s=s.replace('if(batch.cpu_geometry_v41.vertices.size()!=primitive.vertices.size())batch.cpu_geometry_v41.vertices=primitive.vertices;','''if(batch.cpu_geometry_v41.vertices.size()!=primitive.vertices.size()){
          if(!dh2::resources::reserve_cpu_vector_v41(batch.cpu_geometry_v41.vertices,batch.cpu_geometry_v41.capacity,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,primitive.vertices.size(),error))throw std::runtime_error(error);
          batch.cpu_geometry_v41.vertices=primitive.vertices;
         }''')
 s=s.replace('discard_model_textures_v40();}','discard_model_textures_v40();discard_generic_buffers_v41();}')
 return s
change('port/android-native/app/src/main/cpp/model_renderer.cpp',model)
def model_texture(s):return s.replace('if(b.vertices)glDeleteBuffers(1,&b.vertices);if(b.indices)glDeleteBuffers(1,&b.indices);','release_buffer_v41(b.vertices,false);release_buffer_v41(b.indices,false);')
change('port/android-native/app/src/main/cpp/renderer_model_texture_budget_v40.inc',model_texture)
change('port/android-native/app/src/main/cpp/renderer_front_visual_v87.inc',lambda s:gpu_calls(s,'actor'))
change('port/android-native/app/src/main/cpp/renderer_front_draw_v87.inc',lambda s:gpu_calls(s,'actor',False).replace('const auto& batch=actor.draws[i];','auto& batch=actor.draws[i];'))
def loot(s):
 s=gpu_calls(s,'loot').replace('.cpu_vertices','.cpu_geometry_v41.vertices')
 s=s.replace('draw.cpu_geometry_v41.vertices.resize(part.vertices.size());','''if(!dh2::resources::reserve_cpu_vector_v41(draw.cpu_geometry_v41.vertices,draw.cpu_geometry_v41.capacity,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::loot,part.vertices.size(),error))throw std::runtime_error(error);
   draw.cpu_geometry_v41.vertices.resize(part.vertices.size());''')
 s=s.replace('if(lost)record.draws.clear();else{','if(lost){for(auto& draw:record.draws){release_buffer_v41(draw.vertices,true);release_buffer_v41(draw.indices,true);}record.draws.clear();}else{')
 return s
change('port/android-native/app/src/main/cpp/renderer_loot_gpu_v27.inc',loot)
def fx_header(s):
 s=s.replace('#include "objects.hpp"','#include "objects.hpp"\n#include "../engine-resources/cpu_vector_capacity_v41.hpp"')
 s=s.replace(' AuthoredFxGeometryPacketV7 packet_;',''' std::shared_ptr<resources::ContextResourceBudgetV37> cpu_budget_v41_;
 resources::CpuVectorCapacityV41 packet_vertices_v41_,packet_indices_v41_,scratch_vertices_v41_,scratch_indices_v41_,source_indices_v41_;
 AuthoredFxGeometryPacketV7 packet_;''')
 s=s.replace('public:\n bool update(','public:\n bool bind_cpu_budget_v41(std::shared_ptr<resources::ContextResourceBudgetV37>,std::string&);\n void reset_buffers_v41()noexcept;\n bool update(')
 return s
change('port/level-world/authored_fx_geometry_packet_v7.hpp',fx_header)
def fx_source(s):
 start=s.index('bool AuthoredFxGeometryCacheV34::update(')
 api='''bool AuthoredFxGeometryCacheV34::bind_cpu_budget_v41(std::shared_ptr<resources::ContextResourceBudgetV37> budget,std::string& error){
 if(!budget||(cpu_budget_v41_&&cpu_budget_v41_!=budget)){error="V41 same FX CPU budget owner required";return false;}
 if(!cpu_budget_v41_&&(packet_.vertices.capacity()||packet_.indices.capacity()||scratch_vertices_.capacity()||scratch_indices_.capacity()||source_indices_.capacity())){error="V41 FX vector adoption after allocation prohibited";return false;}
 cpu_budget_v41_=std::move(budget);return true;
}
void AuthoredFxGeometryCacheV34::reset_buffers_v41()noexcept{
 std::vector<objects::Vertex>().swap(packet_.vertices);std::vector<std::uint16_t>().swap(packet_.indices);
 std::vector<objects::Vertex>().swap(scratch_vertices_);std::vector<std::uint16_t>().swap(scratch_indices_);std::vector<std::uint32_t>().swap(source_indices_);
 packet_vertices_v41_.reset();packet_indices_v41_.reset();scratch_vertices_v41_.reset();scratch_indices_v41_.reset();source_indices_v41_.reset();
 packet_.source_color_missing=false;validated_vertex_count_=0;counters_={};
}
'''
 s=s[:start]+api+s[start:]
 s=s.replace(' scratch_vertices_.resize(part.positions.size());',''' if(cpu_budget_v41_&&!resources::reserve_cpu_vector_v41(scratch_vertices_,scratch_vertices_v41_,cpu_budget_v41_,resources::ResourceScopeV37::fx,part.positions.size(),error))return false;
 scratch_vertices_.resize(part.positions.size());''')
 s=s.replace(' if(topology){scratch_indices_.resize(primitive.indices.size());',''' if(topology){
  if(cpu_budget_v41_&&!resources::reserve_cpu_vector_v41(scratch_indices_,scratch_indices_v41_,cpu_budget_v41_,resources::ResourceScopeV37::fx,primitive.indices.size(),error))return false;
  scratch_indices_.resize(primitive.indices.size());''')
 s=s.replace(' if(vertices)packet_.vertices.swap(scratch_vertices_);\n if(topology){packet_.indices.swap(scratch_indices_);source_indices_=primitive.indices;}',''' // Source index snapshot allocation precedes packet publication. Numeric
 // assignment and swaps below cannot throw after admitted capacity exists.
 if(topology){
  if(cpu_budget_v41_&&!resources::reserve_cpu_vector_v41(source_indices_,source_indices_v41_,cpu_budget_v41_,resources::ResourceScopeV37::fx,primitive.indices.size(),error))return false;
  source_indices_=primitive.indices;
 }
 if(vertices){packet_.vertices.swap(scratch_vertices_);std::swap(packet_vertices_v41_,scratch_vertices_v41_);}
 if(topology){packet_.indices.swap(scratch_indices_);std::swap(packet_indices_v41_,scratch_indices_v41_);}''')
 return s
change('port/level-world/authored_fx_geometry_packet_v7.cpp',fx_source)
def fx_scene(s):
 s=s.replace('resource.geometry_cache={};','resource.geometry_cache.reset_buffers_v41();').replace('r.geometry_cache={};','r.geometry_cache.reset_buffers_v41();')
 return s.replace('  if(!r.geometry_cache.update(part,s.material,changed,error))','  if(!r.geometry_cache.bind_cpu_budget_v41(dh2::android_resources::budget_lease_v39(),error))throw std::runtime_error(error);\n  if(!r.geometry_cache.update(part,s.material,changed,error))')
change('port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc',fx_scene)
diff=[]
for path,after in changes.items():
 before=(root/path).read_text();(out/Path(path).name).write_text(after)
 diff.extend(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
(out/'buffer-resource-v41.patch').write_bytes(''.join(diff).encode())
(out/'baseline-source-sha256.json').write_text(json.dumps(baseline,indent=2)+'\n')
print(json.dumps({'staged':str(out),'files':len(changes),'patch_bytes':(out/'buffer-resource-v41.patch').stat().st_size}))
