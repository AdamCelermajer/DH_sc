#pragma once
#include "level_savegame_writer_v2.hpp"
#include "../game-data/player_save_named_writer_v1.hpp"
#include "../game-data/player_save_write_owner_v1.hpp"
#include <map>
namespace dh2::level {
// The actual Savegame+8 profile receiver, created by its source profile factory.
// Owns only cache/registration state; SAME PlayerSavegame/Gear/Skills stay borrowed.
class CampaignSaveProfileV45 : public std::enable_shared_from_this<CampaignSaveProfileV45> {
 // Save authority owns profile+8, so profile callbacks must not own the Save
 // authority back. Lock caller-owned writer leases only during delivery.
 struct Writer {std::weak_ptr<void> lease;std::function<bool(SavegameStreamV2&,std::string&)> write;};
 std::string filename_;SavegameFileServicesV2 files_;std::shared_ptr<SavegameJobsOwnerV2> jobs_;
 data::PlayerProfileIndexV1 index_;std::map<std::string,Writer> writers_;
 bool attempted_{},ready_{},cached_{},writing_{},destroyed_v108_{};
public:
 CampaignSaveProfileV45(std::string actual_filename,SavegameFileServicesV2,std::shared_ptr<SavegameJobsOwnerV2> actual_global_jobs);
 bool construct(std::string&);
 bool register_writer(const char* source_tag,std::shared_ptr<void>,std::function<bool(SavegameStreamV2&,std::string&)>,std::string&);
 // Native supported named writer for the SAME retained Save/profile.
 bool register_named_writer(const char*,std::shared_ptr<data::PlayerSaveNamedWriterV1>,std::string&);
 bool save_all(std::string&);
 // Source Savegame filename+4 assignment; does not reload the cache or
 // replace registration/jobs. Failed saveAll keeps the changed filename.
 bool source_filename_store_v83(const std::string&,std::string&);
 bool destroy_source_v108(std::string&);
 bool destroyed_v108()const noexcept{return destroyed_v108_;}
 data::PlayerSaveProfileV1 receiver(){return {reinterpret_cast<std::uintptr_t>(this),shared_from_this()};}
 data::PlayerProfileIndexV1::Borrow cache()const{return index_.borrow();}
 const std::string& filename()const noexcept{return filename_;}
 bool ready()const noexcept{return ready_;}
 bool cached()const noexcept{return cached_;}
 // Offline source SG_Save routes only actual online query + saveAll here.
 // Other source network/quest/checkpoint operations remain typed providers.
 data::PlayerSaveWriteServicesV1 write_services(
  std::function<bool(const data::PlayerSaveWriteRequestV1&,data::PlayerSaveWriteResponseV1&,std::string&)> actual_remaining);
};
}
