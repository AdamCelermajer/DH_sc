# V43 cold primitive readiness gap found by root APK validation

The frozen V43 ZIP is historical **component evidence**, not runtime acceptance.
Root's actual `1e606da4` full-flow candidate failed at the first menu bitmap:
`SwfGpu::initialize` now leaves `buffer_=0` for lazy stream allocation, but the
unchanged `SwfGpu::primitive` entry guard still rejected `!buffer_` before the
retained cache or stream path could admit/create its real VBO.

The component host fixture executed the actual upload callback, cache, stream
storage and shader lifetime bodies. It did **not** execute the whole cold
primitive entry gate. Full NDK translation-unit compilation also could not prove
that startup invariant. The agent missed this dependent guard during review.

Root owns the corrective live hunk: primitive readiness requires initialized
authored programs; retained-cache and stream storage paths enforce their own
buffer readiness/admission. The original V43 ZIP/receipts remain unchanged so
the tested source boundary is inspectable. Do not reapply that historical patch
without the root-owned cold-entry correction. No V/finished marker is justified
until the corrected actual APK passes full menu/game flow validation.

This is a documented test-scope gap, not a claim that the source-bound ownership
fixtures failed or that a presented-FPS improvement was measured.
