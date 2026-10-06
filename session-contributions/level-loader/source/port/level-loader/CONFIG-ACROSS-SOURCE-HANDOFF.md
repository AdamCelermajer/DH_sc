# Documented Config compatibility repair in original source load

All 12 entries in main's `level-config-across-rooms-handoff-a3df90efd191542c.zip`
were verified. Archive SHA256:
`1da0af651b30a4d1a662d05e60b46d73937bf74900221604f2640220024b0773`.
Only its selected `canonical_level_config_module_v1.cpp` replaces the preceding
implementation. The coherent header graph, manager traversal, Module code and
property catalog remain unchanged. Selected CPP SHA256:
`87f7aefe8ec3256ff92e1b0444c23ca7c6948335dac18d91acd84417058ed8ec`.

This is an explicit **modern legacy-UB repair**, choosing native Config byte87=0.
The original malloc/C1 path leaves that byte indeterminate; the incoming proof
preserves 0/1/a5/ff. No original zero initialization or constructor parity for
this field is claimed. The included native regression passes with the SAME
Config, property map, manager and a genuine fresh type7 base receiver.

The original complete Swamp MLX still constructs nine Modules/controllers,
16 floor clones, 19 exits and nine zones, with 386 retained render submissions.
The unfiltered first MGP now passes Config's room-lookup gate and reaches the
next existing receiver: generated RoomZone key11, type11, room-1. Its byte87
is unproduced and its actual canonical reader fails. This is not a missing
Container constructor: that C1 succeeds and is retained. Factory prefix remains
constructed; no Handle/publication/defaults/overrides are fabricated or replayed.

The Container's separately exercised actual InitPost still reports missing
CheckSpawnProbability. Main's existing spawn helper is the reuse point, but its
shared application Random0/1 and network/Handle/lifecycle services are required.
The renderer's demo seed must not be substituted. Retained source fc is a word,
not an alias of native pointer storage; byte82 requires its genuine source write
producer. No copied scalar alias or constructor default was introduced here.

Build each platform using `tools/build_module_graph_source.py`, then run
`tools/run_config_across_source_checks.py`. Host, ASan/UBSan, and loader5590 run
both the original-source checks and the supplied native regression. ARM64 is
compiled. The runner verifies both immutable manifests, all overlay hashes,
the selected CPP path/hash and the explicitly declared compatibility policy.

Main has assigned the RoomZone source audit. This checkpoint does not broaden
the Config repair to unrelated receivers. Native failure reproduction is not
whole-level acceptance. The visible APK and authored mobs/chests remain pending.
