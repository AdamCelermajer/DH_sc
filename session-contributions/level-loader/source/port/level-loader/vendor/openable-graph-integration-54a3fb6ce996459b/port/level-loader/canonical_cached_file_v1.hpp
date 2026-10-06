#pragma once
#include "cached_level_file_v1.hpp"
#include "canonical_source_adapter_v1.hpp"
#include <functional>

namespace dh2::loader {
struct CanonicalFileSourceServicesV1 {
    // Original source parser/report and release continuations, not class init.
    std::function<bool(bool,std::string&)> parse_result;
    std::function<bool(std::string&)> release_load_state;
};
// One source-file occurrence in one canonical candidate. It owns XML and a
// journal of exact factory attempts, never actor/scene/save implementations.
class CanonicalCachedFileV1 final : private LevelFileWalkServicesV1 {
    CachedLevelFileV1 file_;
    world::CanonicalObjectManagerV1& manager_;
    world::CanonicalClassServicesV1 classes_;
    CanonicalFileSourceServicesV1 source_;
    CanonicalModuleContextV1 module_;
    ObjectEntryRouteV1 route_;
    std::optional<std::string> filter_;
    std::vector<std::unique_ptr<CanonicalBoundSourceAttemptV1>> attempts_;
    std::string uri_,root_,error_;
    bool started_{},completed_{},failed_{},discard_requested_{},owner_released_{},discarded_{},dispatching_{},discarding_{};
    bool parse_result(bool,std::string&) override;
    bool load_element(const XmlDocumentV1::Borrow&,std::uint32_t,std::string&) override;
    bool release_load_state(std::string&) override;
public:
    // manager/classes/source continuations belong to module.candidate_owner
    // and outlive this file. That context must not own this file, avoiding cycles.
    CanonicalCachedFileV1(assets::ZipAssetPackV1,
        world::CanonicalObjectManagerV1&,world::CanonicalClassServicesV1,
        CanonicalFileSourceServicesV1,CanonicalModuleContextV1,
        ObjectEntryRouteV1,std::optional<std::string> filter={});
    // Sequential access: source callbacks must not reenter this occurrence.
    // A recursive step latches failure while retaining the reached prefix.
    LevelFileWalkStepV1 step(const std::string& uri,const std::string& root);
    // Owner explicitly releases the failed candidate's objects/visuals first.
    // Cleanup failure preserves diagnostics and supports an explicit retry.
    // This does not implement ObjectManager Flush, renderer cleanup or rollback.
    bool discard_after_owner_release(const std::function<bool(std::string&)>&,
                                    std::string&);
    const auto& attempts()const noexcept{return attempts_;}
    const CachedLevelFileV1& file()const noexcept{return file_;}
    const std::string& error()const noexcept{return error_;}
    bool completed()const noexcept{return completed_;} // source prefix only
    bool discarded()const noexcept{return discarded_;}
};
}
