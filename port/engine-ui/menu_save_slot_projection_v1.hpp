#pragma once
#include "swf_menu_save_slots.hpp"
#include "../game-data/menu_profile_metadata_v1.hpp"
#include "../game-data/level_tables.hpp"
#include <ctime>
namespace dh2::ui {
struct MenuSaveSlotPresentationServicesV1 {
    void* context{};
    bool (*constant)(void*,const char* group,const char* key,std::int32_t&,std::string&){};
    bool (*string_id)(void*,std::uint32_t,std::string&,std::string&){};
    bool (*local_date)(void*,std::uint32_t saved_date,std::tm&,std::string&){};
};
// ARM32 localtime interprets the saved raw word as signed 32-bit time_t.
bool menu_save_slot_local_date_v1(std::uint32_t,std::tm&,std::string&);
// Presentation builder only: no file IO, Player initialization or AS setters.
// -1 difficulty selects the loaded CurrentDifficulty. The caller supplies the
// original GetOnline()->byte5 selection of regular versus volatile quest acts.
// Missing localization/time services and unsafe row/difficulty IDs fail;
// output publishes atomically. Provider effects are not rolled back.
bool project_menu_save_slot_v1(const dh2::data::MenuProfileMetadataV1&,
    const dh2::data::CharacterTable&,const dh2::data::LevelTables&,
    std::int32_t difficulty_override,bool use_volatile_quest_acts,
    std::uint32_t language,const MenuSaveSlotPresentationServicesV1&,
    SwfFrontSaveSlotDetailsV1&,std::string&);
}
