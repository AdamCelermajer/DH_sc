# Skills integration handoff

All feature files are under `port/windows-foundation/features/skills_animation`.
Parent/root owns main, Session core, CMake and shared assets. This worker stopped
edits at lead request for a Luna successor. No full integrated skills claim.

## Implemented

- `source_skill_animation.*`: pinned original SkillTables declarations and full
  recovered SkillAI/CSSkill admission/select/event adapter, actual actor slots and
  native fields borrowed. Marker do_skill→skill_ai_event_v3 (current Use).
- `skill_animation_program.*`: complete direct AnimTable root→source visual plan,
  redirected hierarchy, policies, clip bank, actual AnimationStep metadata. No
  class/map dispatch hardcoding. Root supplies resolved IDs and same visual config.
- `session_skill_animation.*`: typed live bridge with fresh same Session actor,
  PropertyView, actual SkillAIContext/SkillState borrow; saved hotbar→class position
  →native instance mapping; ANIM_Set→required external Session program callback;
  marker and actual whole-finished callbacks. Compiles; core seam is still pending.
- `native_skill_lua_services.*`: actual V3/V6 Session/SkillOwner helper, recovered
  Check/Pre/Use/Post callback transport, native state queries/rows/properties and
  source load-step/flags refresh. Factory for Lua DoSkill/Begin/End uses required
  source conversions; prefers enemy lane's scoped DoSkill for actual nested calls.
- `native_skill_lua_services_v1.cpp`: canonical V1 Session/CharacterSkillOwner
  equivalent, exact uncached property28/fallback3 row resolution, existing timers.
  No parallel V6 owner for NPC. Both helpers expose `set_callback_transport` for
  scoped original indexed callback transport.
- `native_initialization_contract.md`: exact dependencies and missing providers.

## Assets and evidence

`original_skill_clip_manifest.json` and root/lead-written
`original_skill_clip_staging_receipt.json` contain exact hashes/source/destinations.
Lead staged three originals into shared original-cache paths and animations
aliases; accepted packages remained untouched.

Actual class position0 is BashDown in Knight/Berserker/Paladin, ColdRay in
Mage/Illusionist/Necromancer and JumpKick in Rogue/Archer/Assassin. Global rows are
7/14/56; AnimTable roots347/413/521. These are evidence, not runtime routing.
Source steps speed1.3, BlendOut100, MoveGOtrue; FX164/150/160 respectively.
Original do_skill dispatch700/367/734ms; authored700/366/733ms.

Tests passed:

- `.local-inputs/source_skill_animation_tests.exe`: actual nine-list memberships,
  original sequence/clip/BRES data, denial without selection, stance/moving bytes,
  marker Use/current state, missing provider and foreign owner rejection.
- `.local-inputs/skill_animation_program_tests.exe`: real Prince4-controller body
  and one SAME RetainedSequencePlayback owner across idle→all3 skill clips,
  actual completion and exactly one source marker each, atomic invalidroot reject.

Native Lua service cpp files compile `-std=c++17 -O1 -Wall -Wextra`. No full Lua
gameplay test has run through a live Windows Session owner. An inherited header
warning in actor_combat_runtime.hpp about missing source_clock initializer is
outside ownership; no owned compilation warnings in native Lua helper files.

Program test compile links feature program cpp/test, game-data/animation_tables
and existing static group foundation_data/recovered_content/content_xml/
recovered_trigger_contacts/dh2_freetype237/physics-backend dh2_box2d_201. Source
adapter test directly compiles skill_tables, animation_tables/selection/data,
SkillAI/CSSkill kernels, markers/resources/event_track/events; current old
foundation_data lacks SkillTables symbols. New Lua services require actual
Session/SkillOwner/callback/runtime dependencies; root must add source build
dependencies, not stub unresolved native methods.

## Root core seam pending

Proposed `CombatSession::play_actor_source_sequence(ActorId,externalPlan,
externalPolicies,selection,stateAnimationServices,error)` reuses existing
Entry.retained.prepare_preserving and Session.update as sole advancement. Keep
the default melee plan for later genuine attack reprepare. External skills are
direct AnimTable roots absent from ordinary CharAnimTable state lookup.

`borrow_actor_skill(ActorId,SkillActorBorrow&,error)` must return genuine registered
same actor, mutable PropertyView, actual SkillAIContext/SkillState; absent native
owner explicitly fails. Current Session inhouse ActorState/Entry is not a native
FSM or canonical Character authority. Do not manufacture one in a feature.

Existing CanonicalNpcSkillsV84 uses CharacterScriptSession V1 + CharacterSkillOwner
V1 and exposes `source_ai_fields_v115()` only when original ctor fields exist.
Borrow its existing slots and native machine. CharacterPlayerSkillsV6 owns its
actual V3 Session/V6 instance graph privately and requires __player__ selection;
any loan exposes that same graph, never a new V6 instance. Input shared properties,
combat and temp sheets must stay the same source authority across Lua/mana/buffs/
inventory/damage, not a snapshot of presentation stats.

## Important source correction

Older CharacterSkillContextV4 incorrectly forwards skill_state_use_v4 to
skill_ai_use_v3 index0 (Begin+End). Authoritative IDA _OnAnimEvent3d4434 state6
do_skill→_SkillEvent3d8bf8→skill_ai_event_v3/current Use. Our new adapters are
correct. Existing CharacterPlayerSkillsV6.native_skill_animation_event also uses
the genuine event route. Do not use the older marker wrapper for live path.

Lua DoSkill3b8bd8→AI_UseSkill3d8868. Controller press Begin3ad86c, release End3ad85c
have different source routes. Source cast7/do_spell remains explicit other-state
provider, not Skill Use.

## Coordinated lanes and next bounded tasks

1. Enemy AI worker has exact Lua DoSkill wrapper, signed conversion assembly proof
   and is implementing `scoped_skill_callback.cpp` using
   dh2_script_callback_call_indexed_source_v112. Bind through
   NativeSkillLuaServicesV1.set_callback_transport; transport validates genuine
   same-call capability. Default unscoped callback rejects busy VM. Lead identified
   unsafe cross-owner/scoped architecture as possible medium escalation; do not
   duplicate their implementation.
2. Canonical source owner factory worker prepares genuine candidate actor/machine/
   player_script_owner publication. Share actual source constructor-backed slots,
   fields, PropertyView and native machine loans with them.
3. After root external-program/borrow seams exist, add an actual Session integration
   test: original classposition0 press admitted via same owner, source focus executes
   Pre/mana/targets, same retained slot/event reaches current Use once, whole finish
   through real FSM/Blur/Post and Idle; refusal/missing services must not basicattack.
4. Wire UI saved hotbar positions→class positions→actual owner AI indices; skill UI
   Services already uses class positions and actual progression truth. Never map
   global SkillTable row IDs directly to hotbar/native index.
5. Native required services: actual FSM50005/6 transition ordering, raises30/31→
   Skill Pre/Post, debug, real physical2dc/target late reload (runtime_bound_v48),
   source stance/speed/StopLoop, mana application/debug/PropertyView, original Skill
   info/level/CalcManaCost, room target lists/range/angle/LookAt, damage/RNG/buffs/FX,
   source flags, player trophy/network/difficulty, timer expiry, save/rebind. These
   remain unbound until root publishes genuine actor/backend providers.

Feature code currently has a hard dependency on genuine providers, deliberately
no guessed mana, target, cooldown, source state, private FSM or basicattack fallback.
