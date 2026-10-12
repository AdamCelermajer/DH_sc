# Scoped actor image binding V45 candidate

The new adapter enters the existing V38/V37 checked synchronous draw stream itself. Callers cannot submit detached actor primitives. Container/Character helpers derive the actual published Visual2d8, model CString290 and retained Visual owner from the canonical producers. Each source draw is checked against the same canonical manager handle/lease, registered root, Visual, Scene/BRES allocation, Instance and original material slot before and after resource and consumer callbacks. Actual visibility/render fields remain the V38 producer's input.

V43 decodes all ordered sampler identities directly from that actual bound material and BRES. A source UINT32_MAX no-image remains an explicit record with no bytes, cache URI, admission or fallback texture. For authored images, the actual main resolver produces a canonical full URI once, the real ZIP key validates it, and main's required admission provider supplies a lease BEFORE the actual read callback allocates bytes. The adapter creates no parallel budget. It aliases the actual returned byte vector with the main admission and filesystem pins, with no additional byte copy. Even a byte-only shared_ptr copy keeps those ownership/accounting leases alive.

The source consumer receives the same native skin/world draw and the complete ordered image bindings. Main still owns which samplers the real technique consumes, GPU admission, texture format/mips/filter/program/context-generation, draw/pass registration and rollback/discard of partial queues. The adapter's GPU residency claim is always false. It does not imply original mount/format routing; the V43 supplied-cache compatibility callback is explicitly just a policy option. It never searches global basenames or invents Character Visuals or whole-level readiness.

## Candidate proof status

Both generic_actor_image_binding_v45.cpp and the V41-derived new chest probe pass syntax checking with the current selected V5 physical header tree and the existing target's compiler flags plus the selected asset-payloads include. Dependency-file output flags were stripped. No gameplay library was linked or run while the parent changes RetainedLevelModuleGraph layout; parent must perform a coherent rebuild before the candidate's actual proof can pass.

The prepared source probe uses the same real V41 chest producer and original ZIP. Expected assertions are two native skinned primitives, actual materials37/38, eight sampler records, two no-image records and six original-cache reads. It checks required admission before reads, absent/null admission provider, foreign actual receiver resource lookup, noncanonical original drive URI, missing original image, stale shared handle, deliberate Visual release in the final callback, released owner on the next consume, and byte-only admission lifetime. CPU admission and backend consumer are explicitly host endpoint fixtures; passing this probe will not prove main's production resource-budget/GPU services.

Normal source release checks reject stale owners before dereferencing source arrays; the deliberate Visual-release callback test must stop the V38 stream before it touches another freed source reference. Owning runtime thread only: receiver destruction/reinitialization during callbacks remains prohibited by V37's lifetime contract. These checks are not a generation tracker for adversarial release-and-reinitialize at the same addresses. Out statistics are atomic on failure; external callback effects are not rolled back.

## Adoption

Add generic_actor_image_binding_v45.cpp with V38/V39/V43 and the same selected Scene/BRES/Visual header selection. The new source includes zip_asset_pack_v1.hpp; add the selected port/asset-payloads include (V43 also needs the selected engine-resources include). Existing actual ZIP reader libraries provide key/read. Parent owns CMake/manifest and coherent builds. No selected/common/shared sources were changed by this task.

tests/generic_actor_image_binding_v45_probe.cpp copies the real source fixture and inserts tests/generic_actor_image_binding_v45_capture.inc after the V41 helper proof. It retains common graph cleanup after the adversarial Visual release. Parent may regenerate that independent fixture from the coherent successor source using reference/actor-image-binding-v45/prepare_probe.py; never compile/link it against old V4 or dirty V5 archives.

| Subtask | Status |
|---|---|
| Typed same-producer V38 -> V43 image transport | Implemented; syntax passed |
| Main admission-before-read and byte-alias ownership seam | Implemented; syntax passed |
| Real-chest positive/foreign/stale/release test candidate | Prepared; syntax passed |
| Coherent actual chest test execution | Pending parent rebuild/test |
| Main original resolver/budget/GPU/pass service adoption | Open |
