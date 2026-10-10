# POINTS (BUG B049): Stats point allocation in one visit

Status: investigation done, implementation in progress (see bottom).

## Evidence: original behaviour

Visual (reference video `Dungeon Hunter 2 (v1.0.3) Part 1`, ffmpeg 1 fps sheets in
`.local-inputs/claude-preview15/points/frames/`):
- t=789-797 s: Skills page open; at t=798 s the dialog "Confirm character point
  allocation?" (check = yes, X = no) appears over the menu; t=799 s shows the HUD
  (menu closed). Observed on the Skills page, not the Stats page. No Stats-page
  allocation is visible in Part 1 frames sampled (700-830 s).
- No Part 2 file exists in `.local-inputs/reference-video/dh2-act1/`.

SWF (`port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`):
- CharacterMenu onPush (0xe265): `AddedStatsThisTurn=false`, `Starting*=0`.
- btn_train_* onRelease (0x149de, 0x14a4e and siblings): spend only when
  `AddedStatsThisTurn == false`; then `NativeSaveGame` (skipped when
  useSkillPoint), `StartingX += 1`, `AddedStatsThisTurn = true`,
  `NativeStatsAssignPoint(stat)`, sound MenuSpending.
- CharacterMenu btnBack onRelease (0xe55b..): if `AddedStatsThisTurn` and not
  `useSkillPoint` -> WarningType "SettingStats", menu_confirm2 with
  GAMEPLAYMENUS_POINTS_CONFIRM.
- Confirm yes (0x345dc, btn_yes): SettingStats -> `AddedStatsThisTurn=false`,
  NativeSaveGame, NativePopAllMenus, NativeBackToHud (menu closes).
- Confirm no (0x34a47, btn_no): SettingStats -> NativeReloadSkills,
  NativePopMenu, flag false, btnBack again (menu closes).

IDA (`pseudocode-all.c`):
- NativeStatsAssignPoint 0x43eb7c -> Character::IncStatStr/Dex/End/Nrg
  (0x3bd810..): each debits property 148 by 1 and credits the stat by 1,
  then UpdateBaseProperties. No gate in native code; the gate is in the SWF.
- Character::ResetStatsChange 0x3bd... (NativeStatsRemoveAssign 0x43d43c):
  refunds the four deltas to property 148 and recalculates (ValidateHPMP, Save).
  Not referenced by the SWF in this flow (refund is the inverse operation).
- NativeReloadSkills -> Character::ReloadSkills -> SG_Load(32) (reload of the
  last save).
- Character::LevelUp 0x3beb88: level+1, UpdateBaseProperties (2 points per
  level come from the class recalc data, not a hard-coded grant).

Conclusion from the SWF: the original permits ONE stat point per
Confirm cycle. A Back with staged points asks "Confirm character point
allocation?"; Yes saves and closes the menu, No reverts the last spend and
closes. Because Yes closes the menu, the original is one stat point per menu
visit, which is what the Preview 14 gate implemented. The brief's assumption that
the original lets several points be staged is NOT supported by the bytecode.

## Decision (user-requested deviation, to be stated to the user)

The user reported B049 as a bug: two granted points must both be spendable in
one visit. The implementation therefore removes the one-spend gate and keeps the
original's confirmation model: staged spends in one visit; Back/close with
staged spends shows the confirmation; Yes persists and closes; No refunds all
staged spends (inverse of the spend, via the same class recalculation) and
closes. Escape with the dialog open only dismisses the dialog. This is a PC/user
adaptation; the original's one-per-cycle gate is documented here, not hidden.

## Changes

(in progress)

## Package files required

(to fill)

## Verifier script

(to fill)

## Open risks

- Original No reloads the last save; the port refunds the staged points through
  the live recalculation (same as NativeStatsRemoveAssign). Current HP/MP are
  clamped to the restored maxima rather than reloaded from disk.
- Skill confirm (SettingSkills/SettingBoth) is not implemented here.
