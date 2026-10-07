V49 connected frame and FX publication investigation
==================================================

The live receipt is `port/android-native/reports/skill-control-v49/receipt-v49.json`.
The guarded existing emulator had exact foreground game PID3845. Bash's actual
`do_skill` delivered Cultist07 HP25600 ->20994, status0, without a native frame
failure. Quick Attack tap dispatched zero commands; 500ms hold dispatched25.
Root owns the synchronous modern down-branch correction and further input tests.

Source receipts
---------------

`scene-ctor-v49.json` proves CSceneManager C1 stores vtable+0x1c at58d984;
SceneManager C1 does the same at352cc4. Therefore Application::_Update32c438
virtual60 at32c918 is CSceneManager::update58b9f0, not a guessed vtable entry.
The normal loaded/unpaused path returns to32c518 after this scene update and
then calls StateMachine::Update at32c574. GSLevel::Update386630 calls
Level::Update3f82d8: physical3f84c8, ObjectManager3f84e8, FXmanager3f8500.
Complete original bodies/hashes are in `fx-frame-chain-v49.json`,
`scene-animate-v49.json`, and `fx-global-order-v49.json`.

`fx-setters-v49.json` proves SyncIrrData492aa0 calls VisualObject::SetPosition
470c24, which performs root virtuala4(position), then virtualb8(false).
The actual RootSceneNode vtable+0x1c b8 entry is updateAbsolutePosition35c27c;
CRootSceneNode's corresponding entry is597c60. Original Sync therefore publishes
the new absolute visual transform before the subsequent draw.

Actual connected mismatch
------------------------

Our composite retains outer_ only in source_scene_frame_v4. Manager frame then
updates the actual retained visual transform, but mesh_draw_sources_v4 uses the
previous outer_. The live first mesh basis is -pi while the same-frame player
root is already -3.1001904. Sample2 catches up. This is a concrete one-frame mesh
publication delay, independent of camera arithmetic or forced target retention.

Required correction
-------------------

1. Preserve the original scene phase before actor/ObjectManager updates, then
   FX manager state/anchor synchronization after actors. The old combined FX
   frame cannot represent actors between the two phases. Do not merely reverse
   its scene_frame/manager_frame calls.
2. At draw collection, compose mesh geometry with CURRENT synced source visual
   position/rotation/scale. This must be a draw-only typed outer borrow: do not
   sample animation, advance timeline, emit particles, reset force history or
   mutate RNG a second time.
3. World-space particles already emitted retain their genuine birth transforms;
   do not rotate their stored positions to conceal timing differences.
4. Existing submitted camera is a real retained provider; placement of its update
   must be coordinated with root's camera phase. No guessed inverse/view basis.

The draw-only correction in point2 is applied with root approval: the additive
composite method `mesh_draw_sources_at_outer_v49` delegates to the same retained
mesh graph, and manager submission supplies CURRENT synced source TRS. V4/V5/V6/
V32 concrete resource owners all implement it. No new production TUs are needed;
all composite consumers require a coherent rebuild because the interface gained
a virtual method. Both-ABI strict compilation passes for all5 changed production
TUs and the actual-cache regression. Point1 remains a separately staged global
frame-order investigation; this packet does not change frame phases.

The evidence does not claim all skills/headings/slopes are visually accepted.
Bash's Lua local captured target did not populate current408/last40c/OOI14a4;
its authored Post ClearTarget remains required. Universal lock preservation would
contradict the actual script and is not proposed.
