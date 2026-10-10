#pragma once
#include "../../../level-world/character_ai_pointer_fields_v105.hpp"
#include "../../../game-data/ai.hpp"
#include "../../../script-runtime/script_runtime.h"
#include <array>
#include <functional>
#include <string>

namespace dh::foundation::companions {
struct SourceMasterBorrow {
    dh2::character::CharacterAiPointerFieldsV105* fields{};
    std::uintptr_t same_character{};
    const dh2::data::AiTables* tables{};
    std::function<bool(std::int32_t&,std::string&)> get_ai_id;
    std::function<bool(std::uintptr_t,std::uint32_t&,std::string&)> is_dead;
    std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> target_position;
    std::function<bool(std::uintptr_t&,std::string&)> hosting_player_character;
};
// Whole CharAI::AI_SetMaster3d4d80; mutates the SAME fields before any reached
// service. No reciprocal target/observer update exists in this original body.
bool set_source_master(SourceMasterBorrow&,std::uintptr_t,std::string&);
// Original Character wrappers. HasMaster and nil-master host check need no
// PlayerManager query; SetMaster ignores absent/non-object first values.
bool source_has_master(const SourceMasterBorrow&,bool&,std::string&);
bool source_is_master_host(const SourceMasterBorrow&,bool&,std::string&);
bool source_set_master_values(SourceMasterBorrow&,const dh2_script_value*,std::uint32_t,std::string&);
}
