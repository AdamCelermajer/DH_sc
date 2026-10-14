# v21.8 wave 2: Paladin (Knight subclass) skills on the general engine

Branch `v21.8-w2-paladin`, worktree `.claude/worktrees/w2-paladin` (subst `P:` during the session). Build dir
`C:/dh218/w2-paladin-build` (Release, llvm-mingw). Package for the real runs:
`.local-inputs/windows-source-clock-v21.8-staged-w2-paladin` (a copy of the staged2 base plus the Paladin FX and passive
scripts; it holds all 11 FX files and the four passive `.luac` files listed in `asset-selection-w2-paladin.json`). No shared
staged folder was regenerated. Not pushed, not merged into windows-foundation (the lead merges).

Case folders (outside the repo, fixed by the runner): `C:/dh218/act1rogue/cases/w2p-*/` (`run.log`, `f<frame>.png`).
Seeds: `C:/dh218/seeds/w2p/*.save` (made by `subclass_save_writer_v21_8` from the Knight seed, level 30).

## Verdict summary

| Skill | Kind | Verdict | Main evidence |
|---|---|---|---|
| OffensiveAura | active buff | PARTIAL (self buff WORKED; ally loop not built) | `w2p-oa` |
| GuardingAura | active buff | PARTIAL (self buff WORKED; ally loop not built) | `w2p-ga` |
| PurifyingAura | active buff + heal | PARTIAL (heal and buff WORKED; ally heal and regen not built) | `w2p-pa3` |
| DivineJudgement | active hit | WORKED (cast, damage, kill); push displacement not seen | `w2p-dj3` (`w2p-dj1`/`dj2` were misses) |
| HolyWeapon | passive, on hits taken | WORKED (attacker HP drain); kill-at-0 path not exercised | `w2p-hw2` |
| Shield (GuardianShield) | passive, on hits taken | WORKED for the stun state; push roll is a no-op as shipped | `w2p-sh` |
| EnergyBalance | passive, periodic buff | PARTIAL (Lightning row WORKED; other elements not run) | `w2p-eb` |
| Conversion | passive, on hits dealt | PARTIAL (owner built; no conversion proc observed in any run) | `w2p-cv`, `w2p-cv2` |

## Per-skill table (real runs, hidden window, real keys: Enter, Space, Tab, key 2)

Frames are game frames (fixed step .016 s, so 1 s is about 62 frames). "Cast" is the `Source skill key=2 ... phase=1` line.

| Skill | Cast accepted | Animation | FX | Sound (step cue, dispatched) | Numbers before -> after | Expiry | Verdict |
|---|---|---|---|---|---|---|---|
| OffensiveAura (`w2p-oa`) | frame 728, MP 63.5 -> 43.5 (cost 20, rank 1) | cast pose visible (f740: sword raised, sigil above) | `skill_dh2_..._offensive_aura` presentation FX created at frame 729; sigil visible at f740 and still at f950 (buff persist) | cue ordinal 560 -> uid 105 `sfx_skill_warrior_pld_crusade.wav` (voice started, not listened) | Damage_Bonus_Sword (83) 612 -> 1221 raw (2.39 -> 4.77), Axe/Mace (84/85) same, Dagger (86) 255 -> 509, Staff/Wand (88/89) 228 -> 455 | not reached (run ended frame 1000; expiry = 12000 ms = frame ~1478) | PARTIAL |
| GuardingAura (`w2p-ga`) | frame 728, MP 63.5 -> 43.5 | cast pose as above | `skill_..._guarding_aura` FX created frame 748 | ordinal 561 -> uid 106 `sfx_skill_warrior_pld_spiritual_sight.wav` (not listened) | Physical_Armor (71) 6144 -> 7512 raw (24.00 -> 29.34) | `Timed buff expired frame=1497` (12000 ms from the Use at 748) | PARTIAL |
| PurifyingAura (`w2p-pa3`, seed HP 120) | frame 728, MP 63.5 -> 48.5 (cost 15 = 10 + 5 x rank 1) | not separately viewed | `skill_..._purifying_aura` FX created frame 748 | ordinal 558 -> uid 104 `sfx_skill_warrior_pld_alastor_blessing.wav` (not listened) | HP **120 -> 135** at the Use (Heal(self) = 15 x rank 1, `Source skill self heal frame=748 hp=120 -> 135 max=283.93`); Regen_HP (39) 5472 -> 7008 (+6.0), CombatRegen_HP (40) 1078 -> 1846 (+3.0) | expired frame 1497 | PARTIAL |
| DivineJudgement (`w2p-dj3`) | frame 788, MP 63.5 -> 33.5 (cost 30) | frames f790 (cast, sword raised, red ring under the Bogwomp) | OnPre per-target `spell_..._divine_judgement` FX created on the lizard at 788 (the red ring in f790 may be the standard melee ring; not separable) | ordinal 559 -> uid 107 `sfx_skill_warrior_pld_condemnation.wav` (not listened) | Use hit at 816: requested 67.125, lizard HP 62.5 -> 0, `died=1` (f850 shows "67" and the death). Outcome 0x80 (push bit) set; no displacement recorded | n/a | WORKED (push PARTIAL) |
| HolyWeapon (`w2p-hw2`) | passive; slot 0 = OffensiveAura, HolyWeapon rank 1 | n/a | none (Lua has none) | none (Lua has none) | lizard HP 62.5 -> 60.5 -> 58.5 -> 56.5 -> 54.5 over four non-miss hits (`[holy-weapon] drain ... TempProp1 2 at rank 1`); miss hits (outcomes 1) do not drain | n/a | WORKED |
| Shield (`w2p-sh`) | passive; slot 0 = OffensiveAura, Shield rank 1 | no stun pose visible (f460) | none | none (Lua has none) | stun chance 11% (10 + L); stun roll 9 < 11 on the hit at 420; stun tick at 450 `applied=1 duration_ms=3000`; no monster hit on the player between 450 and 653 (3 s + recovery); second proc at 653 (roll 5), tick 683, next hit 886. The push roll (16%) did not pass in this run (no knockback line) | n/a | WORKED (stun); push path not exercised |
| EnergyBalance (`w2p-eb`) | passive; rank 1, equipped faery Celest (element 0, Lightning) | n/a | none (Lua has none) | none (Lua has none) | `Skill_Paladin_EnergyBalance_Lightning` at frame 0 (rank change): Damage_Elemental_Min/Max_Main_Hand (95/96) 0 -> 512 (+2.0), Damage_Elemental_Type (97) -256 -> 512, Off_Hand (98/99) +2.0, Off_Hand type (100) -256 -> 512 | 3000 ms: expired 187, re-applied 188 (timer), 375/376, 563/564 | PARTIAL |
| Conversion (`w2p-cv`, `w2p-cv2`) | passive; rank forced to 20 (`w2p-cv`) and 40 (`w2p-cv2`) with `--test-skill-rank` | n/a | n/a | n/a (`sfx_passive_skill_activate_1` not dispatched) | 0 procs in 3 player hits (7% per hit) and 2 player hits (12% per hit); no `[conversion]` line | n/a | PARTIAL (not demonstrated) |

