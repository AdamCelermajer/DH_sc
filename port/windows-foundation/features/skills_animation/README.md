# Source skill animation development adapter

`SourceSkillAnimation` borrows the actual `SkillAIContextV3`, `SkillStateV4`,
actor skill slots and pinned `SkillTables::Borrow`. The host supplies its existing
slot-to-global-row resolver and genuine native services. Every host object and
callback context must outlive the adapter. The adapter creates no FSM, target
registry, property sheet, HP/mana store, VM, timer or animation clock.

`declarations(tables, skillListName, output, error)` exposes authored class/list
membership and scalar metadata for UI. These declarations do not imply the
actor has unlocked a row. Saved/native skill slots remain the slot authority.

`command(operation, slot, answer)` executes the entire original skill AI body.
Use the constants in `character_skill_ai_v3.hpp`: Usable for admission, Begin
or Use for the corresponding original command, End for release, and
Focus/Event/Blur for genuine AI callbacks. Status 0 may return answer 0 as an
ordinary denial. Status -2 preserves effects already delivered by original code.
Do not retry after a partial failure as if nothing happened.

The row and SetSkillState services are bridged to the same skill table and
borrowed state. The host's state-event/transition services must execute the real
FSM admission, blur/focus ordering and notifications. `state_operation` exposes
the recovered CSSkill focus, blur, event and selection entries without creating
a transition owner. Debug, animation, speed, body, timer, monster classification,
constant/stance and heading/sneaking providers remain explicit.

During source ANIM_Set processing, before the override is consumed, call
`animation_start(tables, borrowedRandom, start, error)` to resolve its source
three-layer sequence selection. `start.step` carries actual FX, camera, sound,
MoveGO, speed and blend metadata. Pass the genuine sequence and metadata into
the existing animation owner; this helper selects only the initial leaf and does
not implement subsequent phase completion or type2 reselection. It is not an
ordinary visual clip-select shortcut.

Call `authored_event(name)` synchronously where the original animation callback
fires. Existing playback owns timestamps, occurrence identity and deduplication.
Exact `do_skill` in source state6 routes to the same AI Use callback; fx_/sfx_/an_
and ev_ require their original host dispatch. Source Cast state7 goes to the
required other-state provider; this adapter does not implement spell casting.
Avoid routing an event through both this adapter and an existing observer that
already calls the same Skill Use service.

Native tests read original Skill/SkillList and animation tables and three real
BRES clips. They verify class membership, native denial without selection,
stance arithmetic, raw moving bytes, authored marker routing, foreign-owner
rejection and missing service failure. Callback fixtures test the bridge;
they do not prove real Lua mana/target/damage/FX delivery or live integration.
The original Skill table has no Type1/Type2 rows in this data bank; native support
for those branches is reused without an authored-content execution claim.

Build dependencies: this cpp, `character_skill_ai_v3.cpp`,
`character_skill_state_v4.cpp`, `skill_tables.cpp`, `animation_selection.cpp`.
Test dependencies additionally include `animation_tables.cpp`, `data.cpp`,
`animation_markers.cpp`, `resources.cpp`, `event_track.cpp`, `events.cpp`.
The test defaults to `.local-inputs/windows-shared-assets/original-cache/data/pydata`
and uses `.local-inputs/actors/animations_dictionary_pyarraynames.bin` plus the
original three clips under `port/level-world/reference/character-visual-v6/cache`.

Development only: the live v15 runtime still needs one same-owner skill VM,
true FSM focus services, source mana/targets/effects, interruption/closure and
save/hotbar binding. Full gameplay integration is not claimed.

The subsequent live bridge is `session_skill_animation.hpp`. It validates fresh
same-actor/property/AI/SkillState loans, maps saved hotbar positions through actual
native indices and intercepts source ANIM_Set into an external complete source
program callback. `skill_animation_program.hpp` compiles direct AnimTable roots
and preserves redirected hierarchy and actual step FX/camera/sound/swoosh data.
Root must load its appended clip bank before Session owns the visual.
`CombatSession::play_actor_source_sequence` now accepts the external plan,
policies, selection and callbacks through the existing retained slots. Bind
`SessionSkillServices::play` to that API; clip aliases must match the bank actually
loaded for this actor. The API preserves the actor's pose owner and sole session
clock; it does not supply the native skill/FSM/Lua owners or admit skill gameplay.

`skill_animation_program_tests.cpp` now passes using the staged original three
clips and one retained Prince body: idle→BashDown→ColdRay→JumpKick, actual
completion and one marker per clip, same animation owner throughout. It is not
a proof of live Session/Skill VM registration. The original clip staging manifest
and root receipt are in this directory.

`native_skill_lua_services.hpp` provides V3/V6 and canonical V1 owner composition.
These services compile against existing actual callback/Session APIs. See
`native_initialization_contract.md` for exact owner versions, initialization,
scoped same-VM callback transport and remaining native providers. Never create a
parallel V6 owner for CanonicalNpcSkillsV84's V1 graph.
