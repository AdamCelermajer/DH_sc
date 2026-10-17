# CharAI::AI_IsInCombat source caller

`character_ai_in_combat.{hpp,cpp}` reconstructs the 112-byte caller at original
ELF `0x3d4bc4`. Original aliases/ranges/hashes are pinned in
[`original-functions.json`](original-functions.json).

## Exact call and return order

1. Capture the input AI identity and call `AI_HasAggro` (`0x3d49f0`).
2. If false, call `AI_IsAggroed` (`0x3d4a00`) on that captured AI.
3. If false, read the current owner and query its embedded state machine's
   `SM_IsAttacking` (`0x3c02d0`).
4. If false, read owner again and query `SM_IsUsingSkill` (`0x3c02e8`).
5. If false, read owner again and tail-call `SM_IsCasting` (`0x3c0334`).

The first four truthy values return canonical1. The final Casting tail returns
the raw source word. There is no Block predicate, target-distance calculation
or cached combat snapshot in this caller.

The portable State holds logical identities, not original 32-bit class memory.
The five callee bodies and live aggro/FSM storage are explicit borrowed services.
Their completed effects survive later failures. Missing services, zero identity
on a taken path, thrown providers and invalid aliases return port statuses;
they do not fabricate source query results. Alignment/range/overlap preflight
precedes typed reads. Context, state, result and every exposed owner stay live
on one owning thread; nested invocations require independent outputs.

## Verification

```powershell
python port/level-world/tests/run_character_ai_in_combat_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --output port/level-world/build/character-ai-in-combat-root/host.exe --report port/level-world/build/character-ai-in-combat-root/validation.json
```

Nine cases execute the original caller instructions and compare final return,
fresh owner and ordered call arguments with compiled source. All short circuits,
noncanonical truthy words, raw Casting tail and callback owner changes match.
Eight guards cover null/alias/alignment, missing providers, null owner on
taken/untaken paths and retained effects after failed/thrown providers.
Root and independent review runs pass with zero mismatches.

Supporting original ranges are byte evidence, not additional newly rebuilt
bodies. The oracle supplies their return words explicitly. This is a host
component comparison, not game emulation or an Android page-size test.
Native actor ownership/full AI integration is separate work.
