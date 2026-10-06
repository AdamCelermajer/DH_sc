#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::character {
// Borrowed projections of source node+118/+200 and visual+8. Ownership and the
// upstream visibility producer belong to the caller, not this eligibility test.
struct CanUpdateNode8 { std::uint32_t culling; std::uint8_t animate_enabled,reserved[3]; };
struct CanUpdateVisual8 { CanUpdateNode8* node; };
struct CanUpdateOwner40 {
 std::uintptr_t identity;
 CanUpdateVisual8* visual;
 std::uintptr_t player_link; // Character+418 compared with Player+660
 std::uintptr_t bounds; // caller's actual source AABB identity
 std::uint8_t enabled,force_update,interaction,reserved;
 std::uint32_t reserved_word;
};
enum CanUpdateOperation : std::uint32_t {
 can_update_online,can_update_remote,can_update_player,
 can_update_culling,can_update_dead,can_update_respawn
};
struct CanUpdateRequest24 {
 std::uint32_t operation,argument;
 std::uintptr_t owner,subject;
};
struct CanUpdateResponse16 { std::uintptr_t identity; std::uint32_t word,reserved; };
using CanUpdateInvoke = int(*)(void*,CanUpdateOwner40*,const CanUpdateRequest24*,CanUpdateResponse16*);
struct CanUpdateServices24 { void* context; CanUpdateInvoke invoke; std::uint32_t available,reserved; };
static_assert(sizeof(void*)==8 && sizeof(CanUpdateOwner40)==40);
static_assert(sizeof(CanUpdateNode8)==8 && sizeof(CanUpdateVisual8)==8);
static_assert(sizeof(CanUpdateRequest24)==24 && sizeof(CanUpdateResponse16)==16 && sizeof(CanUpdateServices24)==24);
}
// Original Character::CanUpdate 3a52a4. 0 complete (accepted is source bool),
// 1 malformed borrowed model, 2 required service unavailable, 3 delivery failed.
// online response.word is the raw receiver byte5; Player response.identity is
// actual Player+660. Other results use word!=0. Player request.argument=1 means
// GetPlayer(index0,true); culling subject is the borrowed AABB identity.
// Every reached provider is required, including IsDead and the online query.
// Callbacks are synchronous and may mutate the LIVE owner/visual/node model.
// Initial/final node stores use captured visual; the post-Player culling load
// uses reloaded owner.visual. Prefix effects survive failure; accepted only
// changes on complete delivery. Caller pins every captured projection.
extern "C" int dh2_character_can_update(dh2::character::CanUpdateOwner40*,
 const dh2::character::CanUpdateServices24*,std::uint32_t* accepted);
