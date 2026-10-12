# Complete ColBox production source batch

SOURCE ONLY / UNCOMPILED / UNRUN. No test, compiler, syntax, configure, native
job, emulator, fixture expansion, proof, hash or checkpoint execution. Original
ELF reading was static symbol/instruction/data inspection, not execution.
Root and adoption-ready V67/V70 remain unchanged.

New actual class: canonical_colbox_v71.hpp/.cpp. This supersedes the two starting
C1-only files retained in this candidate; do not attach those historical shells.
ColBox is a distinct final native C++ class with SAME actual GameObject base,
borrowed actor runtime, dimensions374 and native physical lifetime. It has actual
property writers, source InitPost/InitFinal/SetPosition, IsUpdatable=false and
ordered D1/D0 control. No Floor/Decor/Zone or Trigger aliases are introduced.

New typed production part: catalog_colbox_v71.hpp/.cpp. Call
make_colbox_catalog_part_v71(scope, real_services, part, error, &typed_owner),
append this exact ColBox340fe0 PartV67 BEFORE the existing single common catalog
create, and return that same combined callback to the Character bridge. It
advertises only ColBox. The services callback prepares engine bags for an empty
weak receiver slot; only loader code constructs the actual ColBox C1 and fills
that slot. The release journal is admitted before C1 and retains the actual
runtime/source record, including failed constructor prefixes. Factory authority
is independent, containing World/Level/manager are weak and construction pins
are transient. Main service closures/Arrays/physical native leases must not
strongly capture those containing owners indirectly.

Native ownership and source control:

- Factory340fe0..341040: GameObjectC1(20), dimensions374/378/37c=0, static84=1.
  Source object factory34b5d0 writes the class-name field BEFORE manager Add;
  this C1 does not invent an earlier class-name producer.
- DeclareProperties389938..389978 calls GameObject38cee8 then the SAME map's
  vector AddProperty389894 for literal "dimensions", offset374, default
  0x43480000 on XYZ (200.f). The coherent property-map successor adds exactly
  this whitelist/declaration branch, preserving V66 save descriptors and V67
  LightPoint descriptors. No new map or descriptor registry is constructed.
- InitPost3884fc calls GameObject38be5c and continues even if its void base path
  returned for spawn/performance. It scales dimensions, computes signed half
  bounds (original sign-bit operation preserved), calls exact absolute bounds
  update38aac8 without flat-box/padding repair, constructs actual POColmap on
  application PhysicalWorld44, then reached SetPhysicalObject394bf8(false).
- Exact physical constructor args from registers/stack: bbbb=1,0,0,0; s=0;
  tt=1,ffff; i=1. Native subclass vtable is POColmap964000. Engine provider
  constructs this genuine physical resource for THIS ColBox base/runtime;
  it never constructs a whole ColBox receiver or returns another class alias.
- Reached SetPhysicalObject queries real "MP_NoPhysics". True deletes the newly
  produced POColmap without changing current physical2dc; false deletes a
  distinct old physical owner, assigns the same new native receiver, and calls
  actual UpdatePFObject393ea0. ColBox passes false, so Start46eb20 is not reached.
- ColBoxD1=3884b8/D0=388e60 enter GameObjectD2=38d378. Owned-native release order
  is visual2d8, physical2dc, attached2e0, LuaScript300; then VoxSoundManager.Stop
  with actual signed-short370 and stoptype0; CString358; TargetList304 D1;
  CString2c0/2a8/290/278; SAME PFObject1c8 D1; actual ObjectBaseD2. D0 storage
  retirement follows successful source destruction, manager/Room/transport
  unpublication and native/draw alias exhaustion through the actual journal.
- A failed physical constructor/installation retains the produced actual token
  and an independent owner pin; its genuine uninstalled resource release is a
  separate journal prefix before inherited D2. No passive shared_ptr drop is D0.
  Destruction failure stays sticky; it cannot replay the original Dtor prefix.
  Completed D0 is cached. Main's genuine continuation/journal drains any failed
  native prefix before release_completed allows unique C1 retirement.

Exact original evidence read statically from .local-inputs/libDungeonHunter2.so:
ColBox symbols/vtable964228; factory340fe0; DeclareProperties389938 size72;
InitPost3884fc size300; D1=3884b8 size64; D0=388e60 size72; IsUpdatable3883f4
size8. Declare's PC-relative name is8c22e8="dimensions". Reached methods are
GameObject SetPhysicalObject394bf8 size316, GameObjectD2=38d378 size300,
PhysicalObjectC2=46f2f0 size904, TargetListD1=38d18c, PFObjectD1=524f50,
ObjectBaseD2=33e998, VoxSoundManager.Stop=369fec. These are source boundaries,
not a native agreement receipt. Retained text C1 is also Root read-only
port/level-world/reference/canonical-object-factory-v1/original-source.asm:1414.

Production integration is one owner operation:

1. Replace the actual owner's canonical_property_map_v1.cpp with this coherent
   successor (baseline V67 catalog's V66-preserving map CPP). Rebuild it once at
   the milestone. Never append a second map TU or discard its existing header.
2. Include this production-targets.cmake and call
   dh2_loader_attach_colbox_v71(existing_owner CATALOG_DIR ...
   COMMON_CATALOG_DIR ... LOADER_DIR ... WORLD_DIR ...).
   It attaches only two new class/part TUs once. Existing canonical base,
   GameObject initialization/position, actor runtime and common catalog owners
   stay the same. Starting C1-only files are not production inputs.
3. Main binds actual native engine leaves: current scope validation, existing
   initialization/Arrays and SetPosition graph, genuine application PhysicalWorld
   and POColmap constructor, Debug MP_NoPhysics, native D0 by pointer/source
   field for visual/physical/attached only, dedicated LuaScript300 D0, UpdatePFObject, VoxSoundManager.Stop, TargetList/PFObject/ObjectBase
   source destructors. Every leaf acts on THIS actual base/runtime/native facet;
   missing reached providers fail. No populated callback alone proves readiness.
4. Main's pre-C1 admission receives ColBoxRecordV71. After C1, its receiver is
   the actual class for InitFinal/native lifecycle dispatch. typed_owner.find(id)
   is an alias retaining SAME runtime/C1, not a copied receiver. Call real
   receiver->destroy at the authentic release point; only after complete native
   journal and unpublication call typed_owner.retire_after_source_release(id).

There is no new Draw reconstruction claim. Original ColBox Draw3896f0 is a
Debug-only path; normal IsUpdatable is literal false. Generic runtime/native
agreement and actual Main leaf binding remain pending the ONE menu-to-native
SWAMP/player/equipment/camera/movement/HUD milestone.

Field300 type correction before adoption: original GameObject::LoadExternalScript
38ef60 size216 deletes current300 via its own vtable slot4, allocates98 bytes,
calls LuaScriptC1(bool)37c584, publishes that SAME pointer at300, binds GameObject
methods into its Lua context10, appends argument bytes at68, then calls
LuaScript::Load37b574; failed Load deletes that SAME LuaScript and clears300.
GameObjectD2=38d378 uses the same300 deleting virtual then clear0. Therefore300
is LuaScript, not ragdoll. ColBox D2 now routes it ONLY through the dedicated
ColBoxServicesV71::destroy_lua_script300 callback; generic visual/physical/
attached destroy_native does not receive300. Main must bind its actual LuaScript
owner/deleting destructor, never a physics/ragdoll disposer. Null300 follows the
original skip. Successful genuine deletion clears exactly the original300 cell;
a replacement during delivery is rejected instead of erased.
