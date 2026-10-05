# Module gameplay/visual loading and context ownership

`module_load_v1.hpp/.cpp` implements the ordering of original
`Module::LoadModule` (`0x38a88c`), with required owner services. The actual ARM
caller and `Level::SetObjectModuleId` leaf execute across nine fixtures and 39
file-service polls. `_ChooseXmls` and `Level::LoadFile` are explicit modeled
service boundaries in this comparison; this receipt does not prove their
selection, parsing or class behavior. The separate file-walk milestone verifies
the ready-buffer XML caller. No combined live object/level acceptance is claimed.

Host and ASan/UBSan comparisons pass for the complete observed event sequence,
selected filenames/root spelling, held context, file poll counts and final
context. Twelve native checks cover seven failure sites, three failed cleanup
retry sites, rejected class preparation and stable completion. Source is retained
after XML facade destruction and through failed services/cleanup. Both Android
static libraries compile. No APK or emulator mutation was performed.

## Original ordering

1. Write class-owned `Module+0x40c` to `Level+0x18c` through SetObjectModuleId.
2. Copy class-owned position words `Module+0x160/+0x164/+0x168` to the same Level
   offsets. There is no transform arithmetic in this operation.
3. `_ChooseXmls` supplies the gameplay and visual filenames from post-property
   module fields. Real template/default/override/alternative selection remains
   required; native code does not substitute raw attributes for this service.
4. Poll nonempty gameplay filename through `Level::LoadFile(uri, "Module")`
   until its terminal return; then do the same for the visual filename.
5. Zero all three level translation words, then set the level module ID to -1.

This ID is neither an authored auroraID, a declaration index nor a save identity.
The original Module constructor obtains it from the module counter; real owner
construction must supply it. The position is the class position after properties,
not proof that raw XML position equals the final runtime value. Module/Block
source spellings stay distinct although the original registry shares a factory.

## Native policies

The original call synchronously drains pending file polls. The native step yields
on `pending`, keeping the same selected filename, source and bound level context.
Providers must serialize candidates using one level context. The boolean
object-initialization service cannot report success while this step is pending.

File service `complete` means its own successful completion. Original terminal
`true` after parse failure must instead become `failed` at the file adapter.
Any failed provider latches a diagnostic and prevents subsequent visual loading
or automatic replay. Explicit discard releases a pending/failed file first,
then clears translation and ID in that order. Failed cleanup retains the source
and completed cleanup steps for an explicit retry. These are new adapter policies,
not a claim about original failure rollback or campaign transitions.

The caller requires a live bound Level. Its original missing-Level/assert behavior
was not executed as a safe result. Native context services must reject unavailable
owners. Selection and file service stubs in the comparison do not construct objects
or decide authored conditions. The current module step is an internal candidate;
no shared factory/event/restoration ABI is selected.

## Reproduce

From `C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`:

```bat
C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe port\level-loader\tests\module_load_original.py --engine C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so --dependency-root C:\Users\adamc\.codex\worktrees\generic-level-loader\dependencies --out port\level-loader\reports\module-load-original.json
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml --target dh2_loader_module_load_probe -j 4
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/module_load_host.py --original /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/module-load-original.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml/dh2_loader_module_load_probe --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/module-load-host.json
```

Repeat using `host-sanitizers` configured with
`-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined` and write
`reports/module-load-sanitizers.json`. Private SDK CMake builds target
`dh2_level_loader` in `build/android-arm64` and `build/android-x86_64`.
`tools/capture_module_load_checkpoint.py` verifies current source/receipts and
compiled members. Earlier checkpoints bind earlier CMake/library hashes and
are historical; map/rendering coverage is not rerun or broadened by this work.

## Remaining acceptance

The main-session agreement in `INTERFACE-PROPOSAL.md` remains pending. Factory,
property/template/selection services, class handles, object visuals/physics,
conditions/events/scripts, restoration and full transition ownership still need
real integration. SWAMP map rendering already has its own earlier evidence;
the complete first level with eligible mobs and chests is still unverified.
