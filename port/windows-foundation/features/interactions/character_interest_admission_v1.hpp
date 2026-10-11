#pragma once
#include "../../actor_state.hpp"
#include "../../../game-data/ai.hpp"
#include <cstdint>

namespace dh::foundation {
// Source Search 0x4a2f34 -> IsInteractive 0x3a4870 -> mask89 0x4a1ab8.
// Values belong to the live owner; enabled138 is distinct from visible128.
struct CharacterInterestAdmissionV1 {
    bool enabled138{}, deleted129{}, dead5193{}, faery{}, summoned{}, monster{};
    bool friend_to_viewer{}, enemy_to_viewer{}, viewer_is_player{}, candidate_is_player{};
    bool viewer_friend_to_candidate{}, viewer_enemy_to_candidate{};
    bool zonable{}, zoned750{}, in_zone752{}, interactive1045{};
    bool registered_talk762{}, merchant{};
    std::uint32_t flags1312{};
    int owner_state{}, candidate_state{};
    std::int32_t owner_word4884{}, candidate_word4880{};
};
// IsSummoned0x3a30ac reads CharType5 only; pool ownership is not a type.
// Relationships0x3d511c/0x3d574c read current actor faction where bound.
inline void project_character_interest_identity_v1(CharacterInterestAdmissionV1& s,int source_type,
    int initial_faction,const ActorState* live,const ActorState& viewer,const dh2::data::AiTables& tables) noexcept {
    s.faery=source_type==3;s.summoned=source_type==5;s.monster=source_type==4;s.candidate_is_player=source_type==1;
    s.friend_to_viewer=s.enemy_to_viewer=s.viewer_friend_to_candidate=s.viewer_enemy_to_candidate=false;
    const int faction=live?live->faction_id:initial_faction;
    if(faction>=0&&std::size_t(faction)<tables.factions.size())for(const auto& r:tables.factions[faction])if(r.id==viewer.faction_id){s.friend_to_viewer=r.value>0;s.enemy_to_viewer=r.value<0;break;}
    if(viewer.faction_id>=0&&std::size_t(viewer.faction_id)<tables.factions.size())for(const auto& r:tables.factions[viewer.faction_id])if(r.id==faction){s.viewer_friend_to_candidate=r.value>0;s.viewer_enemy_to_candidate=r.value<0;break;}
}
inline bool character_interest_interactive_v1(const CharacterInterestAdmissionV1& s) noexcept {
    if(s.dead5193 && s.friend_to_viewer && !s.monster)return true;
    return !s.deleted129 && s.enabled138 && !s.faery && !s.summoned && !s.dead5193 &&
           (s.flags1312&0x2000u) && s.interactive1045;
}
inline bool character_interest_admitted_v1(const CharacterInterestAdmissionV1& s) noexcept {
    if(!s.enabled138 || (s.zonable&&s.zoned750&&!s.in_zone752) ||
       !character_interest_interactive_v1(s) || s.owner_word4884<s.candidate_word4880)return false;
    if(s.merchant || s.registered_talk762)return true;
    if(!s.dead5193&&s.viewer_enemy_to_candidate&&(!s.candidate_is_player||!s.viewer_is_player))return true;
    return s.dead5193&&s.viewer_friend_to_candidate&&s.owner_state!=15&&s.candidate_state!=16;
}
inline int character_interest_type_v1(const CharacterInterestAdmissionV1& s) noexcept {
    if(s.enemy_to_viewer&&!s.viewer_is_player)return 8;
    if(s.summoned||s.dead5193)return -1;
    return s.monster?8:3;
}
}
