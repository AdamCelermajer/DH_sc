#include "../../level-world/private_save_file_transport_v45.hpp"
#include "../../level-world/campaign_save_profile_v45.hpp"
#ifdef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
#include "../../level-world/application_save_files_owner_v61.hpp"
#include "../../level-world/application_services_owner_v5.hpp"
#endif
#include "../../game-data/fresh_player_profile_v1.hpp"
#include "../../game-data/player_profile_index_v1.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
std::vector<std::uint8_t> read(const std::filesystem::path& path){
 std::ifstream file(path,std::ios::binary);check(bool(file),"Required original Character table: "+path.string());
 return {std::istreambuf_iterator<char>(file),{}};
}
data::Bytes bytes(const std::vector<std::uint8_t>& value){return {value.data(),value.size()};}
}
int run_private_save_transport_win32_tests(int argc,char** argv
#ifdef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
 ,const std::function<bool(const std::shared_ptr<application::ApplicationSaveFilesOwnerV61>&,std::string&)>& selected_profile_probe,
 const std::shared_ptr<application::ApplicationServicesOwnerV5>& actual_application
#endif
 ){try{
 check(argc==2,"Required repository root");std::string error;
 const auto parent=std::filesystem::absolute(std::filesystem::path(argv[1])/".local-inputs");
 const auto folder=parent/("native-save-transport-"+std::to_string(GetCurrentProcessId())+"-"+std::to_string(GetTickCount64()));
 check(folder.parent_path()==parent&&std::filesystem::create_directory(folder),"Create private test directory");
 struct Cleanup {std::filesystem::path path;~Cleanup(){std::error_code ignored;std::filesystem::remove_all(path,ignored);}}cleanup{folder};
 bool rejected=false;try{level::PrivateSaveFileTransportV45 invalid("relative",4096);}catch(const std::invalid_argument&){rejected=true;}
 check(rejected,"Relative private directory rejected");
 const auto folder_utf8=folder.u8string();
 const std::string directory(reinterpret_cast<const char*>(folder_utf8.data()),folder_utf8.size());
 auto transport=std::make_shared<level::PrivateSaveFileTransportV45>(directory,1024*1024);
 const auto files=transport->services();auto jobs=std::make_shared<level::SavegameJobsOwnerV2>(files);
#ifdef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
 auto application=actual_application;check(bool(application),"Required same native backend Application");
 std::shared_ptr<application::ApplicationSaveFilesOwnerV61> application_files;
 check(application::ApplicationSaveFilesOwnerV61::acquire(application,directory,application_files,error,transport,jobs),error);
 check(application_files->belongs_to_application(application)&&application_files->files()==transport&&application_files->jobs()==jobs,"Application adopts same actual file and queue owners");
 std::shared_ptr<application::ApplicationSaveFilesOwnerV61> reacquired;
 check(application::ApplicationSaveFilesOwnerV61::acquire(application,directory,reacquired,error)&&reacquired==application_files,"Application reacquire preserves sole queue");
 check(!application::ApplicationSaveFilesOwnerV61::acquire(application,directory+"/other",reacquired,error),"Application directory change rejected");
#endif
 const auto tables=std::filesystem::path(argv[1])/".local-inputs/publication/checkpoint/port/android-native/app/src/main/assets/data";
 const auto raw=read(tables/"character_properties_pyarray.bin"),names=read(tables/"character_properties_pyarraynames.bin"),schema=read(tables/"character_properties_pystructnames.bin");
 data::CharacterTable characters;check(data::load_characters(bytes(raw),bytes(names),bytes(schema),characters,error),error);
 data::FreshPlayerProfileV1 fresh;check(data::fresh_player_profile_v1(characters,"KnightPlayerBase","Native Save",123,456,fresh,error),error);
 const std::string name="dh2_000.savegame";bool found{};std::vector<std::uint8_t> disk;
 check(files.read_file(files.context,name,found,disk,error)&&!found,"Fresh slot absent");
 check(jobs->add_write(name,std::make_unique<level::SavegameStreamV2>(bytes(fresh.bytes)),error),error);
 check(jobs->pending()==1&&files.read_file(files.context,name,found,disk,error)&&!found,"Enqueue does not write");
#ifdef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
 check(application_files->read_save(name,found,disk,error)&&found&&disk==fresh.bytes&&jobs->pending()==0,"Application read drains same matching source queue before consuming selected file");
#endif
 check(jobs->flush(name.c_str(),error)&&jobs->pending()==0,error);
 check(files.read_file(files.context,name,found,disk,error)&&found&&disk==fresh.bytes,"Source fresh profile survives Windows disk roundtrip");
 data::PlayerProfileIndexV1 profile;check(profile.load(bytes(disk),error),error);
 check(profile.borrow().source_sections().size()==7,"Original seven metadata sections");
#ifdef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
 auto actual_profile=std::make_shared<level::CampaignSaveProfileV45>(name,files,jobs);
 check(actual_profile->construct(error)&&actual_profile->ready()&&actual_profile->cached(),error);
 check(actual_profile->cache().bytes()==fresh.bytes,"Actual native Campaign profile consumes same queued disk bytes");
 if(selected_profile_probe)check(selected_profile_probe(application_files,error),error);
#endif
 // A large byte fixture exercises the SAME queue's multi-block/commit leaves;
 // it is deliberately not claimed as an initialized gameplay profile.
 std::vector<std::uint8_t> large(5003);for(std::size_t i=4;i<large.size();++i)large[i]=static_cast<std::uint8_t>(i*17);
 large[0]=1;
 check(jobs->add_backup(name,error)&&jobs->add_write(name,std::make_unique<level::SavegameStreamV2>(bytes(large)),error),error);
 bool progressed{};check(jobs->update(progressed,error)&&progressed&&jobs->pending()==1,error);
 check(files.read_file(files.context,name+".bak",found,disk,error)&&found&&disk==fresh.bytes,"Backup precedes replacement write");
 check(jobs->update(progressed,error)&&progressed&&jobs->pending()==1,error);
 // The live writer remains exclusively owned; opening its receiver must fail.
 check(!files.read_file(files.context,name,found,disk,error),"Open live writer denied");
 check(jobs->flush(nullptr,error)&&jobs->pending()==0,error);
 check(files.read_file(files.context,name,found,disk,error)&&found&&disk==large,"Three source blocks and final header commit preserved");
 check(jobs->add_backup(name,error)&&jobs->add_write(name,std::make_unique<level::SavegameStreamV2>(bytes(fresh.bytes)),error)&&jobs->flush(nullptr,error),error);
 check(files.read_file(files.context,name+".bak",found,disk,error)&&found&&disk==large,"Existing backup replaced in original order");
 check(files.read_file(files.context,name,found,disk,error)&&found&&disk==fresh.bytes,"Shorter replacement truncated old tail");
#ifdef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
 // Interrupted source writes expose the original invalid-count marker. The
 // actual Campaign profile constructor must load the valid prior backup.
 check(jobs->add_backup(name,error)&&jobs->flush(nullptr,error),error);
 {std::ofstream corrupt(folder/name,std::ios::binary);const char marker[]{char(255),char(255),char(255),char(255)};corrupt.write(marker,4);check(bool(corrupt),"Write interrupted-header fixture");}
 auto fallback=std::make_shared<level::CampaignSaveProfileV45>(name,files,jobs);
 check(fallback->construct(error)&&fallback->cache().bytes()==fresh.bytes,"Actual profile recovers original backup after invalid source commit marker");
 check(jobs->add_write(name,std::make_unique<level::SavegameStreamV2>(bytes(fresh.bytes)),error)&&jobs->flush(nullptr,error),error);
#endif
 check(transport->source_exists_v115(name,found,error)&&found,error);
 std::filesystem::create_directory(folder/"directory.savegame");
 check(!files.read_file(files.context,"directory.savegame",found,disk,error),"Directory read rejected");
 void* handle{};check(!files.open_write(files.context,"directory.savegame",handle,error)&&!handle,"Directory writer rejected");
 for(const char* invalid:{"../outside","dh2_000.savegame:stream","trailing.","trailing "}){
  check(!files.open_write(files.context,invalid,handle,error)&&!handle,"Invalid filename rejected");
 }
 auto tiny=std::make_shared<level::PrivateSaveFileTransportV45>(directory,4);const auto limited=tiny->services();
 check(!limited.read_file(limited.context,name,found,disk,error),"Oversize read rejected");
 check(limited.open_write(limited.context,"budget.bin",handle,error),error);
 std::uint64_t written{};check(!limited.write(limited.context,handle,bytes(fresh.bytes),written,error)&&written==0,"Oversize write rejected before I/O");
 check(!limited.seek_write(limited.context,handle,5,error),"Seek beyond budget rejected");
 check(limited.close(limited.context,handle,error)&&!handle,error);
 check(jobs->release(error),error);
 std::cout<<"PASS actual Windows private transport: original fresh profile, queued writes, 2048-byte blocks, source backup/commit, truncation and file policies; whole Character bootstrap incomplete\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
#ifndef DH2_NATIVE_PROFILE_TRANSPORT_EMBED
int main(int argc,char** argv){return run_private_save_transport_win32_tests(argc,argv);}
#endif
