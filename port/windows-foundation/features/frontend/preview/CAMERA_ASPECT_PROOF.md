# Original class camera aspect ownership

The original class preview perspective camera retains a4/3 aspect, bits
0x3faaaaab, float1.3333333730697632. It does not derive this value from the
physical viewport960x540 or logical SWF480x320. The physical scene viewport and
the camera's projection aspect are distinct source state owners.

Exact original binary/decompilation ownership chain, all read from the existing
IDA export of libDungeonHunter2.so under
.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode:

1. Base scene CCameraSceneNode C1 at583734 initializes object+340(FOV) to
  1067506044, and object+344(aspect) to1068149419. The latter is exactly
  0x3faaaaab. The constructor receives id, position, target and a flag; it does
   not take a driver or a viewport. There is no screen width/height calculation.
2. Collada camera C1 at6e5550 invokes that base constructor. Its perspective
   branch computes tan(serialized horizontal FOV*.017453*.5)/serialized aspect,
   calls atan and doubles it, then calls setFOV. It never calls setAspectRatio
   on the perspective branch. Only the orthographic branch explicitly sets
   serialized aspect and magnitude. Thus the perspective4/3 field survives.
3. MenuCharacterSelect::Show428f38 constructs CLASS_SELECTION and retrieves
   its camera. Show assigns up(0,0,1), near50, far50000 and local(0,0,-200),
   makes it active, and constructs/binds its animator. It has no aspect setter.
4. setAspectRatio582084 writes float+344 and invokes recalculateProjectionMatrix.
   recalculateProjectionMatrix58364c passes exactly fields+340/+344/+348/+352
   to buildProjectionMatrixPerspectiveFov; no viewport dimensions are read.
5. Collada onRegisterSceneNode6e5238 refreshes the target from its attached
   target node or transforms local(0,0,-100) by camera world, then delegates to
   base onRegister583534. Neither changes camera aspect.
6. RenderClassSelectPane428b74 derives physical viewport bounds from SWF
   class_select absolute bounds and inverse pixel scale, submits SceneManager
   drawAll, then restores the previous viewport. It does not set camera aspect.
7. IRenderTarget::setViewport6dc5a8 retains/clamps viewport bounds and dispatches
   the active target's driver setViewportImpl. Programmable GL driver
   setViewportImpl5b0a9c calls glViewport and may refresh its2D projection.
   It never calls camera setAspectRatio or modifies an active3D camera field.

The earlier preview aspect0 selected Renderer viewport-derived projection. On
a16/9 physical surface that widened camera horizontal view beyond the original
4/3 projection and exposed edges of the finite authored terrain. The adapter
now assigns the original literal bits0x3faaaaab; source FOV, camera transforms,
near/far planes, scene geometry and physical viewport remain unchanged. No
invented camera shift, FOV adjustment, terrain extension or crop is introduced.

Verification: native LLVM MinGW C++17 strict compile and actual-original-asset
tests passed after this change. Tests assert exact float4/3 through the sampled
source camera for the class scene. All three initial source transitions and all
nine source destination-routing combinations still pass. Integrated Rogue1600ms
960x540 capture is assigned to the frontend frame owner for visual review against
the supplied original footage screenshot.

Independent raw original ELF verification passed via
original_elf_camera_literals.py, without using reconstructed engine code:

    ARM583830 MOVW/MOVT r3 bits3fa0d97c -> STR583838 object+154(FOV)
    ARM58383c MOVW/MOVT r3 bits3faaaaab -> STR583848 object+158(aspect)

The helper decodes the original ELF32 load segments, checks the exact ARM
MOVW/MOVT constant pairs and the STR instructions into those object fields.
Verified input SHA256:
36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80.

Live verification attempt: query_camera_proof.py queried read-only endpoint
http://127.0.0.1:8746/mcp database dh2-libdungeonhunter2. It returned Worker for
session is not reachable. A subsequent idb_list returned sessions[]/count0.
The evidence above is the existing original ELF's complete cached IDA source,
not a reconstructed Android output or a claim of live confirmation. The helper
can rerun the same read-only queries after root restores the original session.
