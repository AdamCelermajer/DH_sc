# Material image identity V43

The new adapter preserves the actual BRES image identity that the current Scene decoder reduces to a basename. Call it using the SAME Scene/material/BRES pair from the checked actor draw callback or pinned module resource frame. The additive scoped actor wrapper proposal binds that pair directly. The low-level function validates material pointer membership, catalog cardinality and material ID; those consistency checks alone cannot establish BRES ownership. The checked producer transport remains required.

Each type11 sampler retains material catalog, parameter record/order/name, image catalog/record/ID, full authored URI and source resource URI. The actual UINT32_MAX image index is a no-image state. An absent sampler is an empty list. A missing/empty image or unsupported sampler array is explicit; it never becomes a white/default image. The decoder preserves all sampler records, including Specular, LightMap and transparent-sampler. Main's actual shader/technique parameter binding selects the samplers it uses; source order is preserved for aliases.

Original CResFactory::getTextureImpl6594a0 tries resource_directory+'/'+SImage.path, then the full SImage.path. Five actual ARM cases with explicit string storage and unavailable texture-manager endpoints match the new native candidate helper. Original getTexture659704 also supplies SImage.id as an optional cache identity when global byte29 selects it. Its CResFile embedded-image continuation is a separate required original service. The adapter does not invent that service or a texture cache key.

OriginalCacheAssetsV1 already exposes the complete original URI directory and read API over ZipAssetPackV1. Wire MaterialImageServicesV43 to that SAME retained owner and reader (main owns allocation admission/accounting). Successful reads return actual owned CPU bytes and retain the filesystem owner independently of actor release. GPU handles, mip/filter/material parameter bindings, pass registration and graphics-context generation remain main renderer services.

supplied_cache_image_uri_v43 is an explicit compatibility policy for the supplied cache: q:/data/iphone/ becomes data/, or already relative data/ stays intact, then the real ZIP key normalizes slash/case and rejects unsafe paths. It preserves full hierarchy. This is NOT claimed as recovered original filesystem mount routing. Unsupported legacy drives/parent exports require the real resolver. FileSystemBase34e96c contains a separate case-sensitive .tga/basename branch; its exploratory oracle uses a fixture root/string owner and cannot establish the actual gameplay filesystem root/archive configuration. Do not adopt it as a level texture resolver. No unrestricted basename search is provided.

## Evidence

- Original cache: 6,833 files, SHA256 3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679.
- Census: 2,904 BRES files, 3,662 image records, 291 unique authored image paths. Strict compatibility candidates exist for184 unique paths;107 are missing or outside that domain. This census includes old/unreached assets and unreferenced image catalog entries, not a current gameplay rendering failure count.
- ASAN/UBSAN standalone native probe: three actual chest/swamp/crypt resources,18 materials,27 sampler records,9 actual no-image sentinels,18 actual original ZIP texture reads,18 foreign-material rejections and5 original factory comparisons. Missing image, legacy path, parent path, absent owner and sampler arrays fail atomically. Actual CPU byte/filesystem ownership survives actor-source release and expires after image release.
- This standalone build compiles current selected scene/resources/math sources directly plus the real ZIP reader. It does not link old V4 gameplay archives or mutate V5/shared source/CMake/manifest.
- Parent has separately confirmed coherent V41 chest draw proof: helper retained for bounds, helper hidden/render-disabled, two visible native skinned primitives, materials37/38. V43 does not claim GPU rendering, Character visuals or whole-level readiness.

| Subtask | Status |
|---|---|
| Actual material/image provenance transport | Finished; independent source proof |
| Actual cache reader and independent CPU byte lifetime | Finished; three real resources under sanitizers |
| Original factory path candidate order | Finished; five ARM/native comparisons |
| Original full filesystem/archive mount routing | Open; required owner service |
| Main adoption and actual GPU texture/material binding | Open |

Adopt material_image_identity_v43.hpp/.cpp with the same Scene/resources include selection and ZipAssetPackV1 target as the retained renderer input. Parent owns CMake/manifest changes. The proposed actor wrapper calls inside V38's checked synchronous consumer; never keep actor references outside that scope. CPU byte ownership here does not establish GPU residency or resource-budget admission.
