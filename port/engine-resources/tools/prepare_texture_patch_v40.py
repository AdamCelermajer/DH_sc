"""Stage model/FX texture admission changes; never edit live shared sources."""
from pathlib import Path
import difflib,hashlib,json
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/texture-resource-v40';out.mkdir(parents=True,exist_ok=True)
changes={};baseline={}
def change(path,fn):
 before=(root/path).read_text();after=fn(before);assert before!=after,path
 baseline[path]=hashlib.sha256((root/path).read_bytes()).hexdigest();changes[path]=after
def zip_header(s):
 needle='    bool read(const std::string& uri,bool& found,std::vector<std::uint8_t>&,std::string&) const;'
 return s.replace(needle,needle+'\n    bool read_admitted(const std::string& uri,bool& found,std::vector<std::uint8_t>&,\n        const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string&) const;')
change('port/asset-payloads/zip_asset_pack_v1.hpp',zip_header)
def zip_source(s):
 signature='bool ZipAssetPackV1::read(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,std::string& e) const{'
 s=s.replace(signature,signature+'\n    return read_admitted(uri,found,out,{},e);\n}\nbool ZipAssetPackV1::read_admitted(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,\n    const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string& e) const{')
 return s.replace('        std::vector<std::uint8_t> candidate(metadata.bytes);','        if(before_payload&&!before_payload(metadata.bytes,e))return false;\n        std::vector<std::uint8_t> candidate(metadata.bytes);')
change('port/asset-payloads/zip_asset_pack_v1.cpp',zip_source)
# Formatting only: strict GCC diagnoses the pre-existing single-line update
# continuation. Statements and ordered source behavior are unchanged.
change('port/engine-textures/texture_owner_v1.cpp',lambda s:s.replace('if(!texture_update_parameters_v1(fields_,d,cb,e))return false;parameters_delivered_=true;return true;','if(!texture_update_parameters_v1(fields_,d,cb,e))return false;\n parameters_delivered_=true;return true;'))
def original_header(s):
 s=s.replace('#include <memory>','#include <memory>\n#include <functional>')
 needle='    bool read(const std::string&,bool& found,std::vector<std::uint8_t>&,std::string&) const;'
 return s.replace(needle,needle+'\n    bool read_admitted(const std::string&,bool& found,std::vector<std::uint8_t>&,\n        const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string&) const;')
change('port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp',original_header)
def original_source(s):
 start=s.index('bool OriginalCacheAssetsV1::read(');end=s.index('bool OriginalCacheAssetsV1::directory(',start)
 clone=s[start:end].replace('OriginalCacheAssetsV1::read(','OriginalCacheAssetsV1::read_admitted(').replace('std::vector<std::uint8_t>& out,std::string& e)','std::vector<std::uint8_t>& out,\n    const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string& e)').replace('state->pack.read(uri,found,out,e)','state->pack.read_admitted(uri,found,out,before_payload,e)')
 return s[:end]+clone+s[end:]
change('port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp',original_source)
def model(s):
 s=s.replace('#include "native_resource_budget_v38.hpp"','#include "native_resource_budget_v38.hpp"\n#include "../../../../../engine-resources/admitted_cpu_bytes_v40.hpp"\n#include "../../../../../engine-resources/retained_bytes_v39.hpp"')
 start=s.index('void release(std::vector<Draw>& batches,std::vector<GLuint>& textures){');end=s.index('void release_objects(',start)
 s=s[:start]+'#include "renderer_model_texture_budget_v40.inc"\n'+s[end:]
 start=s.index('GLuint upload(AAssetManager*');end=s.index('void sync_equipment_draws(',start)
 s=s[:start]+s[end:]
 reset_start=s.index('void reset_context(){');reset_end=s.index('void deactivate(){',reset_start)
 reset=s[reset_start:reset_end]
 assert reset.count('program=0;enabled=false;}')==1
 # Loot has an explicit lost-context release route; let it release its owned
 # registry entries before discarding the remaining generic texture owners.
 reset=reset.replace('program=0;enabled=false;}','program=0;enabled=false;discard_model_textures_v40();}')
 s=s[:reset_start]+reset+s[reset_end:]
 s=s.replace('cache,textures,&original)','cache,textures,&original,dh2::resources::ResourceScopeV37::equipment)')
 return s
