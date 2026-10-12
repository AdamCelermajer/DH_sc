# Retained XML to canonical source factory adapter

This is a partial loader integration checkpoint over the verified map recovery.
It consumes the first frozen canonical owner API, `e8cc6acab5183a2b`, and does
not imply production Character/OpenableContainer construction or a live level.

`CanonicalSourceBindingV1` retains original `XmlDocumentV1::Borrow`, source
element, original entry gates, an owned optional type filter and a canonical
context lease. Its copied `CanonicalSourceObjectRequestV1` keeps that state
alive after the parser facade or binding is destroyed. Missing attributes remain
null; present-empty strings remain empty. `canonical_source_entry_v1` exposes
the original element, all nested data and document ownership to the main
session's receiver adapters. A receiver/candidate must keep the binding or
copied `source_lease` for any callback that continues borrowing source strings
or elements after this construction prefix finishes.

The caller supplies `CanonicalModuleContextV1` from the actual canonical
Module after its registered properties load: diagnostic occurrence, signed
runtime module ID and class-resolved position. The adapter never uses raw XML
placement or the inspection map's translated position as a class position.
At Level scope it requires the reset context: occurrence `UINT32_MAX`, ID -1,
zero offset. A missing context lease or nonfinite position fails atomically.
The context being pinned must not own the binding/attempt itself; candidate
aggregation owns both separately, avoiding a shared-pointer retention cycle.

`CanonicalBoundSourceAttemptV1` applies Level's original Player exclusion and
rejects its unsafe null-type branch. It delegates manager gates, unknown-type
diagnostics and the entire construction/Add/property sequence to the canonical
factory attempt. Each attempt is delivered once. Its exact signed handle,
registered receiver and reached prefix remain inspectable on failure. It does
not invent registry/save keys, clear a partially registered object, commit a
world, force conditions true, or implement gameplay rollback.

## Common owner integration

Link `dh2_loader_canonical_adapter`. Before adding the loader directory, set
`DH2_LOADER_CANONICAL_OWNER_TARGET` to the already-created engine target that
implements the canonical manager/factory API and exports its API header path.
The loader then uses that target's actual headers and symbols; no pinned owner
target is created. An unknown target name fails configuration.

With no target supplied, standalone verification builds the four unmodified
owner files extracted from the frozen handoff. All 32 incoming manifest entries
were individually checked before selecting these files. The map preview does
not link this adapter or create a runtime registry. The engine must not select
the standalone fallback as a parallel gameplay implementation.

Typical receiver flow:

1. Retain the same canonical world/context and actual post-property Module data.
2. Bind the original XML document/element using the Level or manager route.
3. Execute the bound attempt against the same canonical manager and class services.
4. Preserve any failed mutation prefix for canonical candidate cleanup.
5. Only later publish a world after all required class/eligibility/restore phases.

## Verification and remaining providers

`canonical-source-adapter-checks.json` records 14 integration fixture checks in
normal host, address/undefined-behavior/leak sanitizer and Android x86_64 runs.
These verify lifetime and cycle release, missing/empty attributes, nested XML,
catalog parity, distinct diagnostic/registry/module identities, actual receiver
position offset, exact operation order, Level Player/null-type gates, empty
filter, LevelConfig early InitPost, special light room/name, provider failure
prefixes, one-shot delivery and invalid-input retention. ARM64 also compiles and
links. Receiver fixtures are explicitly not production classes.

`canonical-owner-reuse-check.json` covers the existing-target configuration and
linking path. It must report no second canonical owner target.

Full PropertyMap/class constructors, class InitPost/InitFinal, actual
visual/timeline/body services, conditions, world drops, failed-candidate
cleanup and restoration/commit still require the main session's connected
owner handoffs. Later files in its mutable checkout are not assumed to belong
to the pinned first archive. SWAMP's all-nine-module/50-Character/five-container
runtime acceptance remains outstanding.

The prior map recovery archive and tested map APK stay immutable. This delta
contains only adapter/build sources, the selected canonical snapshot, docs
and fixture receipts; it changes no main/menu files or emulator.
