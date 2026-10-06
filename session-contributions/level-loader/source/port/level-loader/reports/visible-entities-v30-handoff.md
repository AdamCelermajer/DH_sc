# Visible authored objects in SWAMP (V30)

The private loader emulator now renders 19 Characters and 5 chests from the actual retained constructor-prefix export. The original assets, texture bindings and native CPU skinning are used at authored placements. A Next object button focuses each asset for visual inspection. Close-up chest and troll screenshots were viewed and checked. Collision marker nodes identified by the existing `_colbox_` source-backed physical classifier remain in the loaded scene, but their white debug geometry is omitted from this inspection renderer with explicit log entries.

This is partial inspection, not complete SWAMP acceptance: 13 visual selections remain explicit (12 template/RNG providers plus the player/faery model branch); the canonical constructor graph has 105 objects and stops at QuestMoveInZone. Character activation, animation execution, quest/condition/loot behavior, persistent saves, whole Level.Init and current-GS publication remain separate unverified integrations. Other map choices do not yet have this entity manifest. The manifest is generated from original placements; it is not a hardcoded SWAMP object list. Rendered Characters include NPCs as well as mobs.

## Isolation and proof

Only private checkout/build outputs and DH2_Loader_API37 on emulator-5590/private ADB5039 were modified. Both Android ABI builds passed. See visible-entities-v30-verified.json, device log/receipt, and chest/troll PNGs beside this file. No shared source edits, merge or push. Root V27 GSLevel and prepared-source packets remain separately retained, not silently selected into the active graph.

## Reproduce in this private checkout

Build host target dh2_loader_preview_entity_export_v30 in receiver-transport-host. Run tools/preview-run-preview-export-v30.py. Build tools/build_preview.py. Run tools/preview-install-capture-visible-v30.py (guards the exact private AVD before every ADB operation). After preparation completes, tools/preview-object-capture-v30.py 1 chest focuses the first chest; another invocation with 5 troll advances to the troll. tools/preview-observe-visible-v30.py captures a settled frame/log. The export target reuses the canonical source-probe include/link graph; no duplicate World/property/ScriptRuntime ownership was introduced.
