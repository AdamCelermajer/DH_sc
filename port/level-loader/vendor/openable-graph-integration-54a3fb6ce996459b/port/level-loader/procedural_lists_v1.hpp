#pragma once
#include "procedural_connections_v1.hpp"
namespace dh2::loader {
struct ProceduralListElementV1 {
    std::uint32_t element{};
    std::string block_name,gameplay,visual;
    std::int32_t chances{100};
    bool block_available{}; // Explicit validation, not original assertion policy.
};
struct ProceduralListDeclarationV1 {
    std::uint32_t element{};
    std::string name; // Original reader lowercases list names, not block names.
    bool replacement{true};
    std::vector<ProceduralListElementV1> elements;
};
struct ProceduralListPlanV1 {
    XmlDocumentV1::Borrow document;
    std::uint32_t root{};
    // Retain every authored declaration, including duplicate map keys.
    std::vector<ProceduralListDeclarationV1> declarations;
    // Sorted unique-name map order; first lowercase-name occurrence is selected.
    std::vector<std::uint32_t> selected_sources;
};
bool interpret_procedural_lists_v1(XmlDocumentV1::Borrow,std::uint32_t root,
                                  const std::vector<std::string>& block_names,
                                  ProceduralListPlanV1&,std::string& error);
class ProceduralListsV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class ProceduralListsV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const ProceduralConnectionsV1::Borrow& connections()const;
        const ProceduralListPlanV1& plan()const;
    };
    // Typed declarations only. No weighted selection, rules, or layout execution.
    bool prepare(ProceduralConnectionsV1::Borrow,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
