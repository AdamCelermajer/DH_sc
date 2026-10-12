# Authored health-panel integration review

Read-only review of the Android adapter on 2026-10-04. No app source, shared
build, emulator, or packaged artifact was changed. This records source control
flow findings; it is not a GPU acceptance report. The parent reported candidate
`25ab5ce4` reaching texture/text delivery before a font bitmap-format failure.
That font producer and the 27 known GameSWF fork diagnostics are separate work.

## Required corrections

1. **A font failure during display can leave the private framebuffer active.**
   `SwfMovie::display_clip` brackets the clip with begin/end commands
   (`swf_movie.cpp:120`). The begin command reaches `SwfGpu::draw`, which binds
   its offscreen framebuffer, disables depth writes, and sets `frame_=true`
   (`swf_gpu.cpp:189-199`). A later glyph diagnostic latches `font_failure`
   (`original_ui_session.cpp:157-159`). The adapter rejects every subsequent
   draw command before invoking the GPU, including the end command
   (`original_ui_session.cpp:185-187`). Its render failure only clears
   `selected` (`original_ui_session.cpp:253`); GPU cleanup is never called.
   The following JNI frame consequently clears the still-bound private target
   (`native_app.cpp:89`). Provide an explicit GPU abort operation or guarantee
   cleanup-only end delivery on a failed display. Preserve the original error;
   cleanup must not turn failed rendering into accepted delivery.

2. **Failed font sessions are retained and cannot recover through reload.**
   `load_health_panel` destroys movie/fonts on an initial load failure but
   never clears `font_failure` or `provider_failure`
   (`original_ui_session.cpp:240-248`). A fresh movie/provider therefore inherits
   the old failure, and both load and draw reject it. A failure arising during
   first display is worse: `loaded` is already true, so a later selection skips
   reconstruction while the failed font provider and failure latch remain.
   Reset the failed session explicitly, destroying movie before font provider,
   clearing diagnostic latches and resetting loaded state. Retention across a
   successful GL context recreation remains valid and should be preserved.

3. **Java's accepted-asset cache outlives failed first rendering.**
   `MainActivity.loadSelected` records `loadedAsset` after the native load
   succeeds (`MainActivity.java:163-165`). Font rasterization can fail only when
   that retained clip is subsequently displayed; native render then deactivates
   the UI without notifying Java (`native_app.cpp:90-94`). Re-selecting the same
   item is suppressed by `loadSelected`'s early return (`MainActivity.java:160`).
   Expose render failure to the Java acceptance state, or validate the first
   display before marking the selection accepted. A successful retained
   session should continue avoiding redundant loads.

## Verified provider contracts

- The adapter loads the real ordered common-text metadata, genuine common_text
  and fonts constants, and routes logical `text/<filename>` through the explicit
  `original-cache/data/text` resource catalog. It does not use a sorted sheet
  index or manufactured translations.
- The standalone player callback returns a null character. This matches the
  original no-character branch of `ParsePlayerName`, leaving `$player` text
  unchanged. The unavailable name provider remains unreachable in that branch;
  this is not a connected live player profile.
- Debug load/query uses the genuine owned backend and real private-directory
  `fopen`; only ENOENT is delivered as a missing file. Existing-file parse
  limitations remain failures. Text and font probe leases close synchronously.
- Movie destruction precedes font-provider destruction. GPU texture CPU storage
  and logical identities survive context recreation and are uploaded anew.
  Callback context points to the retained Impl rather than temporary load data.
- The font resolver matches its frozen API: Arial selects `data/wqy-zenhei.ttf`,
  generic English names use `data/<name>.ttf`, and the adapter reports unsupported
  GFNT/system-font connections explicitly. No format fix was invented here.

## Reviewed source identities

| File | SHA256 |
|---|---|
| app cpp/original_ui_session.cpp | b8f3785744bfa4161a1012129aac5366625c68d3fb422b05f4ae2f98f00917b1 |
| app cpp/original_ui_session.hpp | 7f8f16b0bff5c1feed296ad303233cff00bdf18302fc1fbaf5d8b7973118e27a |
| app cpp/native_app.cpp | 3678560eec8a7e3c67f26b9221fcf97993ff3475aac23f6a8ef73d9a78d6be6c |
| app java/com/example/dh2/MainActivity.java | 7333861c46be542358d3919edf0951391d2e6ca9f4a92a4bf62f9b76960db716 |
| app cpp/swf_gpu.cpp | 22acb961e6a56107c917684a9220a905cf5b63d2be328edbc5abdbf31e469622 |
| engine-ui/localization.cpp | b519ab31e8553ccc808822fc1c9f846ebdff5f9e608eb31cd2fe3245d2860611 |
| engine-ui/swf_font_resolver.cpp | 1876dfb38e04eb4f31243d59d64a746ff5a3d3d62a8b1f0d44a72174d720e96a |

`app` denotes `port/android-native/app/src/main`. Line references and hashes bind
the reviewed versions; parent fixes will naturally supersede them. Frozen
localization proofs remain unchanged.
