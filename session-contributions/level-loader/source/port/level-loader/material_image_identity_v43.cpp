#include "material_image_identity_v43.hpp"
#include <scene.hpp>
#include <resources.hpp>
#include <zip_asset_pack_v1.hpp>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh2::loader {namespace {
struct Reader {
 const resources::BresView& v;
 void range(std::uint64_t p,std::uint64_t n)const {if(!v.bytes||p>v.size||n>v.size-p)throw std::runtime_error("Material image field outside SAME BRES");}
 std::uint32_t word(std::uint64_t p)const {range(p,4);const auto* b=v.bytes+p;return b[0]|std::uint32_t(b[1])<<8|std::uint32_t(b[2])<<16|std::uint32_t(b[3])<<24;}
 std::string text(std::uint32_t p)const {if(!p)throw std::runtime_error("Required source image string");range(p,1);const auto n=std::min<std::size_t>(v.size-p,4096);const auto* end=static_cast<const std::uint8_t*>(std::memchr(v.bytes+p,0,n));if(!end)throw std::runtime_error("Unterminated material image string");return {reinterpret_cast<const char*>(v.bytes+p),std::size_t(end-(v.bytes+p))};}
 std::uint32_t item(resources::Library lib,std::uint32_t i)const {auto* q=dh2_bres_library_item(&v,lib,i);if(!q)throw std::runtime_error("Required source image/catalog record");return std::uint32_t(q-v.bytes);}
};
}
bool decode_retained_material_images_v43(const scene::Scene& s,const scene::Material& m,
 const resources::BresView& v,const std::string& source,std::vector<MaterialImageIdentityV43>& out,std::string& e){try{
 std::uint32_t catalog=UINT32_MAX;for(std::size_t i=0;i<s.materials.size();++i)if(&s.materials[i]==&m){catalog=std::uint32_t(i);break;}
 if(catalog==UINT32_MAX||source.empty()){e="Required SAME retained material and source resource URI";return false;}
 Reader r{v};if(dh2_bres_library_count(&v,resources::Library::material)!=s.materials.size()){e="SAME retained Scene/BRES material catalog differs";return false;}
 auto p=r.item(resources::Library::material,catalog);if(r.text(r.word(p))!=m.id){e="SAME retained Scene/BRES material identity differs";return false;}
 const auto n=r.word(p+16),base=r.word(p+20);if(n>100000)throw std::runtime_error("Material parameter count exceeds bound");r.range(base,std::uint64_t(n)*24);
 std::vector<MaterialImageIdentityV43> next;
 for(std::uint32_t i=0;i<n;++i){auto q=base+24*i;if(r.word(q+8)!=11)continue;
  if(r.word(q+12)!=1||r.word(r.word(q+16))!=1)throw std::runtime_error("Required scalar authored sampler layout");
  MaterialImageIdentityV43 a{};a.material_catalog=catalog;a.parameter_record=q;a.parameter_order=i;a.parameter_name=r.text(r.word(q));a.material_id=m.id;a.source_resource_uri=source;
  const auto index=r.word(r.word(r.word(q+20)));a.image_catalog=index;
  if(index!=UINT32_MAX){a.state=MaterialImageStateV43::authored_image;a.image_record=r.item(resources::Library::image,index);a.image_id=r.text(r.word(a.image_record));a.authored_uri=r.text(r.word(a.image_record+8));if(a.authored_uri.empty())throw std::runtime_error("Authored image URI is empty");}
  next.push_back(std::move(a));
 }
 out=std::move(next);e.clear();return true;
 }catch(const std::exception& x){e=x.what();return false;}}
bool supplied_cache_image_uri_v43(const MaterialImageIdentityV43& a,std::string& out,std::string& e){
 if(a.state!=MaterialImageStateV43::authored_image||a.authored_uri.empty()){e="Required authored image for supplied-cache resolution";return false;}
 std::string path=a.authored_uri;for(auto& c:path){if(c=='\\')c='/';if(c>='A'&&c<='Z')c=char(c-'A'+'a');}
 const std::string prefix="q:/data/iphone/";
 if(path.rfind(prefix,0)==0)path="data/"+path.substr(prefix.size());
 else if(path.rfind("data/",0)!=0){e="Required original mount resolver for legacy/exported image URI: "+a.authored_uri;return false;}
 std::string next;if(!assets::ZipAssetPackV1::key(path,next,e))return false;out=std::move(next);e.clear();return true;
}
bool read_material_image_v43(const MaterialImageIdentityV43& a,const MaterialImageServicesV43& cb,MaterialImageReadV43& out,std::string& e){
 MaterialImageReadV43 next{};next.identity=a;
 if(a.state==MaterialImageStateV43::source_no_image){if(a.image_catalog!=UINT32_MAX||!a.authored_uri.empty()){e="Invalid source no-image identity";return false;}out=std::move(next);e.clear();return true;}
 if(!cb.owner||!cb.canonical_uri||!cb.read){e="Required retained actual material image resolver/cache reader";return false;}
 if(!cb.canonical_uri(a,next.canonical_uri,e))return false;
 std::string canonical;if(!assets::ZipAssetPackV1::key(next.canonical_uri,canonical,e))return false;
 if(canonical!=next.canonical_uri){e="Image resolver must return canonical full cache URI";return false;}
 bool found=false;std::vector<std::uint8_t> bytes;if(!cb.read(canonical,found,bytes,e))return false;
 if(!found){e="Required original image resource missing: "+canonical+" (authored "+a.authored_uri+")";return false;}
 if(bytes.empty()){e="Original image resource empty: "+canonical;return false;}
 next.bytes=std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes));next.resource_owner=cb.owner;out=std::move(next);e.clear();return true;
}
bool source_factory_image_candidates_v43(const std::string& directory,const MaterialImageIdentityV43& a,std::array<std::string,2>& out,std::string& e){
 if(a.state!=MaterialImageStateV43::authored_image||a.authored_uri.empty()){e="Required actual authored SImage URI";return false;}
 out={directory+"/"+a.authored_uri,a.authored_uri};e.clear();return true;
}
}
