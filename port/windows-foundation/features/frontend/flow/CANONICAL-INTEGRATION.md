The adapter is a reusable root-provider contract, not an implemented indexed
save service. Authentic Windows CreateSaveSlot/AssignSaveSlot/StartGame owners
are currently absent, so production creation and launch remain unavailable.

Link `canonical_navigation.cpp` and bind a root-owned
`std::shared_ptr<CanonicalNavigationOwner>` through `bind_canonical_navigation`.
Every call loans the exact `CharacterState&` returned by `live_character()`.
The bridge stores no character, save graph, profile map, save path or world.
Gameplay, inventory, equipment and frontend must all use that same authority.
Availability is per original operation; a missing operation/loan fails before
dispatch. The bridge preserves source error prefixes. Navigator retains real
successful create/assignment prefixes and does not replay completed creation.

Root must implement these dependencies before returning availability:

1. Complete detached creation: actual `CanonicalCharacterCandidateFactoryV60`
   and `CanonicalCharacterCandidateRecordV60` with `init_complete` produced by
   real source initialization. The candidate must retain native Save/Load,
   inventory/property/gear, player script/VM, skills, faeries and campaign cells.
   Invoke frontend `stage_creation` using a real `CompleteCreationService`,
   resolve full identity/name/class and validate its shared projection before
   profile persistence. `make_default_character` and source metadata-only files
   cannot fulfill this operation.
2. Starting inventory/equipment: actual `FreshInventoryOwnedV4` with reached
   item name/stat/requirements/gear/Skin/vital effects, sharing candidate
   PropertyState. Keep native partial prefixes inside the detached candidate;
   do not publish failed initialization to canonical gameplay or user saves.
3. Indexed source slot constructor/SG_Save: actual next-free slot allocation,
   real seeds/save date, source difficulty0, new profile progression/level row41,
   and actual persistence. `save_character` and the ordinary matching-roster
   GameSave checkpoint serializer are not original indexed save slots.
4. Assignment: actual slot/player owner with native SG_Load/profile/gear/skills
   publication into the SAME already-established canonical graph. Retain native
   difficulty, faeries, quest/level cells and saved rank-zero/slot-map data.
   `CharacterProfileBootstrapV59::prepare` alone is not full player initialization.
5. Launch: actual native progression/difficulty selection and Application-level
   level-start owner, including authoritative world/RNG/player publication. A
   request collector or accepted boolean cannot claim that gameplay loaded.
6. Capture: existing `capture_game_save` with the SAME live canonical character,
   actual playable world/RNG and controlled actor. Campaign owner registry must
   capture/validate every registered source fragment using the acceptance team's
   campaign snapshot helpers. Missing lifecycle codecs must remain explicit
   failures. Detached GameSave capture destination publishes only on success.

Source order remains Name->Class confirmation:
NativeCreateSaveSlot, NativeAssignSaveSlotToPlayer(slot,0), publish current_slot,
NativePopAllAbove(Main), NativePushMenu(StartGame). StartGame release repeats
NativeAssignSaveSlotToPlayer(slot,0), then NativeStartGame(CurrentDiff).
Failures stop the remainder and retain completed prefixes. No extra source
initialization or retry policy is inferred by this bridge.

The shared model cannot flatten native data without proved mapping: source has
Strength149/Dexterity150/Endurance151/Energy152 and no Intelligence property;
native saved skills contain rank-zero records and two slot maps; faery/quest/
difficulty/level cells are not guessed unlock strings. Complete native owners
must remain attached alongside an explicitly validated shared projection.
See sibling `creation/FULL-CREATION-OWNERS.md` for exact producer boundaries.

`canonical_navigation_contract_tests.cpp` intentionally supplies rejection-only
owner probes and an uninitialized address sentinel. It tests exact reference/
argument forwarding, unavailable ownership/loan gates, source failure prefixes,
create-failure blocking later assignment/start, and failed snapshot destination
preservation. It simulates no successful creation, initialization, assignment,
level start or capture. Strict C++17 compilation passes15 assertions; source
success tests must be supplied by root's actual initialized owner once available.