change('port/android-native/app/src/main/cpp/model_renderer.cpp',model)
def loot(s):
 s=s.replace('if(!lost&&!loot_gpu_textures_v27.empty())glDeleteTextures(GLsizei(loot_gpu_textures_v27.size()),loot_gpu_textures_v27.data());','for(auto name:loot_gpu_textures_v27)release_model_texture_v40(name,lost);')
 return s.replace('loot_gpu_textures_v27,&equipment_platform->cache)','loot_gpu_textures_v27,&equipment_platform->cache,dh2::resources::ResourceScopeV37::loot)')
change('port/android-native/app/src/main/cpp/renderer_loot_gpu_v27.inc',loot)
def front(s):return s.replace('cache,weapon.images)','cache,weapon.images,nullptr,dh2::resources::ResourceScopeV37::actor)').replace('cache,actor.images)','cache,actor.images,nullptr,dh2::resources::ResourceScopeV37::actor)')
change('port/android-native/app/src/main/cpp/renderer_front_visual_v87.inc',front)
def fx_scene(s):
 return s.replace('struct EffectGpuTextureV4 {','''struct EffectGpuTextureV4 {
 std::shared_ptr<dh2::resources::ContextResourceBudgetV37> resource_budget_v40;
 dh2::resources::CpuAdmissionV40 decoded_cpu_v40;
 dh2::resources::ResourceTokenV37 texture_budget_v40;
 std::uint64_t texture_generation_v40{};''')
