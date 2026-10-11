#pragma once
#include "procedural_lists_v1.hpp"
#include <array>
#include <functional>
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
    // Original minimum, maximum and current fields; readers initialize minimum,
    // and production ComputeSizeOfRules advances current before Generate.
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
    bool room_pools_entered_v68{},room_pools_allocated_v68{}; // original ComputeSizeOfRules delivered once
};
// Primitive import only: the actual application libc lrand48 stream, not
// RandomGenerator+0, not gameplay Random channels, and never seeded per level.
// The caller owns any platform/global synchronization; all calls advance that
// SAME shared stream immediately. No private substitute RNG is allocated here.
struct ProceduralPoolRuntimeV68 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uint32_t&,std::string&)> lrand48;
 // Original LoadRuleFile assertion(false,rndApp.cpp,166). Called only when
 // ComputeSizeOfRules returns false; genuine source diagnostic/continue policy.
 std::function<bool(const char*,const char*,const char*,std::uint32_t,std::string&)> failed_pool_assertion;
};
// Mutates actual reader-owned current sizes in source declaration order.
// Failure preserves reached allocation/RNG prefix; it is never rollback.
bool allocate_procedural_room_pools_v68(ProceduralRulePlanV1&,const ProceduralPoolRuntimeV68&,std::string&);
bool interpret_procedural_rules_v1(const ProceduralListPlanV1&,
                                  const std::vector<std::string>& block_names,
                                  ProceduralRulePlanV1&,std::string& error);
class ProceduralRulesV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
    bool generation_load_entered_v68_{};
public:
    class Borrow {
        friend class ProceduralRulesV1;
        std::shared_ptr<const Snapshot> snapshot_;
    bool generation_load_entered_v68_{};
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
    bool prepare_for_generation_v68(ProceduralListsV1::Borrow,const ProceduralPoolRuntimeV68&,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}


