# CharAI EnemySpotted prefix

This stage reconstructs the complete `CharAI::OnEnemySpotted` body at `0x3d14b4` (480 bytes). It composes with the existing complete `dh2_character_ai_event` dispatcher; it does not introduce another router. Source event `9` first passes that dispatcher's forced/controller/global gates, then invokes this prefix, then reloads the owner and forwards the captured event/payload to the FSM. The FSM forwarding still occurs when this prefix takes an early source return. The selected active AIS virtual `+0x34` is only the downstream endpoint within this prefix.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The eight-function capture is `original-functions.json` plus `reference/original-functions.asm`; manifest SHA256 `407819b383c8af8b561b7573dbec370715de5cb10dc1f6ec5ac71febf3257f77`. Capturing a helper does not mean its complete implementation is supplied by this new module.

## Exact choreography

1. Capture the input enemy and receiver. Run DebugSettings load `0x337888`, name construction `0x3140ec` with `isTracingCharAIEvents`, query `0x337a88`, and destruction `0x3139ac`. The query result is ignored.
2. Load AI group `+0x34`. When nonnull, call `GroupInfo::OnEnemySpotted` (`0x3d27cc`) with that group, the live owner, and the original captured enemy. This notification precedes every spawn/limbus gate.
3. Query enemy awaiting-spawn (`0x3c0230`), then the reloaded owner awaiting-spawn. Query enemy in-limbus (`0x3c01c0`), then the reloaded owner in-limbus. Any true result returns immediately. The captured enemy state-machine receiver is reused; the owner is reloaded at each owner query.
4. Call `AI_IsInCombat` (`0x3d4bc4`). If true, query the enemy's virtual `+0x28` player classification. A nonplayer skips the aggro branch but still reaches the active AIS endpoint. Out of combat, the aggro branch always runs.
5. `AI_GetAggro` (`0x3d4ac8`) receives the captured enemy. Exact floating equality to zero triggers addition; both signs of zero qualify, and NaN does not. Load the live owner and the actual first DesignSettings row's member `+0x30`, then call `AI_AddAggro` (`0x3d7c68`) on that owner's embedded AI with the captured enemy and the authored float.
6. Only an AddAggro result strictly greater than zero runs a second load/construct/query/destroy debug sequence, using `isTracingThreatChange`. Its query result is also ignored.
7. Reload active AI `+0x1c`. If nonnull, invoke its virtual `+0x34` with the original captured enemy. Its return is ignored. No source fallback replaces a null active endpoint.

This function does not write target, continued, changed, or group fields directly. Mutations in those fields are synchronous provider effects; later accesses use the source reload points. The outer event dispatch, not this prefix, owns FSM forwarding.

## Genuine design value

The oracle executes the complete actual `Arrays::DesignSettingsTable::read` (`0x4b3cd0`, 328 bytes) and `Structs::DesignSettings::read` (`0x4ee0d0`, 3980 bytes). Explicit stream/allocation services back those original instructions. It does not substitute a supplied threat value for this producer.

The cache archive SHA256 is `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. Its `data/pydata/design_pyarray.bin` is 692 bytes, SHA256 `fd70c8be93cd3d5b7947309e22f00a7c5230a4a7541ec6f9ebfdac67335e6296`. The first table consumes 176 bytes and creates one row. The genuine first `design_pystructnames.bin` block names field index 11 `EnemySpottedAggro`; names SHA256 `d955c29a9ddeb091bc5382eff44002a93e6390228bbe0974769648cc58044dc6`.

The runtime row contains a four-byte vtable/header followed by scalar fields, so this field is at `+0x30`. Its exact word is `0x41200000`, float `10.0`. The original table members global is `0x9a6498`. This is a decoded DesignSettings field, not a GetPyCst constant. The native coordinator borrows a pointer to the genuine resolved word; a complete native DesignSettings owner/loader is outside this module. The corpus also copies the source row and negates only this field to test a live table replacement. That is an explicit mutation fixture, not a second authored value.

## API and ownership

`character_enemy_spotted.hpp/.cpp` exports:

```cpp
int dh2_character_enemy_spotted(EnemySpottedState16*, uintptr_t enemy,
                              const EnemySpottedServices24*);
