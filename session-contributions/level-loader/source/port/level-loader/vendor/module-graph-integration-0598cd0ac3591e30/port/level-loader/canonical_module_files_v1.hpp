#pragma once
#include "canonical_cached_file_v1.hpp"
#include "canonical_level_context_v1.hpp"
#include "module_xml_selection_v1.hpp"
namespace dh2::loader {
// Retained source relay only. Main owns actual Module selection/InitPost,
// canonical class construction, candidate release and runtime publication.
class CanonicalModuleFilesV1 final : public std::enable_shared_from_this<CanonicalModuleFilesV1> {
    assets::ZipAssetPackV1 archive_;
    world::CanonicalObjectManagerV1& manager_;
    world::CanonicalClassServicesV1 classes_;
    CanonicalFileSourceServicesV1 source_;
    std::shared_ptr<CanonicalLevelContextV1> level_;
    std::shared_ptr<void> candidate_owner_;
    ObjectEntryRouteV1 route_;
    std::optional<std::string> filter_;
    std::vector<std::unique_ptr<CanonicalCachedFileV1>> files_;
    CanonicalModuleContextV1 captured_;
    std::string active_uri_,active_root_,error_;
    std::uint32_t occurrence_{UINT32_MAX};
    std::size_t discard_cursor_{};
    bool module_started_{},captured_context_{},file_pending_{},dispatching_{};
    bool failed_{},discard_requested_{},owner_released_{},discarded_{},discarding_{};
    CanonicalModuleFilesV1(assets::ZipAssetPackV1,world::CanonicalObjectManagerV1&,
        world::CanonicalClassServicesV1,CanonicalFileSourceServicesV1,
        std::shared_ptr<CanonicalLevelContextV1>,std::shared_ptr<void>,ObjectEntryRouteV1,std::optional<std::string>);
    bool fail(const std::string&,std::string&);
public:
    static bool create(assets::ZipAssetPackV1,world::CanonicalObjectManagerV1&,
        world::CanonicalClassServicesV1,CanonicalFileSourceServicesV1,
        std::shared_ptr<CanonicalLevelContextV1>,std::shared_ptr<void> candidate_owner,
        std::shared_ptr<CanonicalModuleFilesV1>& out,std::string& error,
        ObjectEntryRouteV1 route=ObjectEntryRouteV1::level,std::optional<std::string> filter={});
    // Call before the SAME actual Module::Load. Occurrence is diagnostic only;
    // real Module::Load subsequently writes Level18c/160 before load_file.
    bool begin_module(std::uint32_t occurrence,std::string&);
    // Owns this relay and therefore its same-Level field lifetime. The callback
    // uses a weak relay capture, preventing a self-owned callback cycle.
    world::ModuleLevelLoadBorrowV1 load_borrow();
    // delivered=true, loaded=false means pending. Completion ends THIS file
    // occurrence; the next call starts another, even when URI/root are equal.
    // Failure preserves journals and prevents replay. Access is sequential;
    // reentrant/interleaved delivery is rejected rather than changing context.
    bool load_file(const std::string&,const char*,bool& loaded,std::string&);
    // Real canonical candidate release first, then retained XML/source journals.
    // A failed cleanup retries only its unfinished tail. No module field reset,
    // manager Flush, rollback, commit or renderer release is fabricated here.
    bool discard_after_owner_release(const std::function<bool(std::string&)>&,std::string&);
    std::size_t file_count()const noexcept{return files_.size();}
    const CanonicalCachedFileV1& file(std::size_t i)const{return *files_.at(i);}
    const std::string& error()const noexcept{return error_;}
    bool discarded()const noexcept{return discarded_;}
};
}
