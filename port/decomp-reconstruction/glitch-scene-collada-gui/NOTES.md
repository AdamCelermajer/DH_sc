# glitch-scene-collada-gui notes

Queue: 4138 functions (all owner engine-glitch). Ledger: 591 rows
(461 reconstructed, 126 partial, 4 skipped). Wave 3 added 157 rows: 84 from the
CColladaFactory, collada database creators and GUI accessors, then 73 from the
scene, timeline, PS and collada-misc batch (two of them GUI partials). Wave 4
added 219 rows (180 reconstructed, 39 partial), all in glitch::scene particle
emitters and affectors (see "Wave 4" below). The `decomp_extract --remaining`
count is 3547 unledgered.
Wave 5 added 228 rows (222 reconstructed, 6 partial; see "Wave 5" below). The current
ledger is 819 rows and the `decomp_extract --remaining` count is 3319 unledgered.

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

- CColladaFactory (collada_factory.hpp): the 30 pure virtuals in vtable order
  (slots +0x08..+0x7c, see decomp_vtable.py). getExternalLightSceneNode has two
  empty base bodies; its error helper (0x62ff80) is not reconstructed.
- CColladaDatabase (wave 3): record at +0x00, factory at +0x04, record+0x20 file
  name, record+0x24 file data, data+0x20 the SDatabaseTables holder. Each table is
  a (count, base) word pair: the base is at the listed offset and the count is one
  word before it. Strides: effects 0x74 (+0x58), geometry 0x10 (+0x6c), controllers
  0x0c (+0x74), forces 0x10 (+0x8c), cameras 0x1c (+0x40), lights 0x18 (+0x48),
  images 0x14 (+0x50), materials 0x24 (+0x60).
  SGeometry notInstantiated at +0x08; SForce kind at +0x08 (0 gravity, 1 wind,
  2 deflector); SInstanceForce url at +0x04.
- CGUIEnvironment (wave 3): factory vector at +0x16c, built-in font list begin
  +0x178 and end +0x17c (the sentinel). SBuiltInFontNode keeps the font pointer at
  +0x18. IGUIElement: type index guiType at +0x154.
- CSceneManager: hierarchyChanged +0x288, visibilityChanged +0x289.
  CCameraSceneNode: input-receiver byte +0x165, bounding box +0x1d4.
  CBillboardSceneNode: size +0x134, bounding box +0x13c.
  CBatchMesh::SSegment: indexBegin +0x10, indexEnd +0x14, visible byte +0x20.
  ISceneNode::automaticCulling at +0x118 (set by setAutomaticCulling).
- ITimeController (base: update and destructor) and ITimelineController (derived:
  currentFrame +0x04, jumpTo, getLoop, setScale, and getScale fixed at 1.0).
  Collada CTimelineController keeps scale at +0x30.
- CSceneNodeAnimatorSynchronizedSet: segment array at +0x60, count +0x6c, reachedEnd
  byte +0x80. SSyncSegment is 0x20 bytes (timestamp +0x00, length +0x04).
  ISkinnedMesh: skinning-enabled byte +0x18. Collada ISceneNodeAnimator: length +0x14.
- Shape types PDPoint/PDLine/PDBox/PDCylinder return tags 3/4/0/2. They are
  stand-ins without the PDomain base, so each empty destructor (D1) is its own body.
- IParticleSystem<P>: world-matrix pointer at +0x0c (both particle types). The model
  initialisers (GNPS* and P* for size, colour, motion, forces, spin, life) are
  empty templates. Each instantiation is its own ledger row.

## Type tags and symbols
- Four-character type tags are stored as the little-endian word from the
  pseudocode and commented with the ASCII form: "emty" 0x79746d65, "cam_" 0x5f6d6163,
  "lght" 0x7468676c, "mesh" 0x6873656d, "smgr" 0x72676d73, "bill" 0x6c6c6962,
  "basn" 0x6e736162.
- Every `// 0xADDR _Z...` comment was checked against queue.csv. Hand-typed mangled
  names had 118 length-prefix errors, which were corrected to the queue name. Keep
  the check: the queue is the source of truth for the name, not the header text.
