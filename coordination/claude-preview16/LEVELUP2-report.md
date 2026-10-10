# LEVELUP2 report (level-up light column: vertical billboard mechanism, timing, HUD text)

Branch `p16/levelup` (worktree `DH_wt/p16levelup`, started from `p16/integrate` 261f2e49), build
`DH_wt/build-p16levelup` (`p14_build.ps1 -Name p16levelup -Jobs 6`). Scratch, captures and tools:
`.local-inputs/claude-preview16/levelup2/` (tools/bres_dump.py, bres_anim_keys.py, bres_effects.py, ida_fn.sh, ref30/).
Predecessors: LEVELUPFX (`p15/levelupfx`, investigation only), LEVELUP (`p15/levelup`, presentation wiring).

Status: IN PROGRESS (skeleton). Sections below are filled as the work lands.

## 1. Investigation (evidence)

### 1a. Decoded level_up.bdae (iPad copy, byte-identical to rc4; `tools/bres_dump.py`, `bres_anim_keys.py`)
- Scene `level_up.max`: 7 nodes. `_bone_halolevelup` (identity) children `_mesh_glow_floor_nobatch` (z +27.29) and
  `_mesh_levelup_nobatch` (identity) -> child `_mesh_levelup_nobatch_PIVOT` (identity, SNode+76 != 0, mesh instance
  `_mesh_levelup_nobatch-mesh`). Emitter nodes `glow` (z -12.62), `stars` (x +39.39, animated), `streaks`. Force
  nodes `IrrGravity_Down` (quat 180 about X) and `IrrGravity_Up` (quat 180 about Z, up = +Z = game up).
- Sheet geometry `_mesh_levelup_nobatch-mesh`: 18 verts, x [-100,100], y [-63,1076.8], z [76.9,84.9]. A flat
  200 x 1140 sheet in the local XY plane, thin in Z.
- The only non-zero SNode+76 in the file is on `_mesh_levelup_nobatch_PIVOT` (0x6db8).

### 1b. The vertical column is a glitch collada BILLBOARD node (decided)
- `CColladaDatabase::constructNode` 0x61b2f4: `if (a3[19])` (SNode+76, word index 19) selects factory vtable slot 64
  (`createBillboard`, 0x631830) instead of slot 60 (`createNode`, 0x631800). Factory vtable
  `_ZTVN6glitch7collada15CColladaFactoryE` (vtables-and-rtti.json) decoded: slot 15 = 0x631800, slot 16 = 0x631830.
- The billboard class is `glitch::collada::CBillboardSceneNode` (vtable `_ZTVN6glitch7collada19CBillboardSceneNodeE`,
  `updateAbsolutePosition` 0x60cf88, slot 53). Its SNode+76 block is the billboard mode record: word0 mode = 1,
  word1 sub = 1, words 2-4 axis A = (0,0,1) (game up), words 5-7 axis B = (0,1,0).
- The billboard's target is the active camera: `this+272` (word 68) is the scene manager (ISceneNode ctor zeroes it,
  it is set on registration), and `CSceneManager+228` is the active camera (`CSceneManager::setActiveCamera`
  0x5890c0). Without a manager the billboard is an ordinary node.
- `updateAbsolutePosition` (target branch, mode != 2, sub != 2): D = normalize(cameraPos - parentPos);
  S = normalize(D x C) with C = (m1,m5,m9) of the camera absolute matrix; U = normalize(S x D); the billboard basis is
  F = [-S, U, D] (columns), then absolute = T(parentPos) * F * Frame^-1 * Rot(parent) * Rel, where Frame = [P,Q,A]
  is the axis frame from A and B (identity for an identity parent). With an identity parent the billboard's local
  Y = U is the camera-facing "up", so the 1140-unit sheet becomes a screen-vertical column.
- The port's scene loader (`port/scene-materials/scene.cpp`) reads SNode fields 0..72 only and ignores +76, so the
  billboard is lost and the sheet stays horizontal (the diagonal gold beam in our captures).

### 1c. Animations, emitters and timing (key unit: 1/30 s; `tools/bres_anim_keys.py`)
- Segment 0 [0, 2000] ms; keys in frames (30 fps), so the last key 60 = 2000 ms.
- Sheet material `_7_-_Default1` diffuse colour (track type 86): f0-2 black, f3 (85,50,12), f4-5 (172,122,24),
  f18 (172,122,24), f20 (137,93,19), f22 (102,64,14), f24 (68,36,10), f26 (33,7,5), f28+ black.
- Glow floor `gloow` colour: f2 (111,91,16), f4-f14 ~ (222,180,26) fading, 0 at f33.
- Birth rates (30 fps): `glow-emitter` 30/s to f20; `stars-emitter` 90/s to f30; `streaks-emitter` 30/s to f15.
  Emitter max particles: glow 5, stars 100, streaks 5.
- `stars-node` translation (track 1): x 39 -> 157, z 0 -> 395 over 31 frames (rising stars).

### 1d. Reference video (30 fps, `ref30/`, `part1_z_Zky7qQdYs.mp4`, 640x360)
- Contact sheet `ref30/sheet_a.jpg` (frames 768.5 + n/30, cropped on the hero):
  n16-18 (769.03-769.10): small white-blue flash at body height; n19 (769.13): tall white column appears with a
  bloom at the feet; n19-n29 (769.13-769.47): column present; n30-31 (769.50-769.57): column gone, only the green
  halo remains. Direct observation.
- The column is WHITE with a blue tint and reaches above the top of the frame. Our EXE sheet is gold (see 1c).
