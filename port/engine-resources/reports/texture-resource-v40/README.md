# Model and effect texture admission V40

This is a staged integration handoff. No live shared model renderer, effect adapter, ZIP reader, OriginalCacheAssets, engine texture implementation or CMake file was modified. Root applies `texture-resource-v40.patch` and adds the new owned helper files already present in the workspace. Apply the surgical patch; complete staged source snapshots are inspection/compile inputs, not wholesale replacements for concurrently edited files. The CMake hunk only adds admitted_cpu_bytes_v40.cpp to the existing resource sources.

## Completed component changes

- `CpuAdmissionV40` reserves before an existing vector producer allocates, commits after producer success, and retains the shared ledger lease. `AdmittedVectorV40` aliases the exact vector and its accounting owner through one shared ownership control block. Actual bytes die before their ticket releases.
- `ZipAssetPackV1::read_admitted` invokes an optional callback immediately before the actual metadata.bytes output-vector allocation. Existing local-header, streaming inflate, length and CRC validation remains unchanged. OriginalCacheAssets forwards that callback through the same mounted reader. Other callers retain the original read API.
- The model adapter admits original-cache or bundled encoded bytes before payload allocation, admits checked decoded RGBA before allocating, and admits texture GPU bytes/count before GL. Bundled reads use streaming mode. Existing repeat/linear sampling and actual decoded pixels are preserved. Ownership vectors and map nodes are prepared before GL allocation; failures remove unpublished cache entries and candidates. Model GPU tokens have explicit actual-name/context-generation ownership and cleanup.
- The effect adapter retains the original GeneralFxTextureImageOwnerV2 implementation and all source sampler/update/mipmap/unbind paths. A decoded CPU ticket is admitted before calling its existing initialize; encoded source alias lifetime and decoded ticket lifetime match the actual image. GPU storage admission includes the full mip chain selected by the actual texture_image_descriptor_v1 and driver options. An additional final allocation barrier catches mip-generation GL failures before GPU publication.
- FX state restoration checks the same actual context generation; it discards the borrowed source grid after context loss instead of binding recycled old names into the replacement context. GPU release pairs with original unbind on a live context or name discard on a lost one.
- Generic model textures from world, actor previews, equipment and loot share the registry. Loot's explicit lost-context release runs before remaining model registry discard during reset; this avoids a double release. Active FX has no exemption.
- One pre-existing texture_owner_v1.cpp continuation is split across two lines for strict GCC diagnostics. It changes formatting only, not statement order or behavior. No global engine texture header changes are needed.

## Source-bound verification

`host-receipt.json`: O1 and O2 strict GCC (`-Wall -Wextra -Werror`, no warning suppression) with ASan/UBSan each pass 183 checks; final live records and fake GL names are zero. The fixture compiles the actual new model helper, staged effect adapter/owner fields, staged ZIP reader and actual existing decoder, sampler, mipmap, binding and unbind implementations. The test archive is below 4 KiB and contains 82-byte stored/deflated TGA payloads; the driver holds metadata and a four-byte sample only.

Checks cover encoded/decode/GPU quota denial before the relevant allocation, injected pixel allocation failure, cache-node failure before GL, zero texture name, base upload OOM, generated-mip OOM, exact source default mip and disabled-mip behavior, actual decoded pixel values, repeat/linear sampler state, warm-cache reuse, retained external FX ownership, bundled-reader short failure, original-cache equipment and stored ZIP loot scope, and reset cleanup composition. Model and FX context-loss tests reuse the old numeric name for a newly admitted unrelated owner; cleanup preserves that owner and the FX guard does not restore old bindings.

The fixture's OriginalCache transport forwards to the actual staged ZIP reader but uses a tiny mounted fixture instead of the APK's required 6833-entry mount. Therefore it validates the actual payload hook and source read/CRC paths, not a fresh production APK descriptor mount. Existing mount behavior is unchanged and whole-device acceptance belongs to root.

`android-compile.json`: eight strict component compilations pass (CPU helper, complete source-bound adapter fixture, staged ZIP reader and staged OriginalCacheAssets for ARM64/x86_64). Header overlays make the staged API visible consistently without modifying live sources. Whole model_renderer.cpp and the final APK/link/device flow remain root validation.

The earlier actual Crypt/Swamp/target/fire descriptor census is in resource-budget-v37/asset-census.json. The HUD catalog census is in swf-resource-v39/ui-asset-census.json. No large decode, GLES driver, device or emulator operation was performed for this handoff.

## Explicit coverage limits

The ZIP hook admits metadata.bytes output allocation. It does **not** bound the reader's total heap: mount/index/directory storage, local-name strings, the fixed compressed input scratch, zlib's internal allocations, and other reader metadata remain separate. Both stored and deflated requested output are admitted; do not describe this as a whole-reader heap cap.

Ownership maps/vectors, shared control blocks, source mip-offset vectors and registry metadata are not charged by these pixel leases. Their growth remains subject to texture object/count/record limits, but that is not byte accounting for those containers. Framebuffer/UI programs, generic world/actor VBO/EBO, CPU geometry/skin/resource graph allocations and other producers still require separate migration. These changes do not establish whole-runtime memory bounds, physical driver residency, FPS or the emulator leak's cause.

CPU charges for model reads/decode are transient. FX keeps its actual encoded and decoded images, so their CPU charges remain until the last real owner releases them, even after GPU cleanup. Context loss alone does not advertise those retained bytes as free. Static descriptor fit does not guarantee all scene/resize/concurrent-effect combinations fit the configurable engineering limits.
