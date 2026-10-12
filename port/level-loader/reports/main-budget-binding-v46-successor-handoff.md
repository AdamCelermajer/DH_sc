# Main resource budget binding audit V46

Main now has authentic source-owned encoded, decoded and GPU allocation phases. The generic loader must borrow those existing APIs and owners. This audit creates no budget system, GL backend, original mount resolver or runtime readiness claim. The moving main files were copied into an isolated reference snapshot and hashed; snapshot-readback.json reports whether they remained unchanged during this audit.

| Phase | Current main API and source | Required V45 binding |
|---|---|---|
| Single ledger | native_resource_budget_v38.cpp budget_lease_v39 | Borrow this same retained lease; do not instantiate another ledger |
| Encoded source bytes | OriginalCacheAssetsV1::read_admitted -> ZipAssetPackV1::read_admitted before_payload | Reserve the actual verified ZIP byte count before payload vector allocation; commit after read/CRC success |
| CPU admission | CpuAdmissionV40 reserve/commit/reset; AdmittedVectorV40 admission-before-bytes layout | Return the actual lease through V45; its final lifetime must outlast actual bytes and filesystem pins |
| Decoded pixels | RetainedBytesV39::allocate/reset | Separately admit exact decoded capacity before allocation; it frees storage before releasing accounting |
| GPU texture | renderer_model_texture_budget_v40.inc upload reserve_create(texture,scope,RGBA bytes,0), GL success, commit | Main owns this physical allocation; encoded admission never substitutes for it |
| Live teardown | release_model_texture_v40(name,false) | Delete only in the same live generation, then release token and owner registry |
| Context loss | native_app.cpp context_lost_v38 -> model_renderer::reset_context -> begin_context_v38 | Discard old GL charges/names through the existing generation-aware owner; retain legitimate CPU charges until bytes expire |

The current root upload helper accepts a basename, prepends data/3d/textures/ in original-cache reads and supplies a 1x1 white image for an empty name. It also rereads encoded bytes before decoding. It cannot consume V45 unchanged: the generic binding already has canonical full URI and an owned encoded allocation; no-image must bypass that read/upload and be interpreted by the actual material/technique owner. A small main-owned factoring point after the existing encoded read can accept those actual bytes and preserve every current decoded/GPU reservation, publication and rollback step. This proposal does not implement that GL factoring or original sampler/pass behavior.

EquipmentPlatform owns a retained OriginalCacheAssetsV1 cache but is itself a unique_ptr and retains WorldScriptContext. Do not alias its entire owner into long-lived encoded byte pins. Once the actual cache is mounted, an OriginalCache copy shares its existing immutable private State/descriptor; a small filesystem-only retained wrapper can serve the exact current archive. Bind the actual renderer scope lease weakly for new read validation, so immutable encoded bytes do not keep the whole world alive. Main must supply that actual live renderer/Level scope; a generic liveness token created by this adapter would not establish it.

## Concrete encoded callback proposal

main_encoded_image_budget_provider_v46.hpp/.cpp is an outside-graph adoptable source API binding. Current OriginalCache directory() exposes names, not allocation metadata. The provider uses its real read_admitted before_payload hook as a metadata-only preflight stop, after local ZIP entry/header/name verification and before candidate vector construction. It reserves the returned count with main CpuAdmissionV40 before V45's actual read, then checks the unchanged count and commits only after the real CRC-checked read succeeds. The preflight performs bounded header/name I/O twice; it does not allocate encoded payload bytes or substitute a guessed size. A later explicit metadata API could remove that duplication.

Only a weak pending ticket is kept between the admission and read callbacks. If V45 rejects stale source state before reading, its pin drops and the actual pending reservation aborts. Tickets cannot reenter/replay or change canonical URI/size. The actual filesystem owner and actual main budget remain retained; renderer scope is weak. This is callback coordination over main's existing budget, with no independent counters, limits, GPU names or actor state. main-binding-sites-v46.hpp.proposed shows the exact main wiring points but is not integrated.

The independent ASAN/UBSAN source proof uses snapshotted current main CpuAdmission/ContextResourceBudget/budget_lease implementations and the actual supplied ZIP before_payload reader. It passes exact actor-scope encoded admission of 524,348 bytes, reserve before allocation, commit after delivery, cancellation, reentry/replay, changed metadata, absent URI, no-image and dead renderer checks. Renderer liveness is an explicit host fixture. It does not prove Android OriginalCache wiring, V45 fullgraph integration, decoded buffers, GPU uploads or actual rendering.

## Destructor correction and regression

The V45 frozen candidate declared bytes, filesystem, admission. Reverse field destruction could release accounting before the final actual byte vector freed. Parent has applied the narrow correction to current V45 source: declare admission, filesystem, bytes and initialize in that same order. The frozen V45 ZIP remains immutable. v45-ordering-fix.proposed.patch records the exact two-line correction.

The standalone regression reads the actual 524,348-byte original chest atlas into a real heap vector, reserves/commits through current main admission APIs and observes the actual vector final delete and filesystem close. The old-order negative control reports three violations; the corrected holder reports zero and the exact order bytes_destroyed, filesystem_released, admission_released. Byte-only shared_ptr aliases keep all charges and owners until their final release. This proves the corrected scalar ownership kernel; it is not the complete V45 graph test.

actor_image_destruction_v46_capture.inc and byte_free_observer_v46.hpp/.cpp prepare an optional actual V45 chest hook for parent adoption. The delete observer is HOST-TEST-ONLY and must never enter production/shared libraries. It watches the actual returned vector.data final delete, then checks filesystem/admission lease cleanup. The existing host admission endpoints remain explicit fixtures. That unique hook has not been linked/run and does not alter current root probes.

## Actual chest expectation correction

The first parent V45 run exposed my fixture assumption, not a production adapter failure: Material__37 has absent AlphaMap and LightMap, while Material__38 has absent LightMap. The actual original sampler sequence is Diffuse, AlphaMap, LightMap, Specular with orders2/3/4/5. Material37 indices are0/FFFFFFFF/FFFFFFFF/1; Material38 indices are0/2/FFFFFFFF/1. The owned test now checks those exact identities and expects eight samplers, three source no-image records and five actual reads, preserving all no-bytes/no-URI/no-admission/no-fallback guards. The original frozen candidate expected two/six; do not reuse that assertion. Parent's coherent rerun determines the final V45 proof status.

| Subtask | Status |
|---|---|
| Current main phase/API and scope mapping | Finished; hashed source snapshot |
| Preflight/admit/read/commit binding proposal | Source proof passed outside graph |
| Corrected destruction order and byte-only lifetime | Real main API/ZIP kernel passed |
| Actual V45 optional final-delete observation hook | Prepared; not linked/run |
| Actual main provider/decoded/GPU integration | Open; main adoption required |

## Verified current-main successor

Main added ResourceKind::shader, explicit unknown shader/program GPU-storage counts, per-scope peaks and budget_report_v46 during this audit. This changes the public snapshot/usage ABI. The initial proof remains a captured historical source proof; the successor rebuilt every budget-dependent source against one complete new header/source snapshot and reran both ASAN/UBSAN kernels. Both pass with the same actual 524,348-byte encoded allocation. The complete successor snapshot's hashes remained stable on readback. No old/new budget objects or headers were mixed. Neither proof claims actual driver residency, which main's new diagnostics explicitly mark unmeasured.

The current V45 production source uses the parent-corrected destruction order; current owned probe assertions preserve exact material37/38 source metadata and expect3 no-image records/5 reads. The optional V46 actual chest deletion hook remains prepared for parent adoption and is not part of the standalone kernel proof.
