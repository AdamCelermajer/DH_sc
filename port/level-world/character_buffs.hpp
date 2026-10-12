#pragma once
#include "character_timers.hpp"
#include "../game-data/properties.hpp"
namespace dh2::character {
struct BuffOwner;
enum BuffService : std::uint32_t { buff_fx_load=0,buff_fx_release=1,buff_fx_object=2,buff_fx_enable=3,buff_recalculate=4 };
struct BuffRequest32 {
 std::uint32_t service=0;std::int32_t id=-1,index=0;std::uint32_t enabled=0;
 std::uintptr_t subject=0,character=0;
};
// Return 1 after delivering the real service, otherwise 0. Recalculate must
// execute original class-to-base then all-property resolution; it receives the
// current signed-key/deque-ordered groups through the borrowed PropertyView.
using BuffInvoke=int(*)(void*,dh2::data::PropertyView*,const BuffRequest32*,std::uintptr_t*);
struct BuffServices16 {void* context=nullptr;BuffInvoke invoke=nullptr;};
struct BuffBindings32 {
 dh2::data::PropertyView* properties=nullptr;TimerStore32* timers=nullptr;
 const TimerServices32* timer_services=nullptr;const BuffServices16* services=nullptr;
};
struct BuffResult24 {std::uintptr_t instance=0;std::uint32_t calls=0,phase=0;std::int32_t status=0;std::uint32_t reserved=0;};
struct BuffSnapshot48 {
 std::uintptr_t instance=0;std::int32_t id=0;std::uint32_t strength=0;
 std::int32_t timer_id=-1;std::uint32_t reserved=0;
 const std::int32_t* sheet=nullptr;const char* name=nullptr;std::uintptr_t fx=0;
};
struct BuffDictionary16 {const char*const* names=nullptr;std::uint32_t count=0,reserved=0;};
static_assert(sizeof(BuffRequest32)==32&&sizeof(BuffServices16)==16&&sizeof(BuffBindings32)==32&&sizeof(BuffResult24)==24&&sizeof(BuffSnapshot48)==48&&sizeof(BuffDictionary16)==16);
}
extern "C" {
// The view initially has no groups; all borrowed bindings outlive the owner.
// Owner updates its group pointers after each structural change. Callbacks may
// inspect these snapshots but cannot mutate/destroy the owner synchronously.
dh2::character::BuffOwner* dh2_character_buffs_create(const dh2::character::BuffBindings32*);
int dh2_character_buff_add(dh2::character::BuffResult24*,dh2::character::BuffOwner*,std::int32_t id,std::uint32_t duration_ms,std::int32_t capacity,std::uint32_t strength,std::int32_t fx_oid,const char* name);
// Source assertion-safe domain: duration>0, amount>=0, element -1..4. Names are
// the actual two source dictionaries, not a synthesized DoT ID table.
int dh2_character_buff_add_dot(dh2::character::BuffResult24*,dh2::character::BuffOwner*,std::int32_t duration_ms,std::int32_t amount,std::int32_t element,const dh2::character::BuffDictionary16* buff_ids,const dh2::character::BuffDictionary16* fx_ids);
int dh2_character_buff_delete(dh2::character::BuffResult24*,dh2::character::BuffOwner*,std::int32_t id,std::uintptr_t instance);
// Actual Timer object delivery (GetReference then GetID). A mismatched ID in
// the original non-trapping assertion mode still deletes the referenced buff.
int dh2_character_buff_expired(dh2::character::BuffResult24*,dh2::character::BuffOwner*,const dh2::character::Timer32*);
int dh2_character_buffs_remove_all(dh2::character::BuffResult24*,dh2::character::BuffOwner*);
// CharProperties D1 buff portion: free sheets, release nonnull FX, no timer
// stops and no recalc. Caller must end timer delivery before destruction.
int dh2_character_buffs_destroy(dh2::character::BuffOwner*);
std::uint32_t dh2_character_buffs_count(const dh2::character::BuffOwner*);
std::uint32_t dh2_character_buffs_declarations(const dh2::character::BuffOwner*);
int dh2_character_buff_snapshot(dh2::character::BuffSnapshot48*,const dh2::character::BuffOwner*,std::uint32_t ordered_index);
// 1 delivered, -1 atomic malformed/reentrant call, -2 service/allocation/native
// storage failure preserving the already executed source prefix. Add success
// may legitimately return instance=0 (capacity or source StartTimer=-1).
}
