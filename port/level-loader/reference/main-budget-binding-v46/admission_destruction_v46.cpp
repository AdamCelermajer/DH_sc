#include <admitted_cpu_bytes_v40.hpp>
#include <zip_asset_pack_v1.hpp>
#include <native_resource_budget_v38.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
using namespace dh2;
void need(bool b,const std::string& e){if(!b)throw std::runtime_error(e);}
// Exact corrected V45 holder order and aggregate initializer. The old holder
// is a negative control; no second budget implementation is created.
struct Corrected {std::shared_ptr<void> cpu_admission,filesystem_owner;std::shared_ptr<const std::vector<std::uint8_t>> bytes;};
struct Old {std::shared_ptr<const std::vector<std::uint8_t>> bytes;std::shared_ptr<void> filesystem_owner,cpu_admission;};
int main(int argc,char** argv){try{need(argc==2,"actual original ZIP argument required");auto ledger=android_resources::budget_lease_v39();need(ledger->snapshot().live.cpu_bytes==0,"host main ledger fixture is not empty");
 for(bool correct:{false,true}){
  std::vector<std::string> events;unsigned violations=0;std::uint64_t n=0;bool byte_destroyed=false,file_destroyed=false;
  auto file=std::shared_ptr<std::ifstream>(new std::ifstream(argv[1],std::ios::binary),[&](std::ifstream* f){delete f;file_destroyed=true;events.push_back("filesystem_released");if(!byte_destroyed||ledger->snapshot().live.cpu_bytes!=n)++violations;});need(bool(*file),"actual cache unavailable");file->seekg(0,std::ios::end);auto length=file->tellg();
  assets::ZipAssetPackV1 archive;std::string e;need(archive.mount({file,std::uint64_t(length),[file](std::uint64_t at,void* out,std::size_t size,std::string& e){file->clear();file->seekg(at);file->read(static_cast<char*>(out),size);if(!*file){e="actual source file read failed";return false;}return true;}},"com.gameloft.android.GAND.GloftD2SS/files/",e),e);
  auto admission=std::shared_ptr<resources::CpuAdmissionV40>(new resources::CpuAdmissionV40,[&](resources::CpuAdmissionV40* p){if(!byte_destroyed||!file_destroyed)++violations;delete p;events.push_back("admission_released");if(ledger->snapshot().live.cpu_bytes!=0)++violations;});
  bool found=false;std::vector<std::uint8_t> payload;
  need(archive.read_admitted("data/3d/textures/atlas_dh2_game_objects_001.tga",found,payload,[&](std::uint32_t size,std::string& e){n=size;return admission->reserve(ledger,resources::ResourceScopeV37::actor,size,e);},e)&&found,e);need(n==payload.size()&&!payload.empty(),"actual encoded byte allocation differs");need(admission->commit(e),e);need(ledger->snapshot().live.cpu_bytes==n&&ledger->snapshot().live_by_scope[std::size_t(resources::ResourceScopeV37::actor)].cpu_bytes==n,"actual main admission did not account exact actor bytes");
  auto bytes=std::shared_ptr<const std::vector<std::uint8_t>>(new std::vector<std::uint8_t>(std::move(payload)),[&](const std::vector<std::uint8_t>* p){if(ledger->snapshot().live.cpu_bytes!=n)++violations;delete p;byte_destroyed=true;events.push_back("bytes_destroyed");});
  std::shared_ptr<const std::vector<std::uint8_t>> only_bytes;std::shared_ptr<void> extra;
  if(correct){auto owner=std::make_shared<Corrected>(Corrected{admission,file,bytes});only_bytes={owner,owner->bytes.get()};extra=owner;}
  else {auto owner=std::make_shared<Old>(Old{bytes,file,admission});only_bytes={owner,owner->bytes.get()};extra=owner;}
  archive={};file.reset();admission.reset();bytes.reset();extra.reset();
  need(!byte_destroyed&&!file_destroyed&&events.empty()&&ledger->snapshot().live.cpu_bytes==n,"byte-only alias lost actual buffer/filesystem/admission lifetime");
  need(only_bytes->size()==n,"byte-only alias detached actual source allocation");only_bytes.reset();
  if(correct){need(violations==0&&events==std::vector<std::string>{"bytes_destroyed","filesystem_released","admission_released"},"corrected field order released accounting before actual storage/filesystem cleanup");}
  else {need(violations>=2&&events==std::vector<std::string>{"admission_released","filesystem_released","bytes_destroyed"},"old-order negative control failed to expose actual accounting gap");}
  need(ledger->snapshot().live.cpu_bytes==0&&ledger->snapshot().pending.cpu_bytes==0,"actual encoded admission leaked");std::cout<<(correct?"CORRECTED":"OLD_NEGATIVE_CONTROL")<<" encoded_bytes="<<n<<" ordering_violations="<<violations<<" events=";for(auto& event:events)std::cout<<event<<',';std::cout<<'\n';
 }
 std::cout<<"PASS actual_main_ledger=1 actual_original_ZIP_encoded_vector=1 byte_only_alias_retains_all_charges=1 corrected_bytes_then_filesystem_then_admission=1 GPU_context_or_render_proof=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
