# B077 report: Rene and Celeste follow the hero after the opening

Branch fix/b077, build `fix077`. Investigation, implementation, isolated tests and integrated EXE verification are separated below.

## 1. Visual evidence (reference video Part 1, local mp4, direct observation)
- t=174 (New Quest "Escape the swamp with Rene and Celeste"), t=176-200: after the cutscene the hero stands in the cage room, Rene (white-robed priest) to the left of the hero, a yellow glow (Celeste) above/next to Rene. Same formation in our EXE after SKIP (run a6).
- t=300-330 (1 fps): hero walks through rooms; Rene is not always visible and reappears next to the hero (t=314 Rene + glow beside the hero): he trails and catches up.
- t=354-366 (3 fps crop, Trungus fight on the bridge): Rene stands at the left edge, roughly 300-400 game units from the hero, not walking, and fires yellow bolts toward the troll (streak in 2 of 12 frames). Celeste's glow stays at his hand. The hero fights alone in melee.
- Observed: they follow at a distance, catch up when left behind, stop beside the hero, and Rene attacks with ranged shots. Not observed (clips not watched): where they end up at the quest end, whether they leave.
- Preview 16 baseline (b2, same args): Rene and Celeste never move (final positions identical, action idle).

## 2. Logic evidence (libDungeonHunter2.so pseudocode)
- CharAI::_UpdateMaster 0x3cc5a4: master-alive events 18/19; AI_IsInSight edge events 20 (OutOfSight) / 21 (InSight); while in sight and "my turn": ranged characters raise close 24 / ranged range 23 / out of range 22, melee ones out of range 22 / melee range 25.
- AI_IsInSight 0x3d4ed8 / 0x3d4ea0: ViewRadius^2 > squared distance. AI_IsInInteractionRange 0x3d4f98 (used for a friendly master by AI_IsInCloseRange 0x3d63d8 / AI_IsInMeleeRange 0x3d6188): (distance - own melee radius - master radius) <= AI row InteractRadius. AI_GetMeleeRadius 0x3d4c34 = weapon reach + AI row +32.
- CharAI::SetScriptByName 0x3ceeb0: "__faery__" selects native AISFaery (0x3de2d0-0x3de544); other names run Lua (AISExternal). Rows: Priest AI 50 script `rene`, `_prim_Faery` AI 20 script `follower` (recovered records in features/companions).
- Scripts (features/companions/reference/*.luac, plain text): follower: OutOfRange -> MoveTo(master) unless moving, range events -> Stop, OutOfSight -> WarpBehind. rene: far/catching-up state machine, Buff_Rene_Haste, EnableCollisions(false), MoveTo + WarpTo(master) fallback when no path, 200 ms CaughtUp timer.
- Character::_WarpBehind 0x3b8d98: master position + normalize(-lookAt)*100 (z unchanged).
- Gate: RENE_FOLLOW (population activation of the Priest, `--condition-active RENE_FOLLOW` in the package args); unchanged.

## 3. Root cause
All recovered companion code (source_companions, runtime_companion_*) had no production caller: the Priest and Faery were animation-only session actors that only played idle. Second blocker: `applyRetainedMotion` in main.cpp ignored root motion for any actor whose profile has no explicit `--combat-motion-root`.

## 4. Change
- `features/companions/follower_decision_v1.*`: engine-free decision core (AI row script -> behaviour; sight/close predicates; follower, attacking_follower, rene state machines; warp-behind position).
- `features/companions/runtime_follower_controller_v1.*`: per-frame controller over the live CombatSession; selects actors by AI row script only (no map/NPC names), master = host player, uses the shared sewn-floor route + locomotion owner (RuntimeEnemyNavigationV1.approach), warp service, population-enabled and cutscene gates, per-actor failure isolation, `[follower]` log lines.
- `runtime_enemy_navigation_v1.hpp`: `route_found()` accessor (source HasPath).
- main.cpp (small hunks marked "B077 FOLLOW"): include; controller built in `bindEnemyAI`, chained after the monster controller in the decision provider; haste catch-up uses a per-actor navigation instance whose walk rate is the Speed_Modifier_Walk change of the `Buff_Rene_Haste` class row (log: factor 1.2); followers consume root motion while walking.
- CMakeLists.txt: new sources in foundation_runtime_enemy, test target.

## 5. Tests
- `follower_decision_v1` (ctest): script mapping, range edges, follow/stop, warp only on the sight-loss edge, never-seen master, dead master, rene catch-up/haste/timer/out-of-sight/timer after the master leaves, warp-behind geometry. Passes. Full ctest 140/141; only the known `session_skill_binding` worktree failure.

## 6. Real-EXE before/after (quiet_run, same args; scratch .local-inputs/claude-preview161/fix077/)
Args: swamp start, `--campaign-skip-frame 200`, `--move-segment 700:1000:1,0`.
- Before (Preview 16, b2): positions unchanged to the end, action idle.
- After (a6): `[follower] ... rene action=approach haste=on` (factor 1.2), walking positions change, stop beside the hero (rene stop at distance 346, faery stop at 556). Frame a6/f1450 shows Rene and Celeste next to the hero, formation like the video.

## 7. Remaining gaps / unknowns
- Rene's ranged attack and attacking_follower target logic are NOT implemented: animation-only actors cannot enter combat here; the video shows Rene shooting. Needs a ranged-skill owner for non-player actors.
- Rene's "ranged range" band is treated as "in sight and not close" (ranged min/max from vt+296 / ItemInventory::CanRangeAttack not decoded).
- IsMyTurn throttling, master-death events 18/19 and the native AISFaery (the player's own faery) are not modelled; Celeste uses the row-20 `follower` script.
- Master radius = PF radius, own reach = AI row melee radius (no weapon); stop distances come from the formula, not measured in the video.
- Quest end / leaving behaviour not investigated. The source quirk (Rene stops and restarts while the hero keeps walking) is preserved.
- The route targets the master's current position each frame (source MoveTo fixes the destination at call time).

## Package files required
None (no asset or args change). The existing `--enemy-ai` flag in swamp.args is required, as for monsters.
