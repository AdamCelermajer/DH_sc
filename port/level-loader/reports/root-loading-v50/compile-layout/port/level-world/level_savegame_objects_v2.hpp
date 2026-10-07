#pragma once
#include "savegame_stream_v2.hpp"
#include "level_savegame_owner_v1.hpp"
namespace dh2::level {
struct LevelSaveObjectBorrowV2 {
 const void* identity{};void* context{};
 const std::uint8_t* checkpoint28{};
 const std::string* gametype48{};const std::string* map_name{};
 const std::int32_t* room64{};const std::uint8_t* disabled81{};
 bool (*is_character)(void*,bool&,std::string&){};
 bool (*is_player)(void*,bool&,std::string&){};
 bool (*save)(void*,SavegameStreamV2&,std::string&){};
 bool (*load)(void*,SavegameStreamV2&,std::string&){};
};
struct LevelSaveObjectsServicesV2 {
 void* context{};
 // Actual ObjectManager sorted source iterator. found=false is map end;
 // entry.identity=nullptr is a real constructor-owned null map node.
 bool (*first)(void*,std::uintptr_t& iterator,LevelSaveObjectBorrowV2&,bool& found,std::string&){};
 bool (*next)(void*,std::uintptr_t&,LevelSaveObjectBorrowV2&,bool&,std::string&){};
 bool (*network_online)(void*,bool&,std::string&){};
 bool (*locally_controlled)(void*,const void*,bool&,std::string&){};
 bool (*by_name)(void*,const char*,std::int32_t,bool,const char*,LevelSaveObjectBorrowV2&,std::string&){};
 bool (*player)(void*,std::int32_t,bool,LevelSaveObjectBorrowV2&,std::string&){};
};
bool level_savegame_save_objects_v2(SavegameStreamV2&,const LevelSaveObjectsServicesV2&,std::string&);
bool level_savegame_load_objects_v2(SavegameStreamV2&,LevelSavegameFieldsV1&,const LevelSaveObjectsServicesV2&,std::string&);
}
