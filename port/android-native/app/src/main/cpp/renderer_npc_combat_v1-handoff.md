# Same NPC attack/Walk/frame integration — stable handoff

No model_renderer.cpp, CMake, main animation include or APK modifications were
made in this lane. New renderer includes compile against the complete renderer
for both arm64-v8a and x86_64. Do not interpret syntax checks as live gameplay.

## Owners and includes

Retain on the actual MonsterScriptHandle, with destruction before its borrowed
Session/body/World services disappear:

1. `RendererNpcInventoryV1`: SAME `LootTablesV2::Borrow` retained by player
equipment/cache owner + `equipment_random` + `m.object->properties`.
2. `RendererNpcAttackCommandV1`: SAME World targets/geometry/StateOwner/controller
and m.animation_ai. Supply actual retained OOI14a4/networkbyte+a fields.
3. `RendererNpcStateAnimationV1`: SAME NPC command facade/native body/timer
services. See dedicated state-animation handoff for dispatch ordering.
4. `NpcAnimationEventOwnerV1`: SAME legacy Session owner, m.ai_events, actual
playback `last_event_lag`, Session property view, shared World, actual NPC
visual/source root/Scene and cached PFFloor type. Borrow the real shared FX
manager; no new scene, script session, Lua VM or fake footprint coordinates.
5. `RendererNpcMeleeEventV1`: SAME retained NPC inventory and shared execution,
CF context, combat RNG/geometry. Supply the preceding event owner and actual
FX manager; remaining object-animation/sound/projectile/skill/spell/interaction
callbacks stay mandatory if their authored branches are reached.

Add headers `npc_attack_command_owner_v1.hpp`, `npc_actor_frame_v1.hpp`,
`npc_inventory_owner_v1.hpp`, `npc_animation_event_owner_v1.hpp` and existing
`character_melee_animation_event_v1.hpp`. Include attack/state/inventory/melee
adapters after complete PlayerSkillsRuntime/shared execution and NPC commands.
Only new compiled TU is `npc_animation_event_owner_v1.cpp` (attack/frame TUs
previously handed off); inventory constructor is inline over existing V4.

## Same inventory replacement

Replace `attack_npc_inventories` query-only CharacterAttackEmptyInventoryV1
objects with the actual retained NPC inventory owners. Geometry inventory
callback returns `npc_inventory.attack_borrow()`. SkillAttackActorV6 / native
combat actor borrowing returns the same `&npc_inventory.owner.inventory()`;
actual CombatantView categories are both-1 for the source constructor's empty
hands, established by those actual null slot queries. Preserve subsequent source
equipment mutations; never recreate/reset this inventory per frame/reload.
The SAME table borrow must come from the existing private snapshot. A narrow
const `FreshInventoryOwnedV4::tables_borrow()` accessor was requested from root;
this lane did not edit shared inventory headers.

ItemInventory C1 3ff200: cap2c=-1, unlimited2f=0, selected2e=0, gold24=0,
goldlimit28=INTMAX, two sets of nine null references. Constructing V4 with cap-1
reproduces those fields without loot/effect calls or consuming RNG.

## Calls

NPC ScriptCommand attack callback invokes retained attack command with the
genuine current callback scope. This source wrapper routes CmdAttack→CharAI
DoMeleeAttack→SM_OnEvent(c354), not a forced transition or player substitute.

Refresh state facts before source FSM dispatch/update. State body callback
delegates states3/4/5 to retained state-animation adapter; preserve root death
and existing injury/reaction owners. Animation service intercepts source helpers
StepBegin3d4204 / StepEnd3d3ff8 through `ai_helper` before old restricted branch.
Keep actual animation observer; it provides source authored28 and finite22.

In existing source named-event helper3d4434, use retained NPC melee `named(text)`
instead of the old restricted parser. Source `ev_` invokes actual legacy Lua
and foot/floor FX; `fx_` uses actual FX manager and current NPC position;
attack_mainhand/offhand invokes actual selected AIS+a8 / native melee / shared
Apply. Unknown endpoints never return successful no-op results.

After real Scene/FSM/Animator, use `npc_actor_frame_v1` actor phase. Supply actual
NPC runtime, position, raw props, body, visual, scene, same floor registry,
motion policy/workspace and actual target-node absolute-position producer.
Source constructor auxiliary2e0=0 must live with the retained GameObject;
no default value is inferred from absence of renderer effects. Suspend/rebind
command navigation before retiring floor/registry; initialize PF after reset
and actual source bounds are produced. Publish reached position to the same
scene actor, script object and Session even on prefix failure.

## Source identities and limits

Character+408 is embedded CharAI(+3c8)+40=current target. Original melee header
comment claiming distinct pointers is incorrect; actual implementation's two
borrows must alias this authoritative target, as current player binding does.
Actual OOI14a4=0 and signed14a8=-1 are constructor fields, produced once;
AI74/78/79/7a are not reset by this adapter.

NPC Default PreAttack3dbef8 is accepted only from real selected slot+a4 after
full actual AI_CanAttack. OnAttack3dc12c is accepted through the existing World
Melee owner only after real selected slot+a8. OnAnimEvent3dca50 is verified
from slot94, same legacy ScriptOwner identity and private scope/VM. No AIS
function is selected from monster type or guessed from a label.

Root owns lethal Kill8, source aggro/death/loot/XP. NPC→player Hit/Apply tails
must borrow existing player body/FSM services; their absence remains a source
boundary. No direct subtraction of HP exists in these adapters. Positive
authored attack damage/rendered NPC movement still requires live integration
and testing; not established by the bounded fixtures below.

## Verified fixtures

`reports/android-native-owner-tests/npc-inventory-owner-v1/receipt.json` PASS:
actual1322 ItemTable lease, same NPC props/identities, two9nullsets/selected0,
shared RNG unchanged, no player Gear, invalid required lease rejects.

`reports/android-native-owner-tests/npc-animation-event-owner-v1/receipt.json`
PASS: actual13535 commons/private legacy Session; fresh aliases/event/negative
lag; source-selected virtual94, scoped recursive calls, expired capability and
unknownsource rejection. Registration/cache interfaces are explicit fixtures.
Positive footprint graph/FX is NOT covered; genuine absentWorld reached-prefix
failure is tested. New helper TU was compiled into the isolated executable and
linked with current APK libraries, not yet integrated into the APK.

`reports/android-native-owner-tests/npc-attack-actor-frame-v1/receipt.json` PASS:
real Crypt BRES/DWLD/PF with explicitly labelled AI/geometry/body fixtures.
This does not claim actual CPU root motion or production source adapter runtime.
