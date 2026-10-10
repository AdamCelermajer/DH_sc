# Companion handoff

Current worker stopped at lead's explicit model-policy handoff. Successor should
use GPT-6 Luna HIGH. No commits or root main/CMake edits by this worker.

## Owned files and verified results

All code is in `port/windows-foundation/features/companions/`.

- `source_companions.hpp/cpp`: source follower/attacking_follower Lua event bodies,
  actual priest `rene` CatchUp/CaughtUp/master/target/OnUpdate bodies, native
  AISFaery follow/skin update service sequence. `source_companions_tests.cpp`
  native MinGW compile/run passed; `source_evidence_tests.py` passed actual cache
  script byte identities and SHA256, WanderingPriest419->AI50->script rene, native
  faery close-range vtable124->melee 0x3de318 and native200 offset.
- `source_master.hpp/cpp`: whole original CharAI::AI_SetMaster0x3d4d80 recovered
  from IDA. FIRST stores actual native Character identity in same master50.
  Null returns without touching alive54/sight55. Nonnull queries actual owner
  GetCharAIId, master IsDead, master/owner GetTargetPosition, actual selected row
  ViewRadius and strict squared3D comparison; updates alive54=value^1 and
  sight55. Source HasMaster/_IsMasterHostPlayer/_SetMaster wrappers preserve
  nil-master host short circuit and absent/non-object argument no-op.
- `source_master_natives.hpp/cpp`: genuine existing CharacterScriptSessionInput
  gameplay_binding selector handles ONLY original HasMaster3b6f3c,
  IsMasterHostPlayer3b6fc4, SetMaster3b9084. Caller leases actual source receiver;
  callbacks borrow fresh same fields, missing producers raise required-service
  Lua failure. Original commands/FSM/design/properties/host/timers remain with
  existing Session. `source_master_tests.cpp` compile/run passed after adding
  direct native callback tests (boolean outputs, SetMaster clear, missing host,
  strict radius boundary, prefix retention).
- `live_companions.hpp/cpp`: live same-world predicate bridge and original own
  target-node/cache/position selector. IMPORTANT changed LiveActorOwner from
  semantic ActorId* master to `CharacterAiPointerFieldsV105* ai_fields`; master50
  contains native Character identity, not world ActorId. Added required
  native_is_dead and actor_from_identity producers. `borrow_source_master`
  projects those actual fields and maps actual native identities to same world
  actor for source target position. SetMaster now invokes recovered source body
  directly; other commands remain required whole source_operation.
- `live_companions_tests.cpp`: native same actual PlayableActorWorld assertions
  passed after API correction, target-node cache/disabled branch/path count,
  master-host identity, required commands, no-op target publication rejection,
  detached actor rejection. Fixtures use explicit1HP, no gameplay defaults.
- `live_companion_scripts.cpp`: optional raw ScriptOwnerV2 integration. Actual
  pending/active VM/constructor identity/alias checks, source-object calls, native
  binding contexts lease through lua_close. SetMaster callback uses original
  raw source values + recovered master body. It compiles native object. Full link
  still requires real ScriptOwnerV2 + dh2_script_runtime; no stubs created.
- `source_owner_contract.hpp/cpp`: SourceOwnerBorrow directly projects actual
  retained RuntimeState.path/CharAI fields/lease; no allocation. Borrow same
  PopulationActor.visual retained_scene_borrow. Both contract and
  `source_owner_scripts.cpp` compile native objects, but no runtime Scene test
  yet. Source_owner_scripts advances SAME ScriptOwnerV2 owner with authored
  companion ScriptCreationFacts and verifies published AISExternal, and checks
  15 exact native callback source addresses before registration.
- `LIVE-INTEGRATION.md`: initial integration docs; its ActorId-master wording and
  ScriptOwnerV2-only sections need update to latest identity-field contract.

Existing `reports/feature-companions.json` describes initial source components
and runtime gaps; root owns report updates in reactivated phase. Live additions
not yet reflected there.

