# Lane 10 — Player damage, statuses and recovery

Source delivery is limited to this handoff and `port/level-world/player_injury_runtime_v7.cpp`. The existing retained Player FSM/death, combat-status, potion and HUD owners already cover most of the lane; this pass corrected one injury-table fallback and recorded the remaining integration gap instead of adding another Player authority.

## Change

`PlayerInjuryRuntimeV7::animation_table` now returns the actual selected Character property row (`resolved[2]`). The prior fallback silently substituted CharAnim row 17 when the selected row was invalid. Original `CharStateMachine::SM_SetInjureState` sets the 3000 ms gate, obtains `Character::GetCharAnimTableId`, and exits without changing the animation if the row is out of range. The existing `injure_animation` callback now receives that actual row and reports it absent through its existing `found=false` contract.

## Existing owners and root wiring

- Injury state kernel: `player_set_injure_v7` preserves the source 3000 ms gate, Injured field lookup, stance offset, and event 50010 or direct state 11 transition. The production campaign FSM currently calls this kernel directly in `source_campaign_character_fsm_v101.cpp` (around line 555); `PlayerInjuryRuntimeV7` is not currently constructed by a production caller. Root should keep one retained owner and connect its state 11 focus/blur/event callbacks and `tick()` to that same player's FSM/update path if choosing the wrapper.
- Death: `renderer_player_death_v56.inc` already routes actual player event 2 through CharAI `OnDied` 0x3d1000, the selected AIS virtual, AI_SetDead 0x3d6cdc, SM_SetDeadState 0x3c58c8, retained State12 cleanup, timer publication, reciprocal aggro cleanup, and the existing skill/spell owners. Its positive `GroupInfo::OnDied` service still fails explicitly because the actual player GroupInfo owner is unavailable here; the original nullable branch is supported. Wire an actual group owner before claiming death complete for grouped players.
- Combat/statuses: the existing `Character::F_ApplyResult` sources at 0x3b01b8 and 0x3b10b4 lead into the retained combat application/status services. `hit_player_tail_v7` owns the original half-health tutorial/sound and low-health re-arm threshold. Keep these on the same Player properties and retained low-health byte; do not add a second pulse or status clock.
- Recovery: `gameplay_use_potion` already gates on alive player, actual potion inventory, and missing health or mana, consumes one owned potion, increments the source counter, and applies the existing regen service. It reports the unsupported trophy continuation after preserving the consumed-item prefix. HUD currently invokes this function through its player binding.
- Recurring effects: `player_recurring_effects_v7` advances the same Player skill session timer store once per native update and reports owner/session failures. `model_renderer.cpp` currently calls it in the player update. Preserve the once-per-frame call and its original script-blocked input.
- Critical-health pulse: the original HUD pulse/timeline code is in `original_ui_session.cpp`; root must keep its HUD update driven by the live Player HP/max HP and application delta.

## IDA evidence

Use `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so` only. Injury is `CharStateMachine::SM_SetInjureState` 0x3c5d84; its caller sites include `Character::F_ApplyResult` at 0x3b01b8 and 0x3b10b4. `CSInjured::OnEvent` 0x3c0044 and `OnUpdate` 0x3c0040 are literal BX LR; focus/blur are 0x3c33e8/0x3c4ba4. Injury time is Character fields +0x14fc, initialized to -1 at 0x3aa3fc / 0x3aa50c and decremented in Character::Update at 0x3abe98. Death references are `SM_SetDeadState` 0x3c58c8, `CharAI::OnDied` 0x3d1000, and `CharAI::GroupInfo::OnDied` 0x3d2628. The handoff's conclusions were checked against `assembly-functions.asm`, with IDA pseudocode used to identify fields and branch intent.

Source integration is not device/gameplay verification. The positive player GroupInfo death branch remains a concrete blocker; the current potion trophy continuation is also explicitly unsupported after its true source mutation prefix. Root owns final same-owner wiring and integrated runtime acceptance.
