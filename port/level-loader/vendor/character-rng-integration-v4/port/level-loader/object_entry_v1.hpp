#pragma once
#include "xml_document_v1.hpp"
#include <array>
#include <optional>
namespace dh2::loader {
struct OriginalFactoryV1 {
    const char* gametype;
    std::uint32_t original_address; // Original ARM provenance, not a native function pointer.
};
const std::array<OriginalFactoryV1,33>& original_factories_v1();
const OriginalFactoryV1* original_factory_v1(const std::string& gametype);
enum class ObjectEntryRouteV1 { level, manager };
enum class ObjectEntryDispositionV1 {
    registered_requires_services, original_missing_attribute, original_type_filter,
    original_player_exclusion, original_unregistered_type, unsafe_level_missing_type
};
struct ObjectEntryV1 {
    XmlDocumentV1::Borrow document;
    std::uint32_t element{};
    ObjectEntryDispositionV1 disposition{ObjectEntryDispositionV1::original_missing_attribute};
    const OriginalFactoryV1* factory{};
    bool template_present{}, early_init_post{}, force_id_minus_one{};
    const XmlElementV1& source()const;
};
const char* object_entry_disposition_v1(ObjectEntryDispositionV1);
// Source-backed entry gates only. Preserves all attributes/nested data and
// missing-versus-empty strings; does not instantiate objects or select a shared
// factory/save ABI. A registered entry requires real owner services. Unknown
// types remain explicit records even though original lookup returns null.
// Level's null gametype strcmp has no reconstructed safe libc policy; that
// case is reported as unsafe rather than fabricating an original skip.
bool prepare_object_entry_v1(XmlDocumentV1::Borrow, std::uint32_t element,
    ObjectEntryRouteV1, const std::optional<std::string>& type_filter,
    ObjectEntryV1& out, std::string& error);
}
