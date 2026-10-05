#include "menu_save_slot_projection_v1.hpp"
#include "save_slot_date_v1.hpp"
#include <cstring>
#include <utility>
namespace dh2::ui {
bool menu_save_slot_local_date_v1(std::uint32_t raw,std::tm& output,std::string& error) {
    std::int32_t signed_date{};
    std::memcpy(&signed_date,&raw,sizeof(raw));
    const std::time_t timestamp=static_cast<std::time_t>(signed_date);
    std::tm calendar{};
#ifdef _WIN32
    const bool ok=localtime_s(&calendar,&timestamp)==0;
#else
    const bool ok=localtime_r(&timestamp,&calendar)!=nullptr;
#endif
    if(!ok){error="Cannot convert save-slot local timestamp";return false;}
    output=calendar;error.clear();return true;
}
bool project_menu_save_slot_v1(const dh2::data::MenuProfileMetadataV1& profile,
    const dh2::data::CharacterTable& characters,const dh2::data::LevelTables& levels,
    std::int32_t override_difficulty,bool volatile_acts,std::uint32_t language,
    const MenuSaveSlotPresentationServicesV1& services,
    SwfFrontSaveSlotDetailsV1& output,std::string& error) {
    auto fail=[&](const char* message){error=message;return false;};
    if(profile.slot<0||profile.slot>3)return fail("Invalid menu save slot");
    if(override_difficulty < -1 || override_difficulty > 2)
        return fail("Invalid menu difficulty override");
    const auto difficulty=override_difficulty==-1?profile.selected_difficulty:override_difficulty;
    if(difficulty<0||difficulty>2)return fail("Invalid loaded menu difficulty");
    if(profile.character_row<0||static_cast<std::size_t>(profile.character_row)>=characters.rows.size())
        return fail("Invalid saved character row");
    auto level_id=profile.location.levels[static_cast<std::size_t>(difficulty)];
    if(level_id==-1)level_id=0;
    if(level_id<0||static_cast<std::size_t>(level_id)>=levels.levels.size())
        return fail("Invalid saved location row");
    if(!services.constant||!services.string_id||!services.local_date)
        return fail("Menu save-slot presentation services required");
    SwfFrontSaveSlotDetailsV1 details;
    details.slot_id=profile.slot;details.in_use=true;
    details.player_name=profile.name;details.player_level=profile.level;
    details.difficulty=difficulty;details.difficulty_unlocked=profile.unlocked_difficulty;
    details.current_act=(volatile_acts?profile.location.volatile_acts:profile.location.current_acts)[difficulty];
    // Character source row: vtable + 224 words; StrID at offset 0x18.
    const auto class_string=static_cast<std::uint32_t>(characters.rows[profile.character_row][5]);
    if(!services.string_id(services.context,class_string,details.player_class,error))return false;
    std::int32_t level_label_id{};
    if(!services.constant(services.context,"StrID","GAMEPLAYMENUS_LEVEL",level_label_id,error))return false;
    std::string label;
    if(!services.string_id(services.context,static_cast<std::uint32_t>(level_label_id),label,error))return false;
    details.string_class_level=details.player_class+" "+label+" "+std::to_string(profile.level);
    std::tm calendar{};
    if(!services.local_date(services.context,profile.location.save_date,calendar,error))return false;
    if(!format_save_slot_local_date_v1(calendar,language,details.last_save,error))return false;
    // LevelTables original scalar row: localized location ID at offset 0x24.
    if(!services.string_id(services.context,levels.levels[level_id].scalar.words[9],details.player_location,error))return false;
    output=std::move(details);error.clear();return true;
}
}
