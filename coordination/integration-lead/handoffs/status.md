# Status/DOT/leech integration checkpoint

Ownership: previous status worker edited only `port/windows-foundation/features/status_effects`, `reports/feature-status-live-integration.json`, and this authorized handoff. No main, CMake, world core, or shared assets edited. New worker can assume exclusive writing after this handoff. No further worker delegation performed.

## Current code and test state

- `status_effects.hpp/.cpp`: original thin borrower of actual ActorState/Character/PropertyView/BuffOwner/TimerStore/NativeEffects/Slow/tick services. Added read-only `timer_store()` for callback publication identity checking.
- `status_live_binding.hpp/.cpp`: `DotTickBinding` bridges recovered TimerEffect calculate/apply requests to `dh2_character_dot_calculate_result` and `dh2_character_dot_apply`. SAME borrowed DotActor/PropertyView/shared combat; explicit failure for absent real query/full DotServices. CalculateResult entry avoids a duplicate outer F_DotAttack debug pair.
- `StatusTimerDelivery` supplies stable TimerServices32 descriptor before BuffOwner publication; `bind(status,error)` once. Event54/0x33/0x34 dispatch to source adapter; other Character timer events/growth delegate original provider. Native void expiry retains first negative status in `failure()/error()`. Host must inspect immediately after its **sole** source timer Update. No second clock/schedule/health/FSM created.
- `dispatch_skill_status` routes v6 stun/scare/slow to source effects and delegates all other full FApplyResult services/result unchanged. A required missing native state service never falls through to false acceptance.
- `source_status_owners.hpp/.cpp`: bounded **fresh-only** CharProperties C2 projection over caller-retained PropertyState/PropertyView/process temporary/NativeFsm/TimerStore. Allocates only genuine source BuffOwner map, resets base/saved/gear/cached then SAME process temporary. Rejects already published BuffOwner and foreign backing. `SourceStatusInitBinding` calls existing recovered ScriptLifecycle `script_on_init`, real Character IsDead fields, actual design lookup kernel, same native TimerStore operations, mandatory original pending AIS Init callback. Reached errors retain source prefix. These helpers are NOT whole Character constructor implementations and must not reset/adopt existing actors.
- `source_status_constructors.asm`: llvm-objdump extraction from actual original ELF `.local-inputs/libDungeonHunter2.so`; SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80` verified.
- `run_tests.ps1` compiles/runs `.local-inputs/status-effects-live-tests.exe` and `.local-inputs/status-source-owners-tests.exe` with bundled clang, `-Wall -Wextra`, and original source kernels.
- Both PASS. First executes real DoT calculation/application and intentionally fails missing full HitFor, verifies retained combo/context/debug prefix, and actual TimerStore Update->latched failure. Second verifies caller-retained sheet/global scratch identity, all224 default stores, no fake recalc, source OnInit order with old timer stops/new33 publication before second design query, dead gate plus pending AIS Init, reached design/AIS failure prefixes. Constructor tests use explicitly synthetic source-field defaults/constants; no live actor publication or new ARM differential proof is claimed.

## Critical canonical owner discoveries (preferred integration)

`CanonicalCharacterCandidateRecordV60` already retains ONE canonical actor/session/machine/property/life graph. **Do not create alternative owners or replay the fresh property constructor on this record.**

- Player BuffOwner: `record.player_script_owner_v62->native_buffs()`.
- NPC BuffOwner: `record.save_connection_v86->borrow_buffs(out,error)`. `CanonicalCharacterSaveV86::extra` creates its sole NPC source BuffOwner if absent, binding `&actor.session->property_view()`, `&actor.session->timers()`, `&actor.session->native_timer_services()`, and real canonical buff services.
- Native NPC buff recalc executes original class-to-base/all-property resolver over session's actual live buff groups, then mirrors group pointers into record.view.
- Use session's **own** property_view for BuffOwner/status. `record.view` is another projection over SAME sheets and can otherwise have stale group pointers. Player uses V3 native player session's view/timers; inspect `native_session()`/`native_player()` accessors before selecting a generic actor.session.
- Actual native FSM: `actor.machine->native_fsm()`. Source constructor combat scalar authority: `actor.machine->combat_fields()` (combo, invulnerable, push_death, network_id). Do not substitute ActorState/action or compatibility life copies for FSM/scalars.
- Actual generic session timers/lifecycle: `actor.session->timers()`, `native_timer_services()`, `owner().lifecycle()`.
- Shared process CharProperties temporary: `character_properties_temp_global_v62()`.
- `NpcRecurringEffectsV1` already borrows identity/property/network/remote/dead/state/actual outgoing+incoming Aggro trees, SAME `CharacterWorldSkillCombatV6*`, shared combat context and real Debug. It executes source TimerEffect bodies; calculates through original direct-dot kernel; applies through **SAME registered full world combat.apply**, not a health-only callback. Prefer this canonical route over a separate DotTickBinding/FApplyResult when that world is published.
- `NpcSkillReactionsV2` already borrows actual `CharacterWorldNpcStateOwnerV1`, injury, property/AI/animation/design tables, SAME timer services and Slow borrower. Composes force_state/state_event into real machine.transition/event, source table selection, timers, animator, pin/unpin/cancel-sneak. Its remaining heading/random/IsPlayer/StopLoop callbacks stay mandatory. It covers application, state body/update, timer43/44, and outer frame effects without a second clock.

Source factory agent `/root/integration_lead/source_character_owner_factory` received all these findings and retains whole canonical record lease. Coordinate pointer aliases through that lease.

## Exact source constructor/init evidence

- Character C1 `0x3aa1b4`: GameObject, Controllable, Inventory, CharTimers, CharAI, Animator, FSM, CharProperties constructors; later binds SAME Character into timer/AI/animator/FSM/properties, then RegisterState IDs0..19. Existing reference `character-retained-frame/direct-calls.json` and `character-idle-startup/reference/original-functions.asm` retain this.
- CharTimers C2 `0x3dbb0c`: owner0, empty vector, reserve20 via original reserve helper `0x3dba84`. Borrow actual constructor container; do not create another clock.
- CharAI C2 `0x3cebf0`: source timer IDs -1, aggros empty, original flags; mandatory global AI deque registration tail. Do not falsely treat scalar initialization alone as whole constructor success.
- FSM C2 `0x3c1b58`: nullable current, source scalar defaults/registry; actual registered-state factory remains original owner.
- CharProperties C2 `0x3df084`: empty source map, ResetAllProperties `0x3defc4` (base/saved/gear/cached), then `_ResetProperties0x3def34` on global temporary.
- CharAI OnInit `0x3d12b0`: IsDead; if alive Stop old33->CharacterDesign AI_Tick->StartTimer repeat-1/event0x33/ref0->Stop old34->DoT_Tick->StartTimer repeat-1/event0x34/ref0; then pending AIS Init even if dead. Recovered lifecycle kernel observes original reloads. No guessed duration/rate/dt scaling.

## Remaining root wiring and feature completion

Current main/PlayableActorWorld did not publish canonical status/combat owners during this checkpoint. Root must connect canonical record lease into SAME playable actor owner graph, actual timer expiry dispatch and original skill/application callbacks. CMake must add feature sources/native dependencies as needed; root alone owns CMake/main/world files.

Regular skill/melee leech stays original `dh2_character_skill_apply_result_v6` or `character_skill_apply_result_v116` via actual `SkillApplyActorV6` pointers. Never apply damage a second time from a presentation observer. Prefer existing canonical world combat provider with NpcRecurringEffects.

Peer requirements:

- animation_review: same actual FSM; no generic replacement state assignment. Its skill binding also needs this canonical graph.
- effects_feature: executor uses actual CharacterMeshFxOwnerV4; source Buff FX OID cannot be reinterpreted as set ID without producer proof. Keep source manager/instance identity.
- audio_feature: DOT sound uses canonical actual CharSounds/source combat sound selection, RNG/target position/Play3D gates. Convert authoritative CombatSoundPlayV1 through `prepare_3d` and `SourceAudioRouter.submit`; no guessed sound.

Report `reports/feature-status-live-integration.json` remains `feature_complete:false`. Feature done requires packaged active gameplay with positive timed DoT, stun/scare/slow and expiry, regular HP/MP leech, death/end cleanup through SAME original actor/property/FSM/timer/HitFor/FX/text/audio owners and no reached callback failures. Component test success does not satisfy live completion.
