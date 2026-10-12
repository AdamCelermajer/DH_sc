#pragma once
#include "fixed_map_v1.hpp"
#include <optional>
namespace dh2::loader {
// Internal source occurrence index, not an agreed runtime factory/save ABI.
// A source document may be referenced by several distinct module instances.
enum class DeclarationOriginV1 {level,gameplay,visual};
struct ObjectDeclarationV1 {
    std::uint32_t document{},element{},module{no_source_v1},source_order{};
    DeclarationOriginV1 origin{DeclarationOriginV1::level};
    std::array<float,3> module_offset{};
    std::optional<std::array<float,3>> authored_position,rotation_degrees,scale;
    std::optional<std::array<float,3>> translated_position;
    // These values do not determine activation or substitute for native IDs.
    // Name, gametype, template/_templateName, auroraID, conditions, scripts and
    // all other attributes/nested elements remain in the retained source tree.
};
class FixedDeclarationsV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class FixedDeclarationsV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const FixedMapV1::Borrow& map()const;
        const std::vector<ObjectDeclarationV1>& declarations()const;
        const XmlDocumentV1::Borrow& document(const ObjectDeclarationV1&)const;
        const XmlElementV1& element(const ObjectDeclarationV1&)const;
    };
    // Retains every matching root child, including declarations that cannot
    // dispatch to a registered gameplay type. Missing/empty values stay distinct.
    // No factories, template defaults, condition decisions or event execution.
    bool prepare(FixedMapV1::Borrow,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
