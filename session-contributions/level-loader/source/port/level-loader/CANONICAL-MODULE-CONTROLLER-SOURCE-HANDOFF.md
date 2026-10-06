# Original-source Module/controller integration

The original complete `data/scene/001_swamp.mlx` is consumed through the retained
cached-file walker, generic receiver transport, actual canonical factory,
property map, LevelConfig publication and Module graph. No Swamp declarations
are copied into C++. This continuation consumes the main session's immutable
Module/controller package `b3ceccd6833a5ca6`, SHA256
`9bd35ee02aea3aaa1b0c461c527e6071356c331945b8190d1af98423d3a47e01`.
All 17 manifest entries were verified before import. The preceding 340-file
graph is preserved unchanged; the new coherent overlay contains 351 files.

The source load constructs nine Modules, 16 floor clones, 19 exits and nine
generated RoomZones. It executes actual floor post-load, collision-bound
publication and nine Module InitFinal continuations. Each Module has an actual
controller whose retained root identity equals that Module's scene root.
Explicit graph release clears all nine controllers and root identities before
map/registry release. The source manager constructor reserves null key 0;
generated zones are resolved using their published handles, without numeric
key assumptions.

Reproduction in the private checkout:

```
python port/level-loader/tools/build_module_graph_source.py host
python port/level-loader/tools/build_module_graph_source.py sanitizers
python port/level-loader/tools/build_module_graph_source.py x86_64
python port/level-loader/tools/build_module_graph_source.py arm64-v8a
python port/level-loader/tools/run_module_controller_source_checks.py
```

The runner validates the original cache hash, all coherent overlay hashes and
the incoming 17-entry manifest. Android execution targets only emulator-5590,
and checks its AVD is DH2_Loader_API37 before each device operation. ARM64 is
compiled, not executed. The JSON receipt records the exact scope and binaries.

Outer platform, random/network/local-player/condition and sound-name services
remain explicit fixtures. The nine original Swamp Module resources have empty
animation libraries; this receipt does not verify dynamic animator execution.
Explicit graph release is not whole GSLevel unpublication or destruction.
The visible APK remains the earlier map inspection path; no new APK acceptance
or authored actor rendering is claimed.

The actual first Module load reaches the original unfiltered MGP and fails at
OpenableContainer construction with an empty factory prefix. It retains the
source failure journal and SAME Level module context. The next required main
handoff is a canonical catalog receiver binding for OpenableContainer with its
actual initialization/visual services, followed by Character/AnimatedDecor and
remaining authored classes. Production GSLevel event/Lua/save/publication and
condition services also remain required.

This archive is a private reproducible integration checkpoint. Main should
adopt the original-source consumption and verification changes selectively;
its own Module/controller source already supplies the engine implementation.
Do not replace root CMake or create a second canonical manager/world. This does
not complete the generic loader goal or the Swamp map/mobs/chests acceptance.
