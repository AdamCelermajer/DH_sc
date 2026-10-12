#include "main_encoded_image_budget_provider_v46.hpp"
#include <zip_asset_pack_v1.hpp>
#include <native_resource_budget_v38.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
void need(bool b,const std::string& e){if(!b)throw std::runtime_error(e);}
int main(int argc,char** argv){try{need(argc==2,"actual ZIP required");auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary);need(bool(*file),"cache unavailable");file->seekg(0,std::ios::end);auto length=file->tellg();auto cache=std::make_shared<assets::ZipAssetPackV1>();std::string e;
 need(cache->mount({file,std::uint64_t(length),[file](std::uint64_t p,void* out,std::size_t n,std::string& e){file->clear();file->seekg(p);file->read(static_cast<char*>(out),n);if(!*file){e="original file read failed";return false;}return true;}},"com.gameloft.android.GAND.GloftD2SS/files/",e),e);
 auto ledger=android_resources::budget_lease_v39();auto renderer=std::make_shared<int>(1);unsigned metadata=0,reads=0;bool size_mismatch=false;
 loader::MainEncodedImageBudgetInputsV46 input;input.filesystem_owner=cache;input.actual_renderer_owner=renderer;input.actual_main_budget=ledger;input.scope=resources::ResourceScopeV37::actor;
 input.original_read_admitted=[&](const std::string& uri,bool& found,std::vector<std::uint8_t>& bytes,const std::function<bool(std::uint32_t,std::string&)>& before,std::string& e){
  const auto snapshot=ledger->snapshot();const auto output_capacity=bytes.capacity();
  bool called=false;const bool ok=cache->read_admitted(uri,found,bytes,[&](std::uint32_t n,std::string& e){called=true;if(snapshot.pending.cpu_bytes==0)++metadata;else ++reads;return before(size_mismatch&&snapshot.pending.cpu_bytes?n+1:n,e);},e);
  if(called&&!ok)need(bytes.capacity()==output_capacity,"metadata/rejected admission allocated a payload vector");return ok;
 };
 auto provider=std::make_shared<loader::MainEncodedImageBudgetProviderV46>(input);loader::MaterialImageIdentityV43 id{};id.state=loader::MaterialImageStateV43::authored_image;id.authored_uri="q:/data/iphone/3d/textures/atlas_dh2_game_objects_001.tga";const std::string canonical="data/3d/textures/atlas_dh2_game_objects_001.tga";
 std::shared_ptr<void> pin;need(provider->admit_before_read(id,canonical,pin,e),e);const auto size=ledger->snapshot().pending.cpu_bytes;need(size==524348&&ledger->snapshot().live.cpu_bytes==0,"before-read reservation did not use exact real ZIP metadata");
 auto preserved=pin;need(!provider->admit_before_read(id,canonical,pin,e)&&pin==preserved,"pending admission reentered or failure changed output");preserved.reset();pin.reset();need(ledger->snapshot().pending.cpu_bytes==0&&ledger->snapshot().live.cpu_bytes==0,"cancel before read leaked pending reservation");
 need(provider->admit_before_read(id,canonical,pin,e),e);loader::MaterialImageServicesV43 services;need(provider->image_services([](const loader::MaterialImageIdentityV43&,std::string& uri,std::string& e){uri="data/3d/textures/atlas_dh2_game_objects_001.tga";e.clear();return true;},services,e),e);
 bool found=false;std::vector<std::uint8_t> bytes;need(services.read(canonical,found,bytes,e)&&found&&bytes.size()==size,e);need(ledger->snapshot().pending.cpu_bytes==0&&ledger->snapshot().live.cpu_bytes==size&&ledger->snapshot().live_by_scope[std::size_t(resources::ResourceScopeV37::actor)].cpu_bytes==size,"verified producer success did not commit actual encoded actor admission");
 need(!services.read(canonical,found,bytes,e),"same pending ticket replayed an actual allocation");std::vector<std::uint8_t>().swap(bytes);pin.reset();need(ledger->snapshot().live.cpu_bytes==0,"main admission did not release after actual storage cleanup");
 need(provider->admit_before_read(id,canonical,pin,e),e);size_mismatch=true;need(!services.read(canonical,found,bytes,e)&&bytes.capacity()==0&&e.find("changed")!=std::string::npos,"changed actual metadata allocated an unadmitted payload");size_mismatch=false;pin.reset();need(ledger->snapshot().pending.cpu_bytes==0,"changed metadata cancellation leaked reservation");
 auto none=id;none.state=loader::MaterialImageStateV43::source_no_image;need(!provider->admit_before_read(none,canonical,pin,e),"source no-image admitted encoded allocation");
 need(!provider->admit_before_read(id,"data/3d/textures/source_that_is_absent.tga",pin,e)&&!pin,"missing exact cache entry admitted allocation");
 renderer.reset();need(!provider->admit_before_read(id,canonical,pin,e)&&!pin,"dead actual renderer admitted a new read");
 need(ledger->snapshot().live.cpu_bytes==0&&ledger->snapshot().pending.cpu_bytes==0,"main read binding accounting leaked");
 std::cout<<"PASS actual_MAIN_ledger=1 exact_actor_encoded_bytes="<<size<<" metadata_probes="<<metadata<<" payload_attempts="<<reads<<" before_payload_reserve_commit=1 cancel_reentry_replay_changed_metadata_missing_noimage_dead_renderer_rejected=1 GPU_or_decoded_phase_claim=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
