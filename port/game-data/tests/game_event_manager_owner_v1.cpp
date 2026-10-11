#include "../game_event_manager_owner_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace ol = dh2::data::quest_objective_list_v1;
namespace ge = dh2::data::game_event_manager_owner_v1;
namespace ev = dh2::data::v2_event_table_v1;
namespace update_kernel = dh2::data::v2_event_update_kernel_v1;

namespace {
void check(bool ok, const char* why) { if (!ok) throw std::runtime_error(why); }
struct Fixture {
    std::vector<std::unique_ptr<ol::Array>> arrays;
    std::vector<std::unique_ptr<ol::ObjectiveRef>> objectives;
    std::vector<std::unique_ptr<ol::ObjectiveFields>> fields;
    std::vector<std::string> calls;
    bool fail_compile = false;
    bool event_list_valid = false;
    bool event_list_done = false;
    std::uint32_t event_scripts_started = 0;

    static std::int32_t allocate(void* raw, ol::List&, std::uint32_t bytes,
                                 std::uint32_t, ol::Array** out) {
        auto& f = *static_cast<Fixture*>(raw);
        auto a = std::make_unique<ol::Array>();
        a->identity = reinterpret_cast<std::uintptr_t>(a.get());
        a->slots.resize(bytes / 4, nullptr);
        *out = a.get(); f.arrays.push_back(std::move(a)); return 0;
    }
    static std::int32_t factory(void* raw, ol::List&, const ol::Definition& d,
                                std::int32_t kind, ol::ObjectiveRef** out) {
        auto& f = *static_cast<Fixture*>(raw);
        if (!d.event || d.index >= d.event->triggers.size() ||
            d.event->triggers[d.index].type != kind) return 1;
        auto o = std::make_unique<ol::ObjectiveRef>();
        o->action.identity = reinterpret_cast<std::uintptr_t>(o.get());
        auto fields = std::make_unique<ol::ObjectiveFields>();
        o->fields = fields.get();
        o->action.character_10 = &o->fields->character_10;
        *out = o.get(); f.objectives.push_back(std::move(o));
        f.fields.push_back(std::move(fields)); return 0;
    }
    static std::int32_t delete_child(void*, ol::List&, ol::ObjectiveRef*) { return 0; }
    static std::int32_t deallocate(void*, ol::List&, ol::Array*) { return 0; }
    static bool event_valid(void* raw, const ge::Row&, bool& valid,
                            std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); valid = f.event_list_valid;
        error.clear(); return true;
    }
    static bool event_eval(void* raw, const ge::Row&, bool& done,
                           std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); done = f.event_list_done;
        error.clear(); return true;
    }
    static bool event_script(void* raw, const ev::Event& event,
                             const std::string& script, std::int32_t argument,
                             bool check_running, bool online, bool& started,
                             std::string& error) {
        auto& f = *static_cast<Fixture*>(raw);
        if (event.name != "event_update" || script != "update_script" ||
            argument != -1 || check_running || online) return false;
        ++f.event_scripts_started; started = true; error.clear(); return true;
    }
    static bool member(void* raw, std::uintptr_t level, std::uintptr_t manager,
                       ge::Row& row, const ol::MemberCall& call,
                       std::string& error) {
        auto& f = *static_cast<Fixture*>(raw);
        if (level != 0x111 || manager != 0x222 || row.state != 0 ||
            !call.objective || call.objective->action.identity == 0 ||
            call.error != &error) return false;
        const char* op = "unknown";
        switch (call.operation) {
        case ol::MemberOperation::reset_data:
            if (call.function != 0x30 || call.encoded_adjustment != 1 || !call.virtual_call) return false;
            op = "reset"; break;
        case ol::MemberOperation::invalidate_compile:
            if (call.function != 0x479f70 || call.encoded_adjustment != 0 || call.virtual_call) return false;
            op = "invalidate"; break;
        case ol::MemberOperation::compile:
            if (call.function != 0x8 || call.encoded_adjustment != 1 || !call.virtual_call) return false;
            op = "compile"; break;
        case ol::MemberOperation::register_objectives:
            if (call.function != 0x18 || call.encoded_adjustment != 1 || !call.virtual_call) return false;
            op = "register"; break;
        default: return false;
        }
        f.calls.push_back(std::string(op) + ":" + row.definition->name + ":" +
                          std::to_string(call.objective->fields->type_4));
        if (f.fail_compile && call.operation == ol::MemberOperation::compile) {
            error = "source compile failed"; return false;
        }
        return true;
    }
    ol::Services objective_services() {
        return {this, allocate, factory, nullptr, delete_child, deallocate};
    }
    ge::Services manager_services() {
        return {this, member};
    }
};
}

