# Campaign save successor handoff

Worker stopped after a verified bounded source capture registry. Ownership is
`port/windows-foundation/features/campaign_save/` and
`port/windows-foundation/reports/campaign-owner-integration.json`. No root core,
main/CMake, packages or verifier edits. Existing audit reports are separate work.

## Files and tested state

New `source_owner_capture.hpp/.cpp` registers the SAME
CharacterMenuQuestsV51/PlayerSavegame and caller-owned Character lease. Registers
regular+volatile keys atomically, rejects duplicate native owner, obtains fresh
writer0/1 views, and validates QEST with detached native persistence owners and
exact counts/full consumption. Services borrow a weak registry: expired services
fail before output mutation. `require_unsupported` retains required missing
fragments; no silent supported-subset campaign save.

Existing `campaign_snapshot.hpp` only changed to reserve unavailable
`player_profile_source_v1=7`. Existing requirement gate rejects this codec.
`CAMPAIGN-OWNER-INTEGRATION.md` documents whole-source graph publication and
indexed save dependency. `source_owner_capture_tests.cpp` and
`run_source_owner_tests.ps1` are reproducible. Final runner PASS:
`rows=64 collections=2 unsupported-rejection=2 expired-lease=1`.
Actual original quest arrays/names are from
`port/level-world/reference/character-menu-profile-v51/cache/`. Character identity
and revision-token labels are fixtures, explicitly not fresh gameplay factory.
New module strict C++17 Wall/Wextra/Werror passed. Combined native source closure
and actual-data execution passed. No process remains running. Artifacts are
`.local-inputs/campaign-source-owner-component/` (plus earlier scratch exe/object).

## Exact root integration graph

Use one existing canonical Character/native candidate/PlayerInfo/property224/
inventory37c/skills/Save/quest/condition/objective/script graph. Registry is a
capture inventory, not a second persistence graph. Feed `services()` to existing
`capture_campaign_snapshot`. Enumerate ALL native owner fragments; lifecycle,
script/trigger/events/full profile remain unsupported and must reject.

Restore needs one detached whole graph. Quest native registration services must
resolve staged SAME quest owner, not old live owner. After every owner is complete,
root `PreparedCampaignRestore::commit() noexcept` swaps once then rebinds source
actor/visual/PF/controller/camera/UI. Never invoke sequential live native loads.
No new filesystem save format or atomic whole-world commit exists here.

## Creation dependencies and peers

`fresh_player_profile_v1` emits seven source metadata tags, not full initialization.
FS_StartGame New indexed Save C1 0x4655ac differs Character.InitializePlayerSavegame
blank C1 0x465ae0. Avoid constructing a second Save. Source level1/campaign row41
known, but complete world/design/faery/RNG/physical/VM/gear/profile/InitPost/
InitFinal/skill-slot providers remain required. Main make_default_character is
not authentic CreateSaveSlot backend. Native rank-zero skill rows, two slot maps,
faeries/difficulty/quest cells and stats149..152 cannot be flattened into shared
CharacterState/intelligence without an explicit projection contract.

Peers:
- /root/integration_lead/source_character_owner_factory: sole existing
  CanonicalCharacterCandidateFactoryV60, alias bundle; constructed prefix not ready.
- /root/quests_feature: new canonical_quest_graph_factory and SourceQuestServiceBinding;
  actual record Save14e8/load/sync, prepare Load1/2 then staged native objective/
  condition registration; full closure not yet tested by that peer.
- /root/frontend_orchestrator/menu_flow: canonical_navigation contract borrows
  SAME CharacterState; create->assign->Main/Start->assign->start; production blocked.
- /root/frontend_orchestrator/creation_data: source fresh224/gear projection available,
  complete native owner missing; no fake creation success.

## Next bounded task

Coordinate actual completed canonical candidate and quest graph APIs, then bind
registry to those actual initialized owners. Add tests proving no duplicate Save,
complete required fragment inventory and detached registration failures leave old
live graph untouched. Do not enable frontend creation or campaign saving from
prefix/component success. Root alone owns final package/release acceptance.

New human policy: successor must be GPT-6 Luna HIGH. Previous worker has stopped
all source edits after writing this handoff; preserve existing files/packages.
