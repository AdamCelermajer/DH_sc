#include "object_entry_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
const std::array<OriginalFactoryV1,33>& original_factories_v1() {
    static const std::array<OriginalFactoryV1,33> entries{{
#include "original_factory_table_v1.inc"
    }};
    return entries;
}
const OriginalFactoryV1* original_factory_v1(const std::string& gametype) {
    for (const auto& entry:original_factories_v1()) if (gametype==entry.gametype) return &entry;
    return nullptr;
}
const XmlElementV1& ObjectEntryV1::source()const { return document.elements().at(element); }
const char* object_entry_disposition_v1(ObjectEntryDispositionV1 disposition) {
    switch (disposition) {
    case ObjectEntryDispositionV1::registered_requires_services:return "registered_requires_services";
    case ObjectEntryDispositionV1::original_missing_attribute:return "original_missing_attribute";
    case ObjectEntryDispositionV1::original_type_filter:return "original_type_filter";
    case ObjectEntryDispositionV1::original_player_exclusion:return "original_player_exclusion";
    case ObjectEntryDispositionV1::original_unregistered_type:return "original_unregistered_type";
    case ObjectEntryDispositionV1::unsafe_level_missing_type:return "unsafe_level_missing_type";
    }
    throw std::logic_error("Invalid object entry disposition");
}
bool prepare_object_entry_v1(XmlDocumentV1::Borrow document, std::uint32_t element,
    ObjectEntryRouteV1 route,const std::optional<std::string>& filter,
    ObjectEntryV1& out,std::string& error) {
    error.clear();
    if (!document || !document.parsed() || element>=document.elements().size()) {
        error="Object entry requires a parsed retained element";return false;
    }
    if (route==ObjectEntryRouteV1::level && filter) {
        error="Level::_LoadFromXML does not pass a type filter";return false;
    }
    ObjectEntryV1 candidate;candidate.document=std::move(document);candidate.element=element;
    const auto& source=candidate.source();
    const auto* type=source.attribute("gametype");const auto* name=source.attribute("name");
    if (route==ObjectEntryRouteV1::level && !type)
        candidate.disposition=ObjectEntryDispositionV1::unsafe_level_missing_type;
    else if (route==ObjectEntryRouteV1::level && *type=="Player")
        candidate.disposition=ObjectEntryDispositionV1::original_player_exclusion;
    else if (!type || !name)
        candidate.disposition=ObjectEntryDispositionV1::original_missing_attribute;
    else if (filter && *filter!=*type)
        candidate.disposition=ObjectEntryDispositionV1::original_type_filter;
    else {
        candidate.factory=original_factory_v1(*type);
        if (!candidate.factory) candidate.disposition=ObjectEntryDispositionV1::original_unregistered_type;
        else {
            candidate.disposition=ObjectEntryDispositionV1::registered_requires_services;
            candidate.template_present=source.attribute("template")!=nullptr;
            candidate.early_init_post=*type=="LevelConfig";
            candidate.force_id_minus_one=*name=="_prim_PlayerLight";
        }
    }
    out=std::move(candidate);return true;
}
}
