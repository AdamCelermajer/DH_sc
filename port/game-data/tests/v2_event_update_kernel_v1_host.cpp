#include "../v2_event_table_v1.hpp"
#include "../v2_event_update_kernel_v1.hpp"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace data = dh2::data::v2_event_table_v1;
namespace update = dh2::data::v2_event_update_kernel_v1;
namespace {
unsigned checks = 0;
void check(bool value, const char* what) {
    ++checks;
    if (!value) throw std::runtime_error(what);
}
std::vector<std::uint8_t> file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open source event cache file: " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
data::Bytes view(const std::vector<std::uint8_t>& value) {
    return {value.data(), static_cast<std::uint32_t>(value.size())};
}

struct Context {
    std::vector<std::string> trace;
    std::map<std::string, std::int32_t> states;
    std::string unsupported_type;
    std::string invalid_name;
    std::string complete_name;
    bool fail_start = false;
    std::vector<std::string> state_order;
};

bool trigger_supported(void* raw, const data::Event& row, const data::Trigger& trigger,
                       bool& supported, std::string& error) {
    auto& c = *static_cast<Context*>(raw);
    c.trace.push_back("support:" + row.name + ":" + std::to_string(trigger.type));
    supported = trigger.type != static_cast<std::int32_t>(data::ObjectiveType::invalid) &&
                (c.unsupported_type.empty() || c.unsupported_type != std::to_string(trigger.type));
    error.clear();
    return true;
}
bool state(void* raw, const data::Event& row, std::int32_t& value, std::string& error) {
    auto& c = *static_cast<Context*>(raw);
    c.trace.push_back("state:" + row.name);
    c.state_order.push_back(row.name);
    value = c.states[row.name];
    error.clear();
    return true;
}
bool is_valid(void* raw, const data::Event& row, bool& value, std::string& error) {
    auto& c = *static_cast<Context*>(raw);
    c.trace.push_back("valid:" + row.name);
    value = c.invalid_name != row.name;
    error.clear();
    return true;
}
bool eval(void* raw, const data::Event& row, bool& value, std::string& error) {
    auto& c = *static_cast<Context*>(raw);
    c.trace.push_back("eval:" + row.name);
    value = c.complete_name == row.name;
    error.clear();
    return true;
}
bool set_state(void* raw, const data::Event& row, std::int32_t value, std::string& error) {
    auto& c = *static_cast<Context*>(raw);
    c.trace.push_back("set:" + row.name + ":" + std::to_string(value));
    c.states[row.name] = value;
    error.clear();
    return true;
}
bool start_script(void* raw, const data::Event& row, const std::string& script,
                  std::int32_t argument, bool check_running, bool online,
                  bool& started, std::string& error) {
    auto& c = *static_cast<Context*>(raw);
    c.trace.push_back("script:" + row.name + ":" + script);
    check(argument == -1 && !check_running && !online, "source StartScript args differ");
    if (c.fail_start) {
        error = "script service unavailable";
        return false;
    }
    started = !script.empty();
    error.clear();
    return true;
}
update::Services services(Context& context) {
    return {&context, trigger_supported, state, is_valid, eval, set_state, start_script};
}
data::Event row(const char* name, std::int32_t trigger_type) {
    data::Event value;
    value.name = name;
    value.script = std::string("Scripts.") + name;
    value.triggers.push_back({trigger_type, -1, -1, {}, {}, 1, 0, 1});
    return value;
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) return 2;
        const std::string root = argv[1];
        const auto packed = file(root + "/v2eventmanager_pyarray.bin");
        const auto names = file(root + "/v2eventmanager_pyarraynames.bin");
        const auto constants = file(root + "/v2eventmanager_pycst.bin");
        data::Table source;
        std::string error;
        check(data::load(view(packed), view(names), view(constants), source, error),
              "event parser failed for kernel test");

        Context source_order;
        source_order.states.clear();
        for (const auto& item : source.rows)
            source_order.states[item.name] = source.states.completed;
        update::Result result{};
        check(update::update(source, services(source_order), &result, error) == update::Status::complete,
              "source-order kernel rejected parsed 66-row table");
        check(result.rows_visited == 66 && source_order.state_order.size() == 66,
              "kernel did not visit every source row");
        for (std::size_t i = 0; i < source.rows.size(); ++i)
            check(source_order.state_order[i] == source.rows[i].name,
                  "kernel changed source row order");
        check(result.rows_activated == 0 && result.rows_completed == 0,
              "completed rows unexpectedly transitioned");

        data::Table fixture;
        fixture.states = {1, 2, 3, 0};
        fixture.rows = {row("activate", 5), row("complete", 4), row("already-done", 0)};
        Context context;
        context.states = {{"activate", fixture.states.inactive},
                          {"complete", fixture.states.active},
                          {"already-done", fixture.states.completed}};
        context.complete_name = "complete";
        check(update::update(fixture, services(context), &result, error) == update::Status::complete,
              "fixture frame update failed");
        check(result.rows_visited == 3 && result.rows_activated == 1 &&
              result.rows_completed == 1 && result.scripts_started == 1,
              "fixture state transition counts differ");
        check(context.states["activate"] == fixture.states.active &&
              context.states["complete"] == fixture.states.completed,
              "fixture state mutation differs");
        const std::vector<std::string> expected{
            "support:activate:5", "support:complete:4", "support:already-done:0",
            "state:activate", "valid:activate", "set:activate:1",
            "state:complete", "eval:complete", "set:complete:2",
            "script:complete:Scripts.complete", "state:already-done"};
        check(context.trace == expected, "source callback order differs");

        Context unsupported;
        unsupported.unsupported_type = "4";
        check(update::update(fixture, services(unsupported), &result, error) ==
                  update::Status::unsupported_trigger,
              "unsupported trigger did not fail closed");
        check(unsupported.state_order.empty() && result.failed_stage == update::Stage::trigger_preflight &&
              !result.state_committed_before_failure,
              "unsupported preflight ran state callbacks or mutated state");

        Context partial;
        partial.states["complete"] = fixture.states.active;
        partial.complete_name = "complete";
        partial.fail_start = true;
        data::Table one;
        one.states = fixture.states;
        one.rows = {row("complete", 5)};
        check(update::update(one, services(partial), &result, error) == update::Status::service_failed,
              "script failure did not propagate");
        check(partial.states["complete"] == one.states.completed &&
              result.state_committed_before_failure && result.failed_stage == update::Stage::start_script,
              "post-SetState script failure lost source partial-commit state");

        Context unavailable;
        auto no_script = services(unavailable);
        no_script.start_script = nullptr;
        check(update::update(fixture, no_script, &result, error) == update::Status::service_unavailable,
              "missing service did not fail closed");
        check(unavailable.trace.empty(), "missing service caused callbacks");

        std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"source_rows\":%zu,\"ordered_transitions\":true,\"unsupported_trigger_preflight\":true}\n",
                    checks, source.rows.size());
        return 0;
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "%s\n", exception.what());
        return 1;
    }
}
