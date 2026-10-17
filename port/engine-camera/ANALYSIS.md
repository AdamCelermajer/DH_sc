> Imported static research from engine branch `e6da25b`. Current implementation and native wiring are tracked in the [branch audit](../../docs/BRANCH-AUDIT-2026-10-05.md).

# ARM camera and dynamic-light runtime paths

## Source and verification

The source is APK member `lib/armeabi-v7a/libDungeonHunter2.so` (15,938,284 bytes). The supplied APK SHA-256 is `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`; the ELF SHA-256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. `original-functions.json` gives the ELF VA, PT_LOAD file mapping, byte length, symbol, and SHA-256 for each selected function. The copied byte rows were compared byte-for-byte with the APK ELF. It also records direct hashes for the two CCameraSceneNode vtable words used to resolve the projection dispatch.

Addresses are ELF virtual addresses. Function-level provenance and all hashes are in [original-functions.json](original-functions.json); exact selected ARM bytes are in [reference/original-functions.asm](reference/original-functions.asm).

## Camera fields and cached matrices

The getters and setters expose a compact runtime layout. This is an observed object layout for this library, not a serialized BRES record layout.

| Camera member | Observed object offset | Evidence |
| --- | ---: | --- |
| Orthogonal-mode byte | `+0x134` | `ICameraSceneNode::isOrthogonal()` loads this byte; `setProjectionMatrix()` stores its boolean here. |
| Target vector | `+0x138..+0x143` | `setTarget()` copies three words; `getTarget()` returns the address. |
| Up vector | `+0x144..+0x14f` | `setUpVector()` copies three words; `getUpVector()` returns the address. |
| Orthographic magnitude | `+0x150` | `setMAG()` and `getMAG()` access this scalar. |
| FOV | `+0x154` | `setFOV()` and `getFOV()` access this scalar. |
| Aspect ratio | `+0x158` | `setAspectRatio()` and getter access this scalar. |
| Near and far values | `+0x15c`, `+0x160` | Corresponding setters and getters access these scalars. |
| Far-to-infinity flag | `+0x164` | Corresponding setter and getter access this byte. |
| View frustum | `+0x168` | `getViewFrustum()` returns this address. |
| View matrix cache | `+0x1ec` | `getViewMatrix()` returns this address. |
| Projection matrix cache | `+0x274` | `getProjectionMatrix()` returns this address. |

The target and up setters only copy their three input words. Projection-parameter setters store the value and dispatch through the camera vtable slot at byte offset `+0x158`. In the exported CCameraSceneNode table, the word at raw table offset `+0x174` names `recalculateProjectionMatrix()`. The projection recalculation's other virtual call at slot `+0x150` resolves to `ICameraSceneNode::isOrthogonal() const`, whose word is at raw table offset `+0x16c`. This mapping uses the class table's address point at raw offset `+0x1c`; both raw words are preserved and individually hashed in the provenance manifest.

`setProjectionMatrix(matrix, bool)` stores the boolean at `+0x134`, copies `0x41` bytes from the supplied matrix into the cache at `+0x274`, and calls `SViewFrustum::setTransformState(2)`. The `0x41` copy length is directly visible in the body; its final byte is the matrix identity hint used by this engine's matrix type.

## Projection calculation

`CCameraSceneNode::recalculateProjectionMatrix()`, VA `0x0058364c`, size 232, asks the camera whether it is orthogonal:

- When orthogonal mode is true, it calls `buildProjectionMatrixOrtho(magnitude, aspect, near, far)`.
- When orthogonal mode is false and the far-to-infinity byte is false, it calls `buildProjectionMatrixPerspectiveFov(fov, aspect, near, far)`.
- When orthogonal mode is false and the far-to-infinity byte is true, it calls `buildProjectionMatrixPerspectiveFovInfinity(fov, aspect, near)`.

Each branch copies `0x41` bytes into the projection cache, then calls `SViewFrustum::setTransformState(2)`. The exact helper bodies for all three matrix builders are included in the assembly reference. This establishes the branch inputs and matrix handoff; it does not claim a high-level graphics API convention beyond the recovered helper names and instructions.

## View matrix and frustum flow

`CCameraSceneNode::recalculateMatrices()`, VA `0x00583280`, size 692, reads the node's absolute position through `ISceneNode::getAbsolutePosition()`, reads the stored target and up vectors, and normalizes/corrects vector inputs before calling `buildCameraLookAtMatrix`. The generated matrix is copied into the view cache at `+0x1ec`. The body then calls `SViewFrustum::setTransformState(0)` and `recalculateViewArea()`.

`CCameraSceneNode::onRegisterSceneNode()`, VA `0x00583534`, calls `recalculateMatrices()` before its active-camera registration path. `CCameraSceneNode::render()`, VA `0x005820e4`, sends the cached projection matrix to a video-driver virtual call using transformation-state value `2`, then the cached view matrix using state value `0`.

`recalculateViewArea()` obtains the camera's absolute position, writes it into the frustum base fields, and calls `SViewFrustum::setFrom()` with the cached matrix at camera offset `+0x2b8`. `SViewFrustum::setFrom()`, VA `0x005826c0`, size 580, combines the matrix terms into six plane records and normalizes each plane in a six-iteration loop. The exact transformation-state meanings beyond their call sites are not expanded here.

## Dynamic light path

`CLightSceneNode::render()`, VA `0x00583d18`, obtains its scene manager's video driver and, when present, tail-branches to `IVideoDriver::addDynamicLight()` at VA `0x005aaa40`, passing the member at node offset `+0x134` by reference. The callee reads its active count and capacity; it appends the light pointer only when the count is below capacity, then increments the active count. This is the bounded engine-side handoff from a light scene node to the driver's dynamic-light list.

`CLightSceneNode::getBoundingBox() const`, VA `0x00583cf0`, compares the cached 16-bit value at node offset `+0x138` with the value at light offset `+0x58`; if they differ, it calls `doLightRecalc()`. That recalculation reads the value at `light+0x58`, branches on its numeric value, uses a scalar at `light+0x40` on one path, updates six coordinates stored from node `+0x13c` through `+0x150`, and changes automatic culling. The evidence does not establish the semantic names of these fields or the numeric type-to-shape mapping, so they remain numeric observations.

`glitch::video::CLight::setAbsoluteTransformation(matrix)`, VA `0x0059fc00`, size 64, checks the byte at light offset `+0x54`. If it is zero, it copies `0x41` matrix bytes into the matrix storage reached through `+0x50` and returns true. If nonzero, it reports an error through the engine logger and returns false. The meaning and owner of that byte are unresolved by this function alone.

`CLightSceneNode::onRegisterSceneNode()` is included as a supporting range. It contains an indirect scene-manager call; its callback semantics are left unresolved here.

## Boundaries and unresolved work

- This is a runtime slice for the engine scene/video classes, not a camera/light reconstruction for the whole library.
- It does not decode serialized camera or light records from BRES; scene-data fields remain unresolved in the separate scene analysis.
- Game classes such as `CameraLevel`, `CameraTarget`, `LightSetManager`, and level lighting policy are excluded.
- The renderer implementation behind its virtual matrix calls, the exact graphics API clip-space convention, camera input behavior, and driver light upload are not reconstructed here.
- No C++ helper was added because a trustworthy standalone port would need to preserve the engine matrix ABI, its identity byte, frustum state, and renderer calls. This checkpoint adds assembly evidence only and does not claim engine completeness.

