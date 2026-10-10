# Full IDA-to-source audit — 2026-10-08

## Purpose and baseline

Read-only audit to map the complete supplied IDA inventory to the current reconstruction, find missing or disconnected behavior and cross-boundary callbacks, and leave runtime status separate from static evidence. This is not a promise that a static pass can discover every future defect.

Source snapshot marker: `HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c`; the working tree was already dirty with 4,227 file-level status rows when this audit began. The audit reads the working tree. Record a source file's current hash when citing it and flag any file that changes during review. Agents must not edit game source, build, test, install, or launch an emulator. Each agent writes only its own report under this directory.

IDA root: `.local-inputs/ida-apk-export-2026-10-07/`

Inventory size: 31,725 `libDungeonHunter2.so` records, 3,728 `libStormGLOFT.so` records, 50 `libnativeinterface.so` records, and 2,310 `classes.dex` methods. Native totals are 35,503. Include failed decompilations, imports/thunks, and DEX methods with explicit dispositions. Existing function-inventory labels are candidate leads, not semantic parity evidence.

## Primary coverage lanes (non-overlapping function-number ranges)

Every record is assigned exactly once. Primary lanes must classify every row in their manifest and deeply compare nontrivial, relevant bodies to the current source. For external imports, exact thunks, identical-body clones, compiler/runtime helpers, and failed decompilations, record the evidence and disposition; never call a textual-search miss a missing implementation. Follow callers/callees and group findings by subsystem within each range. Use assembly and xrefs when pseudocode, ABI, order, ownership, or indirect calls matter.

| Lane | IDA input | Function numbers |
|---|---|---:|
| 01 | `libDungeonHunter2.so` | 0–1865 |
| 02 | `libDungeonHunter2.so` | 1866–3731 |
| 03 | `libDungeonHunter2.so` | 3732–5597 |
| 04 | `libDungeonHunter2.so` | 5598–7463 |
| 05 | `libDungeonHunter2.so` | 7464–9329 |
| 06 | `libDungeonHunter2.so` | 9330–11196 |
| 07 | `libDungeonHunter2.so` | 11197–13062 |
| 08 | `libDungeonHunter2.so` | 13063–14928 |
| 09 | `libDungeonHunter2.so` | 14929–16794 |
| 10 | `libDungeonHunter2.so` | 16795–18660 |
| 11 | `libDungeonHunter2.so` | 18661–20526 |
| 12 | `libDungeonHunter2.so` | 20527–22393 |
| 13 | `libDungeonHunter2.so` | 22394–24259 |
| 14 | `libDungeonHunter2.so` | 24260–26125 |
| 15 | `libDungeonHunter2.so` | 26126–27991 |
| 16 | `libDungeonHunter2.so` | 27992–29857 |
| 17 | `libDungeonHunter2.so` | 29858–31724 |
| 18 | `libStormGLOFT.so` | 0–1863 |
| 19 | `libStormGLOFT.so` | 1864–3727 |
| 20 | `libnativeinterface.so` and `classes.dex` | 0–49; 0–2309 respectively |

Ranges are inventory partitions only; semantic review follows the IDA call and data references beyond a range and records cross-lane edges. Rows in all function-index files should be covered exactly once.

## Cross-cutting lanes (intentional overlap)

| Lane | Scope |
|---|---|
| 21 | Script, SWF/AS, JNI, and native callback closure: actual level/entity/script assets through init and first updates, callback name/address to registration, handler, and exact live owner. Search the full project and assets; include the Stage 17 `MarkAsFlying`, `RegisterAnim`, `GetPropHP`, and `GetHP` miss as a regression seed, then audit other paths rather than stopping there. |
| 22 | Data/resource/serialization and lifecycle closure: serialized rows and asset/resource loads through parse, ownership, runtime consumer, failure/rollback, teardown, virtual dispatch, and object identity. Include DEX and script/data assets where they cross native paths. |
| 23 | Adversarial review: compare earlier runtime failures and the 1,156-check behavior audit to the primary evidence; challenge unsupported `implemented`, `missing`, and `out of scope` claims; identify unexamined neighboring branches and false confidence from stage-level checks. Do not modify the existing checklist during this pass. |

## Required report schema

Each primary lane writes `lane-XX.md` plus `lane-XX.csv`, unique to that worker. The CSV needs one row per assigned IDA record/method: library, function number, address, name/demangled name, decompilation status, body category, callers/callees or relevant xref, exact source location or reason none was found, comparison status (`matched`, `partial`, `disconnected`, `missing`, `unclear`, `import/thunk`, `clone`, `failed-body`, `out-of-scope`), evidence, confidence, and whether integrated runtime acceptance exists. Use `not_run` unless evidence proves otherwise.

Cross-cutting lanes write `lane-21.md` through `lane-23.md` and a separate edge/findings CSV. Every finding must state the failing or unverified path and cite both IDA and source evidence. Distinguish source absence from a disconnected implementation and from runtime-only uncertainty. Keep reports bounded and link repeated identical body hashes rather than duplicating long pseudocode.

## Completion gate

Do not call the audit complete until the merged coverage counts equal all 37,813 function/method records exactly once (with zero missing or duplicate IDs), every failed decompilation and cross-boundary edge has a disposition, every negative claim has a scoped source search, and the unresolved list names what could not be established. Static findings do not count as gameplay verification. Fixes and runtime acceptance happen in a later integration pass.
