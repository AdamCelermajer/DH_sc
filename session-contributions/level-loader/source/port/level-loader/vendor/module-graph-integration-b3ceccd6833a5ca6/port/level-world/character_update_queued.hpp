#pragma once
#include "character_update_startup.hpp"
#include "character_deferred_queue.hpp"
namespace dh2::character {
struct UpdateQueuedBinding24 {
 CharacterDeferredQueue* queue;
 const DeferredQueueOwner16* actor;
 const DeferredQueueServices24* queue_services;
};
static_assert(sizeof(UpdateQueuedBinding24)==24);
}
// Complete bounded Character.Update pre-controller prefix. It executes directly
// and never calls/replays the frozen partial startup entry. Borrow one shared
// queue and keep it/all actor projections pinned through every synchronous call.
// actor.identity must match eligibility.identity. StartupOwner.queue is a legacy
// view and is not read/written: the owned queue supplies actual topology/count.
// Required deferred LoadNInit and queue Kill/full AIUnload providers are explicit.
// Controller Update, post-controller bot logic, timers/AI/animator/GameObject and
// later suffix are separate mandatory downstream phases, not run by this entry.
// 0 complete (stage=ineligible/controller-ready),1 malformed,2 provider unavailable,
// 3 delivery failure. Failure retains delivered prefixes; stage is unchanged.
extern "C" int dh2_character_update_queued(dh2::character::UpdateStartupOwner64*,
 const dh2::character::UpdateStartupServices24*,const dh2::character::UpdateQueuedBinding24*,
 std::uint32_t* stage);
