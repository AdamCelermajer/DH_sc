#pragma once
#include "character_save_restore_v3.hpp"
#include "retained_character_actor_v1.hpp"
#include "level_savegame_objects_v2.hpp"
#include <functional>
namespace dh2::character {
struct RetainedCharacterSaveExtraV3 {
 world::ObjectSaveRestoreBorrowV3 inherited;
 CharacterSaveMetadataV3* metadata{};BuffOwner* same_buffs{};
 ControllerCommandState32* same_controller{};
 const float* initial_position1450{};const float* initial_rotation145c{};
};
class RetainedCharacterSaveConnectionV3 {
 RetainedCharacterActorV1& actor_;std::weak_ptr<void> receiver_;
 void* query_context_{};
 bool(*is_character_)(void*,bool&,std::string&){};
 bool(*is_player_)(void*,bool&,std::string&){};
 std::function<bool(RetainedCharacterSaveExtraV3&,std::string&)> extra_;
 CharacterSaveRestoreServicesV3 services_;bool busy_{},failed_{};
 CharacterSaveRestoreResultV3 result_{};
 static void publish(void*);
 bool borrow(CharacterSaveRestoreBorrowV3&,std::string&);
public:
 RetainedCharacterSaveConnectionV3(RetainedCharacterActorV1& actor,std::shared_ptr<void> same_receiver,
  std::function<bool(RetainedCharacterSaveExtraV3&,std::string&)> extra,CharacterSaveRestoreServicesV3 s):actor_(actor),receiver_(std::move(same_receiver)),extra_(std::move(extra)),services_(s){}
 bool serialize(level::SavegameStreamV2&,std::string&);
 bool deserialize(level::SavegameStreamV2&,std::string&);
 bool failed()const noexcept{return failed_;}
 const CharacterSaveRestoreResultV3& result()const noexcept{return result_;}
 // Attach these SAME methods to a real manager entry's OBJS projection.
 bool bind_methods(level::LevelSaveObjectBorrowV2&,std::string&);
};
}
