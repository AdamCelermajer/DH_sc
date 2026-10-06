#include "material_image_identity_v43.hpp"
#include <scene.hpp>
#include <zip_asset_pack_v1.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
void need(bool b,const std::string& e){if(!b)throw std::runtime_error(e);}
int main(int argc,char** argv){try{need(argc==3,"cache/original-gold arguments required");
 std::ifstream original(argv[2],std::ios::binary);need(bool(original),"original factory gold unavailable");auto word=[&](){std::uint32_t v{};original.read(reinterpret_cast<char*>(&v),4);need(bool(original),"original gold truncated");return v;};auto text=[&](){std::string s(word(),0);original.read(s.data(),s.size());need(bool(original),"original gold string truncated");return s;};
 const auto original_cases=word();std::string original_error;
 for(unsigned i=0;i<original_cases;++i){auto directory=text();loader::MaterialImageIdentityV43 id{};id.state=loader::MaterialImageStateV43::authored_image;id.authored_uri=text();std::array<std::string,2> want{text(),text()},got;need(loader::source_factory_image_candidates_v43(directory,id,got,original_error)&&got==want&&original_error.empty(),"native candidates differ from original factory operands");}
 auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary);need(bool(*file),"cache unavailable");file->seekg(0,std::ios::end);auto size=file->tellg();
 auto pack=std::make_shared<assets::ZipAssetPackV1>();std::string e="stale";
 need(pack->mount({file,std::uint64_t(size),[file](std::uint64_t at,void* out,std::size_t n,std::string& e){file->clear();file->seekg(at);file->read(static_cast<char*>(out),n);if(!*file){e="actual cache positional read failed";return false;}e.clear();return true;}},"com.gameloft.android.GAND.GloftD2SS/files/",e),e);
 need(pack->entries().size()==6833,"actual cache directory differs");
 loader::MaterialImageServicesV43 cb{pack,loader::supplied_cache_image_uri_v43,[pack](const std::string& s,bool& f,std::vector<std::uint8_t>& b,std::string& e){return pack->read(s,f,b,e);}};
 unsigned materials=0,samplers=0,no_image=0,read_images=0,foreign_rejected=0;loader::MaterialImageReadV43 pinned;
 for(const std::string source:{"data/3d/gameobjects/go_chest_swamp.bdae","data/3d/modules/swamp/swamp.bdae","data/3d/modules/crypt/crypt.bdae"}){
  bool found=false;std::vector<std::uint8_t> data;need(pack->read(source,found,data,e)&&found,e);resources::BresView view{};need(dh2_bres_open(&view,data.data(),data.size())==resources::BresError::ok,"actual BRES open failed");scene::Scene s;need(scene::load(view,s,e),e);
  for(auto& m:s.materials){++materials;std::vector<loader::MaterialImageIdentityV43> ids;need(loader::decode_retained_material_images_v43(s,m,view,source,ids,e)&&e.empty(),e);std::uint32_t last=0;bool first=true;
   for(auto& id:ids){++samplers;need(id.material_id==m.id&&id.source_resource_uri==source,"source provenance differs");need(first||id.parameter_order>last,"source parameter order lost");first=false;last=id.parameter_order;
    loader::MaterialImageReadV43 got;need(loader::read_material_image_v43(id,cb,got,e)&&e.empty(),e);
    if(id.state==loader::MaterialImageStateV43::source_no_image){++no_image;need(!got.bytes&&got.canonical_uri.empty(),"source no-image became fallback texture");}
    else {++read_images;need(id.authored_uri.find("3d/textures/")!=std::string::npos,"full authored URI lost");need(got.canonical_uri.rfind("data/3d/textures/",0)==0&&got.bytes&&got.bytes->size()>0&&got.resource_owner==pack,"actual canonical cache bytes/owner lost");pinned=got;}
   }
   auto copy=m;auto preserved=ids;need(!loader::decode_retained_material_images_v43(s,copy,view,source,ids,e)&&ids.size()==preserved.size()&&(ids.empty()||ids.front().authored_uri==preserved.front().authored_uri),"foreign material accepted or failure not atomic");++foreign_rejected;
   if(!ids.empty()){auto damaged=data;auto record=ids.front().parameter_record;damaged[record+12]=2;auto badview=view;badview.bytes=damaged.data();need(!loader::decode_retained_material_images_v43(s,m,badview,source,ids,e)&&ids.size()==preserved.size(),"unsupported sampler arrays accepted or failure not atomic");}
  }
 }
 need(no_image>0&&read_images>0,"actual no-image/image source branch untested");
 auto bad=pinned.identity;bad.authored_uri="q:/data/iphone/3d/textures/source_file_that_is_absent.tga";auto prior=pinned.bytes;need(!loader::read_material_image_v43(bad,cb,pinned,e)&&pinned.bytes==prior&&e.find("missing")!=std::string::npos,"missing image became fallback or overwrote output");
 bad.authored_uri="Q:/data/old/3D/Modules/Catacombs/env_swamp.tga";need(!loader::read_material_image_v43(bad,cb,pinned,e)&&pinned.bytes==prior&&e.find("mount resolver")!=std::string::npos,"legacy path silently basename-resolved");
 bad.authored_uri="q:/data/iphone/3d/textures/../env_swamp.tga";need(!loader::read_material_image_v43(bad,cb,pinned,e)&&pinned.bytes==prior,"parent path accepted");
 auto unbound=cb;unbound.owner.reset();need(!loader::read_material_image_v43(pinned.identity,unbound,pinned,e)&&pinned.bytes==prior,"missing owner accepted");
 auto uppercase=pinned.identity;uppercase.authored_uri="Q:/data/iphone/3D/Textures/ENV_SWAMP.TGA";std::string uri;need(loader::supplied_cache_image_uri_v43(uppercase,uri,e)&&uri=="data/3d/textures/env_swamp.tga"&&e.empty(),"strict compatibility key differs");
 std::weak_ptr<void> weak=pack;cb={};unbound={};pack.reset();file.reset();need(!weak.expired()&&pinned.bytes==prior,"independent genuine filesystem/byte pin lost");pinned={};need(weak.expired(),"filesystem owner leak after image release");
 std::cout<<"PASS cache_files=6833 actual_assets=3 materials="<<materials<<" sampler_records="<<samplers<<" source_no_image="<<no_image<<" original_cache_reads="<<read_images<<" foreign_material_rejections="<<foreign_rejected<<" original_factory_comparisons="<<original_cases<<" missing_legacy_parent_owner_array_rejected=1 actual_byte_owner_release=1\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
