#pragma once
#include "procedural_lists_v1.hpp"
#include <array>
namespace dh2::loader {
enum class ProceduralRuleKindV1 { root,path,force_block,end_path };
struct ProceduralRuleV1 {
    ProceduralRuleKindV1 kind{ProceduralRuleKindV1::root};
    std::uint32_t element{UINT32_MAX};
    bool read_result{}; // Original reader result; not this preparation API's result.
    std::string exit,list_name;
    std::uint32_t id_hash{};
    std::array<std::int32_t,2> length{1,1};
    std::int32_t list_source{-1},list_index{-1};
    bool list_reference{},list_validation{};
    std::vector<std::string> block_names;
    std::vector<bool> block_validation;
    std::vector<ProceduralRuleV1> children;
    bool dont_go_back{true};
    std::uint32_t connect_from{4};
};
struct ProceduralPoolElementV1 {
    std::uint32_t element{},id_hash{};
    bool recursive{};
    // Original minimum, maximum and current allocation fields. No allocation runs.
    std::array<std::int32_t,3> sizes{-1,-1,-1};
};
struct ProceduralRoomPoolV1 {
    std::uint32_t element{};
    std::int32_t size{};
    std::vector<ProceduralPoolElementV1> elements;
};
struct ProceduralRulePlanV1 {
    ProceduralListPlanV1 lists; // Own referenced list declarations and raw XML.
    bool root_present{};
    ProceduralRuleV1 rule;
    std::vector<ProceduralRoomPoolV1> pools;
};
bool interpret_procedural_rules_v1(const ProceduralListPlanV1&,
                                  const std::vector<std::string>& block_names,
                                  ProceduralRulePlanV1&,std::string& error);
class ProceduralRulesV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class ProceduralRulesV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const ProceduralListsV1::Borrow& lists()const;
        const ProceduralRulePlanV1& plan()const;
    };
    // Typed reader state only. Caller failure policy, pool allocations,
    // randomized selection and generated layouts remain separate.
    bool prepare(ProceduralListsV1::Borrow,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
