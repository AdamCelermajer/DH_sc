#pragma once
#include "player_info_record_v1.hpp"
#include "../game-data/player_savegame_v1.hpp"

namespace dh2::player_metadata_prepare_v1 {
using Record=player_info_record_v1::Record;
// A borrow of the existing PlayerSavegame owner, never a second profile copy.
struct SaveRef {
    std::uintptr_t identity=0;
    const data::PlayerSavegameV1* save=nullptr;
};
struct Services {
    void* context=nullptr;
    std::int32_t (*is_active)(void*,Record*,std::int32_t*)=nullptr;
    std::int32_t (*allocate_save)(void*,std::uint32_t logical_bytes,
                                   std::uint32_t tag,std::uintptr_t*)=nullptr;
    std::int32_t (*construct_indexed_save)(void*,std::uintptr_t allocation,
                                           std::uint32_t slot,std::int32_t mask,
                                           bool skip_load,SaveRef**)=nullptr;
    std::int32_t (*save_by_identity)(void*,std::uintptr_t,SaveRef**)=nullptr;
    std::int32_t (*debug_name_11)(void*,std::uint8_t*)=nullptr;
    std::int32_t (*game_state_name_28)(void*,std::uint8_t*)=nullptr;
};
struct SetterResidues {std::int32_t character_class=0,character_level=0;};
enum class Status : std::uint32_t {
    complete,invalid_argument,invalid_record,service_unavailable,
    service_failed,missing_projection,outside_domain,setter_failed,reentrant
};
enum class Operation : std::uint32_t {
    none,is_active,allocate_save,construct_indexed_save,save_by_identity,
    publish_680,debug_name,game_state_name,compare_name,set_name,set_class,
    set_level,continuation
};
enum class Disposition : std::uint32_t {none,inactive,not_local,no_slot,prepared};
struct Result {
    Status status=Status::complete;
    Operation last_operation=Operation::none;
    Disposition disposition=Disposition::none;
    std::uintptr_t captured_character=0,captured_save=0;
    std::uint32_t service_calls=0,published_save_writes=0,name_setters=0,class_setters=0,level_setters=0;
    netstruct_members_v1::Status setter_status=netstruct_members_v1::Status::complete;
};
class Runtime {
    Record& record_;Services services_;bool busy_=false;
public:
    Runtime(Record& record,Services services):record_(record),services_(services){}
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    // Bounded extraction of _ManageCharacters' selected-player metadata path.
    // Caller already selected the canonical Record through actual GetPlayer.
    // Captures Character660 before IsActive, gates +66c/+664, allocates198/tag0
    // and indexed C1(slot,1,false), then publishes680. Normal Debug+11=false/
    // GameState+28=false name synchronization and, only for captured660==0,
    // fresh Save.class34/level30 mismatch setters use this same record/serial.
    // Stops before current-Level/class/_AddCharacter decision. Nonzero flags
    // report outside_domain; full Manage/online/debug branches are not implied.
    Status prepare(const SetterResidues&,Result*);
};
} // namespace dh2::player_metadata_prepare_v1
