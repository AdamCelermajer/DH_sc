from pathlib import Path
import subprocess,json,hashlib,shutil
p=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=p/'reference/all-map-image-dependencies-v44';build=p.parents[2]/'build/all-map-image-dependencies-v44';build.mkdir(parents=True,exist_ok=True)
cpp=r'''#include "material_image_identity_v43.hpp"
#include <scene.hpp>
#include <zip_asset_pack_v1.hpp>
#include <module_selected_scene_v2.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>
#include <tuple>
using namespace dh2;
void need(bool b,const std::string& e){if(!b)throw std::runtime_error(e);}
std::string quote(const std::string& s){std::string r="\"";for(unsigned char c:s){switch(c){case '"':r+="\\\"";break;case '\\':r+="\\\\";break;case '\n':r+="\\n";break;case '\r':r+="\\r";break;case '\t':r+="\\t";break;default:if(c<32){const char* h="0123456789abcdef";r+="\\u00";r+=h[c>>4];r+=h[c&15];}else r+=c;}}return r+'"';}
int main(int argc,char** argv){try{need(argc==4,"cache/list/output args required");auto f=std::make_shared<std::ifstream>(argv[1],std::ios::binary);need(bool(*f),"original cache unavailable");f->seekg(0,std::ios::end);auto size=f->tellg();assets::ZipAssetPackV1 pack;std::string e;
 need(pack.mount({f,std::uint64_t(size),[f](std::uint64_t p,void* out,std::size_t n,std::string& e){f->clear();f->seekg(p);f->read(static_cast<char*>(out),n);if(!*f){e="actual archive positional read failed";return false;}e.clear();return true;}},"com.gameloft.android.GAND.GloftD2SS/files/",e),e);need(pack.entries().size()==6833,"cache directory differs");
 std::ifstream list(argv[2]);std::ofstream output(argv[3]);need(bool(list)&&bool(output),"list/output unavailable");std::map<std::string,std::pair<bool,std::size_t>> images;unsigned assets=0,instances=0,materials=0,samplers=0,rejected=0;std::string uri;
 while(std::getline(list,uri)){if(!uri.empty()&&uri.back()=='\r')uri.pop_back();if(uri.empty())continue;const auto tab=uri.find('\t');const std::string selector=tab==std::string::npos?"":uri.substr(tab+1);if(tab!=std::string::npos)uri.resize(tab);++assets;bool found=false;std::vector<std::uint8_t> bytes;need(pack.read(uri,found,bytes,e)&&found,"actual BRES dependency missing: "+uri+e);resources::BresView v{};auto opened=dh2_bres_open(&v,bytes.data(),bytes.size());output<<"{\"uri\":"<<quote(uri)<<",\"module_selector\":"<<quote(selector)<<",\"bres_bytes\":"<<bytes.size();
  if(opened!=resources::BresError::ok){++rejected;output<<",\"status\":\"bres_open_rejected\",\"bres_error\":"<<unsigned(opened)<<"}\n";continue;}
  scene::Scene s;bool loaded=false;if(selector.empty())loaded=scene::load(v,s,e);else {world::ModuleSelectedSceneV2 selected;bool found=false;loaded=world::module_selected_scene_v2(v,selector.c_str(),selected,found,e);if(loaded&&!found){++rejected;output<<",\"status\":\"source_module_selector_not_found\"}\n";continue;}s=std::move(selected.scene);}
  if(!loaded){++rejected;output<<",\"status\":\"unsupported_native_scene_domain\",\"error\":"<<quote(e)<<"}\n";continue;}
  output<<",\"status\":\"decoded_authored_asset_scene\",\"graph_nodes\":"<<s.graph.size()<<",\"ignored_instance_kinds\":"<<s.ignored_instances<<",\"materials\":[";
  for(std::size_t m=0;m<s.materials.size();++m){++materials;if(m)output<<',';std::vector<loader::MaterialImageIdentityV43> ids;output<<"{\"catalog\":"<<m<<",\"id\":"<<quote(s.materials[m].id)<<",\"effect_file\":"<<quote(s.materials[m].effect_file)<<",\"effect_uri\":"<<quote(s.materials[m].effect_uri);
   if(!loader::decode_retained_material_images_v43(s,s.materials[m],v,uri,ids,e)){output<<",\"status\":\"unsupported_sampler_layout\",\"error\":"<<quote(e)<<'}';continue;}
   output<<",\"status\":\"decoded\",\"samplers\":[";
   for(std::size_t i=0;i<ids.size();++i){++samplers;if(i)output<<',';auto& id=ids[i];output<<"{\"parameter_name\":"<<quote(id.parameter_name)<<",\"parameter_record\":"<<id.parameter_record<<",\"parameter_order\":"<<id.parameter_order<<",\"image_catalog\":"<<id.image_catalog<<",\"image_record\":"<<id.image_record<<",\"image_id\":"<<quote(id.image_id)<<",\"authored_uri\":"<<quote(id.authored_uri);
    if(id.state==loader::MaterialImageStateV43::source_no_image)output<<",\"status\":\"source_no_image\"";
    else {std::string canonical;if(!loader::supplied_cache_image_uri_v43(id,canonical,e))output<<",\"status\":\"original_mount_route_required\",\"error\":"<<quote(e);
     else {auto cached=images.find(canonical);if(cached==images.end()){bool image_found=false;std::vector<std::uint8_t> image;need(pack.read(canonical,image_found,image,e),e);cached=images.emplace(canonical,std::pair<bool,std::size_t>{image_found,image_found?image.size():0}).first;}
      output<<",\"canonical_compatibility_uri\":"<<quote(canonical)<<",\"status\":"<<quote(cached->second.first?"compatibility_canonical_resolved":"archive_entry_absent_for_compatibility_canonical_uri")<<",\"actual_archive_image_bytes\":"<<cached->second.second;
     }
    }output<<'}';
   }output<<"]}";
  }output<<"],\"instances\":[";
  for(std::size_t i=0;i<s.instances.size();++i){++instances;if(i)output<<',';auto& in=s.instances[i];output<<"{\"instance_index\":"<<i<<",\"node\":"<<quote(in.node)<<",\"node_index\":"<<in.node_index<<",\"node_name\":"<<quote(s.graph.at(in.node_index).name)<<",\"geometry_catalog\":"<<in.geometry<<",\"controller_catalog\":"<<in.controller<<",\"material_slots\":[";for(std::size_t slot=0;slot<in.materials.size();++slot){if(slot)output<<',';output<<"{\"slot\":"<<slot<<",\"material_catalog\":"<<in.materials[slot]<<'}';}output<<"]}";}
  output<<"]}\n";
 }
 output.flush();need(bool(output),"matrix native output write failed");std::cout<<"PASS original_cache=6833 actual_bres="<<assets<<" authored_instances="<<instances<<" materials="<<materials<<" sampler_records="<<samplers<<" distinct_canonical_image_reads="<<images.size()<<" explicit_source_domain_rejections="<<rejected<<" resource_bytes_peak=one_bres_plus_one_image no_devices=1\n";
 }catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
'''
(p/'tests/all_map_image_dependencies_v44.cpp').write_text(cpp);v=p/'vendor/character-rng-integration-v5-loading/port';shared=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc');linux=lambda f:'/mnt/c/'+str(f).replace('\\','/')[3:]
sources=[p/'material_image_identity_v43.cpp',p/'tests/all_map_image_dependencies_v44.cpp',v/'scene-materials/scene.cpp',v/'level-world/module_selected_scene_v2.cpp',v/'engine-resources/resources.cpp',v/'engine-math/math.cpp',shared/'port/asset-payloads/zip_asset_pack_v1.cpp'];exe=build/'probe'
command=['wsl.exe','-d','Ubuntu','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer']+['-I'+linux(s) for s in [p,v/'scene-materials',v/'level-world',v/'engine-resources',shared/'port/asset-payloads']]+[linux(s) for s in sources]+['-lz','-o',linux(exe)]
result=subprocess.run(command,capture_output=True,text=True);(r/'build.log').write_text(result.stdout+result.stderr);print('build',result.returncode);print(result.stderr[-3500:]);result.check_returncode()
run=['wsl.exe','-d','Ubuntu','--',linux(exe),linux(Path(r'C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')),linux(r/'bres-inputs.txt'),linux(r/'native-asset-identities.jsonl')];result=subprocess.run(run,capture_output=True,text=True);(r/'native.log').write_text(result.stdout+result.stderr);print(result.stdout);print(result.stderr);result.check_returncode()
receipt={'validation':'PASS','scope':'Actual84 supplied ZIP BRES assets through current selected native Scene and V43 material identity; authored asset instances/static dependency scope, no runtime/seed/current GS/activation/pass/graphics residency claim','native_evidence':result.stdout.strip(),'compile_command':command,'run_command':run,'source_hashes':{str(s):hashlib.sha256(s.read_bytes()).hexdigest() for s in sources},'input_plan_sha256':hashlib.sha256((r/'dependency-plan.json').read_bytes()).hexdigest(),'output_sha256':hashlib.sha256((r/'native-asset-identities.jsonl').read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(exe.read_bytes()).hexdigest()}
(r/'native-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');shutil.copy(__file__,r/'prove_native.py')
