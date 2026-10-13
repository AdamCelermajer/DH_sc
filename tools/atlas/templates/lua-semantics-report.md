# Lua semantic reference resolution report

## Scope and expected atlas result

The resolver consumes the existing call, script, reference, and table ledgers.
It resolves an expression only when an exact Lua literal/call contract maps to a
unique Android source row, a registered schema member, or a source constant.
Callback keys, enum values, schema labels, and duplicate lexical mentions get a
reasoned `NOT-NEEDED` result. Missing or multiple targets stay `UNRESOLVED`.
The resolver preserves reference IDs and adds a `semantic_resolution` evidence
object for each result it owns. It makes no game behavior change.

## Investigation evidence

The original source is the Android text file
`.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac`.
`RegisterAnims` at lines 231–237 registers the exact strings
`swampking_idle`, `swampking_intimidate`, `swampking_dive`,
`swampking_emerge`, `swampking_fall`, `swampking_fall_idle`, and
`swampking_fall_idle_to_idle` through `GetPyOID("AnimDict", name)`.
`PlayAnim` uses the same exact lookup at lines 454 and 488. The source calls
`GetPyStruct("CharacterProperties", "RangeMinDistance"/"RangeMaxDistance")`
at lines 40–41 and later reads LevelMax/LevelMin/LevelOffset at lines 510–512.
`GetPyCst("AIStates", "Skill")` at line 217 reads the current AI state. The
script registers `OnDied` at line 726 and `OnEndOfAnim` at line 768.

This is internal atlas infrastructure with no new game screen or animation.
The relevant visual consumer evidence is reused from
`coordination/v21.7/B155-B156-HANDOFF.md`, section 5: P1 video, observed
18:53 Mud Lord plate, 18:56 close-range bites, 19:08–19:09.5 dive, and 19:12.5
emerge elsewhere with a blue splash. Those observations establish the visible
animation/encounter context; they do not prove a resolver result or new runtime
playback. The atlas invariant is that these authored names lead to the correct
source record, while callback/enum/schema operands remain searchable without
being presented as files.

Android IDA/recovered-source evidence:

- `pseudocode-all.c` `LuaScript::_GetPyOID` at `0x0037f5fc` passes both exact
  strings to `PyDataArrays::GetOID` at `0x004bd640`. Existing source handoff
  evidence in `tools/atlas/references.py` maps public `AnimDict` to the Android
  `animations_dictionary#group0` names stream.
- `LuaScript::_GetPyStruct` at `0x0037f4a8` uses the same registered name lookup.
  `source_process_array_registration_v101.hpp:234` registers
  `CharacterProperties`, 224 members, getter `0x004af110`; the ordered member
  names are retained in
  `source_process_compiled_members_v121.hpp`.
- `LuaScript::_GetPyCst` at `0x0037f354` calls the generic
  `PyDataConstants::getConstant` at `0x004c4bdc`, which performs exact
  namespace/member lookup. The Android `ai_pycst.bin` contains the proven pairs
  `AIStates.Skill = 6`, `AITargetList_CharacterFilter.Player = 128`,
  `AITargetList_ObjectFilter.NONE = 2`, and
  `AITargetList_Sorting.ClosestFirst = 1`.
- `LuaScript::_AddToVFTable` at `0x0037ec70` receives callback key/function
  pairs. `AISExternal::OnDied` at `0x003dd440` dispatches the exact `OnDied`
  string; `AISExternal::OnEndOfAnim` at `0x003dccd0` dispatches the exact
  `OnEndOfAnim` string. The CharAI event caller at `0x003cbb34` routes the
  source event slots to those callbacks. Other exact callback keys in scope are
  OnInitFinal (`0x003dce44`), OnTargetDied (`0x003dce24`),
  OnTargetInCloseRange (`0x003dcdb0`, guarded by InitVCB bit `0x10`), and
  OnTerminate (`0x003dce34`). Only these literal first arguments to
  AddToVFTable are classified; similarly named values elsewhere are untouched.
- `PlayMusic`, `PlaySound`, and `StopSound` native signatures are recorded in
  `script_links.py`: their documented scalar argument positions are type
  checked before classification. `DoSkill` slot operands remain semantic skill
  choices and are not classified as scalar metadata.

No native ID or table index is sourced from an iOS binary. Android source rows
are used only with Android Lua calls and Android native contracts. The
independently proven `script_links.v1` edge is rechecked against the current
Android table row before the outer symbolic expression is promoted.

## Outcomes and known uncertainties

On the current Android source snapshot, the focused Mud Lord outcomes are:

- `GetPyOID("AnimDict", "swampking_emerge")` points to
  `animations_dictionary#group0[1321]`, exact row `swampking_emerge`.
- `swampking_fall` and `swampking_intimidate` resolve through their exact
  animation dictionary rows. Duplicate nested literal mentions are
  `NOT-NEEDED` only when their exact enclosing expression resolved.
- Exact GetPyCst namespace/member pairs present in Android `ai_pycst.bin` are
  `NOT-NEEDED` as resources and record their signed numeric values. This also
  resolves nested target-list constants such as Player, NONE, and ClosestFirst.
- Exact known AddToVFTable callback keys are `NOT-NEEDED` as resources; their
  evidence names the native registration and event dispatchers.
- `CharacterProperties` members are returned as `table-field` targets with
  exact member index. Duplicate outer `GetProp` tokens are `NOT-NEEDED` only
  when the exact nested `GetPyStruct` call on that source line resolved.
- Proven native `Sounds` member edges resolve to exact rows in the Android
  `sdd_dungeon_hunter_2_iphone` names table. This establishes the catalog row,
  not an available or audible WAV file.
- Exact first-string global Android PlaySound, PlayMusic and StopSound operands
  also reach their unique Sounds rows when no enclosing symbolic record was
  extracted. StopSound's member lookup is at `0x0037e730`; the receiver and
  first-argument role are required, and computed/missing names remain gaps.

Uncertainty remains for escaped or computed Lua strings, aliases not covered by
the existing constant-flow resolver, any absent or duplicate source row, and
semantic consumers outside the explicitly documented API positions. Such cases
remain unresolved. The `swampking_core` AI-table Script-to-file join is owned by
the separate file-reference resolver, so this module does not claim it.

## Verification

Focused checks were defined before coding: resolve the reported exact
animation/schema/enum/callback cases; confirm an absent row, a duplicate exact
row, an unregistered constant namespace, and an absent constant member remain
unresolved; preserve meaningful `DoSkill` slot operands; run the resolver twice
and compare complete outputs.

Isolated verification used the live atlas source ledgers and the private bundled
CPython runtime. It passed the focused callback, all four cited constant values,
nested animation, and CharacterProperties assertions. The exact-row miss,
duplicate-row, unknown namespace, and unknown member fixtures remained
unresolved; the DoSkill slot remained unresolved as a semantic operand; and two
consecutive calls produced byte-equivalent in-memory reference rows.
No atlas outputs were regenerated and no UI/gameplay runtime test was performed
in this bounded module task.
