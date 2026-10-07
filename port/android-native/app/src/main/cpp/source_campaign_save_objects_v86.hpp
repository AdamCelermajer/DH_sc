#pragma once
#include "level_savegame_runtime_v1.hpp"
#include "level_savegame_objects_v2.hpp"
#include <functional>
namespace model_renderer {
struct SourceWorldBorrowV61;
class SourceCampaignSaveObjectsV86;
//Before genuine Level C1: install OBJS + saveAll dispatch while preserving
//the actual FileManager cache read and existing Application job owner.
bool bind_source_campaign_save_application_v86(const std::shared_ptr<SourceWorldBorrowV61>&,
 dh2::level::LevelSavegameApplicationV1&,std::string&);
bool borrow_source_campaign_character_saved_object_v86(const std::shared_ptr<void>& actual_world,
 std::uintptr_t,dh2::level::LevelSaveObjectBorrowV2&,std::string&);
}
namespace dh2::character {struct BuffOwner;}
namespace model_renderer {
bool borrow_source_campaign_character_buffs_v86(const std::shared_ptr<void>& actual_world,
 std::uintptr_t,dh2::character::BuffOwner*&,std::string&);
//Loader class journal supplies actual selected nonCharacter save/load methods;
//no C1, pose-copy, unknown-type skip or manufactured checkpoint eligibility.
using SourceNonCharacterSaveBorrowV86=std::function<bool(std::uintptr_t,
 dh2::level::LevelSaveObjectBorrowV2&,std::string&)>;
bool bind_source_campaign_noncharacter_saved_objects_v86(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<void> actual_class_owner,SourceNonCharacterSaveBorrowV86,std::string&);
}
