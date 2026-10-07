# Whole legacy facade migration V1

NEW `overlays/source-facade-v1/swf_movie.cpp` compiles a versioned copy of the frozen facade with its existing public ABI. The frozen facade/header/vendor/session files are unchanged. This batch is not centrally enabled or in an APK.

The original read/draw validation, player/global registration, native callback ownership and reserved-name checks retain their order and messages. Only afterward does a legacy load allocate a fresh weak source observer service owner. That owner is installed in the candidate Impl and the typed callback context before the source startup hook and any file read/construction. Each load attempt obtains a new owner. Failed candidate startup preserves the old facade graph. Existing original caller hooks still run after observer binding; provider failures stay failures. No generic native acceptance or resource service is created.

NEW include-wrapper translation units compile each exact frozen helper/session body once, and export predicates comparing `graph_start` with that implementation's actual `Provider::start`/`SourceOwner::start`. These are explicit implementation identities, not observed-history scans or inferred flags. Default facade wrapping is skipped only for these three source-complete implementations. V1/V2 session startup therefore retains its existing ownership and cannot double-bind. An already prepared helper owner remains single-player; the default path prepares one fresh owner per attempt.

The source root/sprite/input overlays remain required. A plain facade automatically obtains them through the new default owner. There is no stock mouse/frame scheduling fallback. A raw GameSWF player/root constructed outside the facade must still install genuine history/frame ownership before character creation.

## Direct-core viewport fixture

The unchanged direct-core `swf_viewport_connection` executable initially failed `Required source frame receiver unavailable`, as expected: its Core constructor does not use SwfMovie. NEW `tests/overlays/source-facade-v1/swf_viewport_connection.cpp` preserves every frozen case/gold assertion but supplies history/frame owners before create_root and initializes the actual per-frame empty init-action array beside the existing playlist. Source observation reads that storage; without it the old hand-built fixture had an invalid definition under the source scheduler. This is test construction plumbing, not an accepted no-op or fabricated source result. The frozen file and original gold remain unchanged. Only that target needs a new test TU selection; other legacy executables are untouched.

## Integration

Build a UI source list containing the frozen facade, helper and V1/V2 session TUs, include `gameswf_source_facade_v1.cmake`, then call `dh2_select_source_facade_v1(list_variable)`. It requires exactly one of each old TU, replaces them with the four versioned TUs, and appends `swf_source_startup_v1.cpp`. Never compile the wrappers and included frozen implementation bodies separately. Keep `swf_source_startup_v1.hpp` and the pinned vendor headers visible to clients.

Select the frozen font/input/frame/player-lifetime recipes plus the new loader-lifetime recipe before building the single GameSWF core. Font/text/core overlays replace distinct TUs. Preserve the existing consistent JPEG/PNG/FreeType/thread definitions and one set of GameSWF globals. The app can then compile the frozen NEW input-session adapter. The old status-only facade callers need no source edits.

Replace only `swf_viewport_connection_audit`'s direct test source with its NEW versioned fixture. Central CMake, facade, tests and app were not edited during this proof. Actual Android pointer/button/consumption mapping, native event acceptance and frame advanceFlag remain real caller obligations. HUD manager/init recovery did not establish the original Application bool; false from a one-argument root API is an explicit adapter policy, not that producer's parity. No Android integration, APK promotion or GPU acceptance is asserted here.

## Full isolated proof

`tools/swf_source_facade_host_v1.py` copies and hashes seven transitive central DSOs, verifies them again after copy and after all tests, builds ONLY private UI/core/FT, then runs the unchanged central legacy executables against the new actual UI via LD_LIBRARY_PATH. It checks ldd for the exact private UI and absence of unresolved dependencies. The direct viewport fixture is compiled privately as described above. No shared build is modified. Its current main-runner UI suite definitions are reused; writable diagnostic outputs/settings/debug files are relocated to private scratch.

```sh
python3 port/engine-ui/tools/swf_source_facade_host_v1.py \
 --build-directory .local-inputs/swf-source-facade-v1-host \
 --central-build /home/adampalace/dh2-world-build \
 --report port/engine-ui/reports/NEW-facade-proof.json
```

Actual `reports/swf-source-facade-v1-host-audit.json` passes all34 legacy UI suites plus V1/V2 session suites with ASan/UBSan/LSan zero. It includes actual authored droid/base HUD manager startup, source localization/skill text, status, glyph/font/text rendering and ActionScript/ownership/reentry checks. Every executable, loaded private UI, copied dependency and source is hash-bound. Disabled-JPEG's known RTTI boundary uses `-fno-sanitize=vptr`; address and other undefined checks remain active. Fixtures/services and previous source-parity boundaries remain explicit; this does not establish full original VM/Application/GPU parity.

`reports/swf-source-facade-v1-android-compile.json` binds NDK29 compiler/arguments/headers/body includes and ten O2 objects (facade+three wrappers+predicate dispatcher, bothABIs/API26). It is compile-only, no library/APK/device operation. `normalization.json` proves all other facade bytes normalize exactly to the frozen original and the direct fixture differs only in owner/init-storage plumbing. The source-list CMake recipe is checked independently; reapplying it rejects missing original TU selections.

The prior source-session V2 and input/frame/session V1 freeze bytes remain intact. Parent owns central source selection, real app providers/input policy and eventual combined visible acceptance.
