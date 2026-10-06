# Character panel / same-graph faery V29

The original portrait input already routes to the original character-panel
movie, which owns the original stats, inventory, skills and faery artwork and
controls. Its startup failed because `NativeHUDSetActiveFaery` had no same-graph
faery action owner. The production query supplement now binds the V8 owner
alongside the existing same-player mutation/query owners.

The player's sole missing CharAI faery field is retained by its World and
created once at the fresh CharAI/V6 construction boundary. Its source C1 value
is NULL, and GL restore/menu opens preserve it. The same source GS global is
queried separately from the demo C1. For the current Crypt transport that
global is genuinely NULL, so the original ChangeFaery path updates the same
Save and same skill owner, then takes its actual NULL actor/current-Level
branches. No fairy spawn, unlock, synthetic current Level, phase38 or player
count store has been added.

Positive fairy model/visual/animation and complete Level placement endpoints
remain required if a genuine associated actor/current GS Level is published.
The renderer's demo object list is not substituted for a source Level list.
The existing panel continues to use its actual authored SWF, hit graph,
inventory avatar/Gear and synchronous same-player stat/skill/item actions.
Actual Save/class/profile producers belong to the ongoing player-startup
contract; class restrictions are not bypassed or derived from menu selection.

## Verification and limits

`compile.json`: **10 strict successful compiles**, five consumers on ARM64
and x86_64 (faery connection, placement, character-panel session, native app
and renderer). No APK or emulator operation was performed.

The new composed three-class AS/profile test has been prepared but is **not
accepted**. The historical host snapshot has incompatible current session and
scene/material layouts. Its sanitizer failures are recorded in
`host-receipt.json`; they must not be represented as live Android failures or a
passing menu regression. They were not suppressed. The next validation needs
a wholly coherent current source/library build and actual portrait -> stats
-> inventory -> skills -> faeries -> back flow, with changed action readback.

Source screenshot/art parity and live Android click/visual acceptance remain
unverified for this change. The default Crypt C1/GS staging boundary is intact.

## Already applied shared hooks

- Renderer includes V8 faery connection/placement and V29 supplement headers.
- World owns `player_faery_v8`; fresh player initialization creates it once.
- Panel runtime connection adds faery queries after its existing mutation
  supplement, preserving the same Gear/Save/skills/action graph.
- Level-world CMake compiles the V8 connection and placement bodies.

The frozen predecessor V8 packet remains unchanged:
`character-menu-faery-v8-25ccb90e0bf22dab.zip`, SHA256
`89b65dde441322cd9404300af2a15983227eb36a8a703c3f40b8d66d2c9814ff`.
