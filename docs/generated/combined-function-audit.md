# Combined original-function and evidence audit

Current branch `reconstruction/android17-irrlicht-rebuild-2026-10-03`, HEAD `99780b4bee0f6044c195602cf71f55a58d50d05c`; public Adam evidence pinned to [`45c5348e8076`](https://github.com/AdamCelermajer/DH_sc/tree/45c5348e807607a2825211bb8f26248067ba9106). Generated from the shared working tree; this generator records repository evidence and does not itself run a build or emulator.

## Mapping counts

| Layer | Count | Meaning |
|---|---:|---|
| DH2R root maps before evidence import | 9 manifests / 135 records / 133 unique starts | The nine original root manifests; all starts match function-index. |
| Adam complete maps at pinned commit | 137 manifests / 2210 records / 1455 unique starts / 1454 unique symbol strings | Nested evidence snapshots repeat records; addresses match the ELF index. |
| DH2R maps after evidence restore | 137 manifests / 2210 records / 1455 unique starts | Includes the imported nested Adam evidence manifests; not new DH2R implementations. |
| Original ELF function ranges / names | 31,018 / 31,021 | Body-start denominator includes support and third-party functions; names can alias ranges. |
| DH2R root manifest flags before import | 86 `port_symbol` / 33 `use` | Mapping metadata, not counts of finished functions. |

**The 99.989968% `.text` number is byte accounting for recovered disassembly, not rebuild coverage.** The 1,455-row ledger proves only that those unique ELF starts appear in source evidence maps.

## Current Android checkpoint

The independently tested root APK is **9bf6d312**; see [NATIVE-BUILD-CHECKPOINT-2026-10-04.md](../NATIVE-BUILD-CHECKPOINT-2026-10-04.md). It ran on API 37/Android 17 x86_64 with 16 KiB pages and passed movement, attack-hit, lifecycle, freeze/resume, and fan override/rejection/restoration checks. The three hits reduced HP `12160→11556`. This is distinct from the older inherited Adam 4f5b7d11 checkpoint and b4493ed5 candidate; their artifact-bound results are historical and are not being attributed to 9bf6 or subsequent dirty source edits.

## Subsystem evidence

| Subsystem | Files / records / unique starts | Source/testing notes | Android/game boundary |
|---|---:|---|---|
| `asset-payloads` | 1 / 30 / 30 | 18 complete animation accessors + 8 typed search bodies (26 complete); mesh construction/GPU ownership has evidence only. Type-1 geometry is inspectable, but the original runtime constructor rejects it. 271,970 ARM32-vs-ARM64 comparisons, zero mismatches; complete-cache mesh/key audits and host sanitizer probes are documented. | ARM64 builds exist; this is not full render/GPU lifecycle integration. |
| `engine-animation` | 11 / 374 / 198 | Static/dynamic transform sampling, event tracks, registration and two-slot playback. Root `use` entries are evidence references; 374 records include nested snapshot duplication. Docs record bounded static 11,656, dynamic 117,030, extension 9,376, event 30,775 comparison and 26,228 bank-source cases; scopes differ. Full-bank original pose parity remains unproven. | The inherited Adam b4493ed5 candidate passed build/assets and movement/lifecycle/bank checks with combat incomplete; the independently tested root 9bf6d312 Crypt checkpoint is reported separately above. Neither proves full-bank original pose parity. |
| `engine-math` | 1 / 20 / 20 | 20 mapped physical starts (19 vector/quaternion bodies plus matrix-to-Euler dependency) are documented. 21,477 ARM32-vs-ARM64 comparisons, zero mismatch; all 1,704 instruction addresses in those routines exercised under the stated helper/libm model. | Reusable numeric dependency, not full engine/game integration. |
| `engine-resources` | 1 / 36 / 36 | 35 complete reader/Collada-accessor bodies plus the whole-buffer File::Init branch; do not count all 36 manifest rows as complete bodies. 10,449 original-vs-ARM64 reader comparisons, zero mismatches; 2,577,206 relocation fixups across 2,901 BRES images; host corruption/sanitizer tests documented. | ARM64 component/build evidence; this does not establish complete asset ownership/rendering. |
| `engine-skinning` | 1 / 11 / 11 | Controller/joint/influence parsing and CPU deformation. Original software kernel's zero-influence result is separated from the renderer's rigid-vertex policy; GPU shader parity is pending. Prince audit covers 173 controllers, 25,204 vertices, 35 nodes; 700 differential cases span matrices, vertices and interpolation. | Native saved preview/Prince uses four selected warrior meshes and per-frame CPU uploads; normals, lighting and physical ARM64 remain unverified. |
| `engine-textures` | 1 / 5 / 5 | Portable header/PVRTC/TGA decoder; five mapped symbols do not represent module completeness. 360 header comparisons and 11 pixel cases documented; host sanitizer and seven API 37 x86_64 emulator upload cases. Large 2bpp parity and physical ARM64/16KiB execution are not established. | Decoder/upload used by native preview; complete material/shader rendering remains open. |
| `game-data` | 20 / 209 / 181 | Selected native readers for CharacterTable/ModelDict, class formulas, property/state/animation/combat tables. This does not reconstruct all owner lifecycle or inventory/equipment producers. Docs cover 448×224 character fields, 116 model paths, 260 class lists/1,659 formulas, 3,120 class cases and 119,168 property-resolution comparisons plus subsystem-specific combat/vitals reports. | Selected properties/vitals/authored-event combat are in saved Crypt checkpoint. Full inventory, gear/buffs, skills/status, rewards and progression remain incomplete. |
| `level-world` | 88 / 1187 / 769 | Actor/object, scene, timer, character, navigation, collision and coordinator slices. The 1,187 records include nested evidence snapshots. Source docs report bounded original-instruction, sanitizer and source-linked cases for actor/AI/script lifecycle/combat/floors/navigation; no complete AI or whole-game-flow proof. | Saved Crypt integrates selected Prince movement/animation/combat/physics and 95/166 authored objects. Complete factories, pursuit, all AI services, quests/other levels remain open. |
| `physics-backend` | 1 / 270 / 270 | Box2D 2.0.1 source with two documented DH2/native allocator adaptations; 270 address records are evidence mappings, not translated-function count. Docs report 104 original physics functions across 40 representative worlds; host ASan/UBSan includes 157 world steps and 68 contact callbacks. Joint/tuning branches and all DH2 listener policy are outside that scope. | Supports the saved Crypt runtime through separate owner/contact callbacks; not complete game physics by itself. |
| `scene-materials` | 1 / 11 / 11 | Checked scene/node/material/image-reference parsing and transform reconstruction. Eleven `use` entries are evidence references; original shader/light/GPU ownership is incomplete. 213 bit-exact ARM32-vs-ARM64 node matrices plus host scene/mutation audits; this validates local matrix math, not the full renderer. | Saved native inspector uses a new unlit GLES2 shader. Irrlicht Prince integration remains a separate pending step. |
| `script-runtime` | 11 / 57 / 52 | Distinct paths: bounded host SWAMP trigger scheduler (`port/script-runtime`) and Adam native script alias/runtime plus character script slices. Not full original script ownership. Docs report bounded selection/lifecycle/timer/event comparisons. Unsupported SWAMP commands are explicit no-ops. | The inherited Adam b4493ed5 candidate wires selected timers/AI event routing; Lua common bindings, live observer/level ownership, dialogs/UI and progression remain incomplete. Current root checkpoint coverage is summarized separately above. |

The JSON ledger gives each address, ELF alias/range, original symbol, map file, source hash and mapping-record provenance. It deliberately does not assign an implementation status to individual functions.

## What evidence establishes

- Narrow complete-body counts in module documentation: math 20 mapped routines; resources 35 complete reader/accessor bodies plus the whole-buffer File::Init branch; payloads 26 complete animation accessor/search bodies. These are subsystem-specific claims, not a count for the 1,455 address ledger.
- Other source-tested slices include animation/event playback, CPU skinning, scene transforms, game data/properties/combat, actor/navigation/collision and Box2D. Every report is bounded by its stated corpus, fixtures, modeled imports and unsupported branches.
- Current checkpoint 9bf6d312: the linked checkpoint document records Android 17/API 37 x86_64 with 16 KiB pages, gameplay/lifecycle checks, three actual hits (HP 12160→11556), and fan override validation. Older 4f5b7d11 and b4493ed5 results refer to distinct Adam artifacts and are labeled historical in the JSON.
- Irrlicht is a separate diagnostic renderer today. Prince integration remains a milestone; one runtime must own update/render. Skinned joint matrices already incorporate scene/owner transforms, so a second world transform would be wrong.

## Restored evidence files
Copied 277 missing public evidence/source files (3,677,855 bytes) from the pinned Adam commit: 147 linked targets, 128 additional original-function manifests and 15 reference probes/generators. No existing source was overwritten. Each imported path and SHA-256 is in [`adam-evidence-import.json`](adam-evidence-import.json). Run `python tools/verify_combined_function_audit.py` to verify all imported file sizes/hashes and that all 1,455 unique ledger starts exist in the original ELF function index.
The selective tree initially had 214 broken relative links (160 unique targets). The import restores 201 link occurrences; 13 remain: six compatibility payloads, six omitted APK/source archives and one owner-local JSON fixture. See the import manifest for exact targets.

## Limits

No global completion percentage is assigned. Function mapping, complete body reconstruction, host validation, original-instruction differential validation, Android linkage and playable game behavior are separate states. See [`COMBINED-RECONSTRUCTION-STATUS.md`](../COMBINED-RECONSTRUCTION-STATUS.md) for the wider project roadmap.
