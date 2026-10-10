The frontend creation service remains unavailable until a real complete detached
campaign owner is supplied. `stage_creation` must run and validate before profile
persistence. A fresh source metadata file is not a ready gameplay Character.

Available source owners and boundaries:

- `resolve_original_fresh_player` resolves level 1 with source vital refill and
  retains all 224 property words. Its API explicitly excludes equipment, buffs,
  difficulty and co-op modifiers. It cannot produce a complete profile alone.
- `FreshInventoryOwnedV4` is the newer authoritative native item/slot/equipment
  owner, sharing the actual `PropertyState`. It supports fixed starting loot and
  auto-equip. Reached name/stat/requirements, gear, Skin and vital effects remain
  mandatory callbacks; source partial prefixes survive callback failure.
- `CanonicalCharacterCandidateRecordV60` owns the source graph: `properties`,
  `inventory37c`, `save`, `load`, `profile_bootstrap`, `player_script_owner_v62`,
  equipment and native initialization state. `init_complete` must come from
  actual initialization, not be set by a frontend adapter. Factory services
  include source World/design/RNG/skills/faeries/VM/physical/profile/effects.
- `CharacterPlayerSkillsV6` supplies the actual retained skill/VM graph and
  `native_saved_owner()`. Ready state must come from source initialize/load
  choreography; creating `PlayerSavegameV1` or copying saved levels is insufficient.
- `CharacterProfileBootstrapV59` loads the selected native file into the SAME
  already-created Save/Gear/PlayerInfo. `prepare` is a staged adoption, explicitly
  not a complete PlayerManager/GameState initialization receipt.
- `EquipmentAdapter` updates SAME shared CharacterState/ActorState with source
  gear/stat/requirements rules. Its combat-owner path requires genuine combat
  publication. This adapter does not deliver full native profile initialization.

Representation must be explicit before building a complete service. Actual
`character_properties_pystructnames.bin` has Stat_Strength149, Stat_Dexterity150,
Stat_Endurance151 and Stat_Energy152. There is no Intelligence property; the
shared `CharacterStats::intelligence` has no proved source mapping. Source saved
skills include rank-zero rows and two slot maps, while shared SkillProgress
requires positive rank and has no slot-map fields. Native difficulty, faeries,
quest states and level states cannot be guessed as shared unlock strings. A
complete canonical campaign must retain these native owners alongside a defined
shared projection. Source pointer addresses are not persistent item identities.

No actual completed Windows `CanonicalCharacterCandidateFactoryV60` graph with
all required services has been found. The Windows main diagnostic still starts
from `make_default_character` and projects a subset of actor fields; its fixtures
cannot fulfill CompleteCreationService. The Android source initialization graph
is a useful owner reference, not permission to reconstruct missing Windows
services with successful stubs.

Failure policy: do not persist or assign a profile after missing initialization,
native effect, unresolved identity, skill/unlock mapping or shared-model
validation. Construct native candidates detached from live/user profile state;
abandon failed stages without publishing. Existing source prefix-retention rules
remain inside that detached owner, so retries must use its real source policy.