Evidence lines, verbatim from the logs (the `run.log` of each case):
- OffensiveAura: `Source skill key=2 slot=0 frame=728 ... skill=OffensiveAura MP=43.5`; `Timed buff applied frame=729 ... class=Skill_Paladin_OffensiveAura strength=1 durationMs=12000 cells=83:612->1221,...`; `Source skill presentation FX frame=729 skill=OffensiveAura name=skill_dh2_prince_warrior_paladin_offensive_aura kind=self`.
- GuardingAura: `Timed buff applied frame=748 ... class=Skill_Paladin_GuardingAura ... durationMs=12000 cells=71:6144->7512`; `Timed buff expired frame=1497 ... cells=71:7512->6144`.
- PurifyingAura: `Source skill self heal frame=748 skill=PurifyingAura hp=120 -> 135 max=283.93`; `Timed buff applied frame=748 ... class=Skill_Paladin_PurifyingAura ... cells=39:5472->7008,40:1078->1846`.
- DivineJudgement: `Source skill hit frame=816 skill=DivineJudgement ... applied=1 requested=67.125 removed=62.5 died=1 targetHP=0/62.5 ... outcomes=0x80`.
- HolyWeapon: `[holy-weapon] drain attacker=... hp 62.5 -> 60.5 (TempProp1 2 at rank 1)` and three more lines down to `54.5`.
- Shield: `[shield] stun roll passed attacker=... roll=9 chance=11 stun_ms=3000`; `[shield] stun tick attacker=... applied=1 duration_ms=3000`.
- EnergyBalance: `[energy-balance] class=Skill_Paladin_EnergyBalance_Lightning rank=1 element=0 (rank change)`; `Timed buff expired frame=187 ... durationMs=3000`.

Dispatch and sound note: "Audio step source ... sound=<ordinal> status=1" means the sound was dispatched (status 1 = dispatched). The
ordinal-to-file mapping comes from the phase-1 table (558/559/560/561 = uids 104/107/105/106, which matches the four
labels in `audio-assets/data/sounds/sounds.xml`). The engine does not log the uid for step cues, so the file name is
resolved by table, not by a runtime log, and **nobody has listened to any of these files**.

## What was checked and what was not

- Visual evidence: frames were viewed for OffensiveAura (f740, f950), DivineJudgement (f790, f850), Shield (f460). There is no
  original gameplay footage for the Paladin skills in the repo (phase-1 section 4, Q3), so **nothing is compared to the
  original visually**. The DJ red ring at the target cannot be separated from the standard melee ring.
- Logic evidence: the phase-1 report (Lua and IDA) and the Lua files under `.local-inputs/assets-extra/android/data/scripts/skills/`.
- Numbers are the port's class-sheet evaluations in the live session, not measurements of the original game.
  Two disagreements with the phase-1 hand calculation are not reconciled: Guarding armor delta observed +5.34 (phase-1 hand
  value 0.965 at level 1, which included a +0.742 term that es-passive later removed from formula type 6), and DJ damage
  observed 67.1 with Longsword01 equipped (phase-1 range 24.9 to 39.8 excluded the weapon). Both need an original reference.

