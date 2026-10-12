# Canonical cached XML source composition

This loader delta composes `CachedLevelFileV1`, the original retained XML walk,
`CanonicalSourceBindingV1` and the main session's canonical factory/registry.
It consumes the frozen connected-owner archive `1b96f4edb021619d` plus its
matching declaration-include supplement `78ca667143cef823`.
Both archives and all 166 connected entries were verified before import.

`CanonicalCachedFileV1` represents one file occurrence in one canonical
candidate. The caller supplies the same retained context, actual registry,
class continuations and parser/release services. A module file requires actual
post-property Module ID/position; its diagnostic occurrence remains separate.
This adapter owns retained source and an ordered journal of factory attempts.
It owns no parallel actor, module, scene, quest, save or physical implementation.

Repeated completion cannot restart the occurrence. Failure latches without
replaying callbacks and retains the document, exact source element, canonical
factory prefix/handle and error. Changing URI/root fails the occurrence.
`discard_after_owner_release` requires explicit canonical candidate cleanup
before dropping source. A failed owner cleanup preserves source; a subsequent
source-release failure does not replay successful owner cleanup. Once cleanup
is requested, source delivery cannot resume. Destruction of this adapter is
not an ObjectManager Flush or a renderer/body cleanup operation.

## Executed first SWAMP failure

The actual original cache reaches `data/scene/001_swamp.mlx`, element 1,
`LevelConfig`, name `level_config`, original factory `0x340ca8`.
The frozen typed dispatcher reports:

```
required actual registered class construction: LevelConfig
```

The factory prefix is `empty`: no object was registered, and no class property,
early InitPost, module expansion, condition, visual, PF or restoration service
was reached. The loader does not omit LevelConfig or the next nine Module
declarations to claim a successful level.

`reports/SWAMP-CONNECTED-CLASS-GAPS.md` and its JSON companion list the exact 13
additional class kinds, source factory addresses and first source/context.
SWAMP has 205 source declarations across nine modules. Character,
OpenableContainer and AnimatedDecor comprise 92 declarations with dispatcher
allocation hooks; the other 13 kinds comprise 113 declarations. Hooks alone
do not prove construction. No live SWAMP mob or chest was constructed by this
checkpoint.

## Build and checks

Production continues to supply its existing `dh2_level_world` target through
`DH2_LOADER_CANONICAL_OWNER_TARGET` before adding the loader directory.
The new file adapter is included in `dh2_loader_canonical_adapter`.
The standalone `tests/cmake-connected-owner` project links a narrow immutable
manager/factory/PropertyMap/dispatcher target for verification and explicitly
rejects creation of the old pinned owner target. This fixture target is not a
replacement production world. The earlier standalone fallback remains for
the first owner ABI.

The existing 14-check fixture now supplies the genuine class-name field when
the selected owner ABI exposes it; it also still compiles/runs on the earlier
ABI. Production receiver storage remains provided by the main session.

Final host, ASan/UBSan/leak and Android x86_64 executions each pass 12 file
composition checks and 14 source-adapter checks. Both Android ABIs compile/link.
The composition checks cover the real first failure and source census,
failure replay guards, cleanup ordering/retry, cancellation after partial
cleanup, stable completion, identity mutation, original Player/filter gates,
malformed/missing input and missing candidate context. The manager-route empty
filter is an explicit gate fixture; it is never used to bypass SWAMP loading.
Parser/release and cancellation providers in the probes are declared fixtures.
Receipts retain binary, source, cache, fixture and archive hashes.

No map APK was changed or installed. The visible loader preview keeps its
previous verified Swamp map. Main/menu files and their emulators were untouched.

## Remaining canonical owners

Main must provide genuine LevelConfig and Module construction/properties,
early InitPost, actual module registration/ID and file choice, followed by the
other reached classes. The loader then composes module expansion and signed-key
initialization through actual conditions, SceneManager/PF and per-class
InitFinal. Full failed-candidate visual/body cleanup, saved-state restoration
and commit remain required. False eligibility is distinct from service failure.
Source completion never means runtime readiness or permission to publish.
