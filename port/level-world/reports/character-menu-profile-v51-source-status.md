# Selected profile and QEST integration V51 — source-only RAM hold

V50 is linked by root's c669131 build. Its actual selected-profile publication,
GEAR bootstrap provider and positive campaign-writer binding are not active yet.
No compiler, WSL, sanitizer, emulator or APK work was launched for V51 during
the RAM hold. The new code and test source below remain unvalidated candidates.

## Source recovered

`reference/character-menu-profile-v51/factory-original.json` records the actual
13 Objective factories at original969908, followed by a zero padding word before
Objective's vtable969940. The padding is not an additional supported factory.
Actual virtual load28/save2c entries prove bool-only wire for types4 (MoveInZone),
6 (Automatic),12 (GatherLoot); all other supported factories use SavedQty's bool
plus signed word. Quest.IsVolatileState47f70c's source states2..11 are
`1,0,1,1,0,1,1,0,1,1`. No native code installs those original32-bit vtable values.

Actual cache table/name/schema files and hashes are retained in
`reference/character-menu-profile-v51/cache/manifest.json` (64 Quest rows).
The candidate decoder follows Structs.v2Quest504a94, ObjectiveStub504818,
Objective50358c, RewardStub50589c and Scripts4dd5a8. It preserves full authored
definitions including prerequisites, tier rewards, scripts and start/end
objectives. Once decoded, the row storage cannot be replaced while receivers
borrow it.

## Implemented candidate boundaries

* `game-data/quest_persistence_v51.*`: actual-table decoder and retained source
  persistence cells. Fresh state−1 to authored state stores, same Character
  owner binding, exact objective wire, source failure prefixes and volatile64
  stores. This is a persistence receiver, not accepted gameplay registration,
  prerequisite evaluation, reward delivery or a whole Quest virtual object.
* `character_menu_quests_v51.*`: same Save regular_b8/volatile118 collection
  initialization, regular then rewind/volatile QEST loading, source writers.
  ReInit old states2/3/5/6/8/9 require actual objective/marker continuation;
  absent providers fail before the corresponding authored state replacement.
* `character_menu_profile_load_v51.*`: actual same-profile named field readers,
  including init_quests and QEST. GEAR and networking remain typed actual
  endpoints; no missing reader returns successful empty data.
* `tests/quest_persistence_v51.cpp`: actual64-row wire contrast, all tiers,
  regular/volatile rewind, metadata acts, truncation reached state and lifetime
  guards. Written but not executed during the hold.

## Exact root seams

1. `native_app.cpp` consume_launch_request must retain request.profile instead
   of discarding it; the packet already pins actual NativeStartGame bytes,
   origin, private directory, slot and source-parsed metadata.
2. `model_renderer.cpp` bind_player_equipment currently precedes player Save C1.
   Construct/publish the SAME selected Save/profile before Gear, reuse it for
   SkillV6, and supply PlayerEquipmentRenderInputsV1.saved_gear_v50 from actual
   profile GEAR. A present malformed GEAR must not fall through to starter kit.
   The new profile_saved_gear_v51 callback pins an actual immutable cache Borrow
   for the returned span; retaining only the mutable CampaignProfile owner would
   not pin those bytes across a recache attempt. Getter readiness does not imply
   that the full restore/bootstrap or fresh creation sequence has run.
3. SAME SaveLoadOwner.bind_services_v50 receives V51 named-reader services and
   genuine remaining online/GEAR endpoints; mask initialization requires real
   Character.GetSkillsList and Level/Map defaults. No second Save or copied
   Character stats owner is allowed.
4. FrontUiSession's MenuProfileMetadataServicesV1.load_quest_acts can call
   load_quest_metadata_acts_v51 with the retained decoded table. Its temporary
   Character owner0 applies only to menu metadata, not a gameplay actor.
5. Existing renderer_character_menu_campaign_v50.inc binds the SAME profile
   writer into supplement_character_menu_mutations_v4. Actual Android
   FileManager/private path and Application-global jobs are still required.
   QEST writer must borrow the same retained quest persistence cells, and later
   gameplay quest producers must update those cells rather than clone them.
   Bind CharacterMenuCampaignSaveServicesV50.quest_save_data to a weak retained
   CharacterMenuQuestsV51::save_quest(id,stream,error) callback; it resolves both
   actual collections and rejects an unrelated native identity.

Do not add the three new production TUs to CMake or publish readiness before
coherent compile/cache tests after memory admission resumes. V50 receipts remain
historical accepted proof; they do not validate this V51 candidate.
