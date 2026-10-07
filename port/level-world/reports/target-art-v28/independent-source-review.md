# Independent target-circle source review

Reviewed `CharacterTargetMarkerV28`, the marker methods added to `CharacterMeshFxOwnerV4`, the renderer connection and captured `target-functions.asm`. This review made no production source edits and did not operate the emulator.

## Concrete findings and corrections

1. **Runtime destruction reached dead providers.** `target_marker_v28` was declared before `world`, target-registry and actor providers. Default reverse member destruction therefore destroyed those providers before the marker destructor called `release -> drop -> sync -> combat_fx_remaining`. Root corrected this by resetting the marker first in `PlayerSkillsRuntime::~PlayerSkillsRuntime`, while every required owner remains alive. This also precedes player-manager transfer.

2. **Warm FX reuse reset the wrong field.** Source `_GetAnimFX` at `494bdc..494be0` calls `472708` with float 1. The original ELF symbol and function body identify this as `VisualObject::SetScaling(float)`, setting all three root-scale components to one. The initial native marker and regular play pool branches instead changed material alpha, and the composite factory's material authority is not the fallback `Resource::scene` table. Root corrected warm reuse to reset the retained `visual_scale` to `(1,1,1)`. The next same-owner sync/sample produces the corrected composite outer transform. No material-alpha reset is justified by this source call.

3. **Non-character OOI remains outside the renderer connection.** The core owner calls the reached target's interaction virtual and item tooltip, but `renderer_target_marker_v28.inc` currently requires a registered Character and supplies no tooltip callback. Item/chest/non-character targeting is still a required continuation; the current enemy-circle checkpoint must not describe all nine interaction families as connected.

## Verified source behavior

- InitPost allocates nine slots at `3b5214`, uses the ordered case-sensitive `target_circle_00_chest` name search and consecutive base-plus-index IDs, preserves null resources and hides each obtained marker.
- InitPost `3b5330..534c` is an animator/timeline loop branch, not a material or light branch. `49267c` is `AnimatedFX::GetAnimator`. The animator yields its timeline and the next virtual call enables looping. Grab's existing timeline loop enable is consistent with this suffix.
- Grab preserves the source module check before signed set bounds, pool ownership, anchor publication, two explicit syncs, loop enable, start, SetAnimFX loop override `-1`, and visibility enable.
- Source SetAnimFX does not rewrite the timer. A new marker's constructor timer is `-1`; no invented timer reset should be added to warm Grab.
- Update uses source `last_target40c` and falls back to `OOI14a4` only for null/self. Negative interaction preserves the current selection. A same-type retarget reanchors and syncs without restarting the timeline. An absent marker leaves source selection unchanged. Clearing resets selection only when the current slot owns a marker.
- A separate fabricated alive filter would change source behavior. AnimatedFX Update clears a dead/disabled anchor; actual target AI and OOI updates own target selection and clearing.
- Rebind already releases the marker before destroying its same FX manager and resource/floor providers. The explicit runtime-destructor correction now makes final teardown consistent with that order.

The existing 1261-check actual-cache marker test proves resource geometry, loop persistence, same-type retarget, movement once, visibility switching, OOI fallback and release with declared providers. It does not cover a non-unit-scale pooled reuse, production item/chest virtual dispatch, or live target AI timing. Those limits remain distinct from the source review.
