# First chapter integration and acceptance

The end goal remains a native ARM64 Dungeon Hunter 2 game with its original
assets bundled in one APK. The immediate playable milestone is the complete
first Swamp chapter. Crypt is the temporary development world used to verify
the menu and gameplay flow; it is not completion of the Swamp milestone.

## Agreed order

1. Repair the common skill/combat continuation that currently stops the native
   frame on a lethal targeted cast. Complete Ctrl_Kill, original loot transfer,
   XP distribution, async quest events, same-NPC AI death, target cleanup and
   finite animation completion. Do not implement a special FIRE workaround.
2. Finish canonical LevelConfig/Module construction, property loading, InitPost,
   static scene and PF/floor connection. Supply the loader chat an immutable,
   complete dependency handoff, then test actual Swamp loading.
3. Connect the original Character Sheet, Skills and Inventory SWFs, their touch
   geometry and same-profile actions through the gameplay portrait.
4. Integrate the main-menu contribution and the selected-slot/difficulty launch
   contract. Verify Main Menu -> Character Selection -> Play -> Crypt -> HUD ->
   portrait -> Stats/Skills/Inventory -> back to gameplay.
5. Restore a full last-saved player/world state, then play through Swamp chapter
   one, including its source progression, scripts, interactions and transitions.

## Current verified state

| Area | Evidence | Remaining acceptance |
|---|---|---|
| Renderer | Native Crypt, actual models/CPU animation, authored HUD and nonlethal targeted FIRE rendered in prior live check | Lethal gameplay continuation and coverage of other authored effects/skills |
| Common enemy death | APK-linked composition: 4,478 checks, 41 required-service failure prefixes, actual cached skill/faery/animation tables, reciprocal relations and six skill/spell cleanup callbacks | Live Ctrl_Kill rewards, real death-body endpoints, finite animation completion; script/animation endpoints in this test are declared fixtures |
| Threat cleanup | Original comparison: 48 cases, 11,190 checks, 120 ordered notifications; current APK-linked Player OnDeAggro: 23 checks | Live combined kill/target/HUD verification and real reached audio |
| NPC combat | Walk/Attack, inventory, named-animation/melee and actor-frame owners handed off; compiled into core where applicable | Renderer adapter adoption, authored endpoints, NPC-to-player Hit continuation, live locomotion/damage |
| Level loading | Loader source/context handoffs imported; same logical Level and corrected GSLevel global contract preserved | Whole Level lifecycle/publication; complete Module scene/PF owner connection and Swamp gameplay commit |
| Character screens | Original SWF Stats -> Skills -> Inventory -> Back flow passed isolated test; source owners now build for both ABIs | Real application lifecycle/reload providers, portrait connection and visible GPU/touch validation |
| Main menu | Frozen v87 merged into sole renderer and source GameSWF; one integrated APK passes unused-slot selection, on-screen name, animated Warrior selector, profile creation, Single Player -> Crypt -> original HUD. Persisted slot restart, Android pause/resume and source movement also pass on5554 | Other classes cannot enter the current Knight demo; full selected-profile restore, Character panels and full Swamp acceptance remain pending |
| Saves | Original metadata/profile and separate native Save/Gear pieces exist | Full existing-profile GEAR/PROP/QEST restoration and shared menu/game authority; current menu preview uses starting gear |
| First chapter | Swamp source/assets and loader preview exist | Full playable chapter, not merely displayed geometry |

Accepted **menu-to-Knight-Crypt development flow** checkpoint SHA256:
`e92c2102893845f082e9bb4ccdf4bb6f780520471c8c085f836e43839339f46c`.
APK: `port/android-native/build/checkpoints/dh2-native-main-menu-to-crypt-e92c2102.apk`
(600,712,425 bytes, ARM64 and x86_64, assets bundled). This does not accept the
lethal-skill fix, Character panel flow, full saved-profile restoration, all
classes, or Swamp. The same real APK remains installed on visible emulator5554.
Evidence: `port/android-native/reports/front-game-flow-v1/accepted-menu-to-crypt-v1.json`.
Screenshots17 through24 prove the fresh-character flow on one PID and APK;
10 through16 independently prove persisted slot, pause/resume and movement.
Working source contains later agent work beyond this immutable binary.

The initial Play failure09 was fixed by constructing each NPC's sole actual
inventory before combat actor registration and retaining it for attack. Combat
equipment queries now use each actor's inventory and property state; NPC stats
are no longer overwritten by a borrow of the player's Gear combat view.

The merged front now borrows the actual source movie input-history/frame owner
and installs the reconstructed source font platform before edit-text creation.
Startup ownership is preserved through that real font adapter, so a second input
owner is not bound. Front and gameplay retain their own movie lifetimes while
sharing the existing GameSWF engine and model renderer.

## General skill regression scope

The shared damage/death fix must be exercised by multiple authored skills and
normal melee through the same World execution and receiver identities. Cover:
nonlethal and lethal target damage, misses/resistance/status branches, no target,
dead/expired target, multi-target/area effects, sustained/multi-step skills,
projectiles, summons and self/buff skills where their actual script paths reach
different services. Check genuine mana/cooldown/state transitions and source
animation completion, persistent world/HUD rendering and clear frame logs.
List unreached or unsupported families explicitly; a generic implementation
does not by itself prove that every authored skill is playable.

## Completion evidence

Use one integrated APK and the visible root emulator5554 to verify the entire
agreed flow. Loader5590 and main-menu5580 remain isolated. Full chapter evidence
must include starting a character, fighting and killing enemies, loot pickup,
XP/level gains, equipment/skill changes, potion use, required chapter scripts and
interactions, chapter transitions, save, process restart and restoration.
Physical ARM64 device/resolution testing remains separate from emulator checks.

## Coordination

Root owns integration, shared combat/death, Play connection and final acceptance.
The loader chat owns cached maps/level source loading. The main-menu chat's goal
remains paused; it supplied the existing v87 checkpoint without doing new work.
The three root subagents resumed after the user's usage reset. Combat/death owns
the shared lethal continuation and world loot; level integration owns Module,
PF/scene, RoomZone and loader dependency composition; character UI owns original
panels, reload lifecycle, source stack, portrait and touch integration. Root owns
main-menu merge, Play destination, integration and visible acceptance. Untested
partial loot/module/panel files remain explicitly incomplete.