## Commands used successfully

Native compiler `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`.
Add its bin directory to PATH before running MinGW executables.

Source-master executable: compile source_master.cpp + source_master_natives.cpp
+ source_master_tests.cpp + port/game-data/ai.cpp + data.cpp. Output
`.local-inputs/windows-toolchain/companion-source-master-tests.exe` passed.

Live-world executable: compile live_companions.cpp + source_master.cpp +
live_companions_tests.cpp, link current `.local-inputs/windows-foundation-build/`
libfoundation_data.a, librecovered_content.a, libcontent_xml.a,
libdh2_freetype237.a. Output companions-live-tests.exe passed.

## Exact next bounded dependency

1. Root's new actual source alias seam is
   `features/actor_frame/source_character_owner_factory.hpp`:
   SourceCharacterOwnerAliases holds SAME CanonicalCharacterCandidateRecordV60,
   RetainedCharacterActorV1, properties/property_view/life/FSM/visual etc. Use
   record's actor `source_ai_pointers_v105()`, `runtime.path`, `session` to project
   actual companion owner; do NOT create a new Character, VM, path or master.
2. IMPORTANT discovered just before handoff: canonical retained NPC uses
   `RetainedCharacterActorV1::session` of type **CharacterScriptSession**, whose
   owner is legacy **ScriptOwner**, NOT V2. Current live API raw script_owner
   pointer supports V2 only. Extend callback loan to support this existing
   legacy Session/owner (prefer a templated same-session loan like enemy helper),
   never construct V2 beside it. Existing owner supports active/pending/
   call_discard/alias services. Actual ScriptOwnerV2 factory rejects script_faery
   native selection; native faery stays with its actual AIS native owner.
3. Enemy worker added `features/enemy_ai/authored_monster_session.hpp` template
   callback loan and `external_monster_natives.hpp` exact-address DoSkill3b8bd8
   factory. Existing CharacterScriptSession[V3] already owns source GetState/
   GetStateTime/GetPosition/GetProp/GetPyStruct/GetPyOID/FromFixed/SetLevel,
   command wrappers with ScriptCommandBindings and commands_refresh, timers.
   Prefer chaining `select_source_master_native` in gameplay_binding; do NOT
   override these globals with raw live_invoke presentation GetState etc.
4. Current `project_source_owner` requires runtime.object.user==native identity;
   actual freshly constructed PF has user0 until InitPF. Handle legitimate
   constructor-prefix borrowing only with explicit actual lifecycle phase proof,
   not an implicit0 fallback. Completed owner requires exact native identity.
5. Root added CharacterVisual::retained_scene_borrow() (same loaded impl Scene,
   expires reload/move/destruction). `borrow_companion_scene` uses actual
   population visual. Add native real-loaded-scene test proving target_node token
   belongs to that exact Scene graph and cache uses same source node; current
   tests cover native source getter semantics but not this new loaded Scene seam.

## Coordination and limitations

Lead: /root/integration_lead, root owns main and CMake. Enemy worker:
/root/integration_lead/enemy_ai_live_integration knows gameplay native factory
chain. Effects worker identified faery glow is embedded faeries_02_celeste.bdae
rigid _mesh_glow_nobatch-node_PIVOT / GL_PCloud nodes / bip_faerie_GLOW; NOT an
EffectsTables FaeryGlow row. Supply actual same retained Scene/source animation
ms; do not load second fairy scene or invent FX selection. Interaction worker
reports NPC Character::Interact3a4d78 whole native dialogue owner missing;
TalkToNPC event then SM_SetInteractState and RaiseEvent5 are required, no
priest-specific activation fabricated.

No live root priest follow claimed: retained actual master/path/ScriptOwner
borrow and complete source controller/skill/timer factories remain required.
No named map, guessed distance, fake haste animation, timer store or private
companion AI globals. Source master alive/sight mutation prefixes are preserved.
