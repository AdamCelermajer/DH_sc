# Prince full registration: read-only live integration proposal

This review changes no actor, raw animation, renderer or CMake source. Locations below identify the current pre-integration code, inspected after the source component sampler and full-bank audits. The full-bank host report `port/engine-animation/reports/prince-bank-integration-host.json` passes116 resources,158 compiled occurrences,17 source projection checks and26,228 samples. Its83 first-seen union targets have types1:28,5:28,10:27, with two unbound targets. These raw-bank checks do not establish live two-slot/FSM/frame parity.

## Two identities must remain separate

Original `LoadAnimation` at3659ec appends to the dynamic engine library before unique game-ID insertion. `_UpdateAnimationIndices`364af4 resolves each first-inserted resource identity to its first engine occurrence. The existing `RegistrationSet` implements these rules; `compiled_inputs()` returns synthetic IDs0..157 equal to engine indices. The game dictionary map has116 entries. Neither deduplicating158 inputs nor sorting116 dictionary IDs reproduces the source library.

Current blockers in `actor_blended_playback.cpp`:

- Lines74–88: `compile_dynamic(ClipBank, clip_order, ...)` requires order length equal to the unique map size and rejects repeated IDs. It cannot represent this158/116 bank.
- Line149: `compiled.find_clip(scheduler.clip().anim)` searches dictionary IDs. Synthetic compiled IDs are engine indices, so this rejects or selects the wrong occurrence.
- Lines111–115: initial library0 is correct, but assigning `slot.clip_id=first->id` yields synthetic0 rather than dictionary1111 for live event/log metadata.
- Lines148/159: replay compares `slot.clip_id` against the requested dictionary ID. Source replay compares engine indices, including when distinct game IDs alias the same resource.

Minimal additive API proposal in `actor_blended_playback.hpp` near line77:

```cpp
bool compile_dynamic(const ClipBank& unique_resources,
                     const animation::RegistrationSet& registration,
                     const scene::Scene& scene_bindings,
                     const visual::SceneBinding&, std::string& error,
                     animation::TransformMismatchBehavior mismatch =
                         animation::TransformMismatchBehavior::retain);
```

Keep the existing static and unique-ID fixture overloads unchanged. The new overload uses `registration.compiled_inputs()` and `registration.default_player()`; it never copies/reorders/deduplicates the occurrence sequence. Before committing state, verify sequential engine indices, nonnull resource/default pointers, each occurrence's exact borrowed Player against the supplied dictionary-keyed bank, and every lookup entry's engine index/resource identity against the occurrence vector. Preserve compile errors for unsupported raw/applicator domains. Copy the resolved game-ID lookup and engine-to-dictionary metadata into playback-owned vectors, so later synchronous dispatch does not depend on a temporary RegistrationSet. The actual unique Player bank remains the existing explicit immutable borrowed binding.

The copied lookup must contain the result of `RegistrationSet::lookup`, including its refreshed first-resource-identity resolution, rather than reconstructing a different first-ID policy. Selection resolves dictionaryID -> stored first engine index -> `compiled.clip(index)`; reject a missing/out-of-range mapping. Timeline `clip_index` and `PlaybackSlot::compiled_clip` receive the engine index. `clip_id` and `current_clip()` may retain dictionary IDs for renderer/event metadata, provided no numeric kernel index is taken from them.

Original same-engine replay evidence is exact: `PlayClip`47680c reads old `getCurrentAnimation`65f114 at476868; that getter reads AnimatorSet+50. `SetCurrentAnimation`3674ac maps a game ID through3660f4, selects the mapped index through65f8c8, and returns it at367508. `PlayClip` compares old/new engine indices at4768b4. Therefore save `previous=slot.compiled_clip` before selection and compare `previous==index` for the extra-time jump at native line159. Changing dictionary IDs that alias one engine resource must still take this same-engine branch. Keep source Blend-before-lookup side effects and the synchronous replay/callback order.

## Constructor, default and root

The original first occurrence is template dictionary1111 (`prince_template_anim.bdae`), start0/end3599; MenuIdle1063 is occurrence1. Default designation is the separate second template resource load, not another append. Template SHA256 is `23054c4f06f75cdb677541ca728f5f689f8e018b3f05f25008297172ff49a102`. The modular model dictionary78 is not the animation default and fixture key20000 is not a source registration.

Fresh dynamic construction must retain both slots' engine0 selection, loop1, cursor0 and no clock advance; map metadata back to dictionary1111. Keep the actual default resource passed into raw compilation and source retain1 mismatch policy. Do not replace it with scene rest values or select Idle in the constructor; the later actual character Idle focus chooses an authored sequence.

