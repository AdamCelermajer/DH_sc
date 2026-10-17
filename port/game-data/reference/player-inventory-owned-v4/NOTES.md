# V4 inventory owner and its item-table inputs

This directory imports the V4 original corpus from Adam's `c3ae797` checkpoint and adds DH2R host validation. The imported fixture remains pinned to original `libDungeonHunter2.so` SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; its original freeze manifest and assembly receipts are preserved under `imported-adam-c3ae797/` as historical source evidence. The canonical `fixtures.bin` is the 84-session, 1,720-step V4 original corpus, not a new result from this fork.

`FreshInventoryOwnedV4` owns the item vector, stable heap items/slots, both equipment arrays, potion pointer, gold and source constructor fields. It reuses the existing DH2R `ItemTable` reader and borrows the same `PropertyState` the caller owns. `ItemInstanceV1` is a small shared source payload in `item_instance.hpp/.cpp`; it does not import or instantiate the legacy `ItemInventoryV1` or `PlayerSavegameV1` owner. Loot selection reads the first six LootTable cache sections retained by `LootTablesV2`.

The live V4 constructor takes `PropertyState&` and an `InventoryRandomServiceV4` callback. The callback borrows a caller-owned random stream; V4 does not create or seed a private production stream. The current Android renderer has not yet unified its CombatRandom and AnimationRandom projections into an app-wide `dh2_random_state`, so this interface is not evidence that native runtime RNG binding is complete. The `LootRandom8V2` plus `shared_ptr<PropertyState>` constructor is retained only to replay Adam's frozen host/original fixtures. It is not the native ownership path.

Name/stat/requirements, AddPower, player/difficulty discovery, gold notifications, inventory-full notifications, and Character gear/Skin/HP-MP effects remain explicit providers. Unrecovered assertion/Debug paths reject. This source slice is not a menu-created player or full-game inventory integration claim.

## Verification

Run the 84-session sanitizer replay after placing the three item-discovery cache files in `.local-inputs/items-discovery/`:

`python port/game-data/tools/run_fresh_inventory_owned_v4_host.py`

Its arguments use the imported `fixtures.bin`, the earlier V2 `fresh-fixtures.bin`, and the original item cache. The separate LootTable reader and 512-operation source RNG corpus is driven by `tests/loot_tables_v2_original.py`, then replayed by:

`python port/game-data/tools/run_loot_tables_v2_host.py`

The source-RNG adapter and signed-quantity checks also have a cache-independent mode in the V4 host executable: `--random-only`. That compares the fixture Loot RNG transition against the existing `dh2_random_next` state owner across zero, positive, negative, and boundary inputs; it does not prove that a renderer has selected or initialized the stream. `--borrowed <fixtures.bin> <fresh-fixtures.bin> <cache>` runs all 144 owner cases through the caller-owned properties/RNG path; 676 draws and 3,584 aliases match.

The original Python oracle import chain is kept in `tests/*_v4_original.py`, `fresh_inventory_v2_original.py`, `fresh_inventory_owned_v4_original.py`, and `item_inventory_v1_original.py`. It needs the original library, item-discovery cache, and skill-table names under `.local-inputs/`; these are local test inputs, not redistributed game assets.

The current selected-library gate is `tests/run_adam_integrated_host.py --cache <original data/pydata directory> --build <short build directory> --compiler <g++.exe> --report <output.json>`. It replays checked-in goldens in six suites and needs no original ELF or private manifests. Golden regeneration additionally requires the pinned ELF, local cache and Unicorn/pyelftools; current proof is [the combined host report](../../../../reports/branch-audit-2026-10-05/adam-integrated-host.json), while copied freeze manifests describe upstream snapshots before adaptation.

The V5 gear/power generators now read original ranges from public `adam-c3ae797-item-v5/attribution.json`, accept `--original-elf`, `--library`, `--apk`, `--gold` and `--report`, and reject overwriting the public goldens. Gear `--cache` uses the local `actors/`, `items-discovery/` and `skill-tables/` layout above; power `--cache` uses the original `files/` directory containing `data/pydata`. Their ARM64 execution receipts validate the packaged kernels, with explicit TLS/byte-service fixtures; they are separate from live gameplay.
