# Constants-backed Level and monster initialization

This separate test replaces the frozen Level audit's supplied cap callback with
the genuine parent-owned native PyDataConstants loader/table lookup. Frozen
Level source, tests, original gold and prior reports are unchanged.

`tests/character_level_constants.cpp` creates an owned constants table, loads
exact cache `data/pydata/ai_pycst.bin` followed by `design_pycst.bin`, and binds
`dh2_script_constants_lookup` directly as LevelModel32's persistent design
service. There is no wrapper returning a supplied level cap. It verifies the
actual CharacterDesign/MaxLevelDVeryHard=100 and AIStates/Attack=5 entries.

The runner verifies both byte streams against bundled assets and original CST1
loader records 0 and 5. The source reload metadata is respectively
`{consumed=1042, assignments=58, groups_complete=7, source_name_stop=0}` and
`{3999,152,13,0}`; the test compares all four returned native words. The original
constants corpus SHA256 is
`a0da51c378a8e651f7f0dcca9b3c9a5e09651c978ed662b3015a8cb609b3529f`.
The source application global load order is not inferred from this deliberately
chosen two-file test order; neither file overlaps the relevant cap/state keys.

The test reads the unchanged CHL1 Level oracle corpus and selects its 870
records whose two design results are both 100. Every selected record starts
from the original recorded four-sheet before state, including the recorded
carry-state fixtures, and compares the resulting 896 words and native GetLevel
against original instructions. It skips 266 records with other synthetic caps,
which cannot be supplied by this genuine cache table. It checks 779,520 sheet
words, 1,748 ordered debug-service callbacks, seven malformed guards and zero
mismatches. Constants lookup is called directly and does not manufacture a
recorded callback trace: its original loader/getConstant behavior is separately
proved by the parent constants corpus and exact runtime DSO binding.

Ten real cache monster.lua and _commons.lua initialization sessions run against
the actual main-runtime VM and alias/fixed/design bindings. Each reads the genuine
GetPyCst table, calls the frozen Level/GetProp/class/property chain, and compares
Level mutations with direct native execution. Eighty VM checks pass. The selected
host player receiver is a borrowed Knight properties object whose value is read
through native GetLevel; identifying that object as the live session host remains
an explicit test boundary. World position/difficulty/range, decoded metadata
adapters and DebugSwitch effects remain explicit fixtures/services. This test
does not claim their producer implementation or live monster gameplay.

The host runner compiles only the new test and frozen Level implementation and
links the current world, game-data and script-runtime DSOs under ASan/UBSan.
It binds every source/compiler input, exact scripts/constants/cache, executable,
and the actual dladdr-resolved dependency hashes before and after execution.
Dependency compiler provenance is not claimed by this isolated runner. There
are no sanitizer findings. This is a main-dependency host composition proof;
there is no new combined packaged-ARM64/VM or full-frame/full-game parity claim.

Reproduction from repo root:

```powershell
$env:PYTHONDONTWRITEBYTECODE='1'
& 'C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe' port/level-world/tests/character_level_constants_host.py
```

Report: `reports/character-level-constants-host-audit.json`.
The parent can add a central `character_level_constants_audit` target using only
the new test CPP linked to world/game-data/runtime/dl. The executable accepts
CHL1 corpus, commons, monster, data-assets directory, ai constants and design
constants. Once centrally built, invoke the same runner with
`--main-linked /home/adampalace/dh2-world-build/character_level_constants_audit`.
It writes a separate `character-level-constants-main-linked-host-audit.json`
and leaves the isolated evidence intact. The parent should independently bind
central compiler commands/inputs.
