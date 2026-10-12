# Connected cache acquisition and module XML traversal

`cached_level_file_v1.hpp/.cpp` now acquires real XML bytes from the existing
native `ZipAssetPackV1`, resolves compiled level resource paths, captures through
the original-equivalent level-buffer route, and polls `level_file_walk_v1`.
`module_load_v1` can consume that actual file stage through its required provider.
These are private review candidates; no shared gameplay/menu ABI is selected.

The connected host and ASan/UBSan comparisons pass for all **1,627 MLX/MGP/MVP
cache files**, plus 12 original edge fixtures packed into a separate deterministic
private ZIP. They compare original raw bytes, all 21,218 polling results and
8,191 element callbacks against the original ARM ready-buffer caller receipt.
The canonical cache is SHA-checked and never changed or extracted. Both Android
static libraries compile. No new APK was installed.

## Acquisition and ownership

The adapter owns a copy of the archive facade, which retains its positional-read
backing. It uses `compiled_level_paths_v1` in the checked order and preserves
both authored request and resolved resource identity. Raw bytes remain unchanged;
CR/CRLF normalization applies only to the parser input.

Source capture, parse failures and callback failures keep the acquired source
and diagnostic until explicit discard. Bytes rejected before an XML snapshot
exists remain separately retained. A missing/read-failed resource has a request
diagnostic but no complete acquired source. No partial or rejected input is
silently repaired or treated as ready.

Changing URI or root while a request is pending fails without replacing that
source. Failure latches without implicit retry. Explicit discard runs the
required load-state release before dropping a captured XML borrow; failed release
keeps it for an explicit retry. A completed request releases this adapter's
borrow, while any recipient's retained source stays valid. A subsequent call is
a new occurrence even for the same URI/root, so repeated placements do not
collapse into an earlier completion.

This is a synchronous ZIP acquisition policy, not execution of the original
resource-open/async stream path. The backing provider remains responsible for
its positional-read behavior; concurrent reads and cancellation of original
async I/O are not verified here.

## Connected checks

Twelve additional native checks cover:

- URI/root changes during pending requests, failed release and explicit retry.
- Missing resources and injected backing-read failure, with no element callbacks.
- Unsupported raw NUL bytes retained for diagnosis; no invented original result.
- Unavailable element service and parse failure, without visiting partial trees.
- Repeated same-file occurrences and compiled-path fallback.
- Source lifetime after the original archive, cache and file facades are gone.
- Actual gameplay-then-visual file traversal under a retained module context.
- Failed gameplay parse stopping before the visual file.
- Failed pending-file cleanup retaining the module context until cleanup succeeds.

The probe's element provider **collects/classifies declarations only**. It does
not create classes or certify unsupported/unsafe dispositions as runtime support.
Successful file traversal means that this inspection service processed source,
not that a gameplay object or ready level exists. Real factory callbacks must
report required service failures. File discard does not unwind gameplay handles
created by a real provider; the owning level candidate must do that separately.

## Remaining integration

The source/factory/event/restoration agreement in `INTERFACE-PROPOSAL.md` remains
pending. Real class/default/template/selection services, object handles and
visuals/physics, condition/event/script services, restoration, publication,
transitions and full unload remain required. The private map preview is unchanged;
SWAMP mobs and chests are still unverified. The whole loader goal is incomplete.

Earlier checkpoints bind earlier source/CMake/library hashes and are historical.
This checkpoint does not rerun or broaden earlier map/rendering coverage.

## Reproduce

From `C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`:

```bat
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml --target dh2_loader_cached_level_file_probe -j 4
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/cached_level_file_host.py --original /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/level-file-walk-original.json --cache /mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml/dh2_loader_cached_level_file_probe --fixtures /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/cached-file-fixtures-host.zip --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/cached-level-file-host.json
```

For ASan/UBSan use `host-sanitizers`, configured with
`-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined`, and separate
`cached-file-fixtures-sanitizers.zip` / `cached-level-file-sanitizers.json` outputs.
Private SDK CMake builds target `dh2_level_loader` in `build/android-arm64` and
`build/android-x86_64`. `tools/capture_cached_level_file_checkpoint.py` checks
current receipts, provenance, dependency sources and compiled members.
