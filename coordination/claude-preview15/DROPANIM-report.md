# P15 DROPANIM report (BUG B047: ground-item drop animation)

Branch `p15/dropanim`, fix commit `39c95e31` (motion + tests), this report commit follows. Worktree `DH_wt/dropanim`, build `DH_wt/build-dropanim`.

## Reported symptom

User shot `.local-inputs/claude-preview15/user-shots/drop-potion.png`: a Potion label beside a dropped potion on the swamp bridge. The user reports the drop as "weird and slow, nothing like the original".

## Investigation (evidence)

### Logic (IDA `pseudocode-all.c`)

- `ItemObject::_GetRandomDropPos` 0x3ec668: landing = victim + dir*(150+rand200) + lateral*(rand300-150), dir = killer - victim normalised (the item is thrown toward the killer). Without a killer: XY +-250. Already implemented and unchanged.
- `ItemManager::Spawn` 0x3eacd0: `GameObject::SetPosition(item, victim position)`, `SetDestination(item, landing)`, then `ItemObject::InitAgain`.
- `ItemObject::InitAgain` 0x3ec0f0: plays the 3D drop sound (silent in this port), creates a Box2D `POItem` body (`PhysicalObject` ctor args 0,1,1,0,-3,0x40,4), `ShowGlow` (0x3ebca4, empty stub).
- `GameObject::UpdateTargetPosition` 0x393d74: `velocity = normalize(destination - position) * GetSpeed()`. `ItemObject::GetSpeed` 0x3ebc04 returns the float word 6.0 (set in `ItemObject::InitOnce` 0x3ece80, `+944`). Only X/Y go to the body.
- `PhysicalObject::setPosition` (259131): game units are multiplied by 0.01 (1 m = 100 units). `PhysicalObject::setLinearVelocity` (259055) stores the speed unscaled as m/s. So the item slides at 6 m/s = **600 game units/s**. No linear damping appears in the Box2D use here.
- `GameObject::IsAtDestination` 0x39361c: XY squared distance < 6400, i.e. **80 units**. `ItemObject::Update` 0x3ebee4 calls `GameObject::Stop` when true, so the item **stops 80 units short of its landing point**, at its current position.
- `UpdateTargetPosition` never syncs Z from the body (only X/Y at +88/+89). **The item keeps its spawn Z. There is no arc, apex or bounce in the original.**
- `GameObject::UpdateRotation` 0x393710: the visual turns toward the heading at `GetVirtual(172) * 4pi` rad/s. The value of that virtual for items is not recovered (open).
- `ItemObject::OnCollisionBegins` 0x3ec048 and `ShowTooltip` 0x3ebd5c: the name tooltip (`_text_itemname`, colour from `ItemInstance::GetColor`, `FadeIn`) shows only when the item is the local player's target (`+1321`). Not changed here (label behaviour belongs to the DROPS/pickup stream).

### Reference video (`.local-inputs/reference-video/dh2-act1/...mp4`, 640x360, 30 fps)

- Extracted 30 fps frames 205.0 to 213.0 s (`.local-inputs/claude-preview15/dropanim/ref205`, 240 frames) and crops 207.5 to 209.9 s (`chest-sheet-207.5-209.5.jpg`, `zoom-sheet-207.5-209.9.jpg`).
- Observed: the chest opens, then the item name label ("Useless Mace") appears about 0.3 s after the open flash and stays on screen.
- Not observed reliably: the item sprite is only a few pixels at this resolution and the camera follows the fight. **I could not isolate the item sprite to measure apex, bounce or spin.** The no-arc result comes from the IDA physics, not from the video.

### Our build before the fix (p14/integrate logic)

- `RuntimeWorldItemAdapterV1::advance`: 3D straight-line interpolation at an assumed 180 units/s (6.0 * 30, labelled UNPROVEN in the code), exact stop on the landing point, Z interpolated toward the landing Z.
- Consequences: about 3.3x too slow (for a 210-unit drop: about 1.2 s instead of about 0.35 s), a stop on an exact point instead of 80 short, and Z drift toward the landing Z.

## Changes

