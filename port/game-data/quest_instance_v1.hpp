#pragma once
#include "quest_condition_list_v1.hpp"
#include "quest_objective_list_v1.hpp"
#include "quest_reward_list_v1.hpp"

namespace dh2::data::quest_instance_v1 {
struct Services {
    quest_condition_list_v1::Services conditions;
    quest_objective_list_v1::Services objectives;
    quest_reward_list_v1::Services rewards;
    // Only reached action virtuals, objective marker/unregister operations and
    // the same stream's signed reader reach this mandatory leaf provider.
    quest_runtime_fields_v1::Services leaves;
};

// A real factory backs one stable Instance per Quest allocation. Save vectors
// publish record().ref, and all scalar/list operations use these exact stores.
// The table View keeps its original immutable row/list/stub/name controls alive.
// This adapter connects recovered bodies; type factories and gameplay leaves
// remain explicit dependencies. It creates no Lua VM or second quest authority.
class Instance {
    quest_table_bindings_v1::View definitions_;
    Services services_;
    quest_runtime_fields_v1::Record record_;
    quest_condition_list_v1::List conditions_;
    quest_objective_list_v1::List objectives_;
    quest_reward_list_v1::List rewards_;
    quest_condition_list_v1::Runtime condition_runtime_;
    quest_objective_list_v1::Runtime objective_runtime_;
    quest_reward_list_v1::Runtime reward_runtime_;
    quest_runtime_fields_v1::Runtime runtime_;
    static std::int32_t invoke(void*,const quest_runtime_fields_v1::Request&,
                               quest_runtime_fields_v1::Response*);
    bool output(const quest_runtime_fields_v1::Result*) const noexcept;
public:
    Instance(std::uintptr_t identity,quest_table_bindings_v1::View,Services);
    Instance(const Instance&)=delete;Instance& operator=(const Instance&)=delete;
    quest_runtime_fields_v1::Record& record() noexcept {return record_;}
    const quest_runtime_fields_v1::Record& record() const noexcept {return record_;}
    quest_condition_list_v1::List& conditions() noexcept {return conditions_;}
    quest_objective_list_v1::List& objectives() noexcept {return objectives_;}
    quest_reward_list_v1::List& rewards() noexcept {return rewards_;}
    quest_runtime_fields_v1::Status construct(std::int32_t difficulty,quest_runtime_fields_v1::Result*);
    quest_runtime_fields_v1::Status assign_pydata(std::uintptr_t actual_row,quest_runtime_fields_v1::Result*);
    quest_runtime_fields_v1::Status owner_children(quest_runtime_fields_v1::Result*);
    quest_runtime_fields_v1::Status reinit(quest_runtime_fields_v1::Result*);
    quest_runtime_fields_v1::Status load_quest_data(player_saved_quests_v1::StreamRef&,
                                                  std::uint32_t flag,quest_runtime_fields_v1::Result*);
    quest_runtime_fields_v1::Status destroy(quest_runtime_fields_v1::Result*);
};
// Providers own allocation/child leases, including displaced arrays and failed
// unpublished objects. Complete explicit destroy before retiring an Instance
// and its providers. C++ teardown does not substitute for source virtual deletes.
// Calls are synchronous on the owners' one thread; reached failures retain all
// source stores. Native owner/generation/output checks are separate policies.
}
