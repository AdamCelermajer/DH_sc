# SAME Level Module load field connection

The existing CanonicalLevelContextV1 receiver now owns source module_offset160
and object_module_id18c, alongside its config and sole Kill word150. It adds
no Module class, counter, selector, file service, global slot or campaign state.
`module_load_fields()` returns a retained lease and direct pointers into this
same Level. A held current-Level borrow returns exactly the same pointers.

Original LevelC1 stores float-bit zero at offsets160/164/168 at addresses
3f3228/3f3230/3f3234. It stores ID-1 at18c at3f32d4, after the two LuaScript::Load
calls. Eight poison/argument cases execute the actual ARM prefix through that
ID store. EventManager/LuaScript constructors, string constructor/assignment
and two Lua loads are declared helper boundaries. Full C1 script/event/table/
online/save construction and original publication ordering remain pending;
immediate native field defaults do not claim those effects have executed.

Map the pointers into root's actual existing interface:

```cpp
auto fields = same_level->module_load_fields();
dh2::world::ModuleLevelLoadBorrowV1 native;
native.owner = fields.level_owner;
native.object_module_id18c = fields.object_module_id18c;
native.module_offset160 = fields.module_offset160;
native.load_file = actual_same_level_load_file;
// Pass native to the SAME actual CanonicalModuleV1::load at the source phase.
```

The load callback must deliver the original Level::LoadFile continuation over
retained XML and canonical factories: delivered=false means service failure;
delivered=true/source=false means the original caller polls again. Do not map
an unsupported class/failing parser to success. Preserve each file occurrence,
including identical URI strings used for different authored load calls.
Real class-owned Module IDs/positions and selected alternatives remain root's
source receivers, rather than authored indices/raw XML values.

The exact currently inspected root module_xml_selection_v1 header/CPP are pinned
as a test-only contract under vendor/module-load-contract-cbbf271d1319c732.
The probe executes its module_load_v1 over this same Level: pending MGP calls
precede pending MVP calls; source completion zeros the offset and sets ID-1.
Random or file service failure preserves reached module fields and diagnostics.
The loader adds no successful failure cleanup. Invalid pointers fail before
Random or field mutation. These checks use explicitly declared Module XML,
class position/ID, Random and LoadFile endpoint fixtures, not actual whole Module
construction or gameplay. All earlier same-Level/current-global checks remain.

Set DH2_LOADER_MODULE_OWNER_TARGET to the existing root target when verifying
there. The pinned CPP fallback is compiled only into the standalone field probe,
never into the production loader receiver. Do not introduce a second runtime
selector or import the captured CPP over a newer root implementation. Its header
needs <cstdint> for standalone libstdc++ builds; the private probe supplies
-include cstdint only to that exact immutable TU and disables its formatting
warning. Root should add the missing standard include to its owned source.

The receiver layout changed. Rebuild all root consumers of its header and use
the SAME retained receiver already used for GSLevel::s_level and KillLevel16.
No borrowed shared owner should be stored inside the Level it pins. The actual
candidate/runtime aggregator must manage separate provider leases and scoped
borrows. GSLevel Ctor/Dtor/publication and complete ModuleInitPost/static/PF/
floor services remain unimplemented at this loader checkpoint.

Verification:18 checks pass host, ASan/UBSan/leaks and Android x86_64 on5590;
ARM64 compiles/links. Original constructor field oracle:8 cases, older original
config/gate prefix:8 cases and original Application global getter:6 cases.
Read reports/canonical-level-module-fields-checks.json for exact source/binary/
cache/oracle hashes and fixture boundaries. The map APK, shared sources, and
main5554/menu5580 were not changed. No full SWAMP or visible mobs/chests claim.

Reproduce with the existing private CMake projects and original cache:

```
python -B port/level-loader/tools/oracle_level_module_fields.py
python -B port/level-loader/tools/oracle_level_constructor_fields.py
python -B port/level-loader/tools/oracle_gslevel_current.py
cmake --build ../build/connected-owner-host --target dh2_loader_canonical_level_context_probe -j 4
cmake --build ../build/connected-owner-sanitizers --target dh2_loader_canonical_level_context_probe -j 4
python -B port/level-loader/tools/build_canonical_level_context_android.py x86_64
python -B port/level-loader/tools/build_canonical_level_context_android.py arm64-v8a
python -B port/level-loader/tools/run_canonical_level_module_fields_checks.py
```

Windows host builds/tests use Ubuntu WSL; SDK and NDK paths remain the existing
ones in the helpers. This field handoff layers after GSLevel context f671cb05,
independently of scene-V3 composition f3f06ab9. Earlier thirteen-check context
receipts describe the older frozen layout; do not reuse their binaries.
