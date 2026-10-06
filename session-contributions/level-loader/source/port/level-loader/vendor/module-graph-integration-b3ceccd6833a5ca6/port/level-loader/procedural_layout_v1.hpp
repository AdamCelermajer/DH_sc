#pragma once
#include "procedural_instances_v1.hpp"
namespace dh2::loader {
struct ProceduralLayoutEventsV1 {
    std::uint32_t random_calls{},root_attempts{},place_calls{},unspawn_calls{},step_calls{},path_calls{},rule_calls{};
};
struct ProceduralLayoutTileV1 {
    std::uint32_t block{},block_source{}; // Connection-map and original source indices; not save IDs.
    std::string name;
    std::array<std::int32_t,2> grid{};
    float height{};
    ProceduralListElementV1 list_element;
    std::int32_t list_source{-1};
    std::vector<std::uint32_t> children;
};
struct ProceduralLayoutResultV1 {
    ProceduralRulesV1::Borrow source_owner;
    std::uint32_t seed{},final_state{};
    bool generated{}; // Original Generate result; separate from checked API success.
    std::vector<ProceduralLayoutTileV1> tiles;
    ProceduralLayoutEventsV1 events;
};
// Executes the original no-room-pool generation branch over retained reader
// inputs. Lists mutate only in the private generation working set. No-layout
// is a completed original operation with generated=false; checked-domain/API
// failures preserve out. Raw source references are retained by the facade call.
bool generate_procedural_layout_v1(const ProceduralRulePlanV1&,
    const std::vector<std::string>& block_names,const std::vector<ProceduralBlockV1>&,
    const ProceduralConnectionGraphV1&,std::uint32_t seed,
    ProceduralLayoutResultV1& out,std::string& error);
bool generate_procedural_layout_v1(ProceduralRulesV1::Borrow,std::uint32_t seed,
    ProceduralLayoutResultV1& out,std::string& error);
}
