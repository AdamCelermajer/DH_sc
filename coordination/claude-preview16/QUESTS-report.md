# Preview 16 QUESTS report (quest runtime + Quest Log tab)

Status: IN PROGRESS (skeleton). Branch `p16/quests`, worktree `DH_wt/p16quests`, based on `p15/integrate`.

## Scope (from P16-BRIEF / PLAN)
1. Data load from original v2quests pyarray tables (all acts, all rows).
2. State machine per original Quest::Update / SetState / UpdateCompleted / GiveRewards.
3. Event intake: kills (template), zone entry, NPC talk, item pickup, through `raise_quest_event`.
4. Persistence: CQPG v2 objective counters (schema v4), no double rewards on reload.
5. Rewards: XP/gold via CharacterState; 'QUEST COMPLETED' panel and 'NEW QUEST' banner.
6. Quest Log tab (5th of six) with menu_QuestLogSheetNEW provider; PC mouse hit tests.
7. Tests + quiet-batch verification.

## Evidence (investigation)
- Pending.

## Changes
- Pending.

## Tests run
- Pending.

## Verification (EXE, quiet batches)
- Pending.

## Package files required
- Pending.

## Verifier script
- Pending.

## Open risks / not verified
- Pending.
