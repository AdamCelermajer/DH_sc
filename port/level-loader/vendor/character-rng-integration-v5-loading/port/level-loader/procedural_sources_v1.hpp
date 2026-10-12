#pragma once
#include "xml_document_v1.hpp"
#include "../asset-payloads/zip_asset_pack_v1.hpp"
#include <memory>
namespace dh2::loader {
struct ProceduralBlockSourceV1 {
    std::string filename,name;
    std::uint32_t document{},root{};
};
// Original rule file and file-listed MGX sources only; not a generated layout,
// selected MGP/MVP graph, typed rules/exits or shared runtime ABI.
class ProceduralSourcesV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class ProceduralSourcesV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const std::string& identity()const;
        const std::string& folder()const;
        const std::string& file_list_uri()const;
        const std::vector<std::uint8_t>& file_list_bytes()const;
        const std::vector<std::string>& filenames()const;
        const std::vector<XmlDocumentV1::Borrow>& documents()const;
        const std::vector<ProceduralBlockSourceV1>& blocks()const;
        std::uint32_t rule_root()const;
    };
    // Original LoadRuleFile and MgxBlock::LoadFromXmlStream do not branch on
    // LoadFromBuffer's return. Retain its diagnostic and select the first
    // matching root. No parser error is converted into a clean parse claim.
    // Missing dependencies fail explicitly; original silent block omission is
    // not used to make a falsely complete graph.
    bool prepare(const assets::ZipAssetPackV1&,std::string identity,
                 const std::string& definition,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
