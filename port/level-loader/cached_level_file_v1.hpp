#pragma once
#include "level_file_walk_v1.hpp"
#include "../asset-payloads/zip_asset_pack_v1.hpp"
namespace dh2::loader {
// Synchronous ZIP acquisition + verified ready-buffer traversal. This is a
// private loader adapter, not the original async stream or a shared factory ABI.
class CachedLevelFileV1 {
    assets::ZipAssetPackV1 archive_; // Retains backing after the caller facade dies.
    LevelFileWalkV1 walk_;
    std::vector<std::uint8_t> acquired_;
    std::string requested_,root_,resolved_,error_;
    bool active_{},failed_{},document_input_{};
    std::shared_ptr<const void> assigned_source_owner_;
public:
    explicit CachedLevelFileV1(assets::ZipAssetPackV1 archive):archive_(std::move(archive)){}
    // A pending/failed request owns its source until completion/discard. A
    // completed request releases its own borrow; recipients may retain theirs.
    // Calling again after completion starts a NEW occurrence, even for the same
    // URI/root. A pending request cannot be changed or silently restarted.
    LevelFileWalkStepV1 step(const std::string& uri,const std::string& root,LevelFileWalkServicesV1&);
    LevelFileWalkStepV1 step_document(XmlDocumentV1::Borrow,std::shared_ptr<const void> source_owner,const std::string& root,LevelFileWalkServicesV1&);
    const LevelFileWalkV1& walk()const noexcept{return walk_;}
    bool discard(LevelFileWalkServicesV1&,std::string& error);
    bool active()const noexcept{return active_;}
    bool failed()const noexcept{return failed_;}
    const XmlDocumentV1::Borrow& source()const noexcept{return walk_.document;}
    // Also retains bytes rejected before an XML snapshot could be prepared.
    const std::vector<std::uint8_t>& raw_source()const {
        return walk_.document?walk_.document.source():acquired_;
    }
    const std::string& requested_uri()const noexcept{return requested_;}
    const std::string& resolved_uri()const noexcept{return resolved_;}
    const std::string& error()const noexcept{return error_;}
};
}
