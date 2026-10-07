Character menu V50 current-root integration
==========================================

Implemented production:
- `FrontUiSessionV87::LaunchRequest.profile` is an immutable shared receipt of
  the exact selected file read at NativeStartGame: bytes, backup/base origin,
  slot/class/level/name/selected difficulty, requested difficulty, directory,
  and actual named-section presence. Consume no longer rereads a changed avatar
  slot after AS dispatch. Seven-section personas have no GEAR/PROP/QEST and are
  never classified as saved equipment.
- `CharacterMenuCampaignSaveV50` registers ALL15 existing original named writers
  over an already published SAME SaveLoad/Profile. GEAR uses actual current
  inventory, PROP uses the actual Saved194 source cell, and QEST uses the actual
  mode/collection/Quest serializer. Missing positive Quest leaf fails explicitly.
- SAME `PlayerSaveLoadOwnerV1::bind_services_v50` attaches real platform providers
  without resetting Save/profile/source fields. It rejects active/reentrant
  delivery. `load_services_v50` borrows the writer weakly to avoid a Save/provider
  ownership cycle; positive PROP reload reads the actual current profile cache.
- `FreshInventoryOwnedV4::load_saved_section_v50` restores Source __LoadInventory
  over ONE actual owned vector/equipment/potion graph. Actual ItemTable names,
  power names, Item C1 effects, SetValue/identified, AddPower, forced AddItem,
  BOTH saved equipment sets (source forced=true) and gold are preserved. No
  temporary mutable inventory, replacement Gear, inferred item IDs or RNG reset.
- `PlayerEquipmentRenderInputsV1::saved_gear_v50` is called after the actual
  inventory/text/Gear/Skin receivers exist but BEFORE source InitialGrant.
  Present malformed/effect-failing GEAR stops initialization with ready=false;
  it cannot fall through to starting kit. Genuine absent GEAR keeps existing
  source first-creation grant behavior. Restores/GL resize do not call initialize.
- `renderer_character_mutations_v4.inc` accepts an optional typed writer provider;
  old callers preserve behavior. `renderer_character_menu_campaign_v50.inc`
  provides the exact staged same-authority callback/member/insertion contract.

Root must bind (not yet live-connected):
1. Retain LaunchRequest.profile across the actual World creation call. Forward
   real selected slot metadata/source file to the SAME fresh Player Save C1.
   Reuse that Save in SkillV6; do not construct a second Save for the panel.
2. Use actual Android filesDirectory and shared Application save-job owner with
   PrivateSaveFileTransportV45. Construct the actual CampaignSaveProfileV45 for
   the source filename and publish its real receiver into SAME SaveLoad+8.
   If files changed after captured selection, report the actual new read;
   do not call the immutable launch receipt an updated live cache.
3. Load source metadata into SAME Save. Full original slot17 initialization also
   requires genuine Quest constructors/readers. Source menu metadata-only
   receipt is not whole slot17 C1. Current frontend passes no QEST loader and
   will explicitly fail once positive QEST appears: do not omit QEST to hide it.
4. Restore source PROP at its actual initializer boundary. Provide selected
   profile cache GEAR bytes through saved_gear_v50 BEFORE Gear.initialize;
   pin actual cache/profile as returned shared lease. Root currently initializes
   Gear before constructing Skill Save; boot order must share an earlier actual
   Save producer instead of loading saved Gear after starting kit was granted.
5. Construct/retain CharacterMenuCampaignSaveV50 with actual Character/Skill/
   Power/Level/Map names and tables, source CurrentDifficulty getter, fresh
   SAME actor/property/Gear borrower and actual online selector. Bind once.
   Full positive quest serialization needs actual Quest::_saveQuestData.
6. Supply menu_campaign_writer_v50 to supplement_character_menu_mutations_v4.
   Genuine NULL profile remains the source no-write early return; no persisted
   campaign claim is made for the current development slot-1 graph.
7. Use weak reader services on SAME SaveLoad; release runtime writers/readers
   before actors, profile, Gear and immutable table owners. Keep Application
   files/jobs independently leased, not App->Level->App or Save->writer->Save.

Current menu feature providers:
- Source Stats allocation, skill training/details/mapping, Inventory details/
  equip/unequip/swap, gold/potion count and original tab/back lifecycle already
  run through CharacterMenuQueries/Actions + actual authored movies.
- Real point/level/unlock limits remain authoritative. No points/unlocks are
  added for UI demonstration.
- Fresh faeries remain locked. Positive faery selection retains SAME Save but
  actual faery model/SetVisual/AddAnimation/Level placement are required when
  source CharAI420/currentLevel become nonNULL. Current explicit failures are
  not removed by this packet. Canonical loader/positive faery owner is needed.
- Menu world-drop callbacks remain required on a positive Item drop; no direct
  Gear insertion or swallowed audio/world service is added here.

Evidence:
- Whole15-section disk/.bak/existing readers/SAME PROP reload and reentrant
  service attachment:33 checks each O1/O2 ASan+UBSan+leak checks.
- Actual cached Item/dual equipment/potions/gold/same identity/failure/RNG:
  19 checks each O1/O2 ASan+UBSan. Effects are explicit fixture callbacks.
- Actual source seven-section persona immutable handoff:16 checks each O1/O2,
  no inferred GEAR/PROP/QEST. This is not live profile publication proof.
- Strict Android bothABI receipts cover changed source owners, test closure and
  FrontUiSession/helper. Root owns whole APK build and live all-tab workflow.

Link closure applied with root approval:
native:front_selected_profile_v50.cpp
game-data:fresh_inventory_saved_v50.cpp,player_save_named_writer_v1.cpp,
player_save_state_fields_v45.cpp,quest_savegame_v1.cpp
level-world:character_menu_campaign_save_v50.cpp,campaign_save_profile_v45.cpp,
private_save_file_transport_v45.cpp,level_savegame_writer_v2.cpp,
savegame_jobs_owner_v2.cpp,savegame_stream_v2.cpp,level_savegame_objects_v2.cpp,
player_save_metadata_writer_v45.cpp,player_save_inventory_writer_v45.cpp,
player_save_collections_writer_v45.cpp
Existing SaveWriteOwner is already compiled by level-world and was not duplicated.
Root loot V47/V49 linker closure additionally requires actual definition files
world_loot_canonical_bindings_v44.cpp and player_save_difficulty_global_v29.cpp;
these were added, preserving their genuine source provider requirements.

Source evidence: original inventory body
`game-data/reference/player-inventory-v1/captures/core/reference/original-functions.asm`
46a608..620 force+convertGold=true;46a63c/68c forced=true; temporary selected-byte
stores/restoration at46a648..66c and46a690..6b4. Existing V45 writer oracle/frozen
packet remains unchanged; this is a supplemental integration successor.
