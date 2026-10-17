# Crypt script integration evidence

This is work after source checkpoint `f811f80`; it is not part of that APK or
release. The existing bounded PyData decoder successfully consumes both entire
`007_crypt_01` tables from the original cache. The script command table is
available. The new source session/contact composition now executes GhostAmbush01
in the native development app; see the [checkpoint](../../../../docs/CRYPT-SCRIPT-CHECKPOINT-2026-10-04.md).

`decoded-original-scripts.json` pins input hashes, local/global IDs, every
command byte range and exact payload for five Ghost ambush scripts. There are
15 common names and 25 local Crypt scripts. The first two new direct Ghosts
belong to **GhostAmbush01**, not **GhostAmbushHallway**:

| Script | Source order |
|---|---|
| GhostAmbush01 (local 2 / global 17) | Wait 250; Spawn `_prim_Monster_SURPRISE_01`; Wait 75; Spawn `_prim_Monster_SURPRISE_02`. |
| GhostAmbush02 | Wait 150; Spawn SURPRISE_03; Wait 75; Spawn SURPRISE_04. |
| GhostAmbush03 | Wait 200; Spawn SURPRISE_05; Wait 75; Spawn SURPRISE_06. |
| GhostAmbush04 | Wait 200; Spawn SURPRISE_07; Wait 75; Spawn SURPRISE_08. |
| GhostAmbushHallway (local 6 / global 21) | Wait 200; Spawn `_prim_tmp_ambusher01`; then three Wait 50 / Spawn pairs for 02–04; final Wait 50. |

The module `crypt_straight_c_ns_01.mgp` defines the direct pair and
`_prim_TriggerZone_GhostAmbush01`, `script=GhostAmbush01`.
`crypt_straight_ns_01.mgp` defines the four weighted-template ambushers and
its separate `GhostAmbushHallway` trigger. The two paths must not be conflated.

The source composition owns both paired native PyData tables, rejects programs
requiring unsupported services, and synchronously routes exact-name commands
through the Character factory. Wait checks prior elapsed, updates once and
returns without rechecking that frame. The ScriptManager phase precedes physics
and ObjectManager; contact starts the script for the next frame. At 25 ms ticks
after initial advance0, GhostAmbush01 dispatches at 275/350 ms, not 250/325.

Original TriggerZone contact uses inclusive absolute-AABB overlap through
GameObject::IsTouching; it does not call the separate Zone::IsInside predicate.
The API37/16KiB test approached the unchanged authored trigger with actual touch
root motion from an explicit fan player-start override. Both ghosts reached
Idle with native bodies, and reload/rotation preserved the consumed trigger
without Spawn replay. This does not complete the separate Hallway factory,
full AI/combat or the campaign. Earlier published artifacts remain unchanged.
