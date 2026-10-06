# Scheduling extraction proposal: owners before workers

This is a proposal anchored in the V41 source audit, not an implemented loader
or a claim that diagnostics removal solved loading latency. Root's new frame
wrapper does not change these service ownership boundaries.

The active Java path initializes from onSurfaceCreated, waits for valid dimensions
in onSurfaceChanged, then synchronously calls loadSelected on the GL thread.
loadSelected reads descriptor/provenance into Java byte arrays; JNI copies the
descriptor, and load_world mixes decoding, GL creation and live source services.
The native front-menu consume-launch path also loads the Crypt demo during
NativeBridge.draw. First HUD preparation lazily reads constants/localization,
SWF/font/bitmap data and creates its GPU resources. These are separate first-load
producers and must be instrumented separately using V41 stage IDs/generation.

The SDK37 reference GLSurfaceView onWindowResize waits for its GL thread. Moving
the same monolithic load from resize to a queued GL callback changes where it
blocks; it does not prove a visible/loading frame appears. Retain the root's
valid-size cold-start fix. Observe a genuine visible frame and thread timings
before selecting an extraction; the physical Android16 SDK implementation may
differ from that source reference.

| Proposed packet/stage | Actual owner and allowed work | Commit boundary / required acceptance |
| --- | --- | --- |
| Immutable encoded data prefetch | A dedicated worker with retained AssetManager global ref or owned APK descriptor; separate asset handles; raw descriptor/provenance/design/model/animation/texture bytes | Exact request/source/context generation; measured bytes registered in existing ledger; size/time/cancellation limits |
| Independent decoded bitmap packet | Worker-local decoder scratch/pixels with no shared scene/maps/private Lua | GL thread validates original dimensions/format/material and performs upload; context loss discards old pending GPU commit |
| Authored UI immutable inputs | Read-only bundled constants/localization/SWF/font/bitmap source bytes | Original timeline/native callbacks remain with actual UI session; do not advance hidden timelines to prewarm |
| Incremental GPU commits | Owning GL context thread; same shader/texture/FBO/buffer paths and original error barriers | Bounded items/bytes per genuine frame; no asset resolution/effect reductions; measured first-meaningful output |
| Native C1 / scene / script / inventory | Existing shared world, private Lua, Application/Player/Object/Menu managers and retained Save/Profile/Inventory/VisualSkin owners | Actual typed source service contract; never create a second Save authority or synthesize current GS/progress |
| Live world publication | Existing root renderer publication on owner thread after successful required operations | Exact candidate generation, real failure behavior and resource lifetime; stale/cancelled packets cannot publish |

A candidate CPU packet should contain immutable bytes plus exact descriptor,
layout/cache fingerprint, requested selection, source generation and context
generation. It owns its bytes until success/cancellation. A queue needs bounded
total bytes/items and one explicit cancellation path; it must not move an
arbitrary whole load_world call to a worker. GPU names and live Lua/services are
not transferable CPU preparation data.

Suggested first acceptance: record V41 real stage durations and V42 main/GL
thread scheduling; extract only one independently decoded immutable input path
whose CPU work dominates. Compare identical original outputs/errors, source
callbacks, startup lifetime, cancellation/context recreation and first actual
visible frame. Only then generalize the packet to more resource families.

The Crypt demo still uses staged-GS default false. Genuine Level.Init,
LoadProcess, Unload/destruction and reached scene/module services are the
external loader contract. Demo preparation counters cannot complete that source
contract or substitute phase38/currentGS publication. Loading-work extraction
and first-chapter Swamp source integration remain distinct acceptance tasks.