```

`EnemySpottedState16` borrows a stable `TargetEventState32` receiver and carries the group identity. Existing target projections provide the actual owner/AI identities; `active` supplies the selected active AIS identity. `EnemySpottedServices24` carries a synchronous callback plus a borrowed `initial_threat` word pointer. Get/AddAggro result words contain exact IEEE float bits. AddAggro's request contains the captured enemy, reloaded owner and exact authored threat bits. The native float comparisons preserve zero, signed zero, infinities and NaN behavior.

The twelve service operations cover debug lifecycle, group delivery, source state gates, combat/player queries, aggro queries/addition and the active endpoint. Caller providers must supply their genuine ownership and bodies. These are not automatically accepted predicates. Borrowed projections, callbacks, design storage and pointed-to objects must remain alive throughout the call and all nested calls. Callbacks may mutate owner/active/group/threat and reenter; they must preserve the stable receiver projection and its borrowed storage. Alignment checks cannot establish that arbitrary aligned pointers refer to readable live storage.

Return `0` means complete or a source early return; `1` means atomic malformed entry rejection; `2` means a provider/lifetime failure after the observed prefix. Native malformed-provider guards are explicit safety behavior, not additional successful source branches. In particular, the two callback-following argument construction sites validate projections before dereferencing them.

## Proof and limits

The original ARM32 versus optimized ARM64 comparison passes **409 cases / 4,344 ordered services / zero mismatches**. The cases include 256 Boolean branch combinations, 81 IEEE Get/AddAggro pairs, and 72 service mutation/reentry combinations. Captured traces include ordered names, subjects/arguments, owner/active/group reloads, continued/changed bytes and design table identity. Nested source prefixes execute actual instructions. Group, state, combat/player, aggro-map and active-AIS bodies are explicit provider fixtures in this coordinator proof; their full live implementations are not claimed.

`enemy-spotted-fixtures.bin` uses CES1: magic/count/authored threat word; then each case has twelve input words, six final-state words, trace count, and twelve words per ordered trace record. SHA256: `a3aaad19952cfd3964aeae83dee24fff2101738473afd80074ccbd1db6d86913`.

The sanitized host replay passes **409 cases / 54,582 word checks / 10 malformed-entry guards / 16 provider-failure checks**, with zero ASan/UBSan findings. The provider checks include unaligned replacement of the receiver projection immediately after owner limbus and in-combat callbacks. The valid original corpus is unchanged by these checks. Reports bind source, corpus, original capture, exact O2 DSO and host executable hashes; the standalone report records the compiler command and verifies inputs before/after execution.

Frozen production SHA256:

- `character_enemy_spotted.hpp`: `9b053078e0953c46dddbe2e52b8ab4a0a09fd216c4af684ed9e2e9cd2f4f2a44`
- `character_enemy_spotted.cpp`: `4cde740c42d5058bbd2420101f9d36dd978fe174efbd7b3f558201c4072f9372`
- O2 ARM64 oracle DSO: `8a49973cfbf225ec13b0c3190af4dfb132b7e8bbfe9b2523ff18dd8cac76f929`

No full world ownership, complete group/aggro map, Lua dispatch, physics, live monster AI, whole frame or gameplay parity claim follows from this stage.

## Reproduction and central integration

From the repository root, with the configured Python dependencies available:

```powershell
& port/level-world/tools/build_character_enemy_spotted_oracle.ps1
python port/level-world/tests/character_enemy_spotted_differential.py --library .local-inputs/character-enemy-spotted/libcharacter_enemy_spotted.so
python port/level-world/tests/character_enemy_spotted_host.py
```

Parent integration adds `character_enemy_spotted.cpp` to the world DSO and a `character_enemy_spotted_audit` executable from `tests/character_enemy_spotted.cpp`, linked to that DSO. Its only argument is the CES1 gold path. Once the main build is stable:

```powershell
python port/level-world/tests/character_enemy_spotted_host.py --main-linked /home/adampalace/dh2-world-build/character_enemy_spotted_audit
```

That saves a separate main-linked report and hashes the executable and loaded dependencies before/after. It does not manufacture central compiler provenance; the central build binder must supply that separately. Route event9 through the existing `dh2_character_ai_event`; its AI virtual service invokes this prefix, whose active service reaches the selected AIS endpoint. Preserve the dispatcher's subsequent owner reload and FSM call.
