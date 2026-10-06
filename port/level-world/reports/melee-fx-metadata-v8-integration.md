# Melee FX metadata closure V8

The live normal Attack failure on APK `116381335582c6544e507d3492f60d5e6c229fc3acc48124321d7a2317f7683d`, PID15757, was `Required non-particle source render metadata`. V4 Resource::load selected the typed composite factory only for a nonzero BRES particle library. Mesh-only effects instead entered the older reduced decoder, whose draw-source methods explicitly lack source receiver metadata.

The configured resource factory now owns every authored resource, including zero-emitter composites. The same V6 scene/mesh/material owner supplies actual node identity, matrix68, render metadata, retained geometry, and V7 triangle packets. Particle resources retain the same path. Factory failures remain required errors; resources are never skipped. The legacy decoder remains available only for callers without a configured factory.

Production change: `character_mesh_fx_owner_v4.cpp`, Resource::load selection. No HUD, input, application, RNG, HP, or rendering defaults changed. V1/V2 predecessors remain untouched.

Isolated current-APK-linked Android test: `character_melee_fx_manager_v8.cpp`, actual original cache and effects tables, source debug/module owner, explicit camera/anchor/floor fixtures. PASS1695 checks, 310 manager frames across ten selected actual resources (nine melee swoosh meshes and blood). Every frame traverses scene/manager update, particle metadata collection, mesh metadata collection, and actual geometry packet validation. This proves CPU metadata/geometry/lifetime closure, not live GPU acceptance.

Strict ARM64 and x86_64 production compile PASS. Root must rebuild and repeat the actual normal Attack control on the app to verify the original live failure is gone. Existing Bash V9 live acceptance is preserved separately.

Runner: `.local-inputs/run_melee_fx_manager_v8.py`; source asset extraction: `.local-inputs/prepare_melee_fx_v8.py`; receipt: `reports/android-native-owner-tests/character-melee-fx-manager-v8/receipt.json`.
