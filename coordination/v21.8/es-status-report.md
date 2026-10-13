# v21.8 ES-STATUS report (es-status, branch v21.8-es-status)

Base: windows-foundation at 29bc3efb (the branch was first cut from bb604d97 and rebased onto 29bc3efb before this commit).
CTest: 212/212 passed on the rebased tree (211 base tests plus status_states_v1). The brief's older "205" baseline is no longer
the base count.
Scope delivered: a general status-state engine module with unit tests and one integration path through the real combat roll.
It is NOT yet wired into CombatSession, the monster AI, the per-frame clock, or main.cpp. Live effect in the game is therefore
unchanged. No real-run (hidden-window) verification was done.

## Sources used (separately)
- IDA offline export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`: Character
  Lua entry points, CharStateMachine SM_Set*State, RaiseStateEvent, CharTimers::Update, F_ApplyResult, _F_CalculateResult,
  CF__Calc*, OnInit registrations.
- IDA vtables export `vtables-and-rtti.json` (raw bytes of `_ZTV9Character`), used to resolve virtual slots (see item 5).
- Lua `.local-inputs/assets-extra/android/data/scripts/skills/`: call sites of SetStunState, SetScareState, SetKnockBackState,
  IsBoss/IsMiniBoss (prince_archer_multihit, prince_archer_sidestep, prince_archer_pin_shot, prince_assassin_grapling_hook,
  prince_berseeker_warcry, prince_illusionist_figments, madruktorso_drainlife, edward_melee_01, prince_rogue_vicious_strike,
  prince_rogue_jump_kick).
- Atlas (read-only, via the old worktree before the coordinator's move notice): the SetStunState, SetKnockBackState,
  SetScareState and SetIsTargetable rows give the same addresses as IDA (0x3b9580, 0x3b7680, 0x3b96b0, 0x3b7a64). No
  disagreement found.

## Items

**1. SetStunState(ms, flag) (Character 0x3b9580 -> SM_SetStunState 0x3c5ffc): PARTIAL**
- Engine: `StatusStateOwnerV1::set_stun` in `features/status_states/status_states_v1.cpp`. Boss gate (IsBoss only, as in
  the original; mini-boss is a Lua-side gate), animation-row gate, timer started only when the stun bit is not set, persist
  bit, Idle->Stunned via event 50012 (only if the state accepts it), or immediate.
- Evidence (unit): test_stun_timer_and_gate (3000 ms runs, ends, Idle, can_act), test_stun_no_restart_and_boss_immunity.
- Evidence (unit): test_stun_events_follow_registrations (KnockedBack does not accept 50012, so the stun bit is set but the
  state does not change).
- Not done: no live caller; the Lua wrapper and its mini-boss check are not ported.

**2. SetScareState(ms, flag) (0x3b96b0 -> SM_SetScareState 0x3c6144): PARTIAL**
- Engine: `set_scare` (timer 44, bit 0x4, persist 0x400, event 50013). Event 44 returns Scared to Idle (CSScared registers
  44 -> Idle, IDA).
- Evidence (unit): test_scare_and_knockback (timer end returns to Idle; boss refused).
- Not done: same wiring gap as item 1.

**3. SetKnockBackState(great) (0x3b7680 -> SM_SetKnockBackState 0x3c5ea0): PARTIAL**
- Engine: `set_knockback`, with the bool meaning "great" (the Lua calls `SetKnockBackState(true)` for the big knockback in
  prince_archer_multihit, and no-arg means true in edward_melee_01). Starts no timer; `finish_knockback` ends it at clip end.
- Evidence (unit): test_scare_and_knockback (Idle accepts 50011 -> KnockedBack; finish returns to Idle).
- Not done: the anim clip (CharAnimTable +36 / +72 banks) is not ported; the existing b035 push module keeps its own path.

**4. IsBoss / IsMiniBoss (0x3a3158 / 0x3a3144 = CharAI+20 bits 2 / 1): PARTIAL**
- Engine: `boss_flags_from_ai_word_v1` (bit 2 IsBoss, bit 1 IsMiniBoss). Evidence (unit): test_boss_word.
- INFERENCE, not proven: the CharAI+20 word equals `dh2::data::AiProps::flags` (the AI row). No reader in the port tests
  this against a real boss row. Not done: no accessor is wired to live actors.

**5. Outcome tail: which bits call which state (F_ApplyResult 0x3b10b4): WORKED for the mapping, PARTIAL for effect**
- Mapping (IDA): 0x80 -> SM_SetKnockBackState (great = mask 0x100000; immediate = mask 0x18000000); 0x40 -> SM_SetStunState
  (property 140, or 185 when mask 0x1000; >>8 = ms; immediate false); 0x20 -> SM_SetScareState (property 143, or 187 when
  mask 0x4000; skipped when zero; immediate = mask 0x18000000); 0x100 -> slow, already in runtime_timed_buff_v1
  (unchanged).
- Evidence (unit): test_outcome_plan (the property selection and flags), test_apply_outcome_order (KB, then stun, then scare).
- CONTROL-FLOW FINDING (disagrees with the port comment in `features/combat/b035_session_push_effect_v1.hpp`): the port
  says F_ApplyResult gates the whole status block on the defender being Player and Idle. The decompile says otherwise. The
  Player/Idle test (vtable +40 = Character::IsPlayer, verified from the raw `_ZTV9Character` bytes: word 12 =
  0x003a49f0) guards only the dodge/block/injure bit setting. The dispatch that follows it (`if ((v11&2)) goto LABEL_92;
  goto LABEL_20;`) is outside that block and reaches the 0x80/0x40/0x20/0x100 tail for any defender. INFERENCE from the
  goto structure; needs a second reader before anyone relies on it. If it holds, ViciousStrike's 0x40 and JumpKick's 0x80
  should affect monsters in the original, which contradicts the B165 framing.

**6. Chance units (what the stun/push/fear/slow rolls compare): WORKED (answered from code)**
- `status()` in `port/game-data/combat_result.cpp` matches IDA `CF__CalcStun` / `CF__CalcPush`: chance = property (139 for
  stun, 137 push, 142 fear, 145 slow; magic 184/183/186/188) + 5*Level(attacker, prop 19) - resist - 5*Level(defender);
  success when that value is greater than `roll = (GetRandom(100)) << 8`. So chance is in 1/256 percent: a raw 25600 is
  100 %.
- Evidence (integration): raw 25600 gives outcome 0x40 on every roll; raw 0 gives no bit.
- OPEN: the authored table values are not checked here. The lead's "80+5L" only means 80% if the table stores 80<<8.

**7. B165 (ViciousStrike 0x40 and JumpKick 0x80 rolled but do nothing): PARTIAL**
- The state engine now exists and is unit-tested, so a stun outcome has a host state to land in.
- NOT wired: no CombatSession owner, no call from the source-hit path, no per-frame `advance`, no monster AI gate on `can_act`.
- B165 stays OPEN until the wiring lands.

**8. Integration: ViciousStrike stun changes monster state (animation + cannot act + boss immune): PARTIAL**
- Evidence (unit, real roll): `integration_vicious_strike_stun`. The real `dh2_combat_result` with mask 0x800 and a 100%
  stun chance gives 0x40. Through the outcome plan and owner, the monster cannot act for 3000 ms and then can. Zero chance
  does nothing. The boss rolls the bit but takes no stun.
- Not done: animation (no clip playback), real run, AI gate.

**9. Integration: JumpKick push changes monster state: DID NOT WORK (not attempted)**
- Only the knockback owner exists. JumpKick's roll and the 0x80 wiring into the live hit are not connected. Not verified.

## Open items for the lead
- Stunned -> Idle at timer end is an INFERENCE. No registration of event 43 was found, and the Stunned state's exit path
  (animation end or another event) was not located. The port returns to Idle at expiry. Verify before wiring.
- The F_ApplyResult Player/Idle reading in item 5 conflicts with `b035_session_push_effect_v1.hpp`. Needs a second reader.
- The "great" knockback bool: IDA passes the mask's 0x100000 as the first argument, so true = great, consistent with the Lua.

## Test harness notes
- `.local-inputs` in the worktree: 30 directory junctions plus 4 hard links for the file references found under
  `port/windows-foundation` (all created from the primary `.local-inputs`). The junctions must be removed with `cmd /c rmdir`
  only. `windows-source-floor-fixture` is an empty pre-existing directory, not mine.
- Build dir `C:/dh218/es-status-build`; scripts `C:/dh218/es-status-build.sh` and `C:/dh218/es-status-test.sh`.
