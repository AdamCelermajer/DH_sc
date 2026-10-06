V3 is an opt-in blood draw successor. V1/V2 owners and frozen V1 units are
unchanged. Add `scene-materials/blood_render_pass_v3.cpp` once. Include its
header, `shader_sources.hpp`, then `renderer_blood_draw_v3.inc` at namespace
scope after GLES2 and standard array/vector/string/stdexcept headers.

Retain one `BloodDrawProgramV3` on the same GL context. Decode the actual blood
BRES material `fx_particles_alpha`, technique `default`, through
`scene::blood_render_pass_v3`; verify the actual technique name against the
asset before binding. The decoder rejects missing/external materials,
multipass and nondefault shader variants. Both original blood resources use
the same recovered pass. Compile vertex/fragment plans using the existing
`scene::shader_source_plan` from actual driver flags, actual additional config,
pass filenames/defines and exact bundled shader bytes. Source type4 is vertex;
the fragment plan uses the actual fragment source type. Do not substitute the
generic renderer shader, which has an extra alpha discard. The source default
TEXTURED profile has no ALPHATEST, FOG, LIGHTING or separate transparent sampler.

Call `program.initialize(pass,vertexPlan,fragmentPlan)` once. For each retained
V2 `Draw`, construct a `BloodDrawBorrowV3` from its existing buffer IDs, actual
diffuse atlas texture, ushort triangle count, sizeof(Vertex), offsetof(Vertex,p),
offsetof(Vertex,color), offsetof(Vertex,uv), actual combined submitted projection
times batch placement, actual retained material diffuse color and the actual
texture owner's LOD bias. V2 blood positions are already in world space and its
placement is identity. The V2 baker consumed the same submitted_view/eye before
draw. Do not transform them by an additional actor placement.

`source_texture_ready` is a receipt from the existing texture owner after its
source texture descriptor/parameter/LOD setup, not a constant. The helper does
not upload a duplicate atlas or choose filtering. Original ITexture ctor sets
min filter index1 for no-mips or3 for mips, mag index1 and wrap index0. Original
GL updateParameters5afd40 maps these to LINEAR / LINEAR_MIPMAP_NEAREST, LINEAR,
REPEAT. Actual mip choice is produced by image+28, TextureManager+74 flags and
driver+88 flags in createTextureFromImage5ec460; a loaded texture owner's actual
descriptor is required. Source TextureManager ctor initializes flags0x43 and
ITexture ctor LOD bias0, but later settings must remain owned and observable.
No claim that the generic native upload's fixed LINEAR/no-mips is this owner.

Pass batches in the actual SceneManager transparent queue order and set
`source_queue_order_ready` only after that producer. Individual particle clouds
already preserve source far-to-near sort including ties. Cross-system ordering
is a separate source queue: registerNodeForRendering58f348 selects its transparent
vector from blend bit16; entry354e8c computes squared distance plus node virtuald0,
and comparator3538ac orders signed priority descending then distance descending,
then material/node sub-order. A map by FX ID is not a replacement. This handoff
does not claim reconstruction of the global transparent queue or its material
tie comparator. A sole submitted cloud does not require cross-system sorting.

Call `program.draw(pass,orderedBatches,actualRenderTargetFlip,sourceQueueReceipt)`.
The source pass uses SRC_ALPHA / ONE_MINUS_SRC_ALPHA, FUNC_ADD, LEQUAL, depth
writes false, BACK culling and CCW front face. Driver+4a0 render-target flip
inverts front face and is borrowed explicitly. Scissor rectangle/enable,
color mask and viewport are driver-global and untouched. The helper preserves
and restores program, active texture/unit0 binding, array/index buffers,
attributes0..2, blend functions/equations, depth/cull and auxiliary enables.
Release the program on its live GL context before teardown; lost-context paths
must discard the owner without issuing deletes into a replacement context.

Validation: complete source rich76-to-pass32 conversion5d7a10 exact original
ARM versus ARM64 PASS2,002 cases (both actual asset records plus random all-bit
inputs), O1/O2 ASan/UBSan PASS2,023 checks, full decoder and include both ABI
strict syntax PASS. GL compilation/draw/pixels require live integration; no
pixel equivalence or unknown texture/queue producer acceptance is claimed.

Source combat application handoff:

* Apply uses the existing same World actors, canonical PropertyView/Gear,
  shared combat RNG/context and actual Hit/trophy/aggro owners. No copied HP.
* Source party-count request has subject0 (global). Renderer1876 must route it
  to the actual global player count before EquipmentPlatform's per-player guard.
  The observed skill failure currently stops here, before aggro/HP.
* Service7 routes the real result into CharacterCombatFxRuntimeV2. Its genuine
  invalid-ID no-op and actual positive CPU particle owner are implemented;
  positive presentation additionally requires the texture/program/queue owners.
* Text17 belongs to root's actual TextManager queue. CancelSneaking16 and
  injury11 belong to retained NPC/player reaction owners, not this draw helper.
* Critical camera8; dodge9/block10; push12/stun13/scare14/slow15 each require
  their reached source controller/FSM backend. Do not convert them to success
  merely because another reaction branch completed.
* Sound18 uses new character_combat_sound_v1.hpp/cpp below. Actual audio delivery
  remains mandatory when a nonempty source sound list is selected.
* AI-combat19 executes twice (attacker then target) unless mask20000000. Actual
  current AIS callbacks/lifecycle are required. Player lookup20 must resolve
  the same selected actor; the following PlayerStat functions are proven bx-lr.
* Online, gold conversion, cooperative scaling and unsupported player reactions
  remain explicit source branches in V6, never blanket full-Apply acceptance.

Combat sound: add `character_combat_sound_v1.cpp` once. Bind services borrowing
actual CharSounds rows, the same Debug owner, source IsDead, same Vox manager,
same gameplay Random, target GetPosition and actual Vox Play3D delivery.
Call character_combat_sound_v1(out,result,attacker,target,true,services) for
Character melee/skills. GameObject overload uses false. CharSounds is source
cached Character+1018 (resolved index9); GetCharSoundsId3a3294 validates against
actual table count and selects row2 on negative/out-of-range, then getter3a32d0
uses stride40. Row word4/8 death count/list,12/16 hit,20/24 impact1,28/32 impact2,
bytes36/37 target impact flags. Adapt decoded actual tables, never ARM pointers.

Whole selection always queries both rows (character overload), then actual
Debug `MP_MinimalRandoms`. Nonzero is a source-complete no-sound branch. Otherwise
death selects target death sounds; live positive damage selects target hit.
Positive damage then selects attacker impact1 if target byte36, otherwise
impact2 if byte37. Each nonempty list captures Vox manager, consumes actual
Random::GetRandom(count,false), reads target GetPosition and delivers Play3D
with source bool=false, integer=1 and both source floats=-1. Their downstream
interpretation belongs to the actual Vox delivery owner. Empty lists consume no random.
Missing play returns required failure after its real earlier prefix.
The new selector matches whole original both overloads in 1,536 ARM/ARM64
branch/request cases; service observers explicitly do not prove audio playback.
