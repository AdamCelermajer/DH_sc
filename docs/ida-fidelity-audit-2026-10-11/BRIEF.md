# IDA → Windows reconstruction gameplay fidelity audit (2026-10-11)

Audit only. **Do not edit any source, build or tracker file.** Write only your own
output file in this folder.

## Goal

Map each original IDA function in your batch to where the same gameplay logic lives
in the Windows reconstruction, and check that the reconstruction is **gameplay-faithful**.
There is no 1:1 function mapping: the port deliberately does not copy the original
object graph. What must match is gameplay consequence: numbers, formulas, ranges,
radii, angles, timings/cooldowns, probabilities, gates/conditions, ordering of
effects, state transitions, and which data table/field drives a value.

## Inputs

- Your batch list: `docs/ida-fidelity-audit-2026-10-11/batches/<batch>.tsv`
  (columns: address, byte size, pseudocode path, demangled name).
- IDA export root: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/`
  - `pseudocode/<path from tsv>` per function; `pseudocode-all.c` for grep.
  - `assembly-functions.asm` when pseudocode is ambiguous (float constants, offsets,
    soft-float `__aeabi_*` calls, branch order). IDA prototypes/types are inferred.
  - `xrefs.jsonl` for callers/callees; `strings.jsonl` for property/table names.
- Original authored data: `recovered/assets/source-data` and data parsed under
  `port/game-data` (tables/XML often hold the real numbers — follow the field the
  original reads, then check the port reads the same field with the same units).
- Reconstruction: the Windows build is `port/windows-foundation` (+ `features/`), which
  links `port/level-world`, `port/game-data`, `port/engine-animation`, `port/engine-ui`,
  `port/engine-audio`, `port/level-loader`, `port/script-runtime`, `port/physics-backend`.
  Exclude `build*`, `third_party`, generated data.
- Prior static review (reuse, don't redo; but re-verify anything you rely on):
  `docs/act1-ida-audit/checklist.csv` and `review-batches/`. Known bugs:
  `docs/BUGS-AND-IMPLEMENTATION.md`, `docs/RESOLVED-BUGS.md`.

## Method

1. Skim the whole batch and group functions by behavior. Trivial functions (getters,
   ctors/dtors, thunks, RTTI, serialization boilerplate, debug/console, multiplayer/
   network-only paths) get a one-line group entry, not individual analysis.
2. For every gameplay-consequent function: read the pseudocode (assembly for constants),
   find the port equivalent with grep, and compare logic. Cite `file:line` on both sides.
3. Prioritize depth on things a player would notice. Spend your effort there instead of
   cataloguing boilerplate.
4. Never call something faithful just because a similarly named function exists. Never
   call something divergent from an inference — say `unclear` and what evidence is missing.

## Output: `docs/ida-fidelity-audit-2026-10-11/<batch>.md`

1. **Summary**: counts per status, top 5 risks.
2. **Findings** (most severe first). For each: title; severity (`high` = player-visible
   gameplay difference, `medium` = edge case/rare branch, `low` = cosmetic/unlikely);
   IDA address+name with the relevant pseudocode lines and constants; port `file:line`
   and what it does instead; concrete gameplay consequence; confidence; suggested fix
   (described, not applied).
3. **Mapping table**: `IDA address | IDA function (or group) | port location(s) | status | note`.
   Status ∈ `faithful`, `divergent`, `partial`, `missing`, `unclear`,
   `n/a` (engine/boilerplate/network/debug with no gameplay consequence),
   `intentional-adaptation` (documented PC/port decision — cite where it is documented).

Your final message back: the summary + findings list (short), and the output path.

## Run rules

- Do not spawn subagents; do the batch yourself.
- Write your output file incrementally (create it early, append findings as you go) so
  partial work survives an interruption. If your output file already exists, continue it.
- Resumability: keep a `## Progress` section at the top of your output file with
  `last_address_done: 0x...`, `groups_done: ...`, `next: ...`, updated every ~20
  functions or after each finding. A resumed agent reads it, skips what's done, and
  continues from `next` without redoing or deleting earlier content.
