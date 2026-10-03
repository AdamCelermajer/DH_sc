# Native character skinning

This module reconstructs skin controller data and CPU vertex deformation from the original engine's data consumers. It owns validated joint links, inverse-bind matrices and vertex influences. No original ARM32 code runs in the APK.

The Prince's 173 controllers resolve to 173 meshes, 25,204 vertices and a 35-node skeleton. The idle knight clip contributes 29 transform tracks across two contiguous segments, covering 0 to 3,000 ms. Its bone names resolve through serialized scene node SIDs, while animation targets use node IDs. The Android preview selects the four `default_warrior` equipment meshes explicitly; equipment selection and gameplay have not been reconstructed.

Serialized controller records are 12 bytes: type, ID pointer, skin pointer. In the skin record, inverse-bind matrices are at `+4`, source geometry URI at `+0x70`, joint count/names at `+0x74/+0x78`, and influence data at `+0x80`. The byte at `+0x98` gives one to four influences. Each vertex has four byte indices followed by that many float weights. Complete-file on-demand buffers resolve through the original 16-byte deferred records. Vertex iteration uses the mesh's vertex count, as the original software skin path does; `SSkin+0x7c` is not interpreted as that count.

Palettes use affine `(joint_world * inverse_bind) * bind_shape`. Original `prepareCache` performs both multiplications at `0x66fefc` and `0x66ff10`; the vertex kernel then reads raw mesh positions. The earlier character checkpoint omitted the final multiplication, which was invisible for its identity bind shapes. The original skeleton's bind shape translates Z by 267.352; including it fixes the stretched skeleton found during object screenshot review. Bind shapes and inverse-bind matrices are checked as affine. Position deformation preserves the original accumulation order and stops at the first zero weight. Normals, original hardware skinning shaders, pose blending, skin cache ABI, and original lighting remain pending.

Original candle decors include zero-influence vertices. Their records are now accepted without normalization. The recovered software kernel returns zero for them; the object renderer uses a separate explicit policy that retains their rigid node-transformed positions. This policy preserves the candle bodies but has not been compared with the original hardware shader. The Android renderer uploads CPU-deformed positions each frame and uses an unlit preview shader.

[Captured routines and hashes](original-functions.json) and [assembly evidence](reference/original-functions.asm) document the source of the layouts and arithmetic. The [host audit](reports/host-audit.json) loads every Prince controller, samples eleven poses including the segment boundary, and probes 1,000 mutated images under ASan/UBSan. It also confirms deformation changes between poses.

[The ARM64 differential test](reports/arm64-differential.json) checks 200 affine matrices, 100 skinned vertices, 100 position interpolations and 100 quaternion interpolations byte for byte against original ARM instructions. The vertex comparison executes the original position-only instruction range `0x6705c8..0x6708bc` with caller-provided buffers; it does not execute the runtime GL-buffer setup. The original animation accessor and quaternion blender execute without mocked getters. Imported soft-float and UCRT libm dependencies are modeled identically for both CPUs, so this does not establish historical Android libm parity. The combined validation library is built from the same source but is separate from the APK's split libraries.

From the repository root:

```powershell
.\port\engine-skinning\tools\build_oracle.ps1
uv run --with pyelftools --with unicorn python port/engine-skinning/tests/differential.py --engine .local-inputs/libDungeonHunter2.so --library .local-inputs/character-oracle.so --report port/engine-skinning/reports/arm64-differential.json
```

For the host audit, configure this module with CMake and `-fsanitize=address,undefined`, build, then run `skin_audit path/to/prince_modular.bdae path/to/prince_menu_idle_knight.bdae`. Original binaries/assets stay ignored. See the Android project's character checkpoint reports for GPU playback and lifecycle checks; physical ARM64 testing and a playable game remain pending.

The [object checkpoint comparison](reports/objects-arm64-differential.json) additionally executes `prepareCache`'s actual two-multiply instruction range `0x66fee0..0x66ff14` for 200 nonidentity bind-shape cases. Alongside 200 affine, 100 vertex (including ten zero-influence), 100 position and 100 quaternion cases, all 700 comparisons are byte exact. Historical reports retain their earlier hashes and scope.
