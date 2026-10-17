#pragma once
#include "player_save_load_owner_v1.hpp"

namespace dh2::data {
struct CharacterTable;
struct PlayerMetadataWriteServicesV1 {
 void* context{};
 // Synchronous stream virtual. It must copy bytes before returning and may
 // mutate ordinary Save fields; later source reads observe those changes.
 bool (*write)(void*,Bytes,std::string&){};
};
// Exact seven metadata writers. Registered invalid PCLS IDs below zero or
// above table size produce an empty body; id==size is an unsafe source read
// and fails explicitly. No gameplay sections, recalc or defaults are added.
bool write_player_metadata_section_v1(const char* tag,const PlayerSavegameV1&,
 const CharacterTable&,const std::int32_t* current_difficulty,
 const PlayerMetadataWriteServicesV1&,std::string&);
// Source saveAll's sorted registered-tag stream assembly. Cached tags with no
// writer retain their current payload. The caller publishes the completed
// bytes to its SAME index before the mandatory file persistence operation.
// Failure retains the reached output-byte prefix.
bool serialize_player_metadata_profile_v1(const PlayerProfileIndexV1::Borrow&,
 const std::vector<std::string>& registered_writers,const PlayerSavegameV1&,
 const CharacterTable&,const std::int32_t* current_difficulty,
 std::vector<std::uint8_t>& output,std::string&);

enum class PlayerProfileCreateStatusV1 {complete,rejected,invalid_argument,busy,failed};
enum class PlayerProfileCreateStageV1 {not_started,class_lookup,next_slot,indexed_constructor,
 load_metadata,name,level,class_store,difficulty,seeds,date,locations,save_mode,
 save_all,volatile_tail,result_publication,complete};
struct PlayerProfileCreateResultV1 {
 PlayerProfileCreateStageV1 stage{};
 std::int32_t character_class=-1,slot=-1;
 std::uint32_t name_comparisons{},online_queries{};
 bool source_return_published{};
};
struct PlayerProfileCreateServicesV1 {
 void* context{};
 bool (*next_free_slot)(void*,std::int32_t*,std::string&){};
 bool (*seed_time)(void*,std::uint32_t*,std::string&){}; // GetTime@60b0cc
 bool (*save_date_time)(void*,std::uint32_t*,std::string&){}; // time(NULL)
 bool (*difficulty_count)(void*,std::uint32_t*,std::string&){}; // actual table size
 bool (*online)(void*,bool*,std::string&){};
 bool (*local_hosting)(void*,bool*,std::string&){};
 bool (*hosting_quest_flag)(void*,bool*,std::string&){}; // PlayerManager+719
 bool (*save_all)(void*,PlayerSavegameV1&,const PlayerSaveProfileV1&,std::string&){};
 // Online-only bodies remain required source providers. These must execute
 // the remaining original SG_Save / volatile continuation in source order.
 bool (*online_save)(void*,PlayerSavegameV1&,bool hosting,bool quest_flag,std::string&){};
 bool (*volatile_save)(void*,PlayerSavegameV1&,std::string&){};
};
struct PlayerProfileCreateBindingsV1 {
 PlayerSavegameV1* save{}; // fresh temporary indexed Save, NEVER gameplay/preview
 PlayerSaveProfileV1* profile{}; // same canonical +8 used by loader
 PlayerSaveLoadOwnerV1* loader{};
 const CharacterTable* characters{};
 std::int32_t* current_difficulty{}; // existing source global 9a6060
 PlayerProfileCreateServicesV1 services;
};
// NativeCreateSaveSlot's valid two-string body. AS argc/conversion is caller-
// owned: argc2, convert name first then class; invalid class leaves AS result
// untouched. This borrower creates no Save/index/property/VM/RNG owner. One
// fresh temporary Save/profile is required for each source invocation. Failed
// invocations retain the reached prefix and cannot silently retry that Save.
class PlayerProfileCreateRuntimeV1 {
 PlayerProfileCreateBindingsV1 bindings_;
 bool active_{},entered_constructor_{};
public:
 explicit PlayerProfileCreateRuntimeV1(PlayerProfileCreateBindingsV1);
 PlayerProfileCreateStatusV1 create(const std::string& name,const std::string& class_name,
  PlayerProfileCreateResultV1*,std::string&);
};
}
