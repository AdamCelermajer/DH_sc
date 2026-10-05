# Actual attack query and renderer binding

`CharacterWorldAttackGeometryV1` composes the existing source geometry kernels
with the SAME registered World handles, live cached properties, actual target
position selector, selected Gear inventory and authored ItemTable/AIProps rows.
Its readonly scalar copies are temporary query projections; no player, life,
target, properties, Gear, Save or RNG authority is created.

The owner query's property32 projectile shortcut reads no inventory. Otherwise
the real selected main-hand reference/item ID selects all41 source Item words.
The melee parameter is Item Param5 plus the actual AI row melee radius; invalid
AI IDs select authored row8. Character/type0/interaction8 targets use strict
3D squared melee distance; range uses inclusive signed integer-square bounds.
Both source GetTargetPosition branches use exact registered node/enabled/cache
fields. The original handle AsCharacter executes before the kind/interaction
branches; other target domains require AI_IsInInteractionRange explicitly.

Source melee distance loads DebugSwitches and queries `IsTracingCharAITarget`.
Its true branch also loads/queries `isTracingCharAITarget` with the source case
distinction. Original literals are at8c56e0/8c56f8, calculated from3d62f8 plus
literal3d63d0 and3d6398 plus literal3d63d4 respectively. The diagnostic string
destructor has no gameplay effect. Source attack caller diagnostic queries are
`isTracingCharAICommands` at8c5498 for return sites3d0294/3d03c0, and
`isTracingChar_MeleePotentialTarget` at8c54c8 for sites3d0354/3d0630/3d06b4/3d0538.
No tracing value is fabricated.

The attack owner's Search intercepts only the reference melee-radius service,
using this exact inventory-plus-AI radius. Remaining source registry/filter,
interaction radius, angle, targetability, faction, sort and heap services stay
with the same registered World owner. Heading-active keeps closest sort; the
source unheaded reset uses sort0 and does not choose a nearest actor.

NPC constructor-only inventory query backing is explicit:
ItemInventory C1 at3ff26c writes selected byte+2e=0. The loops3ff288..304 create
two sets, each with nine null reference cells. This query owner can represent
that genuine constructor period only; an actual source mutation must replace
its borrow with the resulting inventory. It does not claim NPC loot/equipment
initialization, and it cannot silently replace a missing inventory with empty.

`renderer_player_attack_v1.inc` and its handoff document supply concrete retained
owner construction and the actual command wrapper. The wrapper maps an explicit
inspected object index to its existing identity; null input invokes source
search. Source SetTarget/BackupTarget, controller gates, Debug, frontal constant,
online mode and same-FSM c354 dispatch are bound. Range redirect, online packet
queue and non-character interaction continuation remain explicit required
backends. An accepted source command is distinct from FSM acceptance and damage.

Existing geometry kernels have a separately preserved9,393-case actual original
ARM/ARM64 differential proof. New adapter O1/O2 ASAN+UBSAN checks exercise same
World/property/handle identities, constructor inventory, strict boundary30,
cached-node selection, inclusive range boundaries10/20, unused-inventory
shortcut, required missing inventory and identity mismatch. The interaction
fallback test is a named fixture. This is not a new whole original-call
differential or production renderer receipt. Strict NDK syntax passes bothABIs.

Root's independently recovered targetability constructor is CharAI nested
byte4d=1 (Character415); no forced visibility/interaction flags are added here.
Root also owns the complete attack animator begin/end producers and the sole
AttackState64 backing. This task did not edit model_renderer.cpp, CMake, Java,
APK, frozen trophy/execution sources or deployment state.
