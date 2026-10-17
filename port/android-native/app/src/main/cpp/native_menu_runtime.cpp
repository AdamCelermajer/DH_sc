#include "native_menu_runtime.hpp"
#include "native_player_profile.hpp"
#include "model_renderer.hpp"
#include "player_profile_create_v1.hpp"
#include "player_profile_filename_v1.hpp"
#include "data.hpp"
#include "campaign_profile_files_v1.hpp"
#include <android/log.h>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <limits>

namespace dh2::native::menu {
bool create_profile(AAssetManager* assets,const std::string& directory,
 std::int32_t* difficulty,const std::uint8_t* online,void* count_context,bool (*count_provider)(void*,std::uint32_t*,std::string&),const std::string& name,
 const std::string& class_name,std::int32_t& slot,bool& published,std::string& error){
 published=false;
 if(!assets||directory.empty()||!difficulty||!online||!count_provider){error="Menu profile platform providers unavailable";return false;}
 try{
  auto rows=model_renderer::read_asset(assets,"data/character_properties_pyarray.bin");
  auto names=model_renderer::read_asset(assets,"data/character_properties_pyarraynames.bin");
  auto fields=model_renderer::read_asset(assets,"data/character_properties_pystructnames.bin");
  data::CharacterTable characters;
  if(!data::load_characters({rows.data(),rows.size()},{names.data(),names.size()},{fields.data(),fields.size()},characters,error))return false;
  data::PlayerSavegameV1 temporary;
  data::PlayerSaveProfileV1 profile;
  player_profile::Transport transport(temporary,profile);
  if(!transport.bind({directory,&characters,difficulty,{},true},error))return false;
  struct Providers {const std::string& directory;const std::uint8_t* online;player_profile::Transport& transport;data::PlayerSavegameV1& save;data::PlayerSaveProfileV1& profile;void* count_context;bool (*count_provider)(void*,std::uint32_t*,std::string&);};
  Providers providers{directory,online,transport,temporary,profile,count_context,count_provider};
  data::PlayerProfileCreateServicesV1 services{};services.context=&providers;
  services.next_free_slot=[](void* raw,std::int32_t* out,std::string& e){
   auto& p=*static_cast<Providers*>(raw);
   std::vector<std::string> catalogue;
   // SG_GetSavegameList(false): filesystem './' list, source strstr filters.
   for(const auto& entry:std::filesystem::directory_iterator(p.directory)){
    const auto name=entry.path().filename().string();
    if(name.find(".bak")!=std::string::npos||name.find("settings")!=std::string::npos||name.find("level")!=std::string::npos||
       name.find(".savegame")==std::string::npos||name.find("dh2_")==std::string::npos)continue;
    if(!entry.is_regular_file()){e="Unsupported campaign catalogue entry type";return false;}
    catalogue.push_back(name);
   }
   std::sort(catalogue.begin(),catalogue.end());
   std::int32_t expected=0;
   for(const auto& name:catalogue){
    // SG_GetSlotFromFilename performs atoi(filename+strlen("dh2_")).
    errno=0;const auto parsed=std::strtol(name.c_str()+4,nullptr,10);
    if(errno==ERANGE||parsed<std::numeric_limits<std::int32_t>::min()||parsed>std::numeric_limits<std::int32_t>::max()){e="Campaign slot atoi overflow is unsupported";return false;}
    if(parsed!=expected){*out=expected;e.clear();return true;}
    bool occupied=false;
    if(expected<4){if(!data::campaign_profile_exists_v1(p.directory,std::uint32_t(expected),occupied,e))return false;}
    else{
     // The read-only menu helper's four displayed slots are not a source
     // creation bound. SG_Exists uses the exact filename and .bak for anyu32.
     const auto path=std::filesystem::path(p.directory)/data::player_profile_filename_v1(std::uint32_t(expected),false,false);
     occupied=std::filesystem::exists(path)||std::filesystem::exists(path.string()+".bak");
    }
    if(!occupied){*out=expected;e.clear();return true;}
    if(expected==std::numeric_limits<std::int32_t>::max()){e="New campaign slot exceeds native signed interface";return false;}
    ++expected;
   }
   *out=expected;e.clear();return true;
  };
  services.seed_time=[](void*,std::uint32_t* out,std::string&){
   // Original getRealTime import: gettimeofday seconds*1000+usec/1000, low32.
   *out=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());return true;
  };
  services.save_date_time=[](void*,std::uint32_t* out,std::string& e){const auto now=std::time(nullptr);if(now<0){e="Save date clock unavailable";return false;}*out=std::uint32_t(now);return true;};
  services.difficulty_count=[](void* raw,std::uint32_t* out,std::string& e){auto& p=*static_cast<Providers*>(raw);return p.count_provider(p.count_context,out,e);};
  services.online=[](void* raw,bool* out,std::string&){*out=*static_cast<Providers*>(raw)->online!=0;return true;};
  services.save_all=[](void* raw,data::PlayerSavegameV1& save,const data::PlayerSaveProfileV1& profile,std::string& e){auto& p=*static_cast<Providers*>(raw);if(&save!=&p.save||&profile!=&p.profile){e="Menu temporary profile identity differs";return false;}return p.transport.save_all(e);};
  data::PlayerProfileCreateRuntimeV1 creator({&temporary,&profile,&transport.loader(),&characters,difficulty,services});
  data::PlayerProfileCreateResultV1 receipt;
  const auto status=creator.create(name,class_name,&receipt,error);
  if(status==data::PlayerProfileCreateStatusV1::rejected){error.clear();return true;}
  if(status!=data::PlayerProfileCreateStatusV1::complete)return false;
  published=receipt.source_return_published;slot=receipt.slot;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native menu profile created | slot %d | class %d | level %d | metadata sections %u | online reads %u | name private",slot,receipt.character_class,temporary.level(),transport.receipt().sections,receipt.online_queries);
  return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
}