- Int-index helpers (effectAt, geometryAt, forceAt, lightAt, cameraAt and the
  constructX(int) overloads) index the table without a bounds check, as the original
  getEffect(int) does (param * stride + base).

## Partial status
generated_stubs.hpp holds 80 empty or constant-return bodies from the collada queue.
Each is a free function template named by its original address, not yet attached
to its owning class, so those ledger rows are `partial`.

## Partial rows added in wave 3
- IGUIElement::getTypeName (0x5350a4): returns guiElementTypeNames[guiType]. The name
  table (glitch::gui::GUIElementTypeNames, 92 bytes of .data.rel.ro) is not mapped, so
  the hook stays null and the function returns null.
- CGUIEnvironment::getBuiltInFont (0x535bf8): the sentinel comparison and the font
  pointer at +0x18 are reconstructed. The list links are a pad.

## Decisions
- Refcounts: boost::intrusive_ptr out-parameters are modelled as `T*&` plus a
  count increment, matching the original's assignment behaviour.
- Destructors and deleting destructors: the D1 body is kept where it is empty;
  D0 (delete this) is `skipped` as a compiler-generated thunk. Wave 3 added no D0
  rows (the 4 skipped rows are from earlier waves).
- Empty bodies are reconstructed, not partial. A body that returns a constant or
  reads one field is reconstructed. Only a stubbed or unmapped table is partial.
- Pointer-chasing through offsets is replaced by typed fields; no raw offset
  arithmetic is used.

## Deferred (dependencies not mapped or claimed elsewhere)
- constructController(SController*) 0x60fa24: claimed by another category.
- constructNode, constructGNPSEmitter, constructCoronas, constructEmitter pointer
  overloads: not in this queue.
- CRootSceneNode::attachSkin 0x65b434: needs the list head at +0x160, which conflicts
  with the SIntrusiveList size.
- constructImage and the CImage constructor; constructMaterial(SMaterial*) (584 bytes).
- CResFileManager load/unload; CColladaDatabase constructors and destructor.
- getAnimation with SChannel; the CVirtualEx family (704 bytes); CDynamicAnimationSet
  small functions (need getDatabaseIndex and SChannel).
- IGUIElement::setName 0x539ec8: the string layout conflicts with getName.
- CGUIEnvironment::getDefaultGUIElementFactory needs a pure virtual
  getGUIElementFactory, which would break the existing test construction.
- CGUIEnvironment::getRootGUIElement: embedded object, layout not mapped.
- CTweakable (glitch::debugger): in the queue but out of scope for these classes.
- Deleting destructors (D0, about 306 queued): left unledgered. The category uses
  `skipped` for them, so add those rows only when the target calls for them.

## Remaining
- CColladaDatabase constructors and remaining construct* creators, CRootSceneNode
  construct paths, CSkinnedMesh/CModularSkinnedMesh virtual paths,
  CSkinnedMeshSceneNode and CModularSkinnedMeshSceneNode destructors and
  getAbsoluteTransformation, CSceneManager node-entry operator< (STransparentNodeEntry,
  SDefaultNodeEntry), SViewFrustum::intersects, CSceneNodeAnimatorSynchronizedBlender
  (segment vtable slots), animation_track::CVisibilityEx/CVector3dEx/CTextureTransformEx
  blend and key-based paths (need SAnimationAccessor and CApplicatorInfo),
  CParticleSystemSceneNode.
- glitch::ps: PDomain, PDCone and PDBlob bases, the copy, transform, within and
  generate members of the shapes, IParticleSystem beyond getWorldMatrixPtr.
- Attach generated_stubs entries to their owning classes, which also makes those
  rows reconstructed.
- Intrusive_ptr add_ref/release (0x60bbd4, 0x60c598) and onUnbindAll need the
  CAnimationBlock layout.