change('port/android-native/app/src/main/cpp/renderer_authored_effect_scene_v5.inc',fx_scene)
def fx_texture(s):
 s=s.replace(' GLint active{},alignment{};std::array<GLint,8> textures{};',''' std::shared_ptr<dh2::resources::ContextResourceBudgetV37> budget_v40;
 std::uint64_t generation_v40{};
 GLint active{},alignment{};std::array<GLint,8> textures{};''')
 s=s.replace('effect_texture_units_v5(units_ref){','effect_texture_units_v5(units_ref),budget_v40(dh2::android_resources::budget_lease_v39()),generation_v40(budget_v40->snapshot().context_generation){')
 s=s.replace(' ~EffectTextureStateGuardV5(){',''' ~EffectTextureStateGuardV5(){
  const auto current=budget_v40->snapshot();
  if(!current.context_ready||current.context_generation!=generation_v40){
   effect_texture_grid_v5={};effect_texture_cache_v5={};effect_texture_units_v5=0;return;
  }''')
 s=s.replace(' if(discard_context){',''' const auto current=r.resource_budget_v40?r.resource_budget_v40->snapshot():dh2::resources::ResourceBudgetSnapshotV37{};
 const bool same=current.context_ready&&current.context_generation==r.texture_generation_v40;
 if(discard_context&&r.texture&&same){error="V40 live effect texture context cannot be abandoned";return false;}
 if(discard_context||!same){''',1)
 s=s.replace('  r.owner().context_lost();r.texture=0;return true;','''  r.owner().context_lost();r.texture=0;
  if(r.texture_budget_v40&&!r.resource_budget_v40->release(r.texture_budget_v40,error))return false;
  r.texture_generation_v40=0;return true;''',1)
 s=s.replace(' r.owner().context_lost();return result;',''' r.owner().context_lost();
 if(!r.texture&&r.texture_budget_v40&&!r.resource_budget_v40->release(r.texture_budget_v40,error))return false;
 if(!r.texture)r.texture_generation_v40=0;
 return result;''',1)
 s=s.replace(' std::vector<std::uint8_t> bytes;std::string error;bool present=false;\n if(!cache.read("data/3d/textures/"+name,present,bytes,error)||!present)throw std::runtime_error("Required actual effect texture "+name+": "+error);',''' auto ledger=dh2::android_resources::budget_lease_v39();
 auto encoded=std::make_shared<dh2::resources::AdmittedVectorV40>();std::string error;bool present=false;
 const auto admit=[&](std::uint32_t bytes,std::string& why){return encoded->admission.reserve(ledger,dh2::resources::ResourceScopeV37::fx,bytes,why);};
 if(!cache.read_admitted("data/3d/textures/"+name,present,encoded->bytes,admit,error)||!present)throw std::runtime_error("Required actual effect texture "+name+": "+error);
 if(!encoded->admission.commit(error))throw std::runtime_error(error);''')
 s=s.replace(' auto r=std::make_shared<EffectGpuTextureV4>();',''' auto r=std::make_shared<EffectGpuTextureV4>();r->resource_budget_v40=ledger;
 dh2::textures::View view{};
 if(dh2_texture_open(encoded->bytes.data(),encoded->bytes.size(),&view)!=dh2::textures::Error::ok)throw std::runtime_error("Required actual effect texture header");
 GLint maximum=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maximum);
 if(maximum<=0||view.width>std::uint32_t(maximum)||view.height>std::uint32_t(maximum))throw std::runtime_error("Effect texture exceeds GPU dimensions");
 std::uint64_t decoded_bytes,gpu_bytes;
 dh2::textures::TextureDescriptorV1 descriptor;
 if(!dh2::textures::texture_image_descriptor_v1(descriptor,{14,view.width,view.height,0},{},&effect_driver_options_v4.flags88(),error)||
    !dh2::resources::rgba_texture_bytes_v37(view.width,view.height,false,decoded_bytes,error)||
    !dh2::resources::rgba_texture_bytes_v37(view.width,view.height,descriptor.mipmapped!=0,gpu_bytes,error))throw std::runtime_error(error);
 dh2::resources::ResourceReservationV37 gpu;
 if(!ledger->reserve_create({dh2::resources::ResourceKindV37::texture,dh2::resources::ResourceScopeV37::fx,gpu_bytes,0},gpu,error)||
    !r->decoded_cpu_v40.reserve(ledger,dh2::resources::ResourceScopeV37::fx,decoded_bytes,error))throw std::runtime_error(error);''')
 s=s.replace('std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes))','dh2::resources::AdmittedVectorV40::borrow(encoded)')
 needle=' EffectTextureStateGuardV5 restore(effect_texture_grid_v5,effect_texture_cache_v5,effect_texture_units_v5);'
 s=s.replace(needle,''' if(!r->decoded_cpu_v40.commit(error))throw std::runtime_error(error);
'''+needle+'''
 // Allocate cache node before GL ownership; rollback every failed candidate.
 auto cached=effect_gpu_textures_v4.try_emplace(name,r);
 if(!cached.second)throw std::runtime_error("V40 effect texture publication changed");
 r->texture_generation_v40=ledger->snapshot().context_generation;''')
 s=s.replace(' try{\n  dh2::textures::TextureInitialBindServicesV1 initial;',' try{\n  dh2::textures::TextureInitialBindServicesV1 initial;')
 s=s.replace(' }catch(...){\n  const auto exception=std::current_exception();std::string cleanup;','''  check("Source effect texture upload/mipmap allocation");
  if(!gpu.commit(r->texture_budget_v40,error))throw std::runtime_error(error);
 }catch(...){
  const auto exception=std::current_exception();std::string cleanup;
  effect_gpu_textures_v4.erase(cached.first);''',1)
 s=s.replace(' effect_gpu_textures_v4.emplace(name,r);return r;',' return r;')
 return s
change('port/android-native/app/src/main/cpp/renderer_effect_texture_v5.inc',fx_texture)
def cmake(s):
 needle='"${DH2_SOURCE_DIR}/port/engine-resources/retained_bytes_v39.cpp")'
 return s.replace(needle,'"${DH2_SOURCE_DIR}/port/engine-resources/retained_bytes_v39.cpp"\n  "${DH2_SOURCE_DIR}/port/engine-resources/admitted_cpu_bytes_v40.cpp")')
change('port/android-native/app/src/main/cpp/CMakeLists.txt',cmake)
diff=[]
for path,after in changes.items():
 before=(root/path).read_text();(out/Path(path).name).write_text(after)
 diff.extend(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
(out/'texture-resource-v40.patch').write_bytes(''.join(diff).encode())
(out/'baseline-source-sha256.json').write_text(json.dumps(baseline,indent=2)+'\n')
print(json.dumps({'staged':str(out),'files':len(changes),'patch_bytes':(out/'texture-resource-v40.patch').stat().st_size}))
