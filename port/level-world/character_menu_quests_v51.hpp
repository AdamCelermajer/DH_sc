#pragma once
#include "../game-data/quest_persistence_v51.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include "player_save_collections_writer_v45.hpp"
namespace dh2::world {class NativeQuestRuntimeV76;}
namespace dh2::character {
// The profile's regular/volatile QuestSavegame cells remain on SAME Save.
// This lease supplies their actual persistence receivers and table lifetime.
class CharacterMenuQuestsV51:public std::enable_shared_from_this<CharacterMenuQuestsV51> {
 std::shared_ptr<data::PlayerSavegameV1> save_;
 std::shared_ptr<const data::QuestTablesPersistenceV51> tables_;
 std::array<data::QuestPersistenceOwnerV51,2> owners_;
 std::function<bool(data::QuestPersistenceStateV51&,std::int32_t,std::string&)> reinit_;
 std::shared_ptr<world::NativeQuestRuntimeV76> runtime_v76_;
 std::string runtime_failure_v76_; //native interrupted-prefix receipt, not a source ready flag
 static bool load_callback(void*,std::uintptr_t,data::Bytes,bool,std::size_t&,std::string&);
public:
 CharacterMenuQuestsV51(std::shared_ptr<data::PlayerSavegameV1>,std::shared_ptr<const data::QuestTablesPersistenceV51>,
  std::function<bool(data::QuestPersistenceStateV51&,std::int32_t,std::string&)> actual_reinit={});
 bool initialize(std::uint32_t source_regular0_volatile1,std::string&);
 bool load(data::Bytes,std::size_t&,std::string&);
 bool save_quest(std::uintptr_t,level::SavegameStreamV2&,std::string&);
 level::QuestSaveCollectionBorrowV45 writer(std::uint32_t source_regular0_volatile1);
 const auto& save()const noexcept{return save_;}
 const auto& tables()const noexcept{return tables_;}
 data::QuestPersistenceStateV51* resolve_v70(std::uintptr_t identity)const noexcept;
 const auto& runtime_v76()const noexcept{return runtime_v76_;}
 const auto& runtime_failure_v76()const noexcept{return runtime_failure_v76_;}
 void retain_runtime_failure_v76(const std::string& e){if(runtime_failure_v76_.empty())runtime_failure_v76_=e.empty()?"Quest native source prefix interrupted":e;}
 bool bind_runtime_v76(std::shared_ptr<world::NativeQuestRuntimeV76>,std::string&);
 bool compile_quest_v76(std::uintptr_t,std::string&);
 bool destroy_collection_v108(std::uint32_t source_regular0_volatile1,std::string&);
};
}