## Engine changes (branch commits, in order)

1. `c7cb69a7` (w2-paladin): aura self BUFF_FX as row data (`self_fx_name`); PurifyingAura self heal as row data
   (`self_heal_per_rank` = 15, `self_heal_target_hp_v1` capped at HP_Max, written with `set_current_health` at Use,
   receipt fields and one log line per cast); HolyWeapon event owner (defender-side hit: attacker HP -= TempProp1 at the
   rank, kill at 0 through `script_kill_actor`).
2. `266999f4`: merge of `windows-foundation` (Berserker, Necromancer, Illusionist, menu preview fix). The positional
   `RuntimeSkillUseProfileV1` rows keep the windows-foundation tail (self_hp_cost, status_arm, pre_range_from_temp_prop1,
   knockback_great_on_character_hit) followed by `self_heal_per_rank`. `EventPassiveKindV1` = killing_frenzy, carrion_eater,
   killing_streak, survival, holy_weapon, shield, energy_balance, conversion.
3. Uncommitted at report time (committed with this report): `event_passive_owner_v1` gains
   - Shield: defender-side hit in melee reach (target radius, as the Putrid owner), push roll and stun roll from a seeded
     engine (`std::mt19937`), stun after a 500 ms tick through `CombatSession::script_status_actor` (bosses refused there).
   - EnergyBalance: rank change removes and re-applies the element row (`Skill_Paladin_EnergyBalance_<Element>`, strength =
     rank, 3000 ms); a 3000 ms repeating refresh with the element at that moment. Element from
     `main.cpp` (`set_equipped_element`, from `faery_ability_for_list_slot_v1`, -1 = none, Lightning fallback).
   - Conversion: hit on a monster by the player, boss and miniboss refused (AI row flags bits 2 and 1), memory of up to six
     converted monsters (a hit on one removes FactionBuff3 and does not re-convert), roll below TempProp1 (percent) gives
     `FactionBuff3` for SnS_Duration with the persist FX, Stop() on both actors through `script_stop_actor`.
   - HolyWeapon log now prints the real before and after HP.
4. Test-only tool: `subclass_save_writer_v21_8` takes an optional HP argument (clamped to max) and a `PASSIVE+ACTIVE` skill
   argument (the passive ranks, the active goes on hotbar slot 0). Passive rows cannot sit on hotbar slot 0 (the HUD fails
   with "no supported authored Anim sequence root").
5. `coordination/v21.8/asset-selection-w2-paladin.json` (the missing Paladin FX and passive scripts; all now present in the
   w2-paladin package).

## Gaps (not built or not demonstrated)

- Ally query (Player filter, N12) and ally buffs/heals: the aura and PurifyingAura ally loops are absent. In single-player the
  Lua loop applies nothing else (inference, no other Player). Not tested with a second character.
- Regen_HP / CombatRegen_HP (PurifyingAura buff cells 39/40): no regen consumer observed. HP stayed at 135 for 750 frames
  with the buff active.
- Conversion: no real proc (chance-limited, and the test runs had 2 to 3 player hits). Not built: the 2 s untargetable window
  (`SetIsTargetable`), the PlaySound dispatch, the Lua `Stop()` on the caster is a real call but its effect is not checked.
  The faction flip (AIFaction 0 by FactionBuff3) is not checked against the monster AI, so whether the converted monster
  stops attacking the player is unknown. Memory is keyed by actor, not by name (the Lua keys by name).
- Shield: the Lua repeats the 500 ms tick until the state is Stunned; the port applies one tick. The Lua `Stop()` on a
  non-Attack state is not called. The push roll is a no-op, as the Lua is (SetKnockBackState(false)).
- EnergyBalance: the element numbers differ between the faery enum (Lightning 0, Earth 1, Water 2, Air 3, Fire 4) and the
  Lua's Elemental constants; the port maps by element name, so the Lightning row is the only one checked. A one-frame gap
  between the old buff's expiry and the timer re-application is observed (frames 187-188); the original's timing is not checked.
- DivineJudgement: push displacement not observed (outcome bit 0x80 only). First attempt missed (outcome 0x1); the second
  attempt hit once.
- HolyWeapon: the kill at 0 HP path was not exercised.
- Unit tests: the new owner branches (Shield, EnergyBalance, Conversion, HolyWeapon) have no unit tests; they are covered by
  the real runs above only. The knight unit test pins the aura self FX names, the PurifyingAura heal value and the cap.
- Sounds: resolved by table, not heard (see above). Visuals: no original footage, nothing compared.

## Tests

- Full CTest on the merged tree (windows-foundation merged at 266999f4, before the Shield/EnergyBalance/Conversion changes):
  214 of 214 passed.
- CTest after the Shield, EnergyBalance, Conversion and writer changes: see the final line of this section.
