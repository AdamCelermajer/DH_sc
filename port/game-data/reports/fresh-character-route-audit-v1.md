# Fresh-character route audit V1

## Finding

There is no currently callable gameplay API that turns the class-confirmation
selection into a fully initialized new campaign character. The existing menu
callback creates a metadata-only profile and requests a menu preview. The
campaign path can restore an already selected profile after the campaign
Application, World, PlayerManager, and canonical Character have been created,
but that API is not a fresh-character factory and cannot safely be called from
the front-menu callback.

## Current callback route

1. `FrontUiSessionV87::create_menu_persona` in
   `android-native/app/src/main/cpp/front_ui_session_v87.cpp` chooses a free
   slot, calls `fresh_player_profile_v1`, and exclusively writes
   `dh2_NNN.savegame`. The producer's documented contract is seven-section
   source metadata; it explicitly does not create an initialized gameplay
   Character or inventory. The caller enables `menu-persona-preview` and
   logs `gameplay initialization pending`.
2. `NativeSetSaveSlotIDToMainMenu` changes only the menu avatar preview slot.
   `NativeAssignSaveSlotToPlayer` publishes a slot to the already existing
   process PlayerManager; it does not construct a Character or Save.
3. `NativeStartGame` retains a validated selected-profile read and queues the
   development campaign route. `native_app.cpp` later tears down the menu
   preview and calls `start_source_campaign_v55` after AS dispatch returns.
4. `create_source_world_v55` then creates the campaign World and its actual
   same-Application PlayerManager. It publishes the selected save slot and
   class row to the first local PlayerInfo before loading.
5. In the campaign World, `RendererPlayerManagerStartupV70::bind` installs
   the source AddCharacter owner before Stage 6. That flow requires the actual
   same PlayerManager, source World, object factory, PlayerInfo and its class,
   visibility, skill, slot and controller fields. It runs the original
   Character save initialization and source AddCharacter continuation.
6. `RendererCharacterCandidateV60::initialize_selected_profile_v67` is reached
   only after source AddCharacter has published the actual Character and the
   selected profile's same slot is on PlayerInfo. It then creates the V59
   profile bootstrap on that Character's existing Save/SaveLoad; Gear restore
   and writer registration follow at their source lifecycle points.

These are separate process phases. The front menu has no campaign
`SourceCampaignCandidateBorrowV55`, canonical campaign Character, published
PlayerInfo character, or source World/Level at the point where it creates the
metadata profile. Calling the campaign initializer there would violate its
same-owner preconditions.

## Relevant source functions and what they do

* IDA `MenuBase::FS_StartGame` at `0x4220a0` is the source new-character helper.
  The saved disassembly shows the indexed Save constructor `0x4655ac`, name,
  level/date/class stores, actual `SG_Save`, and `Application::LoadLevel`
  handoff. It is distinct from the blank `Character::InitializePlayerSavegame`
  constructor at `0x3b36b0` (`0x465ae0`). The present Android callback does
  not invoke `FS_StartGame`.
* The native `PlayerAddCharacterOwnerV5` implementation is a continuation of
  the actual PlayerManager `AddCharacter`; it cannot substitute for the
  original save construction or Spawn. Its integration notes enumerate the
  required source endpoints and explain that the current application does not
  bind all of them.
* `create_native_menu_preview_player_v121` does construct a canonical
  Character for menu preview and runs source preview initialization. It owns a
  preview-domain Spawn/Save/Profile/Gear lifecycle; it does not publish that
  actor as the campaign PlayerManager's `Character660`, run source
  AddCharacter/count publication, or become the campaign actor. Reusing it as
  gameplay creation would create a second owner, not complete the campaign
  handoff.

## Minimum missing handoff

Before class confirmation can create a playable campaign character, the
implementation needs one source-faithful new-save owner in the real process
Application scope. It must execute the `FS_StartGame` New branch against the
selected class/name and actual Save/FileManager/job owners, then hand the
created profile into the campaign `Application::LoadLevel` route without
replacing the selected profile or creating parallel Player/Save owners. On
campaign startup, the existing source AddCharacter route must receive the
same PlayerManager and class-selected `PlayerInfo`; its Spawn, InitializeSave,
InitAll/InitFinal, skill arrays, visibility, current Level/QuickSave,
controller, and light continuations must be bound to real providers. The
existing V59 restore bootstrap can then adopt the same created Save at the
source profile/GEAR lifecycle points.

Until that seam exists, the correct behavior is to keep the metadata save
explicitly menu-only and avoid reporting class confirmation as completed
gameplay-character creation. No production code was changed by this audit.

## Validation scope

This is a source/call-route audit only. It makes no build, APK, emulator, or
live profile-write claim. Existing `fresh_player_profile_v1` tests validate
the metadata bytes and input rejection, not the full class-confirmation to
playable-character route. The PlayerAddCharacter and V59 bootstrap tests
validate their own already-constructed receiver boundaries; neither proves a
fresh `FS_StartGame` handoff.
