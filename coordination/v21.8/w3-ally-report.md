# Wave 3, w3-ally: ally queries, aura loops, regen consumers (Paladin gaps)

Branch `v21.8-w3-ally`, worktree `.claude/worktrees/w3-ally`, subst `X:`, build dir `C:/dh218/w3-ally-build` (Release, llvm-mingw).
Commits: `5a93bb1f` (engine work) and a merge of `windows-foundation` (`28dfa273`, w3-status) into the branch. Not pushed, not merged into
windows-foundation (the lead merges).

Real-run cases (outside the repo): `C:/dh218/w3-ally/cases/<case>/` (`run.log`, `f<N>.ppm`/`.png`), Illusionist cases in
`C:/dh218/w2-ill/cases/w3t-*`. Runner: `C:/dh218/w3-ally/make-case.ps1` (Knight template from `w2p-pa3`) and the w2 Illusionist
`make-case.ps1`. Both drive the hidden window with `quiet_click_sync.ps1`; casts were injected with the `--skill-key-frame <frame>:2`
test aid (the same aid the Illusionist runs used), not with clicks.

## Verdict summary

| Item | Verdict | Main evidence |
|---|---|---|
| 1. Regen consumer (Regen_HP/MP, CombatRegen_HP/MP, AI_Tick) | WORKED | real runs `w3p-base`, `w3p-cast`, `w3t-terror`; unit `w3_ally` |
| 2. Ally loop through the Player-filter query (auras) | PARTIAL | loop built and logged; zero Player-filter candidates in every run; faery not a candidate (see 2a) |
| 2a. Faery as an ally (brief expectation) | DID NOT WORK (by the port's rule; see open question) | `[aura-ally] ... candidates=0 bound: 8886960962655148639(player=0)` |
| 3. PurifyingAura ally heal and buff | PARTIAL (not exercised) | code path present; no ally in any run; self heal WORKED (120 -> 135) |
| 4. Conversion 2 s untargetable window + PlaySound | PARTIAL | unit WORKED (event owner section I); no conversion proc in a real run |
| 5. Terror per-conversion mana | WORKED for the conversion branch; no-target branch PARTIAL (not observed) | `w3t-terror`: cast MP 151, conversion debit 11520 raw (45 MP), MP 106 after |
| 6. Conversion memory keyed by actor | NOT CHANGED (deliberate) | see section 5 |

Frames viewed: `w3p-cast-cap/f260.png` (PurifyingAura cast pose and glow; a "Quest Completed" banner from the save's campaign state
covers the centre, unrelated to this change), `w3t-terror-cap/f230.png` (Illusionist beside the converted Bogwomp, bar shown; the
Terror FX is not isolated). Nothing was compared to original footage (none for these skills in the repo).

## 1. Original evidence

### 1a. Regen tick (IDA `libDungeonHunter2.so/pseudocode-all.c`)
- Timer: `CharAI::AI_ScriptInit` (0x3cfde4), `CharAI::OnRevived` (0x3d0f1c), `CharAI::OnInit` (0x3d12b0) call
  `CharTimers::TMR_Start(CharacterDesign.AI_Tick, -1, event 51)`. The repeat is -1 (forever).
- **AI_Tick = 3000 ms, not one second.** Read from `.local-inputs/assets-extra/android/data/pydata/design_pycst.bin`: the `AI_Tick` row
  holds 0x0bb8 (3000). The `DoT_Tick` row beside it holds 0x03e8 (1000). The brief's "per-second" is wrong; the real-run ticks
  land every 187.5 frames (= 3.0 s at the fixed .016 step).
- Event 51 -> `CharAI::_UpdateRegen` (0x3cb77c): returns when the Character IsDead (vtable +84); otherwise
  `Character::RegenTick(AI_IsInCombat())` (0x3bdd90).
- `RegenTick(combat)`: `RegenHP(cell 40 CombatRegen_HP)`, `RegenMP(cell 45 CombatRegen_MP)`. `RegenTick(idle)`: `RegenHP(cell 39
  Regen_HP)`, `RegenMP(cell 44 Regen_MP)`. Cells 36/38 are HP cur/max, 41/43 MP cur/max (raw x256).
- `RegenHP` (0x3bdca4) / `RegenMP` (0x3bdbb8): `a2` is cut to (max - cur) when the sum would pass the maximum; `PROPS_Add(cell, a2)` runs only
  for a2 > 0 (PROPS_Add 0x3e0708 for a type-8 cell).
- `AI_IsInCombat` (0x3d4bc4) = `AI_HasAggro` (aggro list non-empty, CharAI+140) || `AI_IsAggroed` (CharAI+164) || `SM_IsAttacking` ||
  `SM_IsUsingSkill` || `SM_IsCasting`.
- Cross-check: the sheet values seen in the real runs match the earlier Paladin notes (CombatRegen_HP 1078 raw = 4.21, PurifyingAura
  buff +768 raw = +3.0 on cell 40, idle Regen_HP 5472 raw = 21.4 on cell 39).

### 1b. Aura loops and the Player filter (Lua, `.local-inputs/assets-extra/android/data/scripts/skills/`)
- `prince_paladin_offensive_aura.luac` and `prince_paladin_guarding_aura.luac`: `SetTargetListCharacterFilter(Player)`,
  `TargetListSearch(6000, 360)`, then for every target whose name is not the caster's: `PlayFX(BUFF_FX2)`, `CreateBuff/ApplyBuff`. No
  `IsPlayer` check.
- `prince_paladin_purifying_aura.luac`: the same loop with `if target:IsPlayer() and name ~= self`, plus `Heal(target, 15 x rank)` (capped at
  HP_Max) before the buff.
- `Character::IsPlayer` (0x3a49f0) = `GetCharType == 1` (CharType is the AI row Type, 0x3a3054). The port's faery is AI row type 3, so
  `IsPlayer` is false for it (the port's own `faerieActors` comment, main.cpp ~5966).
- Open: the native population behind `SetTargetListCharacterFilter(Player)` is `ObjectSearcher::PlayerList` (size read from the owner at
  +1732, 0x38d55c). I did not establish what the Session stores there. So whether the faery is in the Player population is
  unresolved. The brief expects faery to be an ally; the Lua's explicit `IsPlayer` re-check in PurifyingAura only tells us the population
  may hold non-players. I did not turn that into a rule.

### 1c. Conversion (Lua `prince_paladin_conversion.luac`)
- On a converted monster: `PlaySound("sfx_passive_skill_activate_1")`, `Stop()` on the caster and the monster, then
  `defender:SetIsTargetable(false)` with `StartSkillTimerCB(2000, 0, BuffExpired, defender)` only when `buff_timer == -1` (one window at a
  time). `BuffExpired` sets targetable true. Boss and miniboss excluded. Memory: six slots, keyed by `GetName()`.

### 1d. Terror (Lua `prince_illusionist_terror.luac`)
- `OnSkillCheck_` = `HasMana`; `OnPreSkill_` is empty. `UseMana(mana_cost)` is inside `add_slot`, i.e. once per conversion, after a
  target is found and before `ApplyPropClass`/`SkillCombatRoll`/`CreateBuff`. A cast with no eligible target costs nothing.

## 2. Engine changes (commit 5a93bb1f)

1. **Regen consumer** (`features/status_effects/vital_regen_v1.hpp`, new; `playable_actor_world.{hpp,cpp}` `regen_vitals`; `combat_session.cpp`
   clock and tick after the DoT loop, `Impl::vitalRegenClockMs`). Per-actor accumulator over the frame dt, one tick per 3000 ms; each tick
   picks the idle or combat cells; `regen_vitals` applies the native cap and moves the saved cell with the resolved cell (as
   `set_current_health` does) and mirrors ActorState health/resource. In-combat predicate: attacking/casting, or a non-player actor with a live
   target, or some live actor targeting this one (inference: the player's `target_id` is a soft selection, so the player's own target does not
   count). Applies to every bound actor (monsters too); the player's ticks are logged (`Vital regen tick serial=...`).
2. **Ally loop** (`runtime_skill_use_profile_v1.hpp`: `RuntimeSkillAllyAuraProfileV1` appended last in `RuntimeSkillUseProfileV1`
   as `ally_aura`; the three aura rows in `runtime_skill_use_profile_knight_v1.hpp`; `runtime_skill_cast_coordinator_v1.cpp` inside the
   `profile.buff` block). Candidates: `query_player_character_targets_v1` (range 6000, full circle, source order), excluding the caster.
   Per ally: FX cue on the ally, the PurifyingAura heal (`self_heal_target_hp_v1`, 15 per rank), and the same `TimedBuffSpecV1` as the
   caster's buff. A failed ally buff is logged, not fatal (the Lua checks neither). Logs `[aura-ally] ... Player-filter candidates=N bound: ...`
   listing every bound actor with its `is_player` flag.
3. **Conversion** (`event_passive_owner_v1.{hpp,cpp}`): on a successful conversion, `sound_cues_` gets `sfx_passive_skill_activate_1`
   (drained by `take_sound_cues`, played in `main.cpp` through `runtimeAudio->play_named_plain`), and if no window is open the monster gets
   `set_selection_suppressed(true)` for 2000 ms (`advance` clears it). Targetability is the ES-BUFF selection split; damage is unaffected.
4. **Terror** (`runtime_skill_cast_coordinator_v1.cpp`, `runtime_skill_cast_prepare_v1.{hpp,cpp}`): rows with `enemy_buff->conversion_slots > 0`
   skip the OnPre UseMana block (the HasMana admission at OnSkillCheck_ still runs). When a conversion happens, `debit_skill_mana_v1` debits
   the cast's fixed cost from MP cell 41 before ApplyPropClass/CreateBuff (Lua order).
5. **Vitals guard** (`runtime_skill_cast_prepare_v1.cpp` `character_vitals_match`): the CharacterState mirror now adopts the live MP sheet
   instead of rejecting. Without this, one regen tick between casts made every later cast fail ("CharacterState/Session vitals do not
   match"); `runtime_skill_activation_session_v1` caught it. The ActorState check is unchanged.

## 3. Tests

- Baseline before any change (branch at `3e5d6dca`): **216/216**.
- New `w3_ally` (`features/generic_skills/w3_ally_tests.cpp`, real Knight session with a bound monster): RegenHP add/saved-cell move/cap at
  max/non-positive no-op/MP cap/dead skip; idle cells (39/44) vs combat cells (40/45) and the in-combat predicate; AI_Tick clock (2999 ms no
  tick, 3000 ms one tick, 6500 ms two ticks with carry); `debit_skill_mana_v1` (cost off MP, zero no-op); `self_heal_target_hp_v1` cap.
- `event_passive_owner_v1`, new section I: Conversion converts within 400 hits at rank 40, the monster is untargetable, one
  `sfx_passive_skill_activate_1` cue, window open at 1.9 s and closed at 2.1 s with selection restored.
- Full CTest: **217/217** on the branch after the merge of `windows-foundation` (`3e5d6dca` -> `28dfa273` merged), run from
  `C:/dh218/w3-ally-build` (`ctest -j4`, llvm-mingw bin on PATH). Before the merge, 217/217 as well.

## 4. Real runs (hidden window, fixed step .016, STATE_TRACE on)

### 4a. PurifyingAura: regen curve (`w3p-base` no cast; `w3p-cast` cast on key 2 at frame 150, save `w2p/PurifyingAura_hp120.save`)
Baseline, no aura (player combat=1 through the run because a monster targets it):
```
Vital regen tick serial=188 ... combat=1 hp=120->124.211 mp=63.5->63.5
Vital regen tick serial=375 ... combat=1 hp=124.211->128.422 mp=63.5->63.5
Vital regen tick serial=563 ... combat=1 hp=128.422->132.633 mp=63.5->63.5
```
With PurifyingAura (cast accepted at frame 150, applied at 170):
```
Timed buff applied frame=170 ... class=Skill_Paladin_PurifyingAura durationMs=12000 cells=39:5472->7008,40:1078->1846
Source skill self heal frame=170 skill=PurifyingAura hp=120 -> 135 max=283.93
Vital regen tick serial=188 ... combat=1 hp=135->142.211 mp=48.5->54.0508
Vital regen tick serial=375 ... combat=1 hp=142.211->149.422 mp=54.0508->59.6016
Vital regen tick serial=563 ... combat=1 hp=149.422->156.633 mp=59.6016->63.5
```
The aura adds +3.0 per tick (cell 40, 1078 -> 1846 raw), so +7.211 per tick against +4.211 without it. Ticks 187.5 frames apart (3.0 s), over
about 9.4 s after the cast. The idle branch was also exercised in the Illusionist run (`combat=0`, cells 39/44):
`hp=133.137->165.797 mp=106->118.898`. Note the MP regen (cell 44/45) fills MP quickly, per the authored sheet; I did not measure it against
the original.

### 4b. Aura on the player and the faery (`w3p-oa` OffensiveAura, key 2 at 150; `w3p-cast` PurifyingAura)
```
[aura-ally] OffensiveAura Player-filter candidates=0 bound: 102377376217503574(player=0,alive=1) ... 8886960962655148639(player=0,alive=1) ...
Timed buff applied frame=151 ... class=Skill_Paladin_OffensiveAura durationMs=12000 cells=83:612->1221,84:612->1221,...
```
The faery actor `8886960962655148639` (DefaultFairy, animation-only, "Faery placed" at frame 0) is bound with `is_player=0`, so it is not a
Player-filter candidate. The player's own aura works. **Faery: DID NOT WORK under the port's rule.** This is the open question in 1b. If the
lead confirms that the Player population includes the faery, the change is one predicate in `query_player_character_targets_v1` (plus
PurifyingAura keeps its `players_only` gate).

### 4c. Terror per-conversion mana (`w3t-terror`: Illusionist save `terror31`, lizard spawned at 990,-212,258; key 2 at 150)
```
Source skill key=2 slot=0 frame=150 ... skill=Terror MP=151            (no debit at cast)
[terror] UseMana 11520 raw per conversion of monster=4141719441445850732
Timed buff applied frame=203 actor=4141719441445850732 class=FactionBuff source=Terror strength=1 durationMs=5000
Source skill lifecycle frame=203 ... phase=2 skill=Terror MP=106     (151 - 45 = 106)
Vital regen tick serial=375 ... combat=0 ... mp=106->118.898
```
11520 raw = 45 MP. The old flow showed MP=106 at the cast; the debit now happens at the conversion. The no-target branch (`w3t-terror-none`,
no spawn) still converted a level monster 4141719441445850732, so no MP-free Terror cast was observed. The branch is covered by its
condition (`!buffed_targets.empty()`) and the cast-time check, not by a run. Observation for the lead, not verified: that converted monster is
about 1085 units from the caster (Use geometry y=840 vs monster y=-245), beyond Terror's RANGE 650, so the port's Terror target query may
not enforce its range. This predates this change and I did not investigate it further.

### 4d. Conversion
No conversion proc happened in any real run (7 % per hit at the sheet's chance, few hits). The window and the sound cue are covered by the
unit test only. Sound labels are resolved by name, not heard (`sfx_passive_skill_activate_1`).

## 5. Not changed, and why
- Conversion memory keyed by actor (`ActorId`), not by `GetName()`. The Lua key is the object name; I could not establish that names are
  unique per instance, so I left the port's stable key in place.
- Native attack-continuation and retargeting for a converted monster (whether it stops fighting the player) is not checked.
- Monsters now regenerate through the same tick (their authored sheet values, per IDA `CharAI::_UpdateRegen`). This was covered by the
  unit test only, not observed in a real run, and the regen magnitudes are not compared with original footage.

## 6. Open items for the lead
1. Faery in the Player population: decide from the native population (ObjectSearcher PlayerList, Session +1732). If it includes the faery,
   change `query_player_character_targets_v1`'s admit predicate; the test and log lines are already in place.
2. The brief's "per-second" regen: the original is 3000 ms (AI_Tick). The port follows 3000 ms.
3. Terror target range (4c) looks unenforced in the real run; needs a separate check.
4. `character_vitals_match` now adopts the live MP sheet (section 2.5). Other CharacterState mirrors (HP) are still written only by the
   paths that wrote them before; regen HP is not mirrored into CharacterState (saves read the sheet).
