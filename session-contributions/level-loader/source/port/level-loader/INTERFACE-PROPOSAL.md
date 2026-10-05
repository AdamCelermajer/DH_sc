# Generic loader boundary — direction accepted, runtime providers unavailable

Goal owner: `01a108c2-0dd8-7842-bf2f-bc28dbbe94f5`.
Gameplay/integration owner: `01a0fb9d-d478-7703-9961-2feb2690280d`.

On 2026-10-05 the main session accepted the retained source/candidate/object
borrow shape as the integration direction. It confirmed that the composing
generic factory, generic templates/properties, containers and complete world
restoration are unavailable. No callable gameplay factory, scene integration
or persistence ABI is implemented. See `OWNER-CONTEXT-REQUIREMENTS.md` for the
concrete operations, actual current owner composition and received decision.

The standalone `object_initialization_v1` dispatcher now matches the original
phase ordering across 16 fixtures (182 calls, 269 service events), including
live module appends and registry insertion. Its service interface is internal,
not an agreed shared ABI. See `OBJECT-INITIALIZATION-HANDOFF.md` for original
ordering, adapter guards, the 33-entry factory registry and remaining services.

The internal `object_entry_v1` candidate now retains/classifies source entry
gates with the exact original factory table. Its explicit unregistered status
matches the original null-handle branch for all 15 raw MGP links when passed
to the XML entry point; it does not silently create or discard them. See
`OBJECT-ENTRY-HANDOFF.md`. Registered entries still require real owner services;
this adds no shared ABI, factory implementation or live object rendering.

The internal `level_file_walk_v1` now matches the original ready-buffer caller
across all 1,627 MLX/MGP/MVP cache files and 12 edge fixtures. It preserves raw
source and exact top-level selection/element callback order. Parse failures are
explicit failures with retained diagnostic state, not ready levels. Resource
open/async handling remains outside this verified kernel. See
`LEVEL-FILE-WALK-HANDOFF.md`; this adds no shared ABI or class construction.

The internal `module_load_v1` now orders bound runtime module ID/translation,
owner-provided XML selection, gameplay file, visual file, then context reset.
Its pending yield and explicit failure/discard policies retain source/context
until cleanup succeeds. The ARM comparison covers the caller across nine
fixtures; selection/file services remain explicit boundaries. See
`MODULE-LOAD-HANDOFF.md`. An initialization provider cannot report success
while a module file is pending. No shared level context may be interleaved
between pending module candidates; real owner services must enforce that.

`cached_level_file_v1` now supplies actual native ZIP acquisition and retained
file traversal to that module stage. The connected comparison covers all 1,627
cache MLX/MGP/MVP files and raw bytes. Native integration checks cover module
ordering, parser failure, pending cancellation and source/backing lifetimes.
Their recipient collects declarations only; no class creation or ready level
is claimed. See `CACHED-LEVEL-FILE-HANDOFF.md`. Synchronous cache acquisition is
an adapter policy; original resource-open/async behavior remains unverified.

Internal preparation now includes `fixed_map_v1`: retained assets, material
bindings, module transforms, assembled scene geometry and a native floor world.
All 16 prepared fixed definitions assemble. This stage does not create gameplay
objects or assign persistence identities. The retained floor world has mutable
query scratch and requires sequential use. Original transform/property behavior
has separate differential receipts. Swamp map mesh/material rendering and
private Android lifecycle checks now pass; runtime objects remain unverified.

Later internal preparation also includes `procedural_modules_v1` and the
private `procedural_map_sources_v1` inspection adapter. Generated overrides
match 70 original runs and 533 module occurrences; 66 generated map/seed cases
assemble, with three original no-layout results and one missing MGP dependency
explicit. The same declaration index retains 7,543 occurrences across those
66 selected runs, covering 17 declaration types. These are occurrence counts
across two seeds, not active-object or unique-object counts. Original rules,
MVX sources and selected MGP/MVP declarations remain owned. The derived Level
document is not full original PropertyMap serialization or an agreed runtime
ABI. See `PROCEDURAL-MODULES-MILESTONE-HANDOFF.md` and
`reports/procedural-declaration-types.json` for evidence and limitations.

