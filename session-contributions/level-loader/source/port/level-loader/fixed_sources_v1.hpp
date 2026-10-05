#pragma once
#include "xml_document_v1.hpp"
#include "../asset-payloads/zip_asset_pack_v1.hpp"
#include <limits>
namespace dh2::loader {
constexpr std::uint32_t no_source_v1=std::numeric_limits<std::uint32_t>::max();
// Source dependency graph, not a scene/factory/save ABI. A repeated reference
// shares immutable bytes, while its referring declaration remains distinct.
struct ModuleSourceLinksV1 {
    std::uint32_t document{},element{},gameplay{no_source_v1},visual{no_source_v1};
};
class FixedSourcesV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
    bool prepare_input(const assets::ZipAssetPackV1&,std::string,const std::string&,
                       XmlDocumentV1::Borrow,std::shared_ptr<const void>,std::string&);
public:
    class Borrow {
        friend class FixedSourcesV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const std::string& identity()const;
        const std::vector<XmlDocumentV1::Borrow>& documents()const;
        const std::vector<ModuleSourceLinksV1>& module_links()const;
    };
    // New preparation policy: fail visibly on missing/malformed dependencies;
    // publish only the complete source graph. Uses retained level-buffer file
    // traversal for source discovery. Does not create runtime objects,
    // choose alternate layouts, evaluate conditions or assemble geometry.
    bool prepare(const assets::ZipAssetPackV1&,std::string identity,
                 const std::string& definition,std::string& error);
    // Internal derived-root adapter. Caller supplies an owned inspection root
    // and its source provenance; dependency resolution follows the same path.
    // It does not establish original PropertyMap serialization or a factory ABI.
    bool prepare_document(const assets::ZipAssetPackV1&,std::string identity,
                          XmlDocumentV1::Borrow,std::shared_ptr<const void> source_owner,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
