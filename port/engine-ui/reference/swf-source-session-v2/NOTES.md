# Source session V2 and default movie ownership

This additive batch preserves the frozen facade, vendor, input/frame V1 and session V1. It is not centrally enabled or present in an Android APK. Its actual linked core host proof uses controlled SWF/layout/native/GPU sink fixtures, not the authored HUD or GPU acceptance.

## Concrete owners

`source_movie_services_v1(original, wrapped, error)` prepares a NEW weak history/frame owner before a legacy `SwfMovie.load`. Its startup callback binds history, binds frames, then forwards the original hook, before any shared/root file or character construction. Original native ownership validation happens before replacement. All resource/draw/native/stencil/diagnostic services forward their original context; glyph provider remains the original borrowed pointer. The wrapper retains the original native owner, but no player, root, movie or AS lease. Borrowed non-native context retains the facade's original lifetime contract.

Prepare a fresh wrapper for every load attempt; its observers bind to one player. Do not reuse it for reload or double-wrap `SwfInputSessionV1/V2`, which already own observers. Central migration of every plain facade caller is still required before enabling input/frame overlays. Root plans a NEW versioned facade TU with explicit already-owned startup metadata; this batch neither modifies that facade nor quietly switches legacy callers to stock scheduling.

`SwfInputSessionV2` preserves the source session operations and adds `bind_status_hud(verified_sha, status, error)`. The status lease pins one generation and its exact `SwfMovie` for `PlayerStatusHud`, stores only a weak session control, and releases status clips before its generation. `current`, `update` and `frames` reject detached generations after reload/release with `Status HUD belongs to a detached session generation`. A failed candidate load preserves the previous current generation and status. No raw reloadable movie pointer is exported.

`OriginalUiInputSessionV1` is a NEW Android-facing outer owner, separate from the existing `OriginalUiSession`. Caller supplies the real independent APK/font/text/settings/texture/draw/native providers; this adapter does not fabricate them. Surface width, height and explicit renderer orientation come from its owned driver. Resize updates the same source camera/viewport without resetting cursor/focus/drag history. Its detached load takes explicit initial milliseconds/advanceFlag, initializes through the genuine source session Update, then binds the SHA-verified status owner. The current native root constructor starts remainder at 1, so zero Update does initialize; the final fixture tests zero without manufacturing a frame interval.

Frame receives an actual resolved property sheet and Character identity, applies existing source player values, optionally executes the supplied manager batch in the exact graph Scope, then performs source input/frame Update and displays the actual HealthBars/HurtCorners paths. This is an explicit application adapter sequence, not recovered whole Application ordering. No manager callback means bounded status/input/frame service, not complete HUD manager parity. If a required provider is reached and fails, the completed prefix remains observable and later calls stop.

Actual CanHandleEvent and native event providers are required at load. No always-accept default exists. Cursor/button/mask/slot routing, consumption, pause/delta clock and advanceFlag remain caller policies; only MenuFX forwarding of the original bool is recovered. The original Android/Application producer is unproved. A source cursor receives screen coordinates and uses the same real viewport as display. Three-dimensional mouse/drag is outside this two-dimensional adapter.

The current root-owned app keeps these resource providers inside its private status-only Impl. Parent must expose/factor a genuinely independent provider owner for this new adapter rather than using an owner which retains the outer input session or strong graph values. Full manager native acceptance, attached world/player profile and Android pointer policy must be connected before live acceptance. Old app files and their prior live scene remain untouched.

## Fresh-player loader safety

The actual recreation proof first failed: shared/main SWF reads returned exact 54-byte fixture files, but fresh root playlists/actions were empty after the last player had been destroyed. Vendor `player::~player` calls `clears_tag_loaders`, while `gameswf_impl.cpp::ensure_loaders_registered` used a function-static bool that remained true. The frozen player-map safety overlay fixed a different UAF, not this registration loss. Earlier session V1's recreation check did not require newly decoded actions; it remains an honest historical proof.

NEW `overlays/loader-lifetime-v1/gameswf_impl.cpp` replaces only the impl TU. It uses `get_tag_loader(0, ...)` to consult the actual registry instead of the stale static bool, then registers the standard table when absent. NEW `gameswf_loader_lifetime_overlay_v1.cmake` enforces exactly one stock replacement. Both frozen vendor and earlier overlays are unchanged. This is a modern lifecycle safety correction, not original ARM32 teardown parity. Current proof destroys the last player, decodes fresh SWF tags, executes native startup and binds/updates status. Frozen V1 regression now records 122 checks rather than historical 121 because fresh startup really executes.

## Central integration recipe

Add `swf_source_movie_v1.cpp` and `swf_input_session_v2.cpp` to the UI DSO alongside the already frozen input/frame/session V1 sources. The app target later adds `original_ui_input_session_v1.cpp` and includes engine-ui. `PlayerStatusHud`, HUD values/sprite timeline/core/advance owner, ActionScript, viewport and facade dependencies are linked once. Keep one set of GameSWF globals.

After `gameswf_sources.cmake`, select input V1, frame V1, player-lifetime V1 and NEW loader-lifetime V1 recipes. They replace separate character/object/sprite/root/button/player/impl TUs. Font/text overlays also use separate TUs. Preserve vendor header consistency and `TU_CONFIG_LINK_TO_JPEGLIB/LIBPNG/FREETYPE/THREAD=0` across core/clients. Do not central-enable until every plain legacy movie obtains a weak source default owner or an explicit already-owned observer strategy. Frozen tests cannot simply run unobserved with this core.

Isolated proof target `source_session_audit` links actual `libsession_core.so`, plus session/input/frame/gold regressions. Reproduce from the repo using WSL:

```sh
python3 port/engine-ui/tools/swf_input_session_host_v2.py \
 --build-directory .local-inputs/swf-source-session-v2-host \
 --report port/engine-ui/reports/NEW-source-session-proof.json
```

Final proof: `reports/swf-source-session-v2-host-audit-final.json`; 59 new checks, 122 session checks, 17 input cases, 5 history cases, 51 native events, 43 actual AS methods, 28 core frame checks; original-derived input6000/frame2400/drag2400 with exact ordered services and zero mismatches. ASan/UBSan/LSan zero. Strict stderr checks preserve only the deliberately missing fixture resource diagnostic. Source hashes, immutable vendor hashes, actual DSO/executable hashes are bound. Prior intermediate report remains historical.

`tools/swf_source_session_android_compile.py --compiler <NDK29 clang++> --output <workspace scratch> --report <new report>` compiles source wrapper/session V2/loader impl/new app adapter for ARM64 and x86_64, O2/API26, preserving argument/source/object/dependency hashes. Receipt `reports/swf-source-session-v2-android-compile.json` is eight objects only: no shared library link, APK, GPU or ADB.

The final freeze binds these proofs and verifies all source/proof paths from the prior two freeze manifests unchanged. Later global facade migration and authored HUD/backend/Android acceptance are separate required steps; this batch does not assert them complete.
