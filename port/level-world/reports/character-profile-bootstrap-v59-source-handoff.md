# V59 source campaign profile/bootstrap connection

## Saved implementation, not runtime acceptance

RAM hold remains active. No compiler, WSL, sanitizer, APK, emulator or test
process was launched. Source captures are bounded local ELF/cache reads.
The candidate code below is uncompiled; root must validate the coherent closure
before publishing readiness. No global model_renderer/native_app/CMake changes
were made by this lane.

## Concrete ordering and authority

`renderer_profile_bootstrap_v59.inc` takes the actual source player receiver's
already constructed Save/SaveLoad, Character14e8 pointer, PlayerInfo664 slot and
SetSlot endpoint. Source SetSlot is executed before genuine CampaignProfile C1
(actual Application jobs flush, private-file primary/backup read and cache).
An execution receipt prevents repeating that preceding scalar store; it is not
a source gameplay-ready flag. The profile cache must exactly match the immutable
NativeStartGame selected file bytes. A mismatch fails rather than switching
profile or manufacturing a fresh save.

`CharacterProfileBootstrapV59` publishes that same profile+8, reads source mask1
fields and initializes source mask2 structures. This is an explicit staged
native adoption of already-created source campaign receivers; it does not
certify whole AddCharacter/GS completion or allocate another Save.

`PlayerEquipmentRenderInputsV1.source_profile_load_v59` runs after real
Inventory/Gear/Skin creation and before the existing source InitialGrant. It
invokes SAME SaveLoad.load(4), retaining source order:

LVLS, SKIL, FAES, actual online/hosting query, CFEE, QEST, PROP, GEAR, FTVL,
then actual volatile-log selector. No offline/network answer is fabricated.

GEAR is parsed once via SAME owned inventory, PowerNames and original effects.
The helper accepts this operation only in the pre-Grant window and rejects
reentry, second attempts and dual saved_gear_v50+source_profile_load_v59 setup.
A failed positive section cannot fall through to starting equipment. Missing
sections use the source no-reader branch; InitialGrant still executes its actual
item/gold conditions, without an invented fresh/saved skip.

After real Gear.initialize completes, `finish` binds all15 V50 writers on that
same profile and replaces the weak named-reader dispatch for future menu
Load20. SkillV6 must reuse `source()->save()` and `source()->load_owner()`.
It must not allocate/reset/initialize a second Save on this campaign path.

## Genuine table producers

The source _InitLevelStates46954c GOT references were resolved exactly to
Arrays.LevelList size/members and Arrays.WorldMap size/members. Source defaults
are Level row+28 (stride72) and WorldMap row+8 (stride20). WorldMap is not
FastTravel's separate stride28 table.

`world_map_profile_table_v59.*` decodes actual worldmap cache13 locations and
3 lockers, including LocationLevels. The native adapter supplies its actual
names/defaults, and signed LevelProjection72.words[10], with independent table
leases. Source proof/cache receipts are under
`reference/character-menu-profile-v51/worldmap-v59/`.

The approved Front patch now retains one actual decoded Quest table and supplies
menu_load_quest_acts_v59 to selected-avatar metadata, occupied-slot details and
NativeStartGame. It preserves source regular/rewound-volatile reads and does not
discard positive QEST. Quest persistence cells remain on SAME gameplay Save.
Full Quest gameplay registration/evaluation/reward operations are not accepted
by the persistence-only projection; reached ReInit states2/3/5/6/8/9 require
actual lifecycle callbacks. This limitation remains explicit.
Source v2ConditionStub506008 contains inherited Type plus Param1 and Param2
words; all three are decoded, rather than trusting the base-only schema group.
The smallest empty v2Quest wire row is173 bytes; impossible counts are rejected
before allocating its native row containers.

## Exact root/cross-agent seams

* level_runtime_integration: provide source-C1 Save/SaveLoad14e8 and actual
  PlayerInfo664/SetSlot before Gear. Source World creation currently has no
  player receiver; do not bind the old Crypt player as its replacement.
* status_progression/root Application: provide ONE Application-global
  SavegameJobsOwnerV2 and SAME actual private FileManager. The adapter checks
  directory and storage identity. Do not allocate a queue per profile.
* At actual player construction: create/retain RendererProfileBootstrapV59;
  configure_equipment(input); call the existing real Gear.initialize; then
  finish(Gear, actual writer services). Keep these owners through GL restore.
* Supply actual Character.GetSkillsList, CurrentDifficulty store/get, immutable
  Character/Skill/Level table leases and source online/volatile endpoints.
  The adapter now derives the actual Level/WorldMap defaults itself.
* Source menus: bind the ready campaign writer into existing mutation services;
  preserve same SaveLoad and original scoped lifecycle/action dispatch.
* Teardown: stop panel/Skill callbacks, release bootstrap/reader/writer before
  Gear/player cells/PM; Application jobs keep their Application lifetime.

## Required link closure and validation source

Game-data new TUs: quest_persistence_v51.cpp, world_map_profile_table_v59.cpp.
Level-world new TUs: character_menu_quests_v51.cpp,
character_menu_profile_load_v51.cpp, character_profile_bootstrap_v59.cpp.
V50 writer/reader/file/jobs closure is already linked by c669131.

Narrow existing changes: Front QEST hunk; Gear hook/guarded load; V51 PROP
dispatch forwards to the actual pre-Grant reader before the writer is bound;
read-only FileManager directory and job-storage accessors (no layout changes).
Gear input/Impl layout changed additively, so rebuild all its consumers.

Written but unexecuted tests: quest_persistence_v51.cpp,
world_map_profile_table_v59.cpp, character_profile_bootstrap_v59.cpp,
equipment_profile_window_v59.cpp. The equipment test reuses the actual source
cache/3-class fixture and tests one restore, source failure-before-grant and
dual-hook rejection. Tests explicitly identify their selector fixtures and do
not claim campaign/player/global readiness or live visual proof.