## Wave 4: particle emitters, affectors, mesh and animated-mesh emitters
Files: include/glitch-scene-collada-gui/particle_emitters.hpp, particle_affectors.hpp,
src/particle_emitters.cpp, src/particle_affectors.cpp, tests/particle_emitters_test.cpp,
tests/particle_affectors_test.cpp. Classes: CParticleCylinderEmitter, CParticlePointEmitter,
CParticleBoxEmitter, CParticleRingEmitter, CParticleSphereEmitter, CParticleMeshEmitter,
CParticleAnimatedMeshSceneNodeEmitter, CParticleAttractionAffector, CParticleGravityAffector,
CParticleRotationAffector, CParticleSizeAffector, CParticleSpinAffector, CParticleFadeOutAffector.

- Emitter layout: vptr at +0x00. Cylinder, Point, Box, Ring and Sphere keep a particle
  buffer (begin, end, capacity) at +0x04. Mesh and AnimatedMesh keep an IMesh or
  IAnimatedMeshSceneNode reference at +0x04 instead. The accumulated time is +0x58 (Cylinder,
  Mesh), +0x70 (Point), +0x4c (Box), +0x48 (Ring) and +0x44 (Sphere), with the emitted
  count in the next word. Settings are at the offsets in the header comments.
- Mesh emitter: the buffer loop in setMesh and the constructor calls IMesh vtable slot
  +0x10 (buffer count) and slot +0x14 (buffer getter). It reads the buffer's vertex stream
  at +0x14 and that stream's count at +0x08, sums the counts into +0x08, and appends each
  count to the int vector at +0x14. The loop is not reconstructed because IMesh and
  CVertexStreams are not mapped. The reference counts are on the pointee at +0x04.
- AnimatedMesh emitter: +0x18 mesh buffer number, +0x28 everyMeshVertex, +0x29
  useNormalDirection, +0x2c normalDirectionModifier, +0x3c direction, +0x48/+0x4c
  particles per second, +0x50/+0x54 colours, +0x58/+0x5c lifetimes, +0x68 max angle. The
  constructor code after the colour copies, the D1 (112 bytes) and setAnimatedMeshSceneNode
  (532 bytes) are not done.
- Affector layout: the IParticleAffector flag at +0x04 is set to 1 by every constructor and
  tested by affect(); it is named `enabled`. Attraction: point +0x08, speed +0x14, flags
  +0x18 (affectX), +0x19 (affectY), +0x1a (affectZ), +0x1b (attract), lastTime +0x1c.
  Gravity: timeForceLost +0x08 (float), gravity +0x0c. Rotation: pivot +0x08, speed +0x14,
  lastTime +0x20. Size: targetSize +0x08, variation +0x0c, growForTime +0x10 and fadeForTime
  +0x14 (both floats, converted from uint in the constructor). Spin: spinTime +0x08 (uint),
  variation +0x0c. FadeOut: targetColor at +0x05 (unaligned in the original), fadeOutTime
  +0x0c.
- Constructors: C2 and C1 store the same members. C1 also writes the virtual-base vtable
  slots, for example +0x78/+0x7c in Point and +0x50/+0x54 in Sphere, and the VTT in r1 is
  not a C++ parameter. The constructors are `partial` for that reason.
- Ghidra marks the memcpy and float-helper tails as noreturn, which cuts the pseudocode of
  constructors, emitt and affect short. Example: CParticleCylinderEmitter::emitt is 124
  bytes in queue.csv but 1340 bytes of ARM. For these bodies read the ARM listing
  (`recovered/native/assembly/libDungeonHunter2.so/`): the header line gives declared_size.
  Helper addresses: 0x30e868 memcpy, 0x30e2e0 __aeabi_ui2f, 0x30ec94 __aeabi_fdiv,
  0x8be2a0 __fixunssfsi (libgcc semantics: negative input gives 0, overflow gives UINT_MAX).
- emitt() and affect(): the time accumulation (emitters) and the enabled/count guard
  (affectors) are reconstructed. The particle generation, array growth and per-particle
  updates are not, so every such row is `partial`. The mesh and animated-mesh emitt() return 0
  in the original and are `reconstructed`.
- Virtual methods: the emitter and affector interface bases are not reconstructed, so the
  methods are plain members and the classes have no vtables here.
