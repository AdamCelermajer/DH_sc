# HUD texture and framebuffer admission V39

This is a staged, source-bound integration handoff. Shared live SwfGpu, UI-session and CMake files have not been edited by this agent. Root applies `swf-resource-v39.patch`; its baseline raw source hashes are recorded separately.

`SwfGpu::initialize` now requires an explicit scope. Production call-site hunks are included. The separate deferred host GLES fixture `swf_gpu_host_pixels_v38.cpp` must pass `swf_gameplay` when it is next compiled; that existing test file is not edited here.

The new `RetainedBytesV39` owns exact-size CPU bytes and a strong shared ledger lease. Admission occurs before `new[]`. Move transfers ownership; release deallocates actual storage before releasing its charge. SwfGpu also retains a strong lease on initialize, and all new allocation/cleanup paths use that held lease directly. This keeps the ledger alive through namespace-static UI destruction without calling the singleton factory during destruction. The existing UI-destructor assumption that GL calls still have a current context is a separate baseline lifecycle boundary.

The staged SwfGpu patch covers:

- Explicit `swf_front` and `swf_gameplay` scopes at their real session initialization sites.
- Retained bitmap/font bytes admitted before allocation; GL_ALPHA/GL_RGB/GL_RGBA bill actual requested channel bytes. GPU texture bytes and counts are admitted before GL name/storage creation. The map node is allocated before GL ownership, preventing allocation failure from losing a moved uploaded texture.
- Two target color textures, two framebuffer names and one shared packed DEPTH24_STENCIL8 renderbuffer. Full candidate bytes/counts are admitted while old targets remain alive; successful completeness/error barriers precede publication. Quota refusal and partial GL failure preserve the old target and clean candidates.
- Context-generation cleanup: old names are discarded after real context loss, never deleted in the new context where another owner may reuse their numbers. CPU bitmap identities and bytes remain retained for reupload. Abandoning names from the still-live same context is rejected.
- Exact CPU pixel buffers for both session decode temporaries before decoding, and stencil-query readback storage before allocation. Retained copies and decode temporaries are separately charged while coexisting.
- Image reset deletes/releases tracked GPU names before destroying the retained CPU owners. No program, vertex-cache or active-effect exemption is introduced.

## Evidence

`host-receipt.json`: source-bound fake-driver tests compile the staged production header (visibility only exposed for inspection) and five actual new SwfGpu methods plus the unchanged error barrier. O1/O2 strict GCC with ASan/UBSan pass 115 checks, final owned records zero. Tests inject CPU allocation failure, CPU and GPU quota denial, unsupported GPU dimensions, map publication failure, zero GL name, bitmap OOM, second-framebuffer incompleteness, target OOM, and context loss at the real barrier with numeric-name reuse. They verify admission before actual CPU allocation and before fake GL mutation, old/new resize overlap, output atomicity, old-target preservation, CPU retention/reupload, channel-specific bytes, front scope, move safety and retained-ledger lifetime. Allocation/cleanup is verified to avoid singleton-factory calls after the owner has been configured. Fake GL owns only name/byte metadata; no GLES driver or device is involved.

`android-compile.json`: complete staged SwfGpu.cpp and RetainedBytesV39.cpp pass ARM64 and x86_64 NDK component compilation at O2 with `-Wall -Wextra -Werror`, no warning suppression. This includes the stencil readback hook. UI-session decode hunks use the same helper but are not independently whole-session compiled by this receipt; root's full APK build must validate them.

`ui-asset-census.json`: eight original hash-verified catalog texture variants, reading 14,451,136 encoded bytes without decoding pixels. One RGBA copy of all catalog variants is 27,525,120 bytes; the largest is 8,388,608 bytes. A static envelope with duplicated front/gameplay catalog copies and both owners' targets fits current engineering limits across the representative viewport table through 3200x1440. At that largest representative size it totals 165,642,240 requested GPU bytes and 128,778,240 texture bytes. These are representative dimensions, not observed device dimensions.

Static fit does not prove all concurrency fits. Old/new target overlap during resize, generated glyphs and world/effect textures share the same quotas. A legitimate request can therefore be refused before allocation; no limits are raised automatically. Glyph atlases, source export selection, live device behavior, visual parity and FPS remain root acceptance work. No emulator/device/driver operations were performed here.

## Remaining boundaries and next texture adapters

This expands admission to HUD image/target GPU owners, retained bitmap/font CPU storage and selected decode/readback CPU producers. Source encoded archive readers, font rasterizer scratch, SwfVertexCacheV36 CPU/GPU storage, streaming vertices, UI programs, world/actor buffers, and CPU FX geometry caches remain separate producers. This does not establish a whole-runtime memory bound or diagnose the prior emulator leak.

The next model texture adapter is `model_renderer.cpp::upload`: reserve encoded reads at the actual reader, checked RGBA decode storage before allocation, then GPU texture count/storage before glGenTextures/glTexImage2D. Publish cache entries only after GL success and handle cache-node allocation before GL ownership. Couple token release to `release`/`release_objects` and actual context loss; keep requested channel/format bytes distinct from driver residency.

The next effect texture adapter is `GeneralFxTextureImageOwnerV2::initialize` plus `EffectGpuTextureV4/effect_texture_v4`: pass the same ledger lease and explicit FX scope to the actual image owner, admit decoded RGBA before allocation, admit checked requested mip-chain GPU storage before GL creation/upload, and release on the actual texture-cache teardown/generation discard. Existing source format conversion, sampler/mip decisions and material/queue semantics must be retained. Source encoded vectors and generated mip storage must be admitted at their actual allocation producers, not merely measured afterward. These adapters are a plan, not implemented by this handoff.
