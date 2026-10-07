# Shared systems and current demo boundaries

Reconstruction should implement a behavior once in shared source, then use the
original data to select assets, parameters and scripts. A successful demonstration
is evidence for the exercised path; it is not proof of every skill or level.

| Area | Shared implementation | Current boundary |
|---|---|---|
| Rendering | BRES/geometry/material readers, transforms, animation, skinning, GPU submission, current-pose culling | Tested resources and shader variants; additional authored variants need implementation and validation |
| Skills | Original skill tables, retained state/event dispatch, targeting, damage and FX ownership | Actual targeted demo skill accepted; broader skill families/positive providers require coverage |
| Effects | Mesh/particle factories, source transforms, queue, shaders and warm pooling | New resource coverage is handed off separately; unintegrated work is not APK support |
| Levels | Shared constructors/object factories and staged loading owners | Loader session still completing required object behavior and initialization; current Play explicitly launches Crypt |
| Enemies | Retained character/AI/animation/health/targeting systems | New enemy types and script behaviors must be exercised, rather than assumed identical |
| HUD and menus | Actual SWF assets, native callbacks, retained character/Gear/Save ownership | Display acceptance does not prove complete progression, faery, item or saved-profile gameplay |
| Progression and loot | Shared source components, item factories and owned queues | Kill → award → XP → world drop → pickup is not yet a fully accepted live chain |
| Audio | Shared catalog, decoder/bank/mixer/output work in separate V34 handoff | Real logical-ID bindings, readiness and gameplay delivery remain integration requirements |
| Performance | Whole native optimization, geometry/pose/FX/UI caching and submission work | Measured Crypt on software emulator; broad content and physical ARM64 still need tests |

New maps should reuse these systems. Work on a later map may reveal a new object
class, enemy behavior, quest condition, effect or transition; that should extend
the corresponding shared system. It should not create another level-specific
combat engine or hand-place replicas of objects already described by source data.

Crypt launch in `native_app.cpp` is explicitly development routing, not campaign
loading or saved-game restoration. `model_renderer.cpp` also selects Crypt geometry,
object descriptors, actor initialization and aggro sidecar/configuration explicitly.
Changing the Play destination alone cannot load a chapter. These must become
validated inputs from the genuine shared loading/save flow before first-chapter
completion; this is a required generalization step, not finished map support.

Skill activation selects the saved slot and original table row through shared V6
execution. Some targeting modes are explicitly rejected, and unhandled combat
operations delegate to a provider that is not yet bound in the live renderer.
Shared execution is therefore partial behavior coverage, not all-skill acceptance.

Acceptance of generality requires multiple contrasting data cases: different
skill families, animated/static/particle effects, enemy types, authored map objects,
and level transitions with retained save state. Unknown required behavior must be
reported as unsupported; a plausible substitute is not source reconstruction.

The first chapter carries the cost of completing these foundations. Subsequent
chapters should need substantially less repeated work, but not zero validation or
script/mechanic integration. No fixed per-map percentage or guarantee is justified
by the current prototype.