- Destructors: D1 with owned storage calls std::free for GlitchFree (core-platform owns
  GlitchFree and categories do not link to each other). Point's D1 is empty in the original
  and stays empty. D0 (deleting) rows are not added, following the wave-3 decision; the four
  skipped rows are from earlier waves.
- Not taken this wave: the CVirtualEx<CApplyValueEx<...>> family (704 queued, 575 of them 120
  bytes or less). Its small bodies forward through virtual slots (+0x98 to +0xa4) on
  CApplicatorInfo or SAnimationAccessor pointers whose dynamic class is not identified. The
  CApplicatorInfo vtable in vtables-001.json has only its destructors, so the slots cannot be
  named. Map the derived applicator vtables before taking this family.

## Remaining after wave 4
- The CVirtualEx family above, and the other animation_track templates (CWeightEx 17,
  CTextureTransformEx 16, CVector3dEx 14, CFloatEx 13, CVisibilityEx 21).
- glitch::scene particle systems: CParticleSystemSceneNode (36 small bodies), PSManager,
  IParticleContext, PRenderDataBillboardModel and the PD shape bases (PDomain, PDCone, PDBlob).
- Mesh emitter setMesh and constructor loop; AnimatedMesh emitter D1, setAnimatedMeshSceneNode
  and constructor tail; the emitt() and affect() particle paths for all emitters and affectors.
- Large unfinished classes with many small bodies: CSceneManager, CColladaDatabase,
  CGUIEnvironment, CGUITable, ISceneNode, CTerrainSceneNode, CGUITTFont, IGUIElement and
  CSceneNodeAnimatorSynchronizedBlender.