Root lookups already use the correct engine field: `reset_current` lines236–239 and the all-slot aggregate loop lines275–281 call `compiled.clip/sample(slot.compiled_clip, ...)`. Preserve these. Root target selection at line103 keeps the last type1 match for the bound animated node; it must not be replaced by a hardcoded target index or dictionary lookup. Pose and root share each slot's per-target key cursor. Outgoing zero-weight root history and the single scratch buffer remain source behavior. There is no extra per-slot owner displacement.

The17 current-coordinator source projections, dictionaryID:first engine index, are:

`1040:2, 1041:7, 1126:8, 1114:9, 956:11, 959:14, 962:17, 967:19, 955:20, 957:21, 969:22, 958:23, 960:24, 971:25, 961:26, 963:27, 1023:41`.

Current full-bank union types1/5/10 already satisfy `bind_compiled` lines98–100 and the typed apply dispatch at267–272. Raw component/angle tracks merge under the first template handlers. This bank does not justify removing those applicator guards or always accepting arbitrary component/material types.

## Zero-track1138 stays registered

Dictionary1138 occurs at engine index155 and has original raw bounds INT_MAX/INT_MIN. Its legacy Player start/end0 is not an authoritative compiled bound. Keep the exact original raw bounds and occurrence. It need not be proactively played; only an actual scheduler request selects a registered clip.

Current `bind_compiled` has no all-clip range validation: lines110–115 initialize only library0. `dh2_timeline_clip` at `visual_timeline.cpp:34` accepts signed start/end without a start<end gate and uses wrapped end-start. Do not introduce a broad range gate, fabricate a duration, or filter1138. The current renderer load rejection is `playback.end<=playback.start` at `model_renderer.cpp:880`; remove that legacy playback eligibility test from full resource registration, while retaining BRES/raw compiler validation. Avoid `std::clamp(value,start,end)` for a reversed source range; the current debug single-clip branch at777–778 cannot be used for1138.

## Renderer wiring points

In `port/android-native/app/src/main/cpp/model_renderer.cpp`:

- Line81 stores a dictionary-keyed unique Player bank; it may continue owning116 resources. Lines872–881 currently collect a deduplicated subset of six states. Replace that staging producer with the saved158 ordered source calls plus the separate default designation, loading all116 validated resources (including the root-troll requests) with the existing explicit unbound-target policy. Do not treat the authored17 projection as the complete bank.
- Line103 owns single `Playback`. Use `BlendedPlayback` after the new occurrence-aware API is audited. At712–717, bind the graph, compile from the already committed unique bank and ordered registration before attaching an observer, then let the existing state initialization at727 select Idle. Fresh detached compile is required; setting an observer before compilation currently rejects.
- State services539/555 retain dictionary-based scheduler start/swap calls. Resolve inside the new selection bridge. Replace renderer direct `.clip_id`/`.timeline` observations with `current_clip()`/`current_timeline()`. The public `stop_loop(false)` method can supply the existing stop-loop service rather than bypassing the coordinator.
- Event callback629 adapts to `BlendedEventObserver`: forward `BlendedPlaybackEvent.event` with dictionary metadata and preserve slot order, outgoing events and retained old-manager batches. Do not filter to the dominant/current slot.
- Scene phase772 remains before Step780; animator phase809 remains after character timers/state and before the actor coordinator815–817. No extra event clock or physics Step belongs in the bank adapter.
- Frozen branch777–778 and draw lookup1108–1109 currently access single legacy Player sampling. A full-bank/two-slot live path cannot use that sampler, which resets rest pose and skips the17 additional raw channels. Keep inspection as an explicit development boundary, preserving the already composed pose or supplying a separately specified debug operation; do not silently collapse the live two-slot pose.
- `reset_context`229 clears the borrowed bank without detaching playback; commit981 replaces the global bank; initialization712 may retain playback on restore. The new coordinator's immutable bank binding requires a concrete reset/recompile or an explicitly validated same-resource restore path. Do not bypass `ready()` or treat pointer-address equality as proof that replaced resource contents are identical. Compile errors should deactivate/report through the existing live error path rather than continue with partial state.

The next integration audit should exercise all158 occurrences, the17 exact selection projections, duplicate/alias same-engine replay, library0/default1111, preserved1138 bounds, actual root engine indices, both-slot authored event order and the existing scene-before-Step/post-Step-animator ordering. Current full-bank/raw and old reduced-coordinator reports remain separate evidence until that adapter is implemented and tested.
