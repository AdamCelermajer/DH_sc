# Current Crypt01 actor initialization sidecar

`CAI1` v1 is an additive owned port sidecar for the **current eleven monsters**, keyed by the existing DACT `(room,name,character)` records. It preserves the requested template and seven fields in two forms: exact raw attribute text with presence, and source descriptor-resolved values. It does not overwrite or extend DACT and is not a reconstructed full XML/property/factory loader.

The generator reads actual MGP XML from the canonical ZIP, validates ZIP SHA `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`, and uses actual DACT monster keys. Room0/3/7 source members and hashes are included. The current DACT SHA is `be44bd6d160973bf70a0cab61399a14d8bea6a7eade41790594cf6b781a04cca`. The generated binary is2357 bytes, SHA `20ae8c16a179c7e626b9d25356779031f71a2f46e304ae62e7f22b2ff1310efe`. The readable companion JSON exposes every raw/resolved value and source binding.

The descriptor/default evidence is the frozen `../character-live-owner-readiness/original-functions.json` SHA `dbff29cffe8c865eed384a0907722dee9ecf6dd272096ebcbab3cbee544cc2e7`, plus its thirteen executed original probes. The actual reset chain is ObjectManager InitProperties→optional SetTemplate→LoadDefaultProperties→LoadOverrides→SetProperty(NULL)→descriptor virtual+0x0c; see that directory's NOTES for addresses. Shipping LoadTemplate is an assertion stub, not an assumed Monster file lookup. Six absent ai_state values resolve to empty; five authored Idle remain Idle; all preset IDs are3. Visible/auto default true, delay0/0, radius float32+0. Cultist group/role are absent→empty; direct group is present-empty and role Normal. The native loader calls the previously recovered `dh2_character_native_fsm_preset_state` instead of an independent preset approximation.

## Native API and ownership

`actor_initialization.hpp/.cpp` defines `dh2::character::ActorInitialization`, owned source/record vectors, `ActorInitializationKey`, and `ActorInitializationValues`. Load with:

```cpp
ActorInitialization snapshot;
std::string error;
bool ok = load_actor_initialization(bytes, size, actual_dact_digest,
                                   monster_keys, snapshot, error);
auto* init = actor_initialization(snapshot, room, name);
```

The caller supplies the SHA256 of the **actual DACT bytes** and all eleven monster keys decoded from them, including CharacterTable link text. Do not supply the sidecar's own asserted digest as independent validation. Hash computation/asset verification is a caller boundary. The isolated audit independently compares the native projection's descriptor digest with the actual DACT hash in Python; it does not claim the loader contains a SHA implementation.

Input byte buffers/keys may die after a successful load. Source/record strings are owned. Borrowed pointers remain stable only until snapshot mutation, move or destruction; perform reload only after borrowers release their views, or retain an immutable snapshot for all actor consumers. The failed loader keeps the entire previous snapshot unchanged and emits an error. It accepts unordered caller keys but requires exact one-to-one current inventory correspondence. It rejects duplicate/missing/extra/wrong-room/name/character keys, wrong source/hash/default-provenance/descriptor bindings, inconsistent raw versus resolved values, reserved fields, invalid presence/text encoding, truncation and trailing bytes.

This version deliberately binds the exact source inventory. Authored bool/delay/radius input outside the actual all-absent numeric fields is rejected, rather than guessed through a generic parser. New authoring or another level requires a separately recovered/versioned producer contract. Source hashes are provenance, not live Application startup or template-manager ownership.

Root can add only `actor_initialization.cpp` to the existing world target, which already supplies the genuine preset getter. No owner/current-state mutation occurs in this loader. Retain raw ai_state for source initialization diagnostics, use resolved ai_state with the recovered preset command at genuine LevelLoadCharStates ordering, and keep source group-pointer lookup separate from the resolved empty group text. No initial state17 is invented.

## CAI1 v1 layout

All words are little-endian; strings are length-prefixed full ASCII bytes, no fixed-width truncation or trailing NUL. ASCII/4096-byte and1MiB input bounds are explicit port validation limits.

- 160-byte header: magic `CAI1`; version1; total byte count; record count11; source count3; field count7; two zero reserved words; four SHA256 byte arrays (canonical ZIP, actual DACT, original ELF, captured default manifest).
- Three source records in room order0,3,7: room word, original byte count, SHA256 bytes, full cache-entry string.
- Eleven records in DACT monster order: room, source index, name string, CharacterTable link string, requested-template authored value, seven authored values, resolved ai_state string, visible/auto words, signed delay pair, float32 radius word, signed preset ID, resolved group and role strings.
- Authored value: presence word0/1 and text string. Absent requires empty text; present-empty remains distinct.
- Field order: `ai_state`, `ai_state_visible`, `auto_spawn`, `spawn_delay`, `spawn_view_radius`, `char_group`, `char_group_role`.

## Proof and reproduction

`reports/actor-initialization-host-audit.json` binds final source, producer, binary, JSON, DACT, executable and original evidence. Isolated g++ O2 uses `-fno-fast-math -ffp-contract=off` and ASan/UBSan/leak checks. Native decoded fields/source bindings equal the direct-XML producer projection for all11 records/3 members. The test rejects all2357 truncated prefixes, all2357 single-byte XOR mutations and ten other malformed key/digest/input cases atomically:4724 rejections,4780 total checks, no sanitizer diagnostics. Loaded strings survive input/key destruction, and reverse key order is accepted with identical output. These are parser/ownership proofs, not full original XML instruction differential, central DSO, Android or live-game claims.

Producer (new paths only; existing artifacts must match exactly, otherwise overwrite is refused):

```powershell
python port/level-world/tools/produce_actor_initialization.py `
  --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip `
  --output port/level-world/reference/actor-initialization/crypt01-actor-initialization.bin `
  --manifest port/level-world/reference/actor-initialization/crypt01-actor-initialization.json
```

Add `--verify-only` for deterministic reproduction. Defaults read the current existing asset DACT and Crypt provenance; explicit `--descriptor`/`--world-provenance` paths are accepted.

Isolated audit:

```powershell
python port/level-world/tests/actor_initialization_host.py `
  --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip `
  --output NEW-report-path.json
```

It uses WSL g++, writes only its new scratch executable/reference/report files, refuses existing output reports, and does not rebuild shared DSOs or Android. The native test executable accepts sidecar and actual DACT paths. For central CMake composition, root can link `tests/actor_initialization.cpp` to the world library and invoke those same two arguments; compare its JSON projection to the readable companion as the Python audit does.
