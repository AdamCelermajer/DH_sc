#pragma once
#include "procedural_sources_v1.hpp"
#include <array>
#include <limits>
namespace dh2::loader {
enum class ProceduralLinkResultV1 { accepted, missing_direction, zero_or_empty_tail };
struct ProceduralLinkDeclarationV1 {
    std::uint32_t element{};
    ProceduralLinkResultV1 result{};
    std::uint32_t exit_index{std::numeric_limits<std::uint32_t>::max()};
    // Includes original comma splitting and stale suffix after space compaction.
    std::vector<std::string> link_types;
};
struct ProceduralExitV1 {
    std::uint32_t element{},index{},direction{}; // north/east/south/west/none = 0..4
    std::array<std::int32_t,2> grid{};
    float height{};
    std::vector<std::string> link_types;
};
struct ProceduralBlockV1 {
    XmlDocumentV1::Borrow document;
    std::uint32_t root{};
    float unit_width{3000.0f},unit_height{3000.0f};
    std::int32_t width{1},height{1};
    std::vector<ProceduralLinkDeclarationV1> link_declarations;
    std::vector<ProceduralExitV1> exits;
};
// Original MGX projection only; not generated rooms or linked connection graph.
// Retains all raw declarations; records original rejection of links explicitly.
// Caller output is unchanged on checked-domain failures.
bool interpret_procedural_block_v1(XmlDocumentV1::Borrow,std::uint32_t root,
                                  ProceduralBlockV1&,std::string& error);
class ProceduralBlocksV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class ProceduralBlocksV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const ProceduralSourcesV1::Borrow& sources()const;
        const std::vector<ProceduralBlockV1>& blocks()const;
    };
    bool prepare(ProceduralSourcesV1::Borrow,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
