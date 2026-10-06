#pragma once
#include "character_mesh_fx_owner_v4.hpp"
#include <array>
namespace dh2::character {
struct TargetMarkerServicesV28 {
 void* context{};
 bool(*interaction_type)(void*,std::uintptr_t target,std::uintptr_t player,std::int32_t&,std::string&){};
 bool(*item_tooltip)(void*,std::uintptr_t target,std::uintptr_t player,std::string&){};
};
// Character InitPost3b5214..5358 and target-circle suffix3ac758..81c/8b4..8fc.
// This receiver shares the existing FX manager; it owns no clock or target AI.
class CharacterTargetMarkerV28 {
 fx::CharacterMeshFxOwnerV4& effects_;
 data::EffectsTables::Borrow tables_;
 TargetMarkerServicesV28 services_;
 std::uintptr_t player_{};
 std::array<std::uintptr_t,9> circles1494_{};
 std::int32_t selected1498_{-1};
 bool attempted_{},initialized_{};
public:
 CharacterTargetMarkerV28(fx::CharacterMeshFxOwnerV4&,data::EffectsTables::Borrow,std::uintptr_t player,TargetMarkerServicesV28);
 ~CharacterTargetMarkerV28();
 CharacterTargetMarkerV28(const CharacterTargetMarkerV28&)=delete;
 bool initialize(std::string&);
 // Borrow exact Character40c (CharAI.last_target) and14a4 each source frame.
 bool update(std::uintptr_t last_target40c,std::uintptr_t object_of_interest14a4,std::string&);
 bool release(std::string&);
 std::int32_t selected()const noexcept{return selected1498_;}
 const std::array<std::uintptr_t,9>& circles()const noexcept{return circles1494_;}
};
}
