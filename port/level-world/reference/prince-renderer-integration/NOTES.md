# Prince renderer integration audit

Read-only review on 2026-10-03. Reviewed renderer snapshot SHA256:
`1d45ddc0d5a107df1ffbe51224af6088cd0269e4d0a3e66e1a17d5b1ece0244e`.
No renderer, coordinator, tests, CMake or production source was edited by this audit. The additive RegistrationSet coordinator bridge was being implemented by the physical-controls worker during this review; its final proof is separate.

## Current renderer behavior

`model_renderer.cpp:103` declares single-slot `actor::Playback`. The world loader at 872-882 collects only the Idle/Walk/Run/AttackStatic/Attack/Died reachable subset into a `std::set<int>`, loads one Player per ID, and rejects `end<=start`. This loses original request order and duplicate occurrences. The actual staged bank is 116 resources and 158 original requests, with a distinct default designation for template1111. Source clip1138 is a valid zero-track library with raw bounds INT_MAX/INT_MIN; it must not be rejected using the legacy Player's start/end convenience values.

The full-bank host proof in `../../../engine-animation/reports/prince-bank-integration-host.json` shows the production dynamic compiler accepts the real 10 position-axis and 7 angle tracks, producing 158 clips and 83 ordered union targets with the 35-node Prince scene. All 17 original coordinator first-index mappings and all158 raw bounds pass. Its 26,228 samples prove compiled-resource ownership and sanitizer safety, not full-bank original-pose or GPU parity.

`advance_native_actor:766` currently executes:

1. Pending prototype death request.
2. Single-slot scene phase with synchronous authored callbacks; or, while frozen, legacy `Player::sample` into the live scene.
3. Genuine world Step.
4. Character timers.
5. Development touch heading/state events, state-machine update.
6. Single-slot animator completion/selection.
7. Native actor path, rotation and subobjects.

The normal source order is early scene/applicator traversal before Level physics, followed by Character timers → CharAI → FSM → animator → GameObject. Exact call sites are recorded in `../character-frame/NOTES.md`: Character timers `0x3ac02c`, AI `0x3ac034`, FSM `0x3ac03c`, animator `0x3ac048`, GameObject `0x3ac054`. Scene events and replay events are synchronous, and their effects precede subsequent pose sampling/path work. The renderer currently has an explicit gap where the complete CharAI update belongs; development touch input is not that missing AI implementation.

`draw:1108` selects a legacy bank Player even in world mode, then records `Playback.timeline.current_ms`. The actual Prince skinning subsequently uses `current_scene` and correctly draws the already composed native graph without another Player sample in the normal world branch. Preserve that property during migration. The frozen world branch at777-778 is the current live legacy overwrite and must be removed from the blended path.

## Concrete bank and coordinator replacement

Use the source RegistrationSet overload, not the vector-of-unique-IDs overload:

```cpp
dh2::actor::BlendedPlayback fresh;
dh2::animation::RegistrationSet registration;
// finalBank contains116 canonical Players in their final stable owner.
for (int id : authored158Requests)
    registration.append(id, std::uint64_t(id), &finalBank.at(id), error);
registration.set_default(1111, &finalBank.at(1111), error);
registration.refresh_indices();
fresh.compile_dynamic(finalBank, registration, immutableFactoryScene,
                      prince_visual, error,
                      dh2::animation::TransformMismatchBehavior::retain);
// Attach the synchronous observer before initial state/start callbacks.
fresh.observer = {context, blended_event};
```

Check all returns. Load the staged manifest's actual asset paths; do not reconstruct the bank by alphabetical map/set iteration. `Player::load(...,MissingTargets::ignore)` preserves the raw resource for dynamic compilation despite legacy skipped/unbound channels. Canonical identity equal to dictionary ID is the proven fixture contract, not an original manager allocation identity claim. For true cross-ID resource aliases the caller must supply canonical identity and refreshed map consistently; the bridge rejects stale alias maps.

The new overload preserves all158 engine occurrences and copies refreshed game→engine entries and engine→dictionary metadata. RegistrationSet can die after compile; the public bank remains the explicit immutable runtime identity. `current_clip()` and event.clip remain dictionary IDs. `current_engine_clip()` identifies the actual occurrence. Source17 mappings must still include, for example, dictionary1040→engine2 and1041→engine7; selecting dictionary IDs must not call `TransformSet::find_clip(dictionaryID)` on the occurrence-keyed compiled set.

Compile only after moving/committing the bank into its final owner. A coordinator compiled against a stack candidate map retains that map's address; moving its Players to the global bank does not repair the coordinator's bank pointer. A stable heap-owned session is another valid arrangement. Compile against an immutable authored factory graph, never a previously sampled/owner-transformed pose. The current factory and rest graph are both loaded directly from Prince BDAE and have35 nodes. SceneBinding must already be bound to the same node identity/order and reference node. Default1111 is a resource default, not a replacement scene or an extra appended occurrence.

Replace single-slot field accesses as follows:

| Current use | Blended use |
| --- | --- |
| `clip_id` | `current_clip()` for dictionary IDs/logging/combat |
| `timeline.current_ms/scale` | `current_timeline().current_ms/scale` |
| source registration occurrence | `current_engine_clip()` |
| `Playback` observer | `BlendedPlayback&, const BlendedPlaybackEvent&`; payload is `event.event` |
| `scheduler.stop_loop()` from a state service | `stop_loop(false)`; the source consumer service's bool argument must be preserved for `stop_loop(argument!=0)` |
| state selection/swap/speed | coordinator `start`, `swap`, `set_speed` with the stable bank/scene/binding |
| source step queries | `animation_depth`, `step_index`, `step_count(tables)`; closed getters must remain UINT_MAX/0 |
| metadata-only SetStep/Skip | `set_step`, `skip_next_step`; do not select/replay a clip to implement public SetStep |

