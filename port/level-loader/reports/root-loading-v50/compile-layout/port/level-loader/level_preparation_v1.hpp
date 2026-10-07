#pragma once
#include "fixed_declarations_v1.hpp"
#include "procedural_map_sources_v1.hpp"
namespace dh2::loader {
// Loader-owned source transaction. None of these states means gameplay ready.
enum class LevelSourceKindV1 {fixed,procedural};
struct LevelSourceRequestV1 {
    std::string identity,definition;
    LevelSourceKindV1 kind{LevelSourceKindV1::fixed};
    std::uint32_t seed{};
    bool repair_known_references{true};
    bool allow_original_backup{true};
};
struct LevelSourceResolutionV1 {
    std::string definition;
    bool original_no_layout{},backup_used{};
};
enum class LevelPreparationStageV1 {
    idle,procedural_sources,blocks,connections,lists,rules,layout,modules,
    backup_sources,sources,map,declarations,publish_source,source_ready,failed,cancelled
};
enum class LevelPreparationStepV1 {pending,source_ready,failed,cancelled};
enum class LevelPreparationFailureV1 {none,preparation,original_no_layout,original_backup_unavailable};
class LevelPreparationV1 {
    struct Snapshot;
    struct Candidate;
    assets::ZipAssetPackV1 archive_;
    std::unique_ptr<Candidate> candidate_;
    std::shared_ptr<const Snapshot> latest_;
    LevelPreparationStageV1 stage_{LevelPreparationStageV1::idle};
    LevelPreparationFailureV1 failure_{LevelPreparationFailureV1::none};
    std::string error_;
    std::uint32_t completed_stages_{};
    LevelPreparationStepV1 fail(LevelPreparationFailureV1,const std::string&);
public:
    class Borrow {
        friend class LevelPreparationV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const LevelSourceRequestV1& request()const;
        const LevelSourceResolutionV1& resolution()const;
        const ProceduralLayoutResultV1& attempted_layout()const;
        const FixedMapV1::Borrow& map()const;
        const FixedDeclarationsV1::Borrow& declarations()const;
        const std::shared_ptr<const ProceduralModulePlanV1>& procedural_modules()const;
    };
    explicit LevelPreparationV1(assets::ZipAssetPackV1);
    ~LevelPreparationV1();
    LevelPreparationV1(const LevelPreparationV1&)=delete;
    LevelPreparationV1& operator=(const LevelPreparationV1&)=delete;
    // Invalid requests or attempts to replace a pending/failed candidate leave
    // its state and latest source output intact. Failed candidates need discard.
    bool begin(LevelSourceRequestV1,std::string& error);
    // Runs one preparation stage synchronously. This is native stage scheduling,
    // not recovered original async IO or a time-bounded background operation.
    // Last completed SOURCE output changes only at publish_source; engine world
    // publication, factories, conditions/events and restoration remain external.
    LevelPreparationStepV1 step();
    // Releases this candidate's source/map working set only. Runtime handles do
    // not exist here. The previous completed source output and all borrows live.
    void discard() noexcept;
    Borrow latest_source()const{return Borrow(latest_);}
    LevelPreparationStageV1 stage()const noexcept{return stage_;}
    LevelPreparationFailureV1 failure()const noexcept{return failure_;}
    const std::string& error()const noexcept{return error_;}
    std::uint32_t completed_stages()const noexcept{return completed_stages_;}
};
// Retained floor query scratch remains sequential as in FixedMapV1. A caller
// must not drive this facade or shared ZIP backing concurrently/reentrantly.
const char* level_preparation_stage_v1(LevelPreparationStageV1);
}
