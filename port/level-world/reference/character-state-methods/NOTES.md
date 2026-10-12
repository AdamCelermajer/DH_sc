# Remaining state method inventory and proven empty bodies

The original ELF is SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`inventory.json` binds all64 Focus/Blur/Update/OnEvent methods for the remaining
sixteen families to the actual source factory/vtable registration plan, raw
symbol size/hash, complete assembly and static call dependencies. Dependency
lists are a static inventory; they do not claim every branch executes or supply
dynamic callback ordering. PIC literal words at function ends remain in the
capture and are not interpreted as genuine behavior methods.

Exactly18 methods consist of the single ARM word `0xe12fff1e` (`bx lr`):

- Update: Limbus0, Spawn1, Despawn2, Skill6, Cast7, KnockedBack10, Injured11,
  Interact13, Reviving15, Revived16, PreSpawn17.
- OnEvent: Limbus0, Despawn2, Cast7, Stunned9, Injured11, Reviving15, Revived16.

No Focus/Blur body among these families is empty. A short body which tailcalls
another method is **not** classified as empty: Anim14's Update/Event can call
SM_SetIdleState; LiftingIdle18 Update tailcalls common Idle Update. Stunned9 and
Scared8 nonempty methods already have separately reconstructed native_effects
kernels, whose service backends still must be supplied. The new catalog does
not silently duplicate or replace those kernels.

`character_state_empty.hpp/.cpp` adds
`dh2_character_state_empty_body(state,operation,source_function)`. Its independent
operation enum is Focus0,Blur1,Update2,Event3; explicitly translate the existing
owner operation enum, which uses a different numbering. Return1 completes only
an exact source-proven empty method;0 identifies the correctly supplied nonempty
method and requires its backend;-1 rejects invalid/mismatched metadata. It does
not mutate a Character, invoke services or claim any nonempty successful effect.
The production source includes the new, hash-bound `native-methods.inc` catalog.
Existing frozen owner/behavior/frame files remain unchanged; root owns adapter
integration and must preserve required failures for other methods.

`character-state-empty-arm64-differential.json` executes all18 actual ARM32
empty bodies with four different receiver/argument projections (72 calls),
verifies data memory/receiver register preservation, and executes optimized
native metadata dispatch for all64 methods plus68 bad-input checks. Correct
nonempty records return0 and are not emulated as successful bodies. The separate
sanitized host proof repeats64 metadata cases and130 rejection checks, SAN0.
Gold `empty-fixtures.bin` uses SEM1 magic. No APK/full-game proof is claimed.

## Startup priorities

Source ID17 is **PreSpawn**, while ID1 is Spawn. Level preset0 or17 actually
selects PreSpawn17. Implementing only empty Update17 does not implement startup:

|State|Focus|Blur|Update|OnEvent|
|---|---|---|---|---|
|Limbus0|0x3c2e58 /292B|0x3c2be4 /344B|0x3bffe4 /4B empty|0x3bffe8 /4B empty|
|Spawn1|0x3c35ec /424B|0x3c2f7c /164B|0x3bfff0 /4B empty|0x3c0b04 /76B|
|PreSpawn17|0x3c688c /404B|0x3c67d4 /184B|0x3c0070 /4B empty|0x3c2810 /312B|

PreSpawn Focus writes flags0x1300. It queries the actual animation table index
and row+0x64; a present clip is ANIM_Set directly. A missing clip selects row+0x80
with stance-mask1, then SetSpeed(+0). It destroys the physical binding through
SetPhysicalObject(NULL,false), may disable the object's virtual enable flag
(v40) depending on Character+0x13e4, then disables collisions. Blur enables via
v40, calls Revive(NULL,false), then EnableCollisions. Those deeper operations
remain mandatory genuine providers. OnEvent0x28 compares the actual event string
and may set flags|0x2000 before InitPhysicalObject. OnEvent9 invokes the source
Spawn predicate and SetSpawnState, then can clear a CharAI-related pointed field.
The source assertion branches are substantive; do not invent accepted targets.

Limbus Focus writes flags0, disables via v40, optionally queries respawn delay
and actual online/local-host policy before starting event0x2f, then clears all
aggro. Blur restores source position/rotation, revives, and walks other objects'
Limbus/visibility policy. These nonempty bodies require actual world/list and
Character services. PreSpawn Focus/Blur/Event is the next bounded priority;
complete Limbus Blur has a larger world-owner service boundary.

The exact original Level versus InitScriptProcess phase remains conditional:
Level._LoadCharStates is called at0x3f7700; Character.InitPost can call
InitScriptProcess0x3ce7c0 at0x3b54c8. Do not infer all initial Focus notifications
occur before active AIS publication. Source OnStateChanged reads activeAIS+1c
live; absent skips, present calls virtual+20. Earlier ownership notes retain the
exact phase evidence; pending startup recovery must preserve this source gate.

The complete table below is generated from the captured source catalog.

|ID /class|Focus address /bytes|Blur address /bytes|Update address /bytes|OnEvent address /bytes|
|---|---|---|---|---|
|0 /CSLimbus|0x3c2e58 /292|0x3c2be4 /344|0x3bffe4 /4 empty|0x3bffe8 /4 empty|
|1 /CSSpawn|0x3c35ec /424|0x3c2f7c /164|0x3bfff0 /4 empty|0x3c0b04 /76|
|2 /CSDespawn|0x3c32fc /236|0x3c3794 /416|0x3bfff8 /4 empty|0x3bfffc /4 empty|
|6 /CSSkill|0x3c4480 /324|0x3c434c /308|0x3c0018 /4 empty|0x3c0ac8 /60|
|7 /CSCast|0x3c39d0 /212|0x3c3934 /156|0x3c0020 /4 empty|0x3c0024 /4 empty|
|8 /CSScared|0x3c45c4 /452|0x3c4834 /172|0x3c4788 /172|0x3c2b28 /188|
|9 /CSStunned|0x3c3cc0 /384|0x3c3b4c /172|0x3c549c /172|0x3c0030 /4 empty|
|10 /CSKnockedBack|0x3c4a48 /348|0x3c48e0 /188|0x3c0038 /4 empty|0x3c5ab0 /140|
|11 /CSInjured|0x3c33e8 /228|0x3c4ba4 /152|0x3c0040 /4 empty|0x3c0044 /4 empty|
|13 /CSInteract|0x3c5548 /316|0x3c4f8c /296|0x3c0080 /4 empty|0x3c1558 /168|
|14 /CSAnim|0x3c324c /176|0x3c2dd0 /136|0x3c1a3c /40|0x3c1a14 /40|
|15 /CSReviving|0x3c5294 /344|0x3c6724 /176|0x3c0058 /4 empty|0x3c005c /4 empty|
|16 /CSRevived|0x3c34cc /288|0x3c53ec /176|0x3c0064 /4 empty|0x3c0068 /4 empty|
|17 /CSPreSpawn|0x3c688c /404|0x3c67d4 /184|0x3c0070 /4 empty|0x3c2810 /312|
|18 /CSLiftingIdle|0x3c3140 /268|0x3c50b4 /228|0x3c0e90 /16|0x3c6608 /52|
|19 /CSLiftingMove|0x3c3e40 /308|0x3c5198 /252|0x3c0ea0 /120|0x3c663c /52|
