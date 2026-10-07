# Same-world Kill production composition V23

Four new production TUs: `character_ais_kill_vm_v23.cpp`,
`character_kill_contributor_event_v23.cpp`, `character_kill_level_events_v23.cpp`,
`character_kill_production_v23.cpp`. They depend on the existing whole V21
Kill/fields/World owner, AI event kernel, ScriptOwner/ScriptOwnerV2, actual
CanonicalLevelContext, EventManager V12 and existing Lua callback capability.
No shared class layout or alternate HP/Save/World/quest queue is introduced.

`CharacterKillProductionV23` borrows the same World, PlayerManager and Kill
globals. Add each actual actor with `CharacterKillLiveBorrowV21` and a retained
`CharacterKillContributorEventV23`. The renderer supplement exposes
`renderer_character_kill_hit_v23`; include it after PlayerSkillsRuntime and
WorldActor declarations. It is intentionally not installed in a live Hit
service by this packet.

Fresh NPC metadata must retain exactly one `CharacterKillFieldsV21` produced
at the existing genuine creation boundary. `kill_metadata_borrow_v23` in
RetainedCharacterActor borrows the existing room64, properties13c8 and
ooi14a4 without replaying defaults. The player must use its real constructor
metadata and actual InitPre publication; Knight table identity is not a
substitute. Its existing sole ScriptOwnerV2 supplies the player OnKill gate.

Required services:

- `KillLevelProviderV23.current` reads the actual GSLevel slot and returns
  `CanonicalCurrentLevelBorrowV1`; weak lifetime pins provider storage only.
- `scope` returns the current actual skill VM capability, nullable outside
  callbacks. `refresh_contributor` rereads the same selected AI and controller
  forced/locked/global gates before event4.
- `died` delivers final event2 through the existing source NPC AI/FSM/death
  owner (renderer_kill_npc_raise_v21). Actual player death needs its own
  positive provider if reached. No global TargetDied broadcast is added;
  source target-update owns that notification and HUD invalidation.
- `remaining` dispatches genuine positive loot/XP/trophy/online/constants
  services. Combat V23 loot and HUD V23 progression are separate same-world
  owners. Do not publish Hit8 before these mandatory reached routes exist.

Source order remains frozen Kill/CtrlKill: dead/HP prefix, conditional loot,
contributor event4 and trophies, credited XP, immediate quest events, final
event2. Required failure retains source effects; no retry/dead reset is added.
Contributor event4 is CharAI.OnKill (3d0d80); default/monster/faery/external
inherit a literal return. Player/PlayerIPhone calls its actual OnKill alias
only with callback_flags400. Lua ordinary error preserves void-call parity;
native required-failure epochs remain errors.

Verification: original ARM OnKill flags0/1/400/ffffffff and IEvent.field4
oracle; native contributor PASS380 with real commons and same busy VM;
native actual Level C1/51rows/Lua/Save PASS465, genuine gate150 zero and four
actual QE constants. The test listener is a declared typed observer, not
quest progress acceptance. Global changes inside the first immediate listener
do not redirect remaining events away from their captured source Level.
All four new TUs pass strict ARM64 syntax. Renderer supplement/live death,
loot, XP and visual completion are not yet accepted by these tests.
