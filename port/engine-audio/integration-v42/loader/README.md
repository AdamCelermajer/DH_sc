# Complete Container InitPost precache seam

`container-precache-complete-v42.patch` changes only the reached Container InitPost audio prefix and appends one borrowed service callback. It preserves the source spawn/MeetCondition/visual/timeline/mesh-box ordering and following script/derived InitPost operations. Shared files were not edited. Apply only with actual root loader receiver wiring.

Bind `services.precache_complete_source_v42` to a closure retaining the actual Container receiver/source row lookup lease. The closure calls `audio_container_precache_v42` with two actual services: Application nullable global borrow and Container GetSound. The helper captures a shared manager before querying GetSound, skips GetSound entirely on actual null, and invokes `manager.precache_raw_uid(rawUID,error)` unchanged. Getter failure remains required. The original row lookup/name/getter must run at this reached point; do not reuse an earlier cached value to avoid the getter.

The old has_sound_manager/load_sound fields remain for other legacy callers but this changed InitPost site requires the complete callback. An empty success lambda or returning false from has_sound_manager would not supply this contract. Each invocation owns a local captured lease, so nested callbacks cannot overwrite it.

Before applying, compare each actual source file's SHA256 against staged-contract.json. CRLF-to-LF-only canonical comparison permits line-ending drift; code or whitespace drift requires restaging. `validation.json` records private git apply/check, exact prospective canonical matches and four strict ARM64/x86_64 syntax checks (prospective owner plus complete helper header). No root objects/build/device were used.

After the Application constructor hooks publish the actual manager, genuine InitPost raw33 precaches the real selected-pack sample at UID33. Play3D source33 still resolves to missing XML162 during interaction. Loader progression and audible interaction are separate acceptance steps.