## Wave 5: collada, scene, gui and PD accessors
Files: include/glitch-scene-collada-gui/collada_scene_accessors.hpp, collada_empty_dtors.hpp,
scene_simple_accessors.hpp, gui_simple_accessors.hpp, ps_types.hpp (extended), and the
matching src/*.cpp (collada_scene_accessors.cpp, scene_simple_accessors.cpp,
gui_simple_accessors.cpp, ps_types.cpp). Tests: tests/collada_scene_accessors_test.cpp,
scene_simple_accessors_test.cpp, gui_simple_accessors_test.cpp, ps_types_test.cpp (extended).
Changed: modular_skinned_mesh.hpp/.cpp, collada_misc.hpp/.cpp, visibility_ex.hpp,
texture_transform_ex.hpp, collada_factory.hpp.
Ledger: 228 rows (222 reconstructed, 6 partial). Queue remaining 3319 of 4138.

- Layouts (offsets are comments; members are typed):
  - SCategoryEntry {reserved[2], moduleCount +0x08, reserved2}; SCategoryTable.entries at +0x04.
    getCategoryModuleCount indexes entries[category].moduleCount.
  - STimelineClip and STimelineClipTable; CTimelineController clipTable +0x34, currentClipIndex
    +0x38. getClipCount returns 0 when there is no table.
  - CDynamicAnimationSet channels at +0x74, stride 16 (getChannel).
  - CCoronasSceneNode box at +0x150; CMesh box at +0x24; CSceneNodeAnimatorSet animationSet at
    +0x24; CSceneNode::getUserProperty is in collada_scene_accessors.cpp.
  - CModularSkinnedMeshSceneNode mesh pointer at +0x134.
  - CMeshSceneNode: holder at +0x130, else the named mesh at +0x134 with its uid at +8.
  - CTextureTransformEx, CVisibilityEx and CAnimatedMeshSceneNode members are named per the
    header comments; the glitch::scene nodes keep their stand-in layouts from wave 4.
- Thunk forwarding: the CSceneNodeAnimatorSet getAnimationLength/Start/End bodies take one
  index that the ARM passes through r1 untouched. The index is forwarded to the collada helper
  (checked against the listing for each). The CModularSkinnedMeshSceneNode getters follow the
  same rule.
- Partial rows (6): the uid words of CParticleSystemSceneNode, CGlitchNewParticleSystemSceneNode,
  CCameraSceneNode and CMeshSceneNode are typed as const char*, and the transformed-box word
  at +0x14c of the two particle systems is typed as a pointer. The string and box types are
  inferred from use and each body reads one stored word; they stay partial until the holder
  types are mapped.
- CTerrainSceneNode getScale, getRotation and getPosition are const and return const
  references (the queue names them _ZNK). CMorphingMesh::clone returns a null intrusive_ptr.
- CFloatEx and CWeightEx getIdentityValue write 0.0f; their defaulted dtors are in
  collada_scene_accessors.hpp. CVisibilityEx and CTextureTransformEx dtors are defaulted.
- Destructors: D1 empties use `= default` (no vtables in the stand-ins, same as wave 4). D0
  deleting rows are not ledgered. CColladaFactory D1 and D2 are the defaulted virtual dtor at
  +0x00/+0x04 of collada_factory.hpp.
- Empty-body families ledgered as `reconstructed`: IEventsManager, CEventsManager,
  IAnimationTrackEx, IColladaSceneNodeSynchronizedAnimatorInterface, CResFileReader, CResFactory
  (D2 and D1), CAnimationTrackEx, animation_track::CApplicatorInfo, CMaterialApplicatorInfo.
- glitch::scene simple accessors: type tags are the four-character words (amsh, text, dmmy,
  sky_, terr, shdw, ptcl). Particle interfaces (IParticle*) return the getType constants.
  CAnimatedMeshSceneNode::setAnimationSpeed scales by 0.001f. CDefaultSceneNodeAnimatorFactory
  reports 9 creatable types and maps indices above 8 to 10.
- glitch::gui simple accessors: CGUITable flags are bool at +0x188/+0x189 and int at
  +0x198..+0x1b0. CGUITTFont kerning setters are empty and the getters return 0. The
  IGUIEnvironment D1 row is attributed to the header (its .cpp holds only a comment).
  CDefaultGUIElementFactory reports 0x16 creatable types.
- PD shapes: getType constants are PDCone 5, PDSphere 1, PDBlob 6 (PDPoint 3, PDLine 4, PDBox 0,
  PDCylinder 2 from wave 4). size() returns a float: the loaded word is returned as-is, and
  the float type is inferred from the 1.0f constant that PDPoint and PDBlob return. The
  transformed-box and uid word types are inferred, as noted in the partial list above.

- Not taken this wave, with the reason:
  - The CVirtualEx family and the key-based forwarders to Ex functions, as in wave 4.
  - Virtual-slot getters: CAnimationSet::getAnimationTrack (slot +0x18),
    CTimelineController::getCurrentClipLength (slot +0x28), and the mesh getters that dispatch
    through IMesh and IAnimatedMesh (slots +0x10, +0x24, +0x30).
  - CMatrix4 layout needed: CDummyTransformationSceneNode matrix accessors, and the PDPoint
    (0x69ae04), PDSphere (0x69bdec) and PDBlob (0x69bf60) transform bodies, which read matrix
    words. PDCone::transform (0x69b9c0) is empty in the original and is ledgered with only a
    forward declaration of glitch::core::CMatrix4 in ps_types.hpp.
  - CAnimatedMeshSceneNode::setCurrentFrame (0x6f6af8): the decompiled tail is truncated.
    Read its ARM listing before taking it.
  - CShadowVolumeSceneNode::load and CColladaBinaryFileLoader::createScene: return type not
    known from the index.
  - PD within(), generate() and copy() bodies: they use PSRandom and the vector stand-ins, so
    they are a separate batch. PDomain's D1 is skipped because PDomain is not a stand-in here.

## Remaining after wave 5
- The CVirtualEx family and the remaining animation_track templates (CWeightEx, CTextureTransformEx,
  CVector3dEx, CFloatEx, CVisibilityEx) beyond the accessors above.
- Virtual-slot getters listed above, and the CMatrix4 layout the matrix bodies need.
- Large classes with many small bodies still unfinished: CSceneManager, CColladaDatabase,
  CGUIEnvironment, ISceneNode, IGUIElement and CSceneNodeAnimatorSynchronizedBlender.
- PD within(), generate() and copy() bodies, and the PSRandom dependency.
