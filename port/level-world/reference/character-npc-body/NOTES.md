# Crypt NPC mesh, owner bounds and physical definitions

The new `character_npc_body.hpp/.cpp` owns copied BRES bytes and a complete
cached model-frame Scene. It composes the existing source-proved marker/skin
mesh-box, Character visual scale, owner bounds and body-definition kernels.
The caller can supply a copied complete Scene with its genuine selected
controllers/cached joints, or explicitly choose the decoded factory scene.
It creates no source AssetManager, equipment/save policy or scene lifecycle.

## Actual source inputs

`source-inputs.json` binds the shipping ELF, canonical cache ZIP, three direct
MGP members and current DACT digest. All eleven MGP objects omit `static`.
ObjectBase constructor0x33f310 stores zero at owner+0x84 at0x33f410.
DeclareProperties0x33f014 passes owner+0x84 and its current value to the
default-bearing bool descriptor0x33e4ac: the literal at0x8c0170 is `static`.
The previously captured default/reset chain supplies that captured false
default; this is static source evidence, not a replay of the complete XML
ObjectManager. Requested `Monster` template loading is the shipping assertion
path documented in character-live-owner-readiness, not an invented template.

| Character row | Index | AI row | AI name | Source Type | Actual model |
| --- | ---: | ---: | --- | ---: | --- |
| CryptSlime | 33 | 40 | Monster | 4 | slime_green_v2.bdae |
| CryptSlime_RE | 34 | 40 | Monster | 4 | slime_green_v2.bdae |
| Crypt_Ghost | 35 | 68 | Ugly_Dog | 4 | ghost.bdae |
| Crypt_Skeleton | 38 | 40 | Monster | 4 | skeleton.bdae |

The native audit decodes actual Character/AI/class files and recalculates
actual class/property sheets. GetCharAIId0x3a2fec then reads **cached resolved
index1**, selecting row8 when out of range. GetCharAI0x3a3024 uses the source
0x44-byte row; GetCharType0x3a3054 reads +0x38. IsPlayer0x3a49f0 returns false
directly for Type4. Its Type0 name-search behavior is outside this bounded
helper: other types fail explicitly. This helper never recalculates the
resolved sheet, matching those source cached reads.

All four actual base rows have Scale_X/Y/Z100/100/100 and Collision_Scale85.
InitPost0x3b4d60 reads **base indices12/13/14** and produces (.9,.9,1) using
the original constants; GameObject's scale clamp applies afterward. DACT
scale is deliberately absent from the new projection API. Passing a
converter-scaled/render scale and multiplying again would be wrong.
SetRelativeAABB0x3a4398 reads **cached resolved index16** only for
already_scaled=false, then invokes source shared padding/absolute/PF
production. VisualObject constructor sets byte+0x28=1 when it finds a marker
at0x472b9c/0x472ba0. ApplyMeshBox0x470a54 loads that byte at0x470a6c and passes
it through owner virtual+0x9c. Consequently skeleton/ghost marker bounds skip
Collision_Scale; the two slime kinds use .85. It does not recompute properties
or use rendered vertices/footZ. The final probe executes actual ApplyMeshBox
through the Character virtual rather than supplying an assumed false flag.

The authored `char_group` string is AI group ownership. It does not supply
the physical filter's groupIndex. Source InitPhysicalObject0x3b4088 Type4,
static=false, IsPlayer=false produces circle/group+2/category0x10/mask0xd3f,
friction1/restitution0/density11.24/sensorfalse. The body's XY is ownerXY*.01,
angle0, fixedRotation/sleep flags true, bulletfalse. Shape mass is evaluated
then POCharacter pin applies zero mass/inertia. Collision-debug override and
disable-physical remain explicit caller predicates; test inputs supply false
fixtures and do not prove their live manager producers.

## Model provider distinction

Skeleton:28 nodes,3 geometry instances,1 controller; first `_colbox_` is
`_colbox_skeleton-node`, one static geometry/no child groups. Ghost:45 nodes,
3 geometry instances,1 controller; `_colbox_ghost-node` has the same bounded
topology under root_camera. Their marker path bypasses skin bounds, exactly
as original CalcMeshBox. The marker SNode's own translation does not enter
its group's local geometry box; its immediate parent's scale does.

Green slime:9 nodes,1 controller geometry instance, no marker. Its actual
authored per-joint boxes and cached factory joint world matrices feed the
source skin provider. Both slime Character kinds share these same bytes.
No extra equipment selection is inferred: the caller's complete Scene is
the explicit selected-provider/pose boundary.

The original algorithm transforms/recenters **two endpoints**, not eight
corners. Room3 CryptSlime_RE has authored Z90; its bounds swap the relevant
axes through the existing Euler/root matrix kernels. Root/cached matrix
inputs are services in the new original composition probe; Euler/node math
already has independent original/ARM64 evidence in decor-scene.

