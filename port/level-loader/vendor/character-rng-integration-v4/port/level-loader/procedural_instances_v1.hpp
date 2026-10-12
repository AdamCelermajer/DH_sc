#pragma once
#include "procedural_rules_v1.hpp"
#include "procedural_random_v1.hpp"
namespace dh2::loader {
// Initial state of one original runtime rule constructor. Lazy graph
// registration, tile placement, traversal and rollback are separate operations.
struct ProceduralRuleInstanceV1 {
    std::shared_ptr<const ProceduralRulePlanV1> source;
    ProceduralRulesV1::Borrow source_owner; // Full ZIP/MGX graph and visit identity when prepared from a facade.
    std::vector<std::uint32_t> source_path;
    ProceduralRuleKindV1 kind{ProceduralRuleKindV1::root};
    bool parent_present{};
    std::int32_t length{1};
    std::int32_t exit_direction{-1}; // No exit restriction if -1.
    std::vector<std::string> block_names;
    std::uint32_t child_count{},progress{};
    bool tile_present{};
    std::uint32_t path_direction{4};
};
// The plan owns raw source/list/rule state. A constructor consumes random state
// only where the original constructor does. Failure publishes neither state.
// Pool current sizes are inputs here; this does not allocate room-pool sizes.
bool initialize_procedural_rule_instance_v1(
    std::shared_ptr<const ProceduralRulePlanV1>,const std::vector<std::uint32_t>&,
    bool parent_present,ProceduralRandomV1&,ProceduralRuleInstanceV1&,std::string& error);
bool initialize_procedural_rule_instance_v1(
    ProceduralRulesV1::Borrow,const std::vector<std::uint32_t>&,
    bool parent_present,ProceduralRandomV1&,ProceduralRuleInstanceV1&,std::string& error);
}
