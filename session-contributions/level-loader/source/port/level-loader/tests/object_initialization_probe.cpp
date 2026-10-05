#include "object_initialization_v1.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace dh2::loader;
namespace {
std::string quote(const std::string& value) {
    std::string out = "\"";
    for (char c : value) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out + "\"";
}
struct Object {
    std::string label, type;
    bool updating{}, deleted{};
    InitializationFieldsV1 fields;
    std::uint64_t append{};
    std::uint64_t insert{};
    std::int32_t key{};
    bool deferred{};
    int init_a8{-1}, room_cc{-1};
};
struct Services : ObjectInitializationServicesV1 {
    ObjectInitializationV1& state;
    std::vector<Object> data;
    std::vector<std::string> events;
    unsigned call{};
    std::string failing_operation;
    explicit Services(ObjectInitializationV1& s) : state(s) { data.emplace_back(); }
    std::string list(const std::vector<InitializationObjectV1>& values) const {
        std::string out = "[";
        for (const auto token : values) {
            if (out.size() > 1) out += ',';
            out += quote(data.at(token).label);
        }
        return out + "]";
    }
    bool event(const std::string& kind, std::uint64_t token, std::string& error,
               const std::string& extra = "") {
        events.push_back("{\"call\":" + std::to_string(call) + ",\"phase\":" + std::to_string(state.phase)
            + ",\"kind\":" + quote(kind) + (token ? ",\"object\":" + quote(data.at(token).label) : "") + extra + "}");
        if (failing_operation == kind) { error = "explicit unavailable service"; return false; }
        return true;
    }
    bool load_module(InitializationObjectV1 token, ObjectInitializationV1& s, std::string& error) override {
        if (!event("load_module", token, error)) return false;
        const auto appended = data.at(token).append;
        if (appended) {
            s.modules.push_back(appended);
            if (!event("append_module", appended, error)) return false;
        }
        const auto inserted = data.at(token).insert;
        if (inserted) {
            require_insert(s.objects.emplace(data.at(inserted).key, inserted).second);
            if (!event("insert_object", inserted, error)) return false;
        }
        return true;
    }
    bool make_handle(InitializationObjectV1 token, InitializationHandleV1& handle, std::string&) override {
        handle.words[0] = token; return true;
    }
    bool get_object(const InitializationHandleV1& handle, bool, InitializationObjectV1& token, std::string&) override {
        token = handle.words[0];
        if (data.at(token).deleted) token = 0;
        return true;
    }
    bool init_post(InitializationObjectV1 token, std::string& error) override {
        if (!event("init_post", token, error)) return false;
        auto& object = data.at(token);
        if (object.init_a8 >= 0) object.fields.a8 = static_cast<unsigned>(object.init_a8);
        return true;
    }
    bool test_enable_condition(InitializationObjectV1 token, bool force, std::string& error) override {
        return event("test_enable_condition", token, error, std::string(",\"force\":") + (force ? "true" : "false"));
    }
    bool type_name(InitializationObjectV1 token, std::string& type, std::string&) override {
        type = data.at(token).type; return true;
    }
    bool is_updatable(InitializationObjectV1 token, bool& value, std::string& error) override {
        value = data.at(token).updating; return event("is_updatable", token, error);
    }
    bool room_init_object_list(InitializationObjectV1 token, std::string& error) override {
        if (!event("room_init_object_list", token, error)) return false;
        auto& object = data.at(token);
        if (object.room_cc >= 0) object.fields.cc = static_cast<unsigned>(object.room_cc);
        return true;
    }
    bool fields(InitializationObjectV1 token, InitializationFieldsV1& fields, std::string&) override {
        fields = data.at(token).fields; return true;
    }
    bool clear_list(std::uint32_t offset, const std::vector<InitializationObjectV1>& objects, std::string& error) override {
        std::ostringstream hex; hex << "0x" << std::hex << offset;
        return event("clear_list", 0, error, ",\"list_offset\":" + quote(hex.str()) + ",\"objects\":" + list(objects));
    }
    static void require_insert(bool inserted) {
        if (!inserted) throw std::runtime_error("duplicate inserted fixture key");
    }
};
void require(bool ok, const char* reason) { if (!ok) throw std::runtime_error(reason); }
void adapter_checks() {
    unsigned checked{};
    // Failed class service cannot be silently skipped or rerun after latching.
    for (const auto& failure : {"load_module", "init_post", "clear_list", "room_init_object_list"}) {
        ObjectInitializationV1 state;
        Services services(state);
        Object object; object.label = "actor"; object.type = "RoomZone";
        services.data.push_back(object); state.objects.emplace(7, 1); state.modules.push_back(1);
        services.failing_operation = failure;
        for (unsigned i = 0; i < 20 && !state.failed; ++i) {
            services.call = i;
            step_object_initialization_v1(state, services);
        }
        require(state.failed && !state.completed && state.error.find("explicit unavailable service") != std::string::npos,
                "failure did not preserve service error");
        const auto events = services.events.size();
        require(step_object_initialization_v1(state, services) == ObjectInitializationStepV1::failed
                && services.events.size() == events, "failed candidate replayed side effects");
        ++checked;
    }
    ObjectInitializationV1 first, second;
    Services a(first), b(second);
    Object room; room.label = "room"; room.type = "RoomZone";
    Object decor; decor.label = "decor"; decor.type = "Decor"; decor.updating = true;
    a.data.push_back(room); b.data.push_back(decor);
    first.objects.emplace(42, 1); second.objects.emplace(-7, 1);
    for (unsigned i = 0; i < 10 && (!first.completed || !second.completed); ++i) {
        if (!first.completed) step_object_initialization_v1(first, a);
        if (!second.completed) step_object_initialization_v1(second, b);
    }
    require(first.completed && second.completed && first.rooms.size() == 1 && second.list_2c.size() == 1
            && first.list_2c.empty() && second.rooms.empty(), "candidate cursors mixed");
    ++checked;
    const auto events = a.events.size(); const auto phase = first.phase;
    require(step_object_initialization_v1(first, a) == ObjectInitializationStepV1::complete
            && first.phase == phase && a.events.size() == events, "completion replayed services");
    ++checked;
    std::cout << "{\"validation\":\"PASS\",\"adapter_checks\":" << checked << "}\n";
}
}
int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::string(argv[1]) == "--adapter-checks") { adapter_checks(); return 0; }
        unsigned count{}; require(static_cast<bool>(std::cin >> count), "missing fixture count");
        std::cout << "{\"cases\":[";
        for (unsigned row = 0; row < count; ++row) {
            std::string name; unsigned objects{}, modules{}; bool preseed{};
            require(static_cast<bool>(std::cin >> name >> objects >> modules >> preseed), "missing fixture header");
            ObjectInitializationV1 state; Services services(state);
            for (unsigned i = 0; i < objects; ++i) {
                Object object; unsigned ac{}, d0{};
                require(static_cast<bool>(std::cin >> object.label >> object.type >> object.key >> object.updating >> object.deleted
                    >> object.fields.a8 >> ac >> object.fields.cc >> d0 >> object.append >> object.init_a8 >> object.room_cc
                    >> object.insert >> object.deferred),
                    "incomplete object fixture");
                object.fields.ac = static_cast<std::uint8_t>(ac); object.fields.d0 = static_cast<std::uint8_t>(d0);
                services.data.push_back(object);
                if (!object.deferred) require(state.objects.emplace(object.key, i + 1).second, "duplicate fixture key");
            }
            for (unsigned i = 0; i < modules; ++i) {
                InitializationObjectV1 token{}; require(static_cast<bool>(std::cin >> token), "missing module");
                state.modules.push_back(token);
            }
            if (preseed) { state.list_2c.push_back(1); state.list_34.push_back(1); state.list_44.push_back(1); }
            std::vector<std::string> calls;
            for (services.call = 0; services.call < 100; ++services.call) {
                const auto before = state.phase; const auto events = services.events.size();
                const auto result = step_object_initialization_v1(state, services);
                require(result != ObjectInitializationStepV1::failed, state.error.c_str());
                calls.push_back("{\"phase_before\":" + std::to_string(before) + ",\"phase_after\":" + std::to_string(state.phase)
                    + ",\"returned\":" + (result == ObjectInitializationStepV1::complete ? "true" : "false")
                    + ",\"event_count\":" + std::to_string(services.events.size() - events) + "}");
                if (result == ObjectInitializationStepV1::complete) break;
            }
            require(state.completed, "fixture did not complete");
            if (row) std::cout << ',';
            std::cout << "{\"name\":" << quote(name) << ",\"calls\":[";
            for (std::size_t i = 0; i < calls.size(); ++i) { if (i) std::cout << ','; std::cout << calls[i]; }
            std::cout << "],\"events\":[";
            for (std::size_t i = 0; i < services.events.size(); ++i) { if (i) std::cout << ','; std::cout << services.events[i]; }
            std::cout << "],\"room_list\":" << services.list(state.rooms) << ",\"transient_lists\":{\"0x2c\":"
                << services.list(state.list_2c) << ",\"0x34\":" << services.list(state.list_34)
                << ",\"0x44\":" << services.list(state.list_44) << "},\"module_list\":" << services.list(state.modules) << "}";
        }
        std::cout << "]}\n";
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
