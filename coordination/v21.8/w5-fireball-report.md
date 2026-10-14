# w5-fireball: Mage bolt visual, spawn and impact FX (branch v21.8-w5-fireball)

Verdict: **WORKED for the visual orientation and the impact FX position** (real runs, frames viewed). Contact and damage on Trungus were already applied before the change and are unchanged. Remaining gaps are listed at the end.

## 1. Original evidence (IDA + assets)

- `Projectile::SetInfo` 0x3e58cc (pseudocode-all.c line ~166440): origin = `GameObject::GetTargetPosition(owner)` unless the owner's visual has a `projectile_node` (`VisualObject::GetSpecificNode`). Destination = origin + look vector * 1000. The projectile's rotation is `GameObject::SetRotation(this, owner+364)`, i.e. the owner heading.
- `GameObject::GetTargetPosition` 0x3935dc: target-node cache when the node exists and is enabled, else the object position. The Mage has no target node.
- `projectile_node` does not occur in any 3d asset under `.local-inputs/assets-extra/android/data/3d` (grep -a, all files). So the origin is the owner position. The port already does this (`main.cpp` launchPlayerRangedProjectile uses `transform.position`). **No change to the spawn origin.**
- Actor convention: `GameObject` look vector is `(sin h, -cos h)` (`GetLookAtVec` 0x393ae4) and actor visuals are drawn with `R(h)` (`main.cpp` ~9861 `yaw=rotation[2]`). So actor models have their forward along local **-Y**.
- Projectile BDAE geometry (decoded by `OriginalScene`, staged3 assets), mass on the -Y side for every projectile:
  - elemental_bolt_fire: mesh y -174.5 (flat quad), x -179..145, z 31..355
  - projectile_arrow_default: y -306..-107
  - projectile_dh2_prince_mage_cyclone: y -643..106
  - projectile_dh2_simplified_chain_lightning: y -568..194
  - elemental_bolt_water: y -496..-91
- Fire bolt node world translations (root scale 1.64 applied): `_bone_fire_bolt` (-19.2, -174.5, 193.3), `_mesh_glow_nobatch` (-17.4, -174.5, 193.2), `_colbox_fireball` (-19.0, -174.3, 182.7). The contact box is still centred on the origin (magefix2, `CalcMeshBox`).
- `Projectile::HandleImpactFX` 0x3e4eec (line ~ 3e4eec): a Character peer gets the impact FX at its `GetTargetPosition` (kind 4). Only non-character peers use the projectile x,y.

## 2. What the port did (bug)

1. **Visual orientation.** `runtime_session_projectile_v1.cpp` `visuals()` and `main.cpp` (projectile draw, ~line 10047) used `yaw = atan2(forward.y, forward.x)`, which maps the model's local +X onto the flight direction. The models fly along local -Y, so every projectile was drawn turned 90 degrees. For the Mage bolt at heading 0 the flame (local -Y, 174.5 units out) landed 174.5 units to the **side** of the caster (world -X), at hand height (z +193). That is the "flame to the right of the staff, a body-width away" in the owner's first screenshot. Arrows, cyclone, chain lightning and water bolts were turned the same way.
2. **Impact FX position.** `main.cpp` played the impact FX at the projectile position (`impact.position`, feet height of the caster, z 255), not at the victim's GetTargetPosition. The FX therefore appeared at Trungus's feet, and the flame visually stopped short of his body (second screenshot).

## 3. Change

- `features/effects/runtime_session_projectile_v1.hpp`: `source_projectile_model_yaw_v1(forward) = atan2(fx, -fy)` (model -Y onto forward, the actor convention); `source_game_object_target_position_v1` (same selector as the monster code); `projectile_impact_fx_position_v1` (Character peer -> its GetTargetPosition, expiry -> projectile position).
- `features/effects/runtime_session_projectile_v1.cpp` `visuals()`: uses the helper.
- `main.cpp`: the drawn projectile yaw uses the helper (all ranged projectile visuals that go through this path: Mage bolt, monster ranged projectiles, skill projectiles); the Mage impact FX uses `projectile_impact_fx_position_v1`; the log line `Mage projectile impact FX set=... at=x,y,z` now prints the FX position.
- Unit test: `runtime_session_projectile_v1_tests.cpp` checks that for headings 0, 0.7, -2, 3, pi/2 the yaw maps model -Y onto the forward vector.
- The change is generic: one helper for every projectile, no per-skill branch. Same rule for all classes that go through the shared projectile executor.

## 4. Real runs (hidden window, real keys, frames viewed)

