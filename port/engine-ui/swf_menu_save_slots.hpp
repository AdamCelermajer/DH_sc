#pragma once
#include <array>
#include <cstdint>
#include <string>
namespace gameswf {struct fn_call;struct as_object;}
namespace dh2::ui {
// Front-only pending-loader callback; does not assign or mutate a Player.
bool swf_front_pending_start_v1(const gameswf::fn_call&,std::int32_t& difficulty,std::string& error);
// Front-only fresh-profile producer. The returned slot is independent from
// canonical gameplay assignment/start, which retains separate ownership.
bool swf_front_create_save_slot_v1(const gameswf::fn_call&,void*,
    bool (*create)(void*,const char*,const char*,std::uint32_t&,std::string&),std::string&);
// Canonical save/game owners supply these values after loading the profile and
// resolving localized class/location and the selected difficulty's quest act.
// This structure transfers menu presentation only; it does not own a Player.
struct SwfFrontSaveSlotDetailsV1 {
    std::int32_t slot_id=0;
    bool in_use=false;
    std::string player_name,player_class,string_class_level,player_location,last_save;
    std::int32_t player_level=-1,current_act=1,difficulty=0,difficulty_unlocked=0;
};
// Original 0x44ace4..0x44b22c setter order/types and empty defaults. Setter
// return values are ignored by the source; a null receiver performs no writes.
void swf_front_write_save_slot_details_v1(gameswf::as_object*,const SwfFrontSaveSlotDetailsV1&);
struct SwfFrontSaveSlotServicesV1 {
    void* context{};
    bool (*exists)(void*,std::uint32_t slot,bool& occupied,std::string& error){};
    // Load the selected profile with the original menu metadata behavior
    // (PlayerSavegame flags17, volatile=false). -1 uses CurrentDifficulty;
    // otherwise the optional third AS argument selects difficulty0..2.
    // Providers must resolve real QEST acts and localization/date producers.
    bool (*load_details)(void*,std::uint32_t slot,bool occupied,
        std::int32_t difficulty_override,SwfFrontSaveSlotDetailsV1&,std::string& error){};
};
bool swf_front_select_save_slot_v1(const std::array<bool,4>& occupied,
    std::uint32_t display_index,std::uint32_t& slot,bool& in_use,std::string& error);
bool swf_front_save_slot_details(const gameswf::fn_call&,const SwfFrontSaveSlotServicesV1&,std::string& error);
// Genuine no-campaign branch of NativeGetSaveSlotDetails. The production
// files directory is inspected; existing saves require the full profile owner.
bool swf_front_save_slot_details(const gameswf::fn_call&,const std::string& files_directory,std::string& error);
struct SwfFrontEraseSlotServicesV1 {
    void* context{};
    bool (*flush_jobs)(void*,std::string&){};
    bool (*erase_files)(void*,std::uint32_t,std::string&){};
};
// NativeEraseSaveSlot: AS number -> signed integer, negative no-op; flush
// before deleting all slot files. AS result is untouched, including failures.
bool swf_front_erase_save_slot_v1(const gameswf::fn_call&,const SwfFrontEraseSlotServicesV1&,std::string&);
}