int main() {
    try {
        Fixture f;
        ev::Table source;
        source.rows = {{"event_a", 4, "script_a", {{5, 0, 0, "", "", 12, 0, 1}}},
                       {"event_b", 4, "script_b", {{6, 0, 0, "", "", 0, 0, 0},
                                                       {10, 0, 0, "", "", 7, 0, 1}}}};
        ge::Owner owner;
        ge::Result result{}; std::string error;
        check(owner.initialize(0x111, 9, 0x222, source, f.objective_services(),
                               f.manager_services(), &result, error) == ge::Status::complete,
              "source GameEventManager::Load construction");
        source.rows.clear(); // Owner must keep its immutable source row table.
        check(owner.row_count() == 2 && owner.row(0) && owner.row(1) &&
              owner.row(0)->state == 0 && owner.row(1)->state == 0 &&
              result.objectives_created == 3 && result.rows_reset == 2,
              "retained GameEvent rows, state, and ObjectiveLists");
        const auto id0 = owner.row(0)->identity, id1 = owner.row(1)->identity;
        check(owner.reinit(0x111, 9, 0x222, f.manager_services(), &result, error) == ge::Status::complete &&
              result.rows_visited == 2 && result.rows_compiled == 2 &&
              result.rows_registered == 2 && owner.row(0)->registered &&
              owner.row(1)->registered, "source ReInit reset compile register closure");
        check(owner.row(0)->identity == id0 && owner.row(1)->identity == id1,
              "ReInit must retain existing GameEvent identities");
        check(f.calls.size() == 15 && f.calls[0] == "reset:event_a:5" &&
              f.calls[3] == "reset:event_a:5" &&
              f.calls[4] == "invalidate:event_a:5" &&
              f.calls[5] == "compile:event_a:5" &&
              f.calls[6] == "register:event_a:5" &&
              f.calls[7] == "reset:event_b:6" &&
              f.calls[9] == "invalidate:event_b:6" &&
              f.calls[11] == "compile:event_b:6" &&
              f.calls[13] == "register:event_b:6", "source member-pointer row ordering");
        const auto before_compile_only = f.calls.size();
        check(owner.compile(0x111, 9, 0x222, f.manager_services(), &result, error) == ge::Status::complete &&
              result.rows_reset == 0 && result.rows_compiled == 2 &&
              result.rows_registered == 0 && f.calls.size() == before_compile_only + 6 &&
              f.calls[before_compile_only] == "invalidate:event_a:5" &&
              f.calls[before_compile_only + 1] == "compile:event_a:5",
              "source Load compile pass must not reset or duplicate registration");
        check(owner.reinit(0x111, 8, 0x222, f.manager_services(), &result, error) ==
                  ge::Status::owner_unavailable && result.rows_visited == 0 &&
              f.calls.size() == before_compile_only + 6,
              "stale Level generation must not reinitialize retained rows");
        f.fail_compile = true;
        check(owner.reinit(0x111, 9, 0x222, f.manager_services(), &result, error) == ge::Status::service_failed &&
              result.failed_operation == ge::Operation::compile &&
              result.failed_row == 0 && result.rows_reset == 1 &&
              result.rows_registered == 0 && owner.row(0)->state == 0,
              "source compile failure preserves exact prefix");
        check(owner.close(&result, error) == ge::Status::complete &&
              owner.row_count() == 0, "explicit ObjectiveList cleanup");
        ev::Table update_table;
        update_table.rows = {{"event_update", 4, "update_script",
                              {{5, 0, 0, "", "", 1, 0, 1}}}};
        update_table.states = {1, 2, 3, 0};
        ge::Owner updater;
        check(updater.initialize(0x111, 9, 0x222, update_table,
              f.objective_services(), f.manager_services(), &result, error) ==
              ge::Status::complete, "retained update owner Load");
        ge::UpdateServices update_services{&f, Fixture::event_valid,
              Fixture::event_eval, Fixture::event_script};
        update_kernel::Result update_result{};
        check(updater.update(0x111, 9, 0x222, update_services,
              &update_result, error) == update_kernel::Status::complete &&
              update_result.rows_activated == 0 && updater.row(0)->state == 0,
              "inactive Event remains inactive until ObjectiveList IsValid");
        f.event_list_valid = true;
        check(updater.update(0x111, 9, 0x222, update_services,
              &update_result, error) == update_kernel::Status::complete &&
              update_result.rows_activated == 1 && updater.row(0)->state == 1,
              "source GameEvent Update activates a valid list");
        f.event_list_done = true;
        check(updater.update(0x111, 9, 0x222, update_services,
              &update_result, error) == update_kernel::Status::complete &&
              update_result.rows_completed == 1 && update_result.scripts_started == 1 &&
              updater.row(0)->state == 2 && f.event_scripts_started == 1,
              "active Event completes and runs its original named script");
        check(updater.close(&result, error) == ge::Status::complete,
              "explicit update-owner cleanup");
        std::cout << "{\"validation\":\"PASS\",\"retained_game_event_rows\":2,\"constructed_objectives\":3,\"load_and_reinit_order\":true,\"compile_failure_prefix\":true}\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
