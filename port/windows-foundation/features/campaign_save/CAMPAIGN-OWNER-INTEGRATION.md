# Actual source owners and root publication

`SourceOwnerCapture` pins the caller's SAME CharacterMenuQuestsV51, Save and
Character lease. Register regular and volatile collections together with stable
source keys and the actual immutable definition revision. It obtains fresh
`owner->writer(0/1)` views at capture time; it does not copy a live quest owner.
QEST validation uses the existing detached persistence owner, exact table counts
and complete byte consumption. This proves a complete QEST codec, not a complete
Character, condition/objective dispatcher or campaign restore.

Root must enumerate all additional required owners using `require_unsupported`
until their complete source serializers and reconstruction factories exist.
Lifecycle, trigger/script/event context and full native player-profile codecs
remain rejected. Never register an ObjectBase or QEST subset as an entire actor.
Expired capture inventories and duplicate SAME quest owners reject explicitly.

## Root graph publication request

1. Retain one actual Character/native record, PlayerSavegameV1, property sheets,
   inventory37c, CharacterPlayerSkillsV6 and quest/condition/objective/script
   owners. Canonical CharacterState/GameSave is a projection/snapshot of this
   graph, not a second independently initialized player.
2. Capture the existing canonical GameSave exactly once, enumerate every source
   fragment, capture from its actual owner and validate the entire candidate.
   Reserved required codecs fail before any save-file mutation.
3. Restore into one detached whole graph. Construct quest collections on the
   staged SAME Save, register actual native objective/condition dispatch against
   that staged graph and run source compile/reinitialization there. The source
   `character_quests(character)` provider must resolve the staged owner, never
   the old live owner during staging.
4. Only after all complete-owner preparation succeeds may the root-owned
   `PreparedCampaignRestore::commit() noexcept` swap the graph once. Rebind
   actor/visual/PF/controller/camera/UI providers after publication. Do not call
   native loaders sequentially against live actors.

## Authentic save-slot dependency

`fresh_player_profile_v1` writes seven original metadata sections. It is not a
ready Character/inventory/skill/profile initialization backend. The available
CanonicalCharacterCandidateRecordV60 has real properties, inventory37c and
CharacterPlayerSkillsV6 owners, but a complete Windows factory/provider graph
has not been supplied. Its world/design/faery/RNG/physical/VM/gear/profile and
InitPost/InitFinal/skill-slot services must finish before production creation.

Frontend `create_save -> assign_save -> StartGame assign -> start_game` must use
that SAME authentic indexed owner and original SG_Load/SG_Save route. Root must
supply slot allocation/file delivery, source name/class/level1/campaign row41,
fresh loot/autoequip, grants/skills/faery/quest initialization, native Save and
source graph publication. Existing `make_default_character` is not this service.

Source saved skills include rank-zero rows, two slot maps and faery/difficulty/
quest cells that CharacterState does not represent. Source stats149..152 are
Strength/Dexterity/Endurance/Energy; no proved Intelligence field exists. An
explicit projection contract must retain these native facts without relabeling
or dropping them and then claiming complete creation. The new native profile
codec remains unavailable. No second save-slot graph, transient FSM wire,
filesystem format migration or partial live restore is implemented here.

The component test uses actual64-row quest tables and genuine Save/quest
collection owners, with an explicitly fixture Character identity/lease. It
verifies both QEST selectors, revision/count/full-consumption guards, duplicate
owner rejection, unsupported-fragment output preservation and expiry. It is not
an initialized gameplay Character or accepted live campaign save.
