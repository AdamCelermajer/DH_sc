# Lane 07 — Targeting and focus

## Source delivery

Updated `port/android-native/app/src/main/cpp/renderer_player_target_frame_v2.inc` in `player_target_interaction_spot_v2`. When the actual target's retained scene contains the cached `interaction_position` node, the callback now borrows that same registered actor and copies the node's current world-matrix translation. The cached node index is still invalidated when its source visual changes. When the source has no such node, the callback continues to use the target's live `GetTargetPosition` projection.

This completes the source `GameObject::GetInteractionSpot() const` behavior used by target out-of-range approach: IDA body `0x38b228` caches the specific node named `interaction_position` at `+0x2e8` with the checked byte at `+0x2ec`, calls `ISceneNode::getAbsolutePosition` each time, and falls back to `GetTargetPosition()` (`0x3935dc`) when the cached node is null. The handoff uses the current borrowed native scene graph's world matrix as that absolute-position producer; ARM32 object offsets and pointers are not used as native64 layouts.

## Existing owners and root wiring

- `renderer_player_target_frame_v2.inc` is already included by root-owned `model_renderer.cpp`; `bind_target_frame` constructs the `CharacterWorldTargetFrameV2` over the existing World target registry and same player `TargetState`. The updated callback is already provided as `interaction_spot` to `player_target_out_range_v2` for AIS event `0xe` (`AISPlayerIPhone::OnTargetOutOfRange`, `0x3de1b4`). No new target/FSM/World owner is introduced.
- Shared acquisition is in `character_target_search.cpp` (`dh2_target_search_policy_v108`), called by the current campaign Character and interest owners. `target_frontal_sort_v41.cpp` preserves the source empty-then-sort transition. World click selection is exposed through `world_click_target_v1` and mutates the same Character `TargetState` via `AI_SetTarget` / `AI_SyncLastTarget`.
- Marker lifecycle is exposed through `renderer_target_marker_v28.inc` and the existing `CharacterTargetMarkerV28` / same Effects manager. Facing routes through `renderer_target_facing_v38.inc` to the existing controller query and close-range geometry service. Target-death/aggro cleanup routes through `renderer_target_cleanup_v40.inc` to the retained player/NPC actors and their existing bindings/controllers.
- Player heading transport remains governed by `renderer_player_heading_v1_handoff.md`: root-owned HUD wiring should call `source_player_heading_v1` on the authored active-heading update and stop only on source release. Do not synthesize destination or state events.

## Limits and verification boundary

The original interaction-position lookup, absolute-position call, fallback, and out-of-range event identity were checked in IDA pseudocode and original ARM assembly. This is a source delivery only. No build, tests, emulator, or gameplay confirmation was run; root owns those shared checks. The source still reports unsupported providers explicitly when the registered target/scene disappears or an interaction visual is replaced during a cached query.
