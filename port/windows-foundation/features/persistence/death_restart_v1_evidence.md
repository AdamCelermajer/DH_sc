# Death and restart policy evidence (B006)

## Scope and expected behavior

This feature models the source-authored local death and restart policy for an active single-player gameplay Level. A dead local character enters the offline fade/death-screen route. Only the authored revive action on an active gameplay Level starts a restart: close the fade-to-black, start fade-from-black, load the current Level checkpoint, clear the character's buffs, restore the source revive position, invoke source-equivalent `Character::Revive(..., 1)`, set Idle, ensure `CharacterDesign.MinimumPotionsOnDeath`, and restart level music. The online route is a separate timer/handoff and must not run the offline checkpoint/revive providers. A pause-menu return to main menu is a separate action; it saves and leaves the session but does not itself revive the character.

The minimal reusable implementation is `death_restart_v1.{hpp,cpp}`. It models the local-death admission gates, distinguishes offline from online routing, and requires an explicit single-use revive ticket. Provider ownership remains with the live Level/Character/save/menu systems. The ticket is consumed before effects so a repeated animation callback cannot replay a checkpoint or a partial restart prefix. It does not choose a full-health rule or manufacture a potion count; those belong to the recovered `Character::Revive` and character-design data providers.

## Original visual evidence

Reference: [Dungeon Hunter 2 v1.0.3 gameplay video](https://www.youtube.com/watch?v=z_Zky7qQdYs), length approximately 22:12. The local visual scan is `.local-inputs/v19-frontend-hotfix/death-menu-restart/video-scan-30s.png` and `video-scan-720s.png`, with sampled frames every 30 seconds from 00:00 through 22:00. The sampled images show menu/story/gameplay content. No death screen, revive prompt, checkpoint reload, or death-to-menu sequence appears in those samples. This is only a sparse sample; it does not establish that the full video lacks such a sequence or prove animation continuity. Current-port captures in `death-menu-preview.png` and `single-player-after-death.ppm` are runtime repro evidence, not original-game visual evidence. The original visual result for the death/revive transition remains unobserved.

## Recovered logic evidence

The local IDA/recovered-source export is `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/` (APK v1.0.2; visual video is v1.0.3, so version differences remain possible).

- `PlayerManager::Update` (0x378fb4) calls `_CheckLocalDeaths` (0x376100) and `_CheckGlobalDeaths`. `_CheckLocalDeaths` admits only a Level whose byte field at +304 is 38, an assigned local Character (`PlayerInfo +408`), and a true source character predicate at vtable+52. It then calls `_HandleLocalDeaths` (0x375fe8).
- Offline `_HandleLocalDeaths` pushes `menu_FadeToBlackScreen` once and mutes FX. The online branch uses a death timer and later invokes `ReviveLocalPlayers`; it is not the offline presentation route.
- The recovered `dqhud_droid.swf` action dump at `.local-inputs/swf-layout-trigger/dqhud_droid-actions.txt`, `root/sprite742 FRAME 88`, calls `NativeReviveAllPlayers`; frame 64 checks `NativeScreenIsBlack`, and frame 75 stops. This establishes an authored animation callback/action, not by itself the appearance of the original screen.
- `NativeReviveAllPlayers` (0x43b1e4) requires Level type 38. Online, it sets the online player-manager state and returns. Offline, it pops the fade-to-black screen, pushes fade-from-black, and calls `PlayerManager::ReviveLocalPlayers` (0x37565c).
- Offline `ReviveLocalPlayers` calls `Level::LoadCheckpoint` before the local Character sequence. It then clears buffs, restores the source revive position, calls `Character::Revive(..., 1)`, sets Idle, ensures a minimum potion quantity, and resumes Level music. `Character::Revive` (0x3a59ac) calls `_InitHpMp` / `RegenHP(-1)` and `RegenMP(-1)`, initializes life/body state, floor-corrects position offline, and updates skills. Actual resulting vitals are source-derived and should not be hard-coded by this feature.
- `Level::LoadCheckpoint` (0x3f0428) reloads the current Level checkpoint, reinitializes `GameEventManager`, calls `Character::SG_LoadCheckpoint(..., 16)`, then `SG_Update(1)`. In `PlayerSavegame::SG_LoadCheckpoint` (0x465450) / `_Load` (0x464f4c), flag 16 selects the QEST section rather than LVLS/SKIL/FAES/PROP/GEAR. Thus the evidence supports checkpoint world/quest-state restoration while preserving the other listed player-save sections; it does not support a full profile rollback.
- `Application::GoToMainMenu` (0x32c1f4), called through `NativeGoToMainMenu` (0x43ae9c), quick-saves the current Level and local player, resets the menu manager, switches to the Flash menu, and removes players. It is not process exit and has no focus gate in this source path. `PlayerManager::AllPlayersDead` (0x36e764) is only an admission gate for `ScriptManager::StartScript` (0x4605c0); it is not the death-screen/revive gate.

No distinct hardcore-mode check was found in the audited functions or SWF action above. This is a bounded source finding, not proof that no other hardcore policy exists elsewhere. No hardcore behavior is invented or added here.

## Focused verification

Before implementation, the focused test was defined to cover the Level/local-character/dead admission branches, offline vs online route selection, rejection outside Level type 38, exact offline callback ordering, online handoff without local revival, provider failure stopping later effects, and duplicate-callback suppression. Run:

```powershell
& port\windows-foundation\features\persistence\run_death_restart_v1_tests.ps1
```

Result: PASS under the pinned LLVM-MinGW C++17 compiler. Output: `PASS: source local-death gates, offline checkpoint revive order, online handoff, invalid-level rejection, provider failure, and duplicate suppression`.

This is isolated feature-logic verification only. It does not claim that the production caller is wired or that the normal executable completes the route.

## B006 runtime evidence and open integration gate

Artifacts are under `.local-inputs/v19-frontend-hotfix/death-menu-restart/` and use isolated profile copies. The frozen older executable `dh-foundation.exe` reproduces HP 1 → real Rogue/Lizard death (hit 14, End34 serial 83) → HUD Pause / Main Menu Yes at frame 120 → save at HP 0 → frontend exit 0. A separate-process Single Player load of that HP-0 profile on the older binary fails during manual locomotion selection because the source death sequence owns the actor pose. The integration lead's newer same-process diagnostic reaches the same death and menu/save state, but `FrontendRuntimeV1::attach` then rejects the existing Window because it is not focused; the run stops before the queued Single Player click. The focused feature fix does not solve that adapter failure, and no integrated same-process re-entry/revive result is claimed.

The intended integration must distinguish pause-menu return from the separate offline death-screen `NativeReviveAllPlayers` action. A source-authored checkpoint revive should only occur through that action and source callback timing; simply loading an HP-0 profile must not invoke manual locomotion or silently revive it. The integration lead owns the main/frontend call site and the unfocused existing-window attachment blocker. Keep B006 open until the isolated same-process production route completes and the independent reviewer confirms same profile identity, clean loop epochs, no duplicate End34/reward/body recreation, source-correct revive behavior, and no save corruption.