- `features/loot/world_drop_rules_v1.hpp/.cpp`: constants `physics_units_per_meter_v1` (100), `world_item_speed_units_per_second_v1` (600), `world_item_arrival_radius_v1` (80) replace the assumed 30-ticks constant. New `advance_world_item_step_v1` (XY only, Z kept, stops at the 80-unit radius, travel clamped on the radius).
- `features/loot/runtime_world_item_adapter_v1.cpp/.hpp`: `advance()` uses the step function. Own clock (dt from the game loop), no platform timers.
- Tests in `features/loot/world_drop_rules_v1_tests.cpp`: motion curve (one 1/30 s tick = 20 units; settle at 170 units = 0.283 s; Z constant, so no apex or bounce; farthest scatter about 0.5 s; inside the radius = no move; zero dt = no move) and an adapter test (stop on the 80-unit radius at spawn height, no further move).

## Measured (EXE trace, temporary and not committed)

Kill at frame 148 (seed 1234, drops overlay assets), item id 1 ClothGloves01, landing (-6758.47, 866.07, 255). Per 16 ms frame:

| frame | x | y | z | step |
|---|---|---|---|---|
| 148 | -6957.32 | 935.16 | 255 | (spawn) |
| 149 | -6948.25 | 932.01 | 255 | 9.60 |
| 161 | -6839.43 | 894.20 | 255 | 9.60 |
| 162 | -6834.04 | 892.33 | 255 | 5.71 (stop) |
| 163 to 260 | -6834.04 | 892.33 | 255 | 0 |

Distance from the rest point to the landing: sqrt(75.57^2 + 26.26^2) = 80.0. Settle 14 frames (0.23 s) after spawn. Before the fix this item would still be sliding at about 180 units/s, reaching the landing at about frame 218.

## Frame strips (quiet runs, frames 149 to 215)

- `.local-inputs/claude-preview15/dropanim/before` = `windows-source-clock-v19-preview-14-rc1` EXE (p14/integrate logic); `.../after` = fixed build. Ten frames each (149, 151, 153, 156, 160, 165, 170, 180, 195, 215).
- `strip-before.png`, `strip-after.png`: crops around the drop. The camera follows the fight, so the item is hidden under its label. These strips mainly show the label and player.
- `compare-165-215.png`: full-frame before/after at 165 and 215. In the before run the label is still moving at 215; in the after run it has stopped.
- Reading: the visible difference is small at this resolution; the trace is the quantitative evidence.

## Tests run (real output)

- `p14_build.ps1 -Name dropanim -Test`: `build exit 0`. ctest: 110 of 111 pass. `world_drop_rules_v1` Passed. The one failure is `session_skill_binding` (known worktree junction issue, ignored per brief). `schema_v4` failed once in a parallel run and passed alone (flake, not this change).
- DROPS-verify (`DROPS-report.md` verifier) against the new EXE: 10 PASS, 0 FAIL (`.local-inputs/claude-preview15/dropanim/dropsverify.out`).

## Package files required

None new. Preview 14 rc1 assets plus the existing `data/gfx/effects/GL_Diffuse_L1_VC_iPhone.bdae` overlay (`claude-preview14/drops/assets-overlay`), as in the DROPS report.

## Verifier script

`coordination/claude-preview15/DROPANIM-frames.ps1 -Label before|after -Exe <abs exe> -Frames "149,151,..."` writes one quiet job per frame (replays the kill at frame 148 with seed 1234 up to `--frames N`). Run with `quiet_run.ps1 -JobsFile <label>/jobs.json -Parallel 10`. Expected in `run.log`: `Source death reward frame=148 ... spawned=1 store=1` and `World item target frame=148 item=1 id=ClothGloves01 position=-6957.32,935.157,255` for the fixed build. The per-frame trace needs a temporary print after `worldItems->advance(...)` in `main.cpp` (not committed).

## Open risks and what is not verified

- Visual match with the reference video: not verified. The item could not be isolated at 640x360. The speed and stop come from IDA, not from footage.
- Walls: the original Box2D body collides with walls (the item stops at a wall). The port ignores walls. Open.
- Rotation: the original turns the item toward its heading at a rate set by virtual 172 (value not recovered). Not implemented. Open.
- Rest point: the original stops on the first whole frame inside 80 units (between 60 and 80 short, frame dependent). The port stops exactly on 80, a continuous approximation.
- Tooltip FadeIn timing and OOI-target gating (ShowTooltip 0x3ebd5c, `+1321`): not changed. Belongs to the DROPS/pickup stream.
- Glow: none (ShowGlow is an empty stub in IDA). No change.
- Player drops (`DropInventory`) use the same motion; not measured separately.
- Sound: silent (missing WAVs), unchanged.
