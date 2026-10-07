#include "../campaign_save_profile_v45.hpp"
#include "../private_save_file_transport_v45.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>
using namespace dh2;
int main(){unsigned checks=0;std::string error;char folder[]="/tmp/dh2-campaign-save-v45-XXXXXX";assert(::mkdtemp(folder));
 auto transport=std::make_shared<level::PrivateSaveFileTransportV45>(folder,1024*1024);auto files=transport->services();
 auto jobs=std::make_shared<level::SavegameJobsOwnerV2>(files);
 auto profile=std::make_shared<level::CampaignSaveProfileV45>("campaign.savegame",files,jobs);assert(profile->construct(error)&&profile->ready()&&!profile->cached());++checks;
 auto saved=std::make_shared<data::PlayerSavegameV1>();saved->set_character(0x100000123ull);saved->set_player_level(17);saved->initialize_faeries();
 const std::uint8_t name[]{5,0,0,0,'H','e','r','o',0};std::size_t used{};assert(saved->load_name({name,sizeof(name)},used,error));
 auto authority=std::make_shared<data::PlayerSaveLoadOwnerV1>(saved);assert(authority->publish_profile(profile->receiver(),error));
 auto named=std::make_shared<data::PlayerSaveNamedWriterV1>(authority);
 for(const char* tag:{"PNAM","PLVL","FAES","CFEE"}){assert(profile->register_named_writer(tag,named,error));++checks;}
 assert(!profile->register_named_writer("GEAR",named,error));++checks;
 auto source=profile->write_services([](const auto& q,auto& out,auto& error){if(q.operation==data::PlayerSaveWriteOpV1::online){out.flag=false;return true;}error="Unrecovered fixture network branch";return false;});
 data::PlayerSaveWriteOwnerV1 writer(authority,source);assert(writer.save(error)&&jobs->pending()==2);++checks;
 assert(jobs->flush("campaign.savegame",error)&&jobs->pending()==0);++checks;
 bool found{};std::vector<std::uint8_t> bytes;assert(files.read_file(files.context,"campaign.savegame",found,bytes,error)&&found);++checks;
 data::PlayerProfileIndexV1 index;assert(index.load({bytes.data(),bytes.size()},error));++checks;
 {auto cache=index.borrow();assert(cache.source_sections().size()==4);data::PlayerSavegameV1 restored;restored.initialize_faeries();
 assert(restored.load_name(cache.payload("PNAM"),used,error)&&restored.name()=="Hero");assert(restored.load_level(cache.payload("PLVL"),used,error)&&restored.level()==17);bool mismatch{};assert(restored.load_faeries(cache.payload("FAES"),used,mismatch,error)&&!mismatch);assert(restored.load_current_faery(cache.payload("CFEE"),used,error));checks+=5;}
 saved->set_player_level(18);assert(writer.save(error));assert(jobs->flush(nullptr,error));++checks;
 assert(files.read_file(files.context,"campaign.savegame.bak",found,bytes,error)&&found);data::PlayerProfileIndexV1 old;assert(old.load({bytes.data(),bytes.size()},error));{auto cache=old.borrow();data::PlayerSavegameV1 prior;assert(prior.load_level(cache.payload("PLVL"),used,error)&&prior.level()==17);}++checks;
 // Actual invalid-header primary falls back to genuine prior backup.
 {std::ofstream bad(std::string(folder)+"/campaign.savegame",std::ios::binary|std::ios::trunc);const unsigned marker=0xffffffff;bad.write(reinterpret_cast<const char*>(&marker),4);}
 auto fallback=std::make_shared<level::CampaignSaveProfileV45>("campaign.savegame",files,jobs);assert(fallback->construct(error)&&fallback->cached());{auto cache=fallback->cache();data::PlayerSavegameV1 prior;assert(prior.load_level(cache.payload("PLVL"),used,error)&&prior.level()==17);}++checks;
 // Malformed body with valid marker is not silently accepted as new profile.
 {std::ofstream bad(std::string(folder)+"/broken.savegame",std::ios::binary);const unsigned count=1;bad.write(reinterpret_cast<const char*>(&count),4);}
 auto broken=std::make_shared<level::CampaignSaveProfileV45>("broken.savegame",files,jobs);assert(!broken->construct(error)&&!broken->ready());++checks;
 std::vector<std::uint8_t> unchanged{1,2,3};assert(!files.read_file(files.context,"../outside",found,unchanged,error)&&unchanged.size()==3);++checks;
 void* handle{};assert(!files.open_write(files.context,"../outside",handle,error)&&!handle);++checks;
 assert(!files.close(files.context,handle,error));++checks;
 assert(jobs->release(error));for(const char* file:{"campaign.savegame","campaign.savegame.bak","broken.savegame"})::unlink((std::string(folder)+"/"+file).c_str());assert(!::rmdir(folder));
 std::cout<<"Campaign source profile/registered named writers/jobs/private-file roundtrip PASS "<<checks<<" checks; four actual source sections, offline query fixture; complete15-section Character serialization not claimed\n";
}