Use the same `scene_phase(absolute,...)` before Step and `animator_phase(...,completion.extra_ms,...)` after timers/AI/FSM. Preserve applicator-captured extra milliseconds; do not recompute it from finalized current_ms. `time_phase` is the culled-node path that still advances timelines/events; it is not a frozen-inspector operation. The coordinator already applies composed pose/root and updates SceneBinding; leave GPU skinning as a consumer of this graph. Do not call legacy Player.sample after live blended scene/replay/animator work.

## Reset, reload and Activity recreation

Current `reset_context:229` clears the bank and scene but retains Playback, SceneBinding, FSM and timers. `load_world:981` replaces the map contents; `initialize_native_actor:710-717` only resets Playback if not restoring or visual unbound. Attack/Dead restore repairs body state without selecting an animation. This relied on the old Playback retaining its old scheduler/timeline while resources were reloaded.

A fresh blended dynamic compile initializes both slots to engine library0/template1111 and has constructor sequence_closed=1. Keeping State.current=5 or12 without explicitly selecting its current_animation would leave template slots under an Attack/Dead FSM. Do not do that. Pick one explicit development lifecycle policy:

* Retain a coherent CPU session across GL context loss: bank, compiled coordinator, scene pose, visual binding, clocks, state/timers and native gameplay storage remain together. Recreate only GPU resources. Do not replace/clear the bank behind a live coordinator.
* Or detach/destroy the old coordinator before clearing/replacing its bank, then compile fresh, attach observer and explicitly restart/reselect the saved sequence in the rebuilt runtime. This is a development restart policy, not exact restoration of old fade weights, slot clocks, completion flags or root history. Do not claim it preserves the interrupted blend/combo phase.

For fresh startup, compile before Idle transition and attach observer before start: original constructor closed=1 is observable during first selection24/26, with valid clip selection clearing closed afterward. Do not reconstruct private timelines by copying only the current slot. Do not recompile/reset/erase the bank from an authored/scheduler callback; the outer traversal retains immutable old event-manager data through its synchronous batch.

## Synchronous events and current AI guarantees

The existing `character_playback_event:629` supports only28→prototype melee route and22→`prince_event`. It does not invoke the verified six-event Character/CharAI router, begin/end consumers or selected AIS wrapper. There is no live active/pending AIS allocation lifecycle or common-Lua binding in this renderer. End-v98 and preattack-vA4 are empty for the *selected* AISPlayerIPhone inherited default, but that is conditional on actual active-class binding, not permission to make every AIS callback a no-op. Row44 `__player__` selects this class; its scripted=1 path still loads `_commons` and binds character functions. Its OnUpdate `0x3dc798` is nonempty.

Blended events24/26 occur inside recursive selection, and27/25/23/22 occur inside recursive completion. Route the six IDs through `dh2_character_animation_event_route` with LIVE state/controller/global facts. Its22/23 branches call end virtual before FSM even when normal gates are blocked. State4/5/6/7 selects move/attack/skill begin/end consumers, whose real result governs FSM forwarding. Supply `dh2_character_animation_ai` plus synchronous live services rather than unconditional acceptance. Required consumer services include scheduler getters/control, target position/death, actual melee radius, native combat queries, controller MoveTo/LookAt, target clearing/synchronization, preattack virtual, and nested Character events1a/1b/1c. Native scalar/kernel proofs do not provide unresolved controller/target/AIS producers automatically.

Keep the existing Facts scope/reentry pattern and refresh live AI/FSM projections at source service boundaries. A callback may start/swap/stop/change state synchronously. Do not retain vector/frame references across such calls; re-fetch current scheduler/slot afterwards. The coordinator already retains an old event-manager batch and checks slot generation before writing its old cursor back. Do not add an external deferred event queue or filter all outgoing-slot events: every nonzero-weight slot can deliver authored28, while only the current slot supplies actor completion.

Authored28 currently uses a bounded combat name mapper and supplied unarmed sheets. It does not execute full original `_OnAnimEvent` (`0x3d4434`) or general AIS/event acceptance. Continue naming this boundary. Existing timer expiry clears state gates via events2a/2b/2c and logs missing optional AI expiry; selected pause-expiry31 must call `dh2_character_script_pause_expired` and return without FSM forwarding when the actual script bridge is added. Do not send31 through the22..27 router. Collision counter/update kernel availability does not establish live collision/AIS ownership by itself.

## Frozen inspector

The current world `set_time` clamps against standalone idle Player bounds, then the frozen frame samples one Player over the live composed graph while physics/timers/FSM/animator continue. This cannot preserve a two-slot composed pose or root history.

The smallest safe replacement is an explicit development freeze: retain the last composed scene and skip the whole live actor/gameplay advance while frozen, updating the host frame timestamp so resume does not accumulate the pause into dt. Do not seek or resample the live coordinator merely to render a frozen frame. Resume from its existing logical clock/root histories. A scrubbed standalone preview can remain in the non-world inspection branch. If live simulation must continue while inspecting, copy a separate inspection scene and draw from it; it must not replace the live scene or feed its scratch/root back into the actor. A genuine arbitrary-time blended seek would need a separately bounded operation and proof; the current API has none. These inspector policies are development controls, not the original game's pause semantics.
