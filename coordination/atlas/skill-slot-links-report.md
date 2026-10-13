# Lua actor skill-slot joins

## Original consumer evidence

Android IDA `Character::_DoSkill` (`0x003b8bd8`, `pseudocode-all.c:134604`)
admits a numeric first argument, converts it to an unsigned slot, obtains the
receiver's character skill list, and requires the slot to be inside that list.
`GetCharSkillListId` (`0x003bc5c0`) and `GetCharSkillList` (`0x003bc5fc`) select
the character's list. `GetCharSkill` (`0x003bc784`) translates the zero-based
slot through `SkillListTable.list_entries` into a `SkillTable` row.
`AI_UseSkill` (`0x003d8868`) and `AI_BeginSkill` (`0x003d86bc`) still control
type/usability admission and execution. A linked slot does not establish that
the call ran or that a skill succeeded.

## Expected atlas behavior and implementation

`skill_slots.resolve` joins exact, literal global Android `DoSkill` arguments
to the character rows that declare that exact AI script through `AITable.Script`.
Every declared consumer must have an indexed list entry. One common target is
resolved; differing targets across consumers are ambiguous. Missing consumers,
computed slots, other receivers, imported helpers and iOS native contracts
remain unproven. Source call/reference IDs are preserved, including separately
extracted literal and expression rows.

For Mud Lord, the source chain is CharacterProperties row 358 -> AITable row 63
`Script=swampking_core` -> SkillTree row 33 `SwampKingSkills` -> list entries
`[106,107,108,109]`. Thus literal slots 0, 1, 2 and 3 identify SwampKingAttack1,
Attack2, Attack3 and Attack4 respectively. The script contains 12 such calls.

## Verification and limits

Focused checks cover these source joins, preservation of source IDs, repeated
resolution, shared scripts with divergent skill lists, out-of-range slots,
missing list rows, dynamic expressions, object receivers and iOS isolation.
The atlas source audit and graph checks verify canonical rows and endpoints.
Visual gameplay evidence does not apply to this static index change; original
consumer evidence and the existing B155/B156 Mud Lord investigation define its
boundary. No gameplay, input, cooldown or native fallback behavior is changed.