Harness: `C:/dh218/tools/w5-run-mage.ps1` (= run-mage.ps1 bound to `windows-source-clock-v21.8-staged3` assets, extra args split on `;`). Mage seed `C:/dh218/seeds/mage/profile.save`. Trungus = `Swamp_ActorTroll` (the log names it "Trungus" in the HUD) spawned with `--spawn-test Swamp_ActorTroll@-5171,4220,257@60` in front of the Mage placed at `--player-to -5171,4000,257@50`. Space/Tab injected at real time. Cases: `C:/dh218/cases/w5-sp1b` (baseline exe `w5-fireball-baseline.exe`), `C:/dh218/cases/w5-sp2` (fixed exe `w5-fireball-fix1.exe`). The final exe `w5-fireball-fix2.exe` differs from fix1 only by the added `at=` field on the impact-FX log line; the runs above were made before that field existed, so their log lines do not show the FX position.

- Baseline, first cast (heading 0, no target), frame 122 (`w5-sp1b/f122.png`, viewed): the orange flame is drawn to the **lower-left, out to the side** of the Mage, far from the staff.
- Fixed, same cast, frame 122 (`w5-sp2/f122.png`, viewed): the flame is at the **staff tip, ahead of the Mage** in the direction it is facing.
- Fixed, frame 766 (`w5-sp2/f766.png`, viewed): bolt 3 (targeted, heading 3.07) leaves the staff tip and travels toward Trungus, who stands in front.
- Fixed, frame 776 (`w5-sp2/f776.png`, viewed): Trungus with HP bar and name plate, bolt 3 already hit him.
- Log, fixed run `w5-sp2/run.log`:
  - `Mage projectile launch clockMs=0 ... heading=0` then `impact id=1 kind=1 ... pos=-5171,2016,255` (expiry)
  - `Mage projectile impact id=3 kind=4 target=6003394295803936768 fx=241 pos=-5164.15,4095.75,255 damage=23.9297 requested=23.9297 applied=1 died=0`
  - `Damage frame=... attacker=18446744073709551615 target=6003394295803936768 marker=do_skill removed=23.9297`
- Contact and damage on Trungus were applied in the baseline too (`w5-sp1` run: bolt 3 `kind=4 damage=23.9297`, bolt 4 `damage=22.8086`). The damage marker for a Mage bolt is `do_skill`; `SpellCombatRoll` is not the Mage bolt marker. The request's `marker=SpellCombatRoll` was not seen in these runs, and I did not claim it.

Numbers (game units; origin = Mage position, heading 0):

| Quantity | Baseline | Fixed |
|---|---|---|
| Flame glow centre relative to origin | (-174.5, +17.4, +193) | (-17.4, -174.5, +193) |
| Distance of flame from origin, horizontal | 175 (sideways) | 175 (ahead, travel direction) |
| Impact FX position | projectile position (z 255, caster level) | Trungus GetTargetPosition (his feet at his own origin) |

Measured in the run: launch at the Mage, flight along the facing direction, impact at the troll's position. The on-screen placement is read from the frames above (approximate, not pixel-measured).

## 5. Other projectiles

The same helper now drives every projectile through `main.cpp` ~10047, including the monster ranged projectiles (`source_look_vector` heading, same convention as actors). No real run of an Archer, Rogue or monster projectile was made in this pass, so their visual is verified by the authoring evidence above (all five BDAE models have their mass on -Y), not by a frame. This is the open verification item.

## 6. Tests

- Full CTest on `C:/dh218/w5-fireball-build` (Release, llvm-mingw on PATH): first run 215/220 passed. The 5 failures were environment, not code. The worktree lacked the junctions for `.local-inputs/runtime-session-projectile-v1` and `.local-inputs/windows-source-clock-v21.8-staged` (both read by test paths in CMakeLists.txt). After adding those junctions, the 5 tests were rerun alone and all pass: `runtime_session_projectile_v1`, `runtime_session_projectile_render_v1`, `runtime_rogue_skill_owner_v1`, `runtime_archer_skill_owner_v1`, `runtime_skill_activation_session_v1` (5/5). Net: 220/220 passed. `runtime_skill_session_training` (known flaky) passed in the full run.
- The projectile yaw unit test prints `PASS projectile visual yaw maps model -Y onto the forward vector for headings 0..pi/2`.
- The executable tested in the real runs and the one built for CTest is `C:/dh218/w5-fireball-fix2.exe`, sha256 `316f5f0210fff92ac3212451e3bde3b49305b06e199b01889890143ef9edce0b`.

## 7. Remaining gaps

- No original reference footage of the bolt at cast is in the repo. The -Y forward is an inference from the BDAE and from the actor convention; the owner's statement that the original spawns at the staff is consistent with the glow being 174 units ahead at hand height, but was not independently observed.
- Contact is still tested at the projectile origin (the `_colbox` centred on origin, per magefix2). The visible flame therefore reaches Trungus's body up to ~175 units before the contact test fires. This is what the original's geometry does per the IDA reading, but it has not been compared with original footage.
- Not verified in a real run: Archer, Rogue and monster projectile orientation after the yaw change.
- Packaging: `.local-inputs/windows-source-clock-v21.8-staged3/dh-foundation.exe` was rewritten at 18:56 by another w5 worker (w5-faery-package.json). I did not overwrite it; my exe is in `C:/dh218/w5-fireball-fix2.exe` (sha in the file hash below).
