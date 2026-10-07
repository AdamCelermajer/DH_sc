# Owned SkillsTable to CancelSneaking adapter

`character_sneaking_tables.hpp/.cpp` supplies production-owned, stable `Tables32` views for the frozen source-derived CancelSneaking and CancelSkill kernels. It replaces consumer-side manual projections while retaining the full owned Skill data.

## Ownership and API

Construct `dh2::character::sneaking::SneakingTables` with `SkillTables::borrow()`. An empty Borrow throws `std::invalid_argument`. `view()` returns the immutable `Tables32`; `source()` exposes the pinned loader Borrow including all payloads, row names and field names. The adapter is noncopyable and immovable, so its view and projection addresses remain stable. Retain it until all Characters referencing that view and their synchronous callbacks finish.

All lists remain in source order. Their signed IDs point directly into the immutable pinned snapshot, without fallback, repair or filtering. Each Skill projection copies all 19 scalar words, preserving the source loader's complete 76-byte projection. Script, Icon and DisplayProps payloads remain owned and available through `source()`. The loader's zero header/pointer/padding projection convention remains unchanged; these zero pointer words are not native pointers to those payloads.

The adapter pins the snapshot after the original loader and input buffers are destroyed. Reload through the loader is denied while the pin remains. It does not own Characters, mutable property sheets, AI script vectors, provider contexts or skill VM instances. Those are caller-owned and must remain valid through source callbacks.

Usage:

```cpp
auto tables = std::make_unique<dh2::character::sneaking::SneakingTables>(owner.borrow());
character.tables = &tables->view();
auto status = dh2_character_cancel_sneaking(&character, &services);
```

CancelSkill uses that same view through `ai.owner->tables`. Keep `tables` alive while either operation may execute. A pin owns the original payloads, not the caller's Character/AI backing.

## Source behavior preserved

The adapter adds no gameplay branch. The frozen kernel still applies signed `Special_Sneak > 0`, selects the signed SkillTree index with fallback to authored list 3 for negative/out-of-range indices, scans ordered Flags bit 0x02000000, and passes the list slot to CancelSkill. Negative/invalid list references remain raw and are rejected by the native consumer's existing bounds contract when reached.

CancelSkill still distinguishes null script, authored Type 1, Active return value and the live script pointer reloaded before Pre. Player Buff deletion occurs before the source changed415 assignment; failed delivery does not mark it changed. No source property, animation, state or flags reset is introduced by this adapter.

The actual cache contains 36 lists and 127 Skill records, with no Sneak flag bit in any record. Its complete scan therefore produces no skill VM call. Synthetic records explicitly test the positive flag branch without presenting synthetic data as an authored Sneak configuration.

Original instruction evidence is reused, unchanged, from `../character-cancel-sneaking` and `../../../game-data/reference/skill-tables`. The new adapter is a native ownership projection; it has no separate original ARM32 wrapper to compare. The report explicitly sets `adapter_original_instruction_comparison=false`. Existing O2 instruction reports and gold are hash-bound dependencies, not new instruction counts.

## Audit

`reports/character-sneaking-tables-host-audit.json` records 9,211 checks, 521 cases, 153 ordered provider calls, eight Active calls, six Pre calls and seven guard checks, with zero AddressSanitizer, UndefinedBehaviorSanitizer or LeakSanitizer findings.

The isolated audit compares all list IDs and every scalar word against the pinned source; verifies all actual Script/Icon/DisplayProps payloads after input mutation; deletes the source loader from the synchronous IsPlayer callback; overwrites and deallocates all three input buffers; and verifies schema/names/payload lifetime. It covers every actual list, negative/oversized indices including INT_MIN/INT_MAX, signed sneak values, actual Type-dependent CancelSkill calls, null script, Buff/Pre provider failure, and a live script-pointer mutation in Active. Synthetic full records preserve embedded NUL strings, raw boolean bytes 255/128/254 and negative vector words; ordered first flag selection and fallback list 3 are checked.

The IsPlayer, Buff, Active and Pre callbacks in this audit are declared fixtures. Full buff storage mutation/destruction and skill VM execution remain required production providers. A callback may destroy the original loader because the adapter retains its snapshot; destroying the adapter or live Character/AI during a borrowed call is outside the valid ownership contract.

The runner hashes new sources, frozen dependencies, original/O2 reports, gold and actual cache inputs before and after compilation/execution. It compiles only an isolated executable and never rebuilds a central DSO.

Reproduce from the repository root:

```powershell
python port/level-world/tests/character_sneaking_tables_host.py
```

The runner uses WSL g++ with AddressSanitizer/UndefinedBehaviorSanitizer and leak detection. A future main-linked target uses `tests/character_sneaking_tables.cpp`, links game-data and the world CancelSneaking/adapter implementations, and takes three positional arguments:

```text
.local-inputs/skill-tables/skills_pyarray.bin
.local-inputs/skill-tables/skills_pyarraynames.bin
.local-inputs/skill-tables/skills_pystructnames.bin
```

No existing production files, CMake files, renderer files, source gold or frozen reports were edited for this stage.
