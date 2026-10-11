# B094 report: "she also appear here not targetable if that's supposed to be?"

Branch fix/b094 (from v20.1/integrate dfc67d8d). Verdict: INTENDED, no behaviour change. Only a regression test was added.

## Who is "she"
No female humanoid NPC is visible in shot-10..24. The non-targetable friendly figures near the hero are (a) the robed white-haired escort priest Rene (WanderingPriest, CharacterTable row 419, AI 50, script "rene"; shot-10 bottom-left, shot-11 top, shot-7/8) and (b) the Faery Celeste (white winged figure, shot-10/13 bottom-left; she is the female one the user most likely means). Both are friendly companions. Rene is male in the video and the dialogue; the identification of "she" is an inference.

## Evidence
Video (P1 z_Zky7qQdYs, scratch `.local-inputs/claude-v20.1/fix094/vid/`, s2.png t=1185-1255 s, s3.png t=1244-1304 s): after the serpent dialogue Rene (and Celeste) follow the hero in gameplay (s3 frames 2, 3, 8, 11: Rene beside/behind the hero). OBSERVED: no target ring, no marker and no action-button change on Rene or the Faery in those frames. These are sparse 4 s samples, so absence of a ring is observed only at the sampled frames.

IDA (pseudocode-all.c):
- `TargetList::_IsCharacterValid` 0x4a1ab8 (line 300280): a character is valid for mask bits 0x01 (enemy: not summoned and AI_IsEnemy), 0x08 (friendly SUMMONED only), 0x10 merchant, 0x20 cleaner, 0x40 `Character+762`. A plain friendly escort passes none of them.
- `Character::UpdateObjectOfInterest` 0x3abb9c (line 126080) uses mask 89 = 0x59 (enemy | summoned friend | merchant | +762). Attack targeting uses the enemy bit.
- `Objective_TalkToNPC::Register` 0x47db34 sets +762 only for the NPC of a registered TalkToNPC objective (B088 report, coordination/v20.1/B088-report.md in DH_wt/fix088).
- Rene (AI 50, script "rene") is a follower via AI_SetMaster (coordination/integration-lead/handoffs/companions.md). Faery is a skin/follow companion (AISFaery), not a TargetList enemy/merchant.

Our behaviour: Rene/Faery are loaded `--animation-only WanderingPriest` / `DefaultFairy` (not combat-registered); the context-button NPC registry (main.cpp ~6548) admits only NPCs whose CharacterTable row is registered by `QuestRuntimeV1::talk_objective_registered` (B088, the +762 rule). New test: no quest row (accept/objectives/end) has a TalkToNPC objective on row 419, and `talk_objective_registered(419)` is false at start. Passes with the original v2quests table (`quest_runtime_v1_tests` rows=64, all passed).

## Match statement
- IDA: our behaviour matches (friendly non-summoned, non-merchant NPC without a registered TalkToNPC objective is neither a target nor an OOI).
- Video: consistent with it (no ring/marker on the following Rene/Faery in sampled frames); not a frame-continuous proof.

## Not verified
- Which character the user means by "she" (no female NPC identified in the shots).
- Merchants (bit 0x10) and the quest "end" TalkToNPC objective (state 9) are still not candidates in main.cpp (questTalkOids only gathers accept and objectives); that is B100/B088 territory, not changed here.
- Real-EXE run not done: no behaviour change. 
