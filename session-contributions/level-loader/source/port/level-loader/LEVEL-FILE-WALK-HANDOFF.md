# Retained ready-buffer level file loading

`level_file_walk_v1.hpp/.cpp` reproduces original `Level::LoadFile` traversal
from an already-ready raw buffer. The oracle executes the actual ARM caller,
`TiXmlDocument::LoadFromBuffer`, root/child iteration and XML document destruction.
Initial resource opening, async readiness, `Level::_LoadFromXML` behavior and
the load-data destructor are explicit service boundaries. This is an internal
kernel, not an agreed gameplay/menu ABI.

Host and ASan/UBSan comparisons pass for all **1,627 MLX/MGP/MVP cache files**
and 12 edge fixtures. They compare every poll's return, retained-state presence,
callback order and complete element/attribute/nested-child projection. Six
additional native checks cover service failures, retained diagnostics, explicit
discard retry, stable completion and interleaved candidate cursors. XML ownership
and all 112 original object-entry comparisons pass after the XML changes.
Both Android static libraries compile. No new APK was installed.

## Source behavior and adapter choices

- `capture_level_buffer` normalizes CR and CRLF to LF for parsing, as the original
  buffer wrapper does, while retaining raw bytes unchanged. Existing `capture`
  remains the direct parser route. The walker requires the buffer route.
- The caller scans all top-level nodes, including comments and declarations.
  It compares node values exactly with the requested `Level` or `Module` string,
  then walks element children of matching roots one per poll. Multiple matching
  roots are processed. Selection is case-sensitive.
- A scan reaching the end returns pending; a later poll deletes the original
  XML document and load state. Native release is a required service; the retained
  XML borrow is released only after that service succeeds.
- At assert level zero, an original parse-failure return is terminal `true` but
  retains partial state. A later cleanup poll releases it without loading partial
  elements. Native failure has a separate `failed` result, retains diagnostics
  and source, and requires explicit discard. It cannot become a ready level
  merely because the original returned `true`.
- A matching non-element top-level value reaches an original null-element
  access. The oracle traps that branch before access and claims no original
  return. Native code reports failure instead of filtering the matching node
  or pretending it was an element.
- Failure latching, stable completion and candidate-local cursors are explicit
  adapter policies. A service execution bool does not mean an object is enabled.

The ARM fixtures restore relocated writable ELF segments and allocator state
between cases. Resetting only the arena invalidated original STL cached free
lists; earlier fixture-only output is superseded by the complete cache-wide
`level-file-walk-original.json` receipt. This reset is oracle fixture isolation,
not a claim about real engine unload behavior.

## Remaining integration

The verified element callback can feed retained source into `object_entry_v1`,
then real factory/property/template providers and `object_initialization_v1`.
The main-session factory, event and restoration agreement remains pending.
The kernel does not create classes, evaluate conditions, bind scripts, restore
saves, publish an active level or draw mobs/chests. It does not implement
resource opening or async cancellation. SWAMP first-level acceptance and the
complete generic-loader goal remain open.

Older checkpoints bind earlier source/library hashes. They are historical;
this milestone does not rerun or claim all earlier map/Android visual checks.

## Reproduce

From `C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`:

```bat
C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe port\level-loader\tests\level_file_walk_original.py --engine C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so --dependency-root C:\Users\adamc\.codex\worktrees\generic-level-loader\dependencies --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip --out port\level-loader\reports\level-file-walk-original.json
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml --target dh2_loader_level_file_walk_probe dh2_loader_xml_ownership dh2_loader_object_entry_probe -j 4
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/level_file_walk_host.py --original /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/level-file-walk-original.json --cache /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml/dh2_loader_level_file_walk_probe --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/level-file-walk-host.json
```

For ASan/UBSan use `host-sanitizers`, configured with
`-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined`, and write
`reports/level-file-walk-sanitizers.json`. Private SDK CMake builds target
`dh2_level_loader` in `build/android-arm64` and `build/android-x86_64`.
`tools/capture_level_file_walk_checkpoint.py` verifies provenance, comparisons,
source-ownership regression and compiled members without claiming runtime
factories or preview deployment.
