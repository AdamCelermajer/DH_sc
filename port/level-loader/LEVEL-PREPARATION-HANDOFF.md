# Retained staged level-source preparation

`LevelPreparationV1` is the loader-owned transaction now used by the private
Android map preview. It unifies fixed and original rule-generated preparation
and retains their description, map, declarations and original procedural plan.
It is a source transaction, not an instantiated gameplay world or a callable
shared factory/save ABI. Main's generic factory/provider gaps remain unchanged.

## API and ownership

Construct the facade with the existing ZIP asset pack by value. Its retained
backing survives destruction of the caller's archive/file facade. Call `begin`
with selected level-table identity, exact definition, explicit fixed/procedural
kind and seed. A pending or failed candidate cannot be replaced: explicitly
`discard` first. Invalid input leaves the candidate/latest source intact.

`step` performs one synchronous preparation stage. Fixed inputs run sources,
map, declarations, publish_source; procedural inputs first run original
sources, blocks, connections, lists, rules, layout and modules. This staging is
a native scheduling policy, not recovered original async IO or a time budget.
The stage getter/completed-stage count can feed a later presentation adapter;
it does not claim a percentage of full gameplay loading.

`source_ready` atomically changes `latest_source`. A `Borrow` retains the exact
request, map, declaration index and original procedural module plan after
candidate, parser, ZIP facade or preparation facade destruction. Pending,
failed and cancelled candidates do not replace the prior completed source.
Repeated terminal polls do not rerun resource acquisition or generation.

Original no-layout is a separate failure classification, preserving the
generator's result rather than pretending the level loaded. Other failures
retain stage-qualified diagnostics and candidate working sets until discard.
Discard releases source/map resources only; there are no gameplay handles here.
The retained floor-query scratch and ZIP facade are sequential, not reentrant.

The Android inspection candidate now retains the whole prepared borrow. GPU
preparation still uses an independent candidate and leaves the old rendered
map intact on failure. Four/eleven stage markers precede actual native map
frames for fixed/generated inputs. Rendering continues to report
`gameplay=0 objects=0`; authored declarations do not imply active actors.

## Verification

`tests/level_preparation_host.py` checks 14 real-cache lifecycle cases on normal
and address/undefined-sanitized builds: invalid input, retained ZIP lifetime,
fixed completion, stable completion, cancellation at all four fixed stage
boundaries including unpublished completed data, rejected pending replacement,
latched missing-definition failure, original DESERT_CAVE_02 seed-0 no-layout,
VOID_MAZE_03 seed-1 missing MGP, SWAMP_02 identity/provenance, repeated SWAMP
preparation and retained map/declarations after facade teardown.

`tests/level_preparation_coverage.py` compares 16 fixed and 70 generated cases
against the immutable prior source-pipeline handoff. It checks module, source,
asset, geometry-instance and declaration counts plus exact stage counts;
three original no-layout results and the missing MGP remain explicit. This
regression uses the previously verified kernels and does not establish new
class/template defaults, conditions, transform semantics or save IDs.

Current reports use `level-preparation-*`; prior source-pipeline reports and
the frozen `source-pipeline-handoff-bd48491b34de1b76.zip` remain historical.
The current private APK must be bound to a current preview/checkpoint receipt
before its runtime evidence is claimed.

## Reproduction

From the private worktree, configure/build `dh2_loader_level_preparation_probe`
in sibling WSL host-xml and host-sanitizers directories. Use bundled Windows
Python and these arguments, replacing LABEL/BUILD with host/host-xml, then
sanitizers/host-sanitizers:

```text
C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe port\level-loader\tests\level_preparation_host.py --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip --inventory port\level-loader\reports\canonical-level-inventory.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/BUILD/dh2_loader_level_preparation_probe --out port\level-loader\reports\level-preparation-LABEL.json
C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe port\level-loader\tests\level_preparation_coverage.py --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip --baseline port\level-loader\reports\source-pipeline-handoff-bd48491b34de1b76.zip --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/BUILD/dh2_loader_level_preparation_probe --out port\level-loader\reports\level-preparation-coverage-LABEL.json
```

Build/install only the private APK as in SOURCE-PIPELINE-HANDOFF, then run
`tools/audit_source_pipeline_preview.py --prefix level-preparation
--require-preparation-stages`. This exercises DARKWOOD, SWAMP_02, SWAMP,
reload, pixel-identical failed-load retention and a visible SWAMP module view.

## Required subsequent integration

Main accepted the retained source/candidate/object-borrow direction and is
working toward extraction of its existing canonical actor owners. The source
transaction must feed those actual owners; it must not invent actors, property
sheets, original registry IDs, conditions or campaign state. Original factory
property/initialization order remains in OBJECT-ENTRY-HANDOFF,
OBJECT-INITIALIZATION-HANDOFF and OWNER-CONTEXT-REQUIREMENTS. Their runtime
services must succeed before any future gameplay-ready publication. Main/menu
files and emulators have not been modified by this stage.
