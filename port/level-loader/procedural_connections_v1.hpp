#pragma once
#include "procedural_blocks_v1.hpp"
namespace dh2::loader {
struct ProceduralExitReferenceV1 {
    std::uint32_t block{},exit{}; // Indices in this connection graph, not save IDs.
};
struct ProceduralConnectionGraphV1 {
    // One original insert_unique result per candidate source occurrence.
    std::vector<bool> inserted;
    // Sorted unique-name map order; duplicate names retain the first occurrence.
    std::vector<std::uint32_t> selected_sources;
    // [selected block][exit][connection], preserving matching-type duplicates.
    std::vector<std::vector<std::vector<ProceduralExitReferenceV1>>> connections;
};
bool connect_procedural_blocks_v1(const std::vector<std::string>& names,
                                 const std::vector<ProceduralBlockV1>&,
                                 ProceduralConnectionGraphV1&,std::string& error);
class ProceduralConnectionsV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class ProceduralConnectionsV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const ProceduralBlocksV1::Borrow& blocks()const;
        const ProceduralConnectionGraphV1& graph()const;
    };
    // Internal connection graph only; layout/rule execution remains separate.
    bool prepare(ProceduralBlocksV1::Borrow,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
