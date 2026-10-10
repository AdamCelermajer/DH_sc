#pragma once
#include "../../game_save.hpp"
#include "../../../level-world/player_save_collections_writer_v45.hpp"
#include "../../../level-world/object_save_restore_v3.hpp"
#include "../../../game-data/quest_persistence_v51.hpp"
#include <functional>

namespace dh::foundation::campaign_save {

enum class SourceCodec : std::uint32_t {
    quest_collection_v45=1, object_base_v3=2,
    // Reserved requirements, deliberately rejected until complete source writers
    // and staged reconstruction producers exist. Private runtime FSMs are not wire.
    actor_lifecycle_v3=3, trigger_zone_v22=4, script_context=5, game_events=6,
    player_profile_source_v1=7 // full native Save/skills/faeries/location: unavailable
};
struct SourceRequirement {
    std::string owner_key; // authored scope/name or persistent character identity
    std::string definition_revision; // host's verified immutable source manifest digest
    SourceCodec codec=SourceCodec::quest_collection_v45;
};
struct SourceSection { SourceRequirement requirement;std::vector<std::uint8_t> bytes; };
struct CampaignSnapshot {
    std::uint32_t extension_version=2;
    GameSave canonical; // ONE existing canonical character/inventory/actor authority
    std::vector<SourceSection> sections;
};
struct SourceCaptureServices {
    // Must enumerate EVERY registered source owner and every required fragment.
    // A Trigger's ObjectBase fragment alone is not its complete lifecycle state.
    std::function<bool(std::vector<SourceRequirement>&,std::string&)> enumerate;
    std::function<bool(const SourceRequirement&,SourceSection&,std::string&)> capture;
    // QEST validation needs actual immutable quest tables. Must validate exact
    // record count/definitions and complete consumption on detached source owners.
    std::function<bool(const SourceSection&,std::string&)> validate;
};

bool capture_campaign_snapshot(const GameSave&,const SourceCaptureServices&,
                               CampaignSnapshot&,std::string& error);
bool validate_campaign_snapshot(const CampaignSnapshot&,const std::vector<SourceRequirement>& expected,
                                const SourceCaptureServices&,std::string& error);

class PreparedCampaignRestore {
public:
    virtual ~PreparedCampaignRestore()=default;
    // Factory must stage canonical world/RNG/character/inventory AND every source
    // owner together. Publication is one noexcept owner swap, never a sequence of
    // live loaders or effect callbacks. Destroying without commit abandons stage.
    virtual void commit() noexcept=0;
};
struct SourceRestoreServices {
    SourceCaptureServices source;
    std::function<bool(const CampaignSnapshot&,std::unique_ptr<PreparedCampaignRestore>&,std::string&)> prepare;
};
bool prepare_campaign_restore(const CampaignSnapshot&,const SourceRestoreServices&,
                              std::unique_ptr<PreparedCampaignRestore>&,std::string& error);

// Uses SAME source QuestSavegame writer. No native receiver identities appear
// in its QEST bytes; saved indices retain source difficulty/index semantics.
bool capture_quest_collection(SourceRequirement,const dh2::level::QuestSaveCollectionBorrowV45&,
                              SourceSection&,std::string& error);
struct StagedQuestCollection {
    dh2::data::QuestSavegameV1 collection;
    dh2::data::QuestPersistenceOwnerV51 owner;
};
// Detached persistent cells only. Native objective registration, condition lists,
// character publication and regular/volatile synchronization remain a required
// higher-level restore factory; this helper never publishes to a live profile.
bool stage_quest_collection(const SourceSection&,std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>,
                            std::uintptr_t borrowed_actual_character,
                            std::shared_ptr<StagedQuestCollection>&,std::string& error);
bool capture_object_base(SourceRequirement,const dh2::world::ObjectSaveRestoreBorrowV3&,
                         SourceSection&,std::string& error);

} // namespace dh::foundation::campaign_save
