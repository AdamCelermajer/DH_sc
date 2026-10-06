# Complete SWAMP constructor transport and inspection bridge

The native source walk now constructs all 195 original module objects across all nine MGP/MVP pairs, including 50 Characters, 5 chests, one QuestMoveInZone and one SoundEmitter. The independent original-cache inventory matches the native journal. Placement/property assertions, completed-load no-replay, source module-context reset and explicit fixture resource teardown pass. No constructor is silently skipped. The source probe still reports whole Level.Init, actor activation, current GSLevel publication and full loader acceptance as false.

## Changes and contracts

This delta layers after the previously delivered V28 TriggerObject packet. It includes V29 ExitZone plus its additive TriggerZone GO-ID/base-field support, V31 QuestMoveInZone, and V32 SoundEmitter. All use the existing canonical base, source leases, property map and factory dispatch; no new campaign save, RNG, ScriptRuntime or World ownership is introduced.

SoundEmitter retains loop/sound/distMin/distMax and source C1 state. Its property schema requires the newly appended constant_context/constant_integer producer on CanonicalPropertySourceServicesV1. Borrow initialized GameDesign/PyDataConstants with the caller's real lifetime; AudioConstants.CameraReferenceDistance and CameraMaxDistance are converted using the original integer-to-float behavior. No production defaults are fabricated. A real pinned Arrays::Sounds names snapshot supplies InitPost lookup; audio update/shutdown remain required engine services.

QuestMoveInZone uses exact inherited Zone dimensions and the original physical=true/trigger=false configuration. Zone activation and quest collision execution remain main-session services. ExitZone transition execution remains a main-session service.

The private inspection bridge exports original assets and constructor placements, then draws their original meshes/materials with existing CPU skinning. Collision debug markers are logged and excluded from inspection draws; validated empty-visible assets are explicitly recorded as nonvisual. Character scale now uses the existing dh2_character_visual_scale kernel and original base sheet, matching Character initialization behavior; chest scale remains authored XML. The visible bridge is inspection-only and does not publish/activate the canonical actor graph.

## Evidence and remaining work

swamp-complete-constructor-walk-v32.json is the hash-current host receipt, and swamp-original-object-inventory-v32.json covers every authored module file. Original binary proofs accompany the three new constructor/schema families. Both Android inspection ABI builds pass.

The retained V32 device receipt proves 27 Characters and 5 chests rendered, with one authored nonvisual Character and 22 explicit visual-selection gaps: 21 template/RNG choices plus the player/faery model branch. That device capture precedes the latest native-scale correction. The corrected APK was built, but fresh visual QA is pending because the emulator exited during shared-host resource-exhaustion incidents. Earlier chest/troll screenshots are included as historical V30 visual proof, not proof of the latest scale build.

The direct private emulator launcher is now disabled. All future device runs must use the main session's bounded Windows JobObject/watchdog contract. No shared checkout, other emulator, save state, merge or push was touched. Root GSLevel V27 and prepared-source supplement remain separately retained and unselected; this packet does not claim their runtime integration.

## Reproduce and integrate

In the isolated checkout, build receiver-transport-host target dh2_loader_preview_entity_export_v30 and run tools/preview-run-preview-export-v30.py with the original cache/design inputs. tools/build_preview.py builds the private inspection APK. Review receiver/schema deltas and add the three new cpp files to the existing canonical-owner target. The supplied CMake files document the private build; do not overwrite root/menu CMake or Activity files, which contain independent ongoing work. Supply the genuine constant/name/behavior services and keep their leases alive. Whole-level activation and persistent restoration remain integration requirements.
