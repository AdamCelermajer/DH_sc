#include "player_info_profile_owner_v70.hpp"
#include <algorithm>
#include <cstdio>
namespace dh2::player {
bool PlayerInfoProfileOwnerV70::construct(std::int32_t slot,
 const std::shared_ptr<application::ApplicationSaveFilesOwnerV61>& files,
 const std::shared_ptr<ui::OwnedHudSettingsV1>& settings,
 std::shared_ptr<void> tables,const data::CharacterTable* characters,
 std::shared_ptr<const void> selected,data::Bytes snapshot,std::string& e){
 if(attempted_){e="PlayerInfo profile680 C1 cannot replay a retained attempt";return false;}attempted_=true;
 if(slot<0||slot>=4||!files||!settings||!tables||!characters||!selected||(!snapshot.data&&snapshot.size)){
  e="Required actual selected slot/file/App4c/CharacterTable for PlayerInfo680";return false;
 }
 auto jobs=files->jobs();if(!jobs||jobs->failed()||jobs->busy_v61()){e="Required idle nonfailed actual Application SavegameJobs before profile680 C1";return false;}
 save_=std::make_shared<data::PlayerSavegameV1>();save_->set_slot(slot);
 load_=std::make_shared<data::PlayerSaveLoadOwnerV1>(save_);
 char filename[32];const auto n=std::snprintf(filename,sizeof(filename),"dh2_%03u.savegame",unsigned(slot));
 if(n<=0||std::size_t(n)>=sizeof(filename)){e="Original PlayerInfo selected filename overflow";return false;}
 profile_=std::make_shared<level::CampaignSaveProfileV45>(filename,files->files()->services(),jobs);
 if(!profile_->construct(e))return false;
 const auto cache=profile_->cache();if(!cache||cache.bytes().size()!=snapshot.size||!std::equal(cache.bytes().begin(),cache.bytes().end(),snapshot.data)){
  e="PlayerInfo680 actual profile cache differs from selected file receipt";return false;
 }
 character::CharacterMenuProfileLoadServicesV51 services;services.tables=std::move(tables);services.characters=characters;
 services.store_current_difficulty=[settings](std::int32_t value,std::string& error){settings->source_set_current_difficulty_v67(value);error.clear();return true;};
 reader_=std::make_shared<character::CharacterMenuProfileLoadV51>(save_,profile_,std::move(services));
 if(!load_->publish_profile(profile_->receiver(),e)||!load_->bind_services_v50(reader_->load_services(),e)||!load_->load(1,e))return false;
 ready_=true;return true;
}
}
