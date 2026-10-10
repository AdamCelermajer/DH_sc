# Rogue1600ms original-reference review

Viewed the current actual native capture
.local-inputs/frontend-feature-build/rogue-original-movie.png and the supplied
original class screenshot
C:/Users/adamc/AppData/Local/Temp/codex-clipboard-1428e82b-433b-4258-95e0-363d0a1cc049.png.
The corrected capture has the genuine three-actor arrangement and selected
Rogue showcase, with the seated statue centered. It still differs in lighting,
some pose details and a small clear triangle at the lower-left corner. The
current image is not accepted as original visual parity.

backdrop_motion_probe.cpp loads the actual CLASS_SELECTION resource and original
animation sampler independently of the adapter's baked backdrop. Actual asset
SHA256:87303c458be43bafca5b1d717136b679d4f70623545ca1fea20841aa57033de9.

Source topology census:

- 2 visible original mesh instances; decoder retains both.
- 3 decoded draw ranges,2273 original triangles.
- 0 ignored source instances,0 decoder helper omissions/notices.
- 27 animation tracks,0 skipped unsupported channels.
- Mesh nodes: _mesh_Cylinder0117-node_PIVOT and _mesh__mesh_solide31602-node.

At source times0,650,1299,1333,1983,2633,2666,3316,4000,4033,4683,5333,
6000,6333 and6666, both visual-instance world matrices equal their decoded
rest worlds exactly: maximum difference across all16 matrix cells is0. The
scene camera and class nodes animate, but neither background mesh world does.
Therefore static backdrop baking is faithful for this exact source asset;
adding a guessed animated backdrop transform would change the source scene.

The probe projects all original submitted triangles using the corrected source
Rogue camera at1600ms, source4/3 aspect, planes50/50000, and960x540 physical
viewport. It clips each triangle against all six OpenGL homogeneous clip planes
before perspective division and a barycentric point-coverage check.

| Lower-left framebuffer pixel | Triangle coverage before culling | CCW coverage |
| --- | --- | --- |
| 8,8 | 0 | 0 |
| 32,8 | 0 | 0 |
| 8,32 | 0 | 0 |
| 48,32 | 1 | 1 |
| 120,8 | 1 | 1 |

Covered interior points belong to opaque ground Material__11806. The corner
points have no triangle coverage in skybox, opaque ground or foliage ranges.
This identifies the observed corner as uncovered finite source geometry under
the current camera/viewport submission, not material shading, alpha, backface
culling or skipped backdrop animation.

Next actual source-owner boundary: RenderClassSelectPane428b74 derives its
physical viewport from the authored class_select SWF absolute bounding rectangle
and inverse pixel scales, submits SceneManager, then restores its prior viewport.
The current frame owner must prove its movie callback bounds match that original
producer before full physical960x540 submission can establish original framing.
An original-reference platform/asset variant is another unproven possibility;
the screenshot alone cannot resolve it. No FOV/camera offsets, fabricated ground
extension, crop or background fill was added by this lane.

Strict native C++17 -Wall -Wextra -Werror compile and actual-original-asset probe
passed. External foliage L1 material/light uniform ownership remains the
separate source boundary recorded in ORIGINAL_PARITY_AUDIT.md.
