# Character panel abort: first fault confirmed as bounded texture quota

## Confirmed root diagnostic outcome

Root's subsequent diagnostic run identified the first actual failure as
`V37 aggregate texture/mip byte budget exhausted`, at bitmap_quad command3,
serial29191, GPU133420229576152, entered-frame1/mask0. The engineering128MiB
texture quota was insufficient for the combined retained menu and Crypt assets.
The outside-display error was secondary after that backend failure called
GPU.abort. Movie.first_failure now remains available after root's stage-clip
exception boundary. PID3564 survived without SIGABRT, returned to the source
world and displayed the actual error. These facts are root-reported diagnostic
evidence; this subagent made no device calls.

The speculative font-owner change is cancelled: no renderer/font ownership
rebinding is justified by this diagnosed quota failure. Root owns the ledger
snapshot and bounded texture calibration within its finite total GPU/CPU limits.
Failure containment is observed; successful character-menu rendering still
requires root's calibrated resource build and full-flow acceptance.

The earlier read-only audit below is retained for provenance. Its previously
unresolved first-fault alternatives are superseded by this diagnosis.

The actual main-menu→selection→Crypt→portrait run aborted PID4654/GL TID4671
with libc++ uncaught `SWF command outside display lifetime`. Its post-click
screenshot is the launcher, not a working character panel. Menu acceptance is
FAILED for this run. The saved native tombstone identifies display, not advance
or generic input/hitTest, as the reached throw path.

Confirmed producer chain:

```text
NativeBridge.draw menu branch (outside gameplay try/catch)
 → NativeCharacterMenuV4::frame → authored_source_display_v4
 → SwfMovie::display_source_stage_clip_v5 (begin/clip/end)
 → sprite/display_list → SwfEditTextFieldV1::display
 → edit_text_display_v1::display → display_records
 → text_display_v2::display → retained TextRenderOwnerV2::bitmap/line
 → its pinned SwfServices.draw → SwfGpu::draw returns false
 → checked(false,error) throws runtime_error across JNI
```

SwfEditTextFieldV1 captures `for_player(field.get_player())` at construction.
TextRenderOwnerV2's bitmap/line closures retain their own platform/draw sink and
glyph texture ownership. Shape fallback and inline-image branches use the
active gameswf render handler; bitmap/text services use the pinned platform.
Thus a cross-owner imported field can split those paths.

`edit_text_character_def::create_character_instance` constructs the new field
with **definition.get_player()**, whereas its parent may belong to another
player. This is a concrete source mechanism for the hypothesized cross-owner
field. However ordinary gameswf import/create_movie uses a per-player library;
cross-owner definition use in this actual panel is **not yet confirmed** by the
existing generic error/tombstone. No parent/player/GPU identity values are logged
at the failed text command. Do not label the hypothesis a diagnosed cause yet.

A second source mechanism must be distinguished: SwfMovie::Impl::emit records
first backend failure but continues source display. SwfGpu::draw catches a prior
primitive/mask/resource error and calls abort(), setting frame=false. A later
direct text sink then throws the outside-lifetime error, potentially masking
the true earlier failure. Both mechanisms fit the actual stack.

Normal stage display brackets begin/end. Both upstream shape hitTest forms also
bracket begin/end, so an absent hitTest begin is not the demonstrated issue.
Nested hitTest during an already open display can instead trigger the earlier
nested-begin failure; its relevance requires the first-failure evidence.

## Root-owned diagnostic step

Record only the first failing command for a logical display/attempt: command
kind/serial, actual SwfGpu pointer, prior frame/materialized/mask/submitting-mask
state and original error **before abort**. Preserve SwfMovie's first failure
alongside the direct text exception. A later generic failure must not overwrite
that initial error. Capture the begin/end backend pointer for the failed attempt.

For that text command record field/name/character-ID, field.get_player(),
definition.get_player(), parent.get_player(), actual parent-chain root player,
retained platform identity/context and currently active render-handler identity.
Bound parent-chain inspection and emit diagnostics only on failure. These values
select the fix without loosening the lifetime guard or logging every frame.

| Actual first evidence | Required fix direction |
| --- | --- |
| Begin uses backendA; first text submission uses unopened backendB | Correct imported-instance/platform ownership or implement explicit owned glyph transfer; keep backendB guard |
| Same backendA; earlier command already aborted display | Fix that original mask/resource/lifecycle failure and preserve its first error |
| Nested begin during display | Isolate the source stencil query/render lifetime while restoring outer state; do not allow nested begin without ownership |

If cross-player construction is confirmed, prefer creating the imported text
instance under the actual destination parent player **before** its sidecar,
font projection and glyph caches are built. Retain the original definition/font
data owners; do not mutate a shared definition's player. Audit destination AS
prototype/GC/frame registration and callbacks. Rebinding a mature sidecar only
at draw time is unsafe: layout records/cache images may already carry source GPU
texture identities. A destination platform must produce or explicitly transfer
real glyph pixels/ownership; matching numeric texture IDs is not sufficient.

## Failure containment is separate from visual success

The native menu frame branch lacks the exception boundary used by gameplay.
At minimum root must catch/report required failures before they cross JNI and
clean up the actual owning display target. Returning false/closing the menu is
failure containment, not proof that the menu renders. A cross-owner error can
leave the destination display open, so cleanup must target the actual begin
owner, not just the text source sink. Any new facade abort hook needs retained
typed ownership, preserving source callback failures and original resource state.

Acceptance after the cause is fixed: genuine main-menu launch, live portrait
opening, stats/items/skills navigation and authored back; process survives,
no libc++/F/libc/native required errors, same player/Save ownership and correct
art/text. Include masked text and imported fields; ordinary HUD rendering alone
does not cover this path. Root owns production changes/build/device validation.

This audit changed no globals/renderer/source logic and ran no device operation.
`source-audit.json`/`source-snippets.txt` freeze exact current source and crash
input hashes. Earlier advance-only conjecture was discarded by the tombstone.
