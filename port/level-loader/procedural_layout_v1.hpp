#pragma once
#include "procedural_instances_v1.hpp"
namespace dh2::loader {
// Actual loader-owned generated Tile/list/Array2d body, retained by the SAME
// RandomGenerator14c through Save and D0. These methods execute loader bodies;
// they are not engine callbacks or a second World/parser/cache.
class ProceduralLayoutOwnerV70 {
public:
 virtual bool unload_tiles(std::string&)=0;
 virtual void unload_rule_sources()noexcept=0;
 virtual void destroy_grid()noexcept=0;
 virtual bool live()const noexcept=0;
 virtual std::uintptr_t root114_identity()const noexcept=0;
 virtual ~ProceduralLayoutOwnerV70()=default;
};
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
// Executes original generation over retained reader
// inputs. Lists mutate only in the private generation working set. No-layout
// is a completed original operation with generated=false; checked-domain/API
// failures preserve out. Raw source references are retained by the facade call.
// Production route: reset/deliver original Generate into the SAME generator
// state cell; a checked failure preserves its reached RNG prefix. Pools must
// already have been allocated by original LoadRuleFile before this seed reset.
// Creates original Array2d C1 before LoadRuleFile/config property allocations.
// actual_body is produced only here and reused unchanged by Generate below.
bool create_procedural_layout_body_v70(std::shared_ptr<ProceduralLayoutOwnerV70>&,std::string&);
bool generate_procedural_layout_v70(ProceduralRulesV1::Borrow,std::uint32_t seed,
    std::uint32_t& actual_generator_state,std::shared_ptr<ProceduralLayoutOwnerV70>& actual_body,
    ProceduralLayoutResultV1&,std::string&);
bool generate_procedural_layout_v68(ProceduralRulesV1::Borrow,std::uint32_t seed,
    std::uint32_t& actual_generator_state,ProceduralLayoutResultV1&,std::string&);
bool generate_procedural_layout_v1(const ProceduralRulePlanV1&,
    const std::vector<std::string>& block_names,const std::vector<ProceduralBlockV1>&,
    const ProceduralConnectionGraphV1&,std::uint32_t seed,
    ProceduralLayoutResultV1& out,std::string& error);
bool generate_procedural_layout_v1(ProceduralRulesV1::Borrow,std::uint32_t seed,
    ProceduralLayoutResultV1& out,std::string& error);
}



