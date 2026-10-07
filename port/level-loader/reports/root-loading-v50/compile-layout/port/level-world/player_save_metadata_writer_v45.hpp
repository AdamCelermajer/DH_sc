#pragma once
#include "savegame_stream_v2.hpp"
#include "../game-data/player_save_load_owner_v1.hpp"
namespace dh2::level {
struct PlayerSaveMetadataServicesV45 {
 std::shared_ptr<void> tables;
 const data::CharacterTable* characters{}; // original Arrays::CharacterTable names
 std::function<bool(std::int32_t&,std::string&)> current_difficulty;
};
class PlayerSaveMetadataWriterV45 {
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> authority_;PlayerSaveMetadataServicesV45 services_;
 bool writing_{};
public:
 PlayerSaveMetadataWriterV45(std::shared_ptr<data::PlayerSaveLoadOwnerV1>,PlayerSaveMetadataServicesV45);
 bool write(const char* source_section,SavegameStreamV2&,std::string&);
};
}
