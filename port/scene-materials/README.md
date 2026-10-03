# Checked BRES scene and material reconstruction

`scene.cpp` reconstructs serialized visual-scene selection, recursive 80-byte nodes, local geometry references, 60-byte material bindings, 36-byte materials and image-linked parameters. These are new C++ interfaces, not recovered studio source or ARM32 object layouts. Original instructions and hashes used as evidence are recorded in `original-functions.json` and `reference/original-functions.asm`.

Local transforms follow `ISceneNode::getRelativeTransformation` at `0x598908`: quaternion `getMatrix_transposed`, column post-scaling, then translation in matrix slots 12–14. `dh2_node_matrix` exposes this pure calculation for original ARM32 / compiled ARM64 instruction comparisons. Hierarchical world matrices compose parent × child. Geometry is selected by URI, rather than treating its serialized zero-filled runtime field as an already-resolved index. Material bindings preserve the original primitive order and the renderer verifies each primitive's material ID.

Supported preview parameters: `Diffuse`, `diffuse-sampler`, `AlphaMap`, `texture-matrix`, `Object_Alpha`, `alpha-ref`, `__irrlicht_Diffuse_color`, `__irrlicht_Additive` and `__irrlicht_Backface_Culling`. Image indices use the stored type-11 pointer array; `-1` means absent. Legacy development paths resolve to APK texture basenames. Unknown shader parameters are ignored by this limited preview; external effects, lights and full runtime factories are not implemented. External geometry/material-binding files and sampler arrays are rejected explicitly.

The loader now resolves local skin-controller instances (tag 2), their geometry URIs and material bindings, alongside geometry instances (tag 3). Node SIDs are retained for bone lookup. The [skinning module](../engine-skinning/README.md) owns skin data and deformation; morph controllers and external controller files remain unsupported. The modular Prince's outfit is selected by the Android preview rather than reconstructed game equipment logic.

The immutable loader validates ranges, strings, finite components, counts, graph depth and cycles; it publishes a scene only on complete success. It copies strings/transforms into owned output and never relocates file pointers. The output also retains a parent-first node graph with local TRS values; `update_world` validates and recomputes all transforms atomically for the animation module. A static recomputation matches the originally loaded matrices exactly. Its caller opens complete BRES files using the existing checked resource module. Local asset names in the bundler are unique; basename resolution for a larger corpus will need a collision policy.

## Validation

Two owner-supplied fixtures are not committed: `candle_flame.bdae` and `main_menu_charactere_swamp.bdae`. The native host audit verifies every instantiated mesh, material order, triangle/index data and transformed bounds. It also tests truncated views, an explicit graph cycle and 5,000 byte mutations per fixture under ASan/UBSan.

```sh
cmake -S port/scene-materials -B port/scene-materials/build/host -G Ninja
cmake --build port/scene-materials/build/host
port/scene-materials/build/host/scene_audit path/to/scene.bdae
```

`tests/transform_differential.py` executes original ARM32 node-matrix instructions and the actual compiled ARM64 `dh2_node_matrix`. External memcpy and soft-float helpers are modeled; unknown calls fail. [213 bit-exact matrices](reports/transform-differential.json): 13 stored fixture transforms and 200 deterministic synthetic transforms. This verifies local TRS arithmetic, not full scene creation or original GPU appearance. Unicorn and the original engine binary are never included in the APK.

The [Android inspector](../android-native/README.md) bundles the fixtures and renders them with a new unlit shader and orbit camera. The candle's unused material references `env_crypt_alpha.tga`, which is absent from the cache; neither instantiated candle primitive uses that material. Missing textures required by instantiated draws fail instead of silently substituting a file.
