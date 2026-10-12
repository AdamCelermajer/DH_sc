# Canonical chest graph consumed by original Swamp source

The main session's `canonical-openable-graph-v4-54a3fb6ce996459b.zip` was
verified against all 165 manifest entries. Its SHA256 is
`54a3fb6ce996459bd59d06d756c762d17cdf4ddac5431b696ce623a5d310e737`.
The private loader consumes `CanonicalOpenableGraphV4::factory_receiver()`
through its existing generic receiver transport and the SAME canonical factory
and object manager. No second catalog, manager, world or container behavior is
implemented. Source XML declarations are not copied into C++.

The existing original Box2D2.0.1 backend was frozen as a dependency. One missing
event-track translation unit was added: `port/engine-animation/event_track.cpp`,
SHA256 `e7b70e88bbcbdc54ce28adbc78590fdc1005304cc8d017f9aab91149bf150792`.
It is byte-identical to current main. This is the real implementation referenced
by the animation owner; no missing-symbol stub or suppression was introduced.

## Actual source outcome

The original root Swamp MLX still constructs nine Modules, 16 floor clones,
19 exits and nine generated zones. SAME-root controllers and 386 retained
render submissions are verified as before. The actual first Module load then
consumes its original unfiltered MGP. The first declaration is
`_prim_OpenableContainer_2`, with authored `data_desc=Swamp_Normal_Chest`.

Its actual Container C1 now succeeds. The receiver has GO_ID7 and the source
class name. The next source stage, ObjectManager Add, fails during name lookup:
it examines the already published LevelConfig in room -1, while searching room
0, and attempts to read its unproduced byte87 field. The exact diagnostic is
`Level::_LoadFromXML service: Required actual LevelConfig/Module produced source bool field`.
The factory failure prefix is **constructed**. The failed Container is retained
by the actual receiver transport. It is not published, has no assigned room or
Handle, and has not run later property defaults/overrides. Repeating Module load
preserves the exact failure and does not replay construction.

The original source confirms the room gate precedes the name comparison:
GetObjectByName34aca0 reads room64 at34ad94 and byte87 at34ada0, then compares
the name at34adb4. Reordering those branches is not a source-order fix.
ObjectBaseC2 explicitly stores bytes84/85/86/88/89 at33f410..33f420; byte87 is
not among those stores. LevelConfig DeclareProperties3f4b9c has no inherited
ObjectBase declaration tail. No source value for LevelConfig87 was fabricated.
Actual allocation/lifecycle/publication context or a genuine field producer
must be established by main before this path can continue.

An explicit call to this retained Container's actual InitPost also fails at
its absent `CheckSpawnProbability` outer service. This check does not run that
method as part of the factory; source factory order is unchanged. Visual/PF/
physics/condition/script/audio/key/loot providers remain required when reached.
The complete chest visual graph is linked but is not initialized by this failed
source prefix. No authored chest rendering is claimed.

The original first MGP lists another chest and then
`Character/_prim_NPC_PriestGood` (`WanderingPriest`) after this first declaration.
Those later declarations have not been consumed in this run. Their missing
constructors are not reported as reached failures.

## Reproduction and integration

Run `tools/build_module_graph_source.py` for host, sanitizers, x86_64 and
arm64-v8a, then `tools/run_openable_source_checks.py` in the private checkout.
The runner verifies cache, overlay and incoming manifest hashes and explicitly
targets only emulator-5590/DH2_Loader_API37. ARM64 is compiled, not executed.
The receipt records exact source and binary hashes. Test-only outer services
for the earlier Module graph remain declared fixtures; Container production
services are left absent rather than replaced with success callbacks.

Main should consume the source integration and failure reproduction selectively.
Its existing engine implementation already owns the chest graph. The immediate
required handoff is actual LevelConfig87 lookup context/provenance, followed by
production Container services and later authored class constructors. The visible
APK is unchanged. Full Swamp map/mobs/chests and generic-loader acceptance remain
unfinished.