The internal `fixed_declarations_v1` index now retains 2,331 declaration
occurrences across all 16 assembled fixed definitions. Independent original
XML comparisons cover all attributes, nested element structure, per-instance
module context and checked float32 projections. Its source grouping is not
original object initialization order and its indices are not persistence IDs.
Original `ObjectManager::LoadFromXML` applies the separate XML `template` field
before defaults, applies registered attribute overrides (including
`_templateName`) afterward. It calls `InitPost` immediately only for
`LevelConfig`, checks `IsGameObject()` at virtual slot `+0x20`, and adds the
module offset only for game objects. Ordinary object `InitPost` belongs to the
separate initialization phase. The previous `InitPre` label was incorrect;
`reports/original-loader-vtables.json` resolves the original slots directly.
Factories must preserve that ordering rather than using the projected position
as proof of a final runtime position. Original class default registration and
table/template resolution remain required gameplay services.

## Loader output

A retained, immutable level description owns the original definition and
dependency bytes, parsed trees, source resource identifiers and provenance.
It retains the selected level-table identity and difficulty separately from
shared world geometry. SWAMP and SWAMP_02 must never collapse into one identity.

Each module retains its source declaration, original scene/template identifiers,
authored transform, selected gameplay/visual files and generated-instance identity.
Each object retains its full attribute set, original name/auroraID when present,
parent module, declaration order, source file/node and template/type references.
The original object-ID and transform-composition behavior must be recovered
before choosing native IDs or Euler composition; compound keys are proposals,
not assertions of the original format.

Conditions, scripts, dialogue/cinematic references, triggers, entry points,
exits, checkpoints, camera/light/fog/music configuration remain explicit data.
They are not silently evaluated, flattened or replaced with new defaults.

## Gameplay-owned services

The main session supplies required services to evaluate authored conditions,
resolve template/table-backed object resources, create game objects through
their real owners, attach visual/physics resources, register event/script bindings,
restore saved object state and release retained handles.

The loader orders these operations from original evidence. The main session
executes combat, AI/pathing algorithms, targeting, skills, animation, dialogue,
camera/actor choreography, quests, loot and campaign/object persistence.

No callback is permitted to report success while discarding a required object
or binding. Missing services must identify the original declaration and keep
the acceptance status incomplete. A successful render preview must separately
state whether gameplay/quest/save behavior is connected.

## Lifecycle and publication

Prepare a candidate under its own retained resource owner. Resolve configuration,
layout, references and required services before publishing the active level.
Creation failures unwind handles in the required reverse dependency order.
An existing level must remain usable after failed preparation. Transition and
save ordering must follow recovered source behavior; no competing save format
is introduced. Progress/error delivery feeds the menu session's loading screen.

Prepared descriptions and borrowed source records survive parser/cache-facade
destruction through their retained owners. No pointer into a temporary XML tree
or decoded ZIP buffer may escape. Repeated load/unload, failed load and Android
context recreation need distinct validation.

## First integration acceptance

Opening SWAMP from `001_swamp.mlx`: all nine authored modules; objects and
conditions from each original MGP/MVP; correct map geometry/materials/transforms;
source-backed mob resources and five authored openable containers. Counts are
authored declarations, not an assertion that every conditional object is active.

Capture actual rendering in the loader's own Android emulator. Parser counts,
build success and asset presence are insufficient for this acceptance.

Next cover other fixed levels and original rule-driven generation, preserving
seed/RNG behavior, module selection, exits, collision/backtracking, errors and
generated identities. Unsupported declarations and source-cache omissions
remain visible in the coverage report until resolved.