Using current DACT positions and immutable factory pose gives approximately:
skeleton radius.585 physics units (58.5 game units), green slime1.16710
(116.710 game units), ghost.670060 (67.006 game units). The source radius
reads absolute AABB widths; float32 owner translation causes small per-row
differences. These are input-derived observations, not universal radii or a
substitute for future genuine pose/equipment input.

## Evidence and integration

`character-npc-body-source-host-audit.json`: O2 ASan/UBSan on a private copied
six-DSO prior53 snapshot. 228 checks,36 guards,11 actual DACT placements,
6 marker/5 skin projections,11 genuine source-built Box2D bodies created and
destroyed; one world Step on source-pinned static bodies. No collision callback
is fabricated or invoked by that pinned-body test. Input BRES bytes are
overwritten after initialize to check ownership. The three snapshot core
hashes match character-owned-monster-animation-initialization-main-linked-
host-audit.json; that historical incremental-build report is retained in
handoff.json. It does not capture a fresh compilation of every old mesh/math
source. Current kernel hashes are context, not a claim those old DSOs were
rebuilt from current bytes. The new composition source itself is directly
compiled and bound by the isolated audit; actual old-kernel outputs are also
checked against original instructions by the composition probe below.

`character-npc-body-source-original-composition.json`: actual ARM32 instructions
match host outputs for5 skin-provider cases/all4 techniques(20 observations),
11 CalcMeshBox cases,11 Character owner bounds,33 genuine AI-id/type/player
getter observations,11 body configurations/77 ordered creation requests.
Scene collection, cached root/joint matrices, source table storage and the
physics creation/mass/PF services are explicit fixtures. This executes the
original arithmetic/constructors/order; it does not execute a complete
AssetManager factory or claim new-helper ARM64 execution.

The earlier `character-npc-body-host-audit.json` and
`character-npc-body-original-composition.json` are **provisional** false-marker-
flag input fixtures. They passed those narrower supplied-input comparisons
but do not prove the real VisualObject caller contract. Their original bytes,
scripts and source are preserved under
`.local-inputs/character-npc-body/provisional-before-marker-flag`. The final
source reports and `source-fixtures.bin` supersede them; use only those final
reports for integration.

## Factory pose versus template compilation

The additional hash-bound static capture is initialization-order.asm/json.
VisualObject constructor first loads the model via0x50a504 at0x472b18,
SetParent/Sync0x47295c at0x472b30 and RefreshBoundingBox0x35c854 at0x472b38.
It locates modular/marker providers, computes CalcMeshBox0x47211c at0x472c0c,
applies it at0x472c14, then constructs AnimController0x474d30 at0x472c30 and
assigns it at0x472c3c. Initial bbox production precedes that controller.

Character InitPost produces/recalculates property sheets and base visual
scale, then calls GameObject InitPost0x38be5c at0x3b4f88. It later invokes
CharAnimator SetAnimationSet0x3c9f4c at0x3b5090. That registration selects the
unique set, checks AnimSetManager.Exists, adds template then ordinary tables.
_AddTemplateAnimTable0x3c9c7c tailcalls AddTemplateAnim0x476398; that loads the
resource and calls CDynamicAnimationSet.setDefaultAnimationLibrary0x62fc90.
The latter stores the default database at+0x68/+0x6c and marks+0x70 dirty;
it does not apply an animation value to a SceneNode or sample a joint pose.

Late source0x3b53d4 SetInitialPosition→0x3b53e8 SetPosition precedes
0x3b541c ApplyMeshBox, which passes the **stored** visual+0x10 bounds. Then
0x3b542c Revive(NULL,true) reaches InitPhysicalObject0x3b4088. Earlier save/
equipment paths remain explicit: their complete live transform producers
are not reconstructed by this helper.

Current CharacterAnimationInstance::create copies resources.factory() into
its private Scene. Registration/default compile accepts const Scene;
BlendedPlayback::bind_compiled initializes metadata, target buffers and
timeline clip0, with no scene sampling/apply. Thus `animation.scene()`
**immediately after create, before start/scene_phase**, is still the factory
cached Scene and is suitable for this bounded fresh body projection. A
sampled template/Idle/current pose cannot silently replace that input.
Default-library merge inputs and a sampled default pose are different.
The native code observation and original initialization chronology here are
static evidence; no full original InitPost/save/equipment replay is claimed.

Minimal caller flow: initialize a stable model owner from actual model bytes
and complete cached Scene; retain it through any borrowed playback Scene;
finish actual property/class/equipment producers; fill NpcBodyRequest with
that live PropertyView/AI tables, genuine owner/new-physical identities,
source static/debug facts and owner XYZ/Euler; call project; route body config
through NativeWorld.create_character and source ordered attach/PF services.
Do not apply DACT/render scale again. Source selected initial pose/save paths,
floor projection, full scene factory, live collision owner services and full
NPC AI/frame ownership remain their own producers. Reinitialize invalidates
borrowed Scene/marker/entry views; stop borrowers before reinitialization.

No shared CMake, renderer, DACT or APK was edited. Root integrates source and
adds its central-linked test separately. Physical ARM64/gameplay parity and
full NPC AI remain false.
