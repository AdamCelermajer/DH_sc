# Isolated scene-V3 build dependency audit

Verified original connected-owner archive166 and scene-V3 archive22 plus the
previously verified PropertyMap include supplement form the new immutable
184-file snapshot. The existing frozen snapshots remain unchanged.

The exact V3 chest test is selected with assertions enabled. It uses the three
original swamp chest BDAEs. Its asset-read and PF providers remain declared
boundaries; this build does not claim real authored chest factories or visible
SWAMP placement. Current executed compilation stops at the missing lifecycle
header, before linking or running. No mutable root header or copied lifecycle
fields have been substituted to conceal the failure.

Reproduce from the private loader worktree after installing the required exact
owner header supplement:

```
cmake -S port/level-loader/tests/cmake-scene-v3 -B ../build/scene-v3-host -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build ../build/scene-v3-host --target dh2_loader_scene_v3_probe -j 4
```

`scene-v3-private-dependency-audit.json` contains every reached local include
edge, exact resolved source/header hashes and the missing header list. This is
a dependency receipt, not passing scene/runtime evidence. The LevelConfig/
Module provider handoff is separately needed to advance actual SWAMP traversal.
