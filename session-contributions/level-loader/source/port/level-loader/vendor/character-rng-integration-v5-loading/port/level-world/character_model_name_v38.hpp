#pragma once
#include "properties.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::character {
// References only. Bind to the SAME retained actor/property/cache/CharAI owner.
// A missing field producer differs from a genuinely produced NULL master.
struct CharacterModelFieldsV38 {
 std::shared_ptr<const void> receiver_lease;
 std::uintptr_t identity{};
 const data::PropertyView* properties{};
 const std::int16_t* properties13c8{};
 const std::uintptr_t* master418{}; // actual CharAI+50, never a sidecar owner
};
struct CharacterModelNamesV38 {
 std::shared_ptr<const void> dictionary_lease;
 const std::vector<std::string>* files{}; // actual retained CharacterTables.models
};
struct CharacterModelServicesV38 {
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> character_type; // whole source GetCharType3a3054
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_player; // actual virtual28; never inferred from AI type1
 std::function<bool(bool&,std::string&)> high_performance;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_local_player;
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t&,std::string&)> saved_current_faery; // actual SG getter; difficulty=-1
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t&,std::string&)> faery_model; // actual GetCharFaery(row)->Model+c
 std::function<bool(std::uintptr_t,std::int16_t,std::string&)> unknown_remote_class_debug;
};
enum class CharacterModelBranchV38 {missing_model,authored_nonplayer,faery_null_master,faery_owned,player_default,player_low_remote};
struct CharacterModelResultV38 {
 std::shared_ptr<const void> dictionary_lease;
 const std::string* file{};std::int32_t model_id{-1};
 CharacterModelBranchV38 branch{CharacterModelBranchV38::missing_model};
};
// Whole model-name selection only. It does not select class/template, mutate
// caches/RNG, assign a master, load saved equipment, create visuals or activate.
bool character_model_name_v38(const CharacterModelFieldsV38&,const CharacterModelNamesV38&,
 const CharacterModelServicesV38&,CharacterModelResultV38&,std::string&);
}
