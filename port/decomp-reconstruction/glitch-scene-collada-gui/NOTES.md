# glitch-scene-collada-gui notes

Queue: 4138 functions (all owner engine-glitch). Ledger: 215 rows
(129 reconstructed, 82 partial, 4 skipped). Only collada functions are done so far.

## Classes and inferred layouts
- CVisibilityEx: visible flag is a 32-bit word at +0x11c, bit 0 = visible. Base
  track state before +0x11c is not mapped (stand-in pad in visibility_ex.hpp).
- CAnimationSet (animation_set.hpp): channels +0x0c, tracks +0x18, database refs
  (8-byte) +0x24, bindings (12-byte) +0x30, animated track count +0x3c, start
  range +0x40/+0x44, end range +0x4c/+0x50, length range +0x58/+0x5c, animation
  template +0x64. Ranges are read with a bounds check (index < count else 0).
  Destructor and remAnimationTemplate are not done (they need the full base class).
- CVector3dEx (vector3d_ex.hpp): value is 3 floats (12 bytes). Getter helpers
  only; blended/added/key-based paths need SAnimationAccessor and CApplicatorInfo.
- CRootSceneNode (root_scene_node.hpp): UID holder at +0x14c (uid at +0x20).
  Circular intrusive lists with sentinel heads at +0x158 (particle systems), +0x168
  (morphing meshes), +0x170 (cameras), +0x178 (materials), +0x188 (lights); nodes
  are {next, prev, value}. removeMaterial has an empty body in the original.
- CSkinnedMesh (skinned_mesh.hpp): per-buffer table of 0x14-byte entries at +0x5c
  holding mesh buffer (count at +0x04), material and vertex-attribute map (count at
  +0x00). Accessors write an owning reference and increment the count.
- CSceneNodeAnimator (scene_node_animator.hpp): target blocks at +0x44 with a
  16-byte stride. Track and trackEx are 32-bit ids. getBindURI and the update paths
  need the ISceneNodeAnimator base, not yet mapped.
- CModularSkinnedMesh (modular_skinned_mesh.hpp): SModularBuffer is 0x20 bytes
  (buffer +0x00, material +0x04, map +0x08, flag +0x1c set to 1 by the ctor);
  buffers vector at +0x30..+0x34, category table pointer at +0x1c (count is its
  first word), current module ids are 8-byte records at +0x24, needOutputBuffer
  flag at +0x58.
- CSceneNodeAnimatorSynchronizedBlender: only the ordering predicate and the
  elapsed-time stub are done. The rest forwards through virtual calls on segment
  objects whose vtable slots are not mapped yet.
- CTextureTransformEx (texture_transform_ex.hpp): value is 5 words (20 bytes).
  Identity is translation (0,0), rotation 0, scale (1,1).
- IAnimationTrackEx, CVirtualEx, ISceneNodeAnimator, CColladaFactory and
  CSceneNodeAnimatorSynchronizedSet: empty or constant-return defaults, in
  track_defaults.hpp and generated_stubs.hpp.

- CColladaDatabase (collada_database.hpp): the named-asset lookups reach the
  table holder through two pointers (*this + 0x24, then +0x20). The holder is
  modelled as SDatabaseTables with one std::vector of entry pointers per kind.
  Original count/base offsets are in the header comments (animation +0x24/+0x28,
  animationClip +0x34/+0x38, image +0x4c/+0x50, effect +0x54/+0x58, material
  +0x5c/+0x60, geometry +0x68/+0x6c, controller +0x70/+0x74 (name at entry+4),
  light +0x44/+0x48, camera +0x3c/+0x40, visualScene +0x98/+0x9c, force +0x88/+0x8c,
  coronas +0x90/+0x94, gnpsEmitter +0x80/+0x84, emitter +0x78/+0x7c). Entry types
  (SImage, SEffect, ...) are name-only stand-ins until the loader structs are mapped.
  Segment table (holder +0x30) is ranges [begin, end) with a 0x18 stride.
- CColladaDatabase constructors, construct* creators, getAnimation(int)/SChannel
  overloads and getDefaultValue/getBlendableAnimation are not done: they forward
  through SChannel and the ISceneNode/collada vtables.

## Partial status
generated_stubs.hpp holds 80 empty or constant-return bodies from the collada queue.
Each is a free function template named by its original address, not yet attached
to its owning class, so those ledger rows are `partial`.

## Decisions
- Refcounts: boost::intrusive_ptr out-parameters are modelled as `T*&` plus a
  count increment, matching the original's assignment behaviour.
- Destructors and deleting destructors: the D1 body is kept where it is empty;
  D0 (delete this) is `skipped` as a compiler-generated thunk.
- Pointer-chasing through offsets is replaced by typed fields; no raw offset
  arithmetic is used.

## Remaining
- CColladaDatabase constructors and construct* creators (60 left), CRootSceneNode construct paths, CColladaFactory creators,
  CSkinnedMesh/CModularSkinnedMesh virtual paths, CSceneNodeAnimatorSynchronizedSet,
  animation_track::CVisibilityEx/CVector3dEx/CTextureTransformEx blend and key-based
  paths (need SAnimationAccessor and CApplicatorInfo), CParticleSystemSceneNode,
  glitch::scene, glitch::gui and glitch::ps classes (not started).
- Attach generated_stubs entries to their owning classes, which also makes those
  rows reconstructed.
- Intrusive_ptr add_ref/release (0x60bbd4, 0x60c598) and onUnbindAll need the
  CAnimationBlock layout.
