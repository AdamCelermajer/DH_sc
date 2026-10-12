#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh2::loader {
// Internal dispatcher contract, not a selected gameplay/factory/save ABI.
// These tokens borrow objects/handles from the service owner. Zero is null.
using InitializationObjectV1 = std::uint64_t;
struct InitializationHandleV1 { std::uint64_t words[3]{}; };
struct InitializationFieldsV1 {
    std::uint32_t a8{}, cc{};
    std::uint8_t ac{}, d0{}; // Semantics of the original field names unresolved.
};
struct ObjectInitializationV1 {
    // Original ObjectManager uses std::map<int,ObjectListItem>, not authored
    // XML order. Keys must come from the runtime owner, never source indices.
    std::map<std::int32_t, InitializationObjectV1> objects;
    std::vector<InitializationObjectV1> modules, rooms, list_2c, list_34, list_44;
    std::uint32_t phase{};
    bool completed{}, failed{};
    std::string error;
    // Original has function-static cursors: it is not reentrant. This adapter
    // keeps them per candidate to avoid sharing state between level owners.
    bool cursor_at_end{true};
    std::int32_t cursor_key{};
    std::size_t module_cursor{};
};
class ObjectInitializationServicesV1 {
public:
    virtual ~ObjectInitializationServicesV1() = default;
    // Module loading may append modules and insert objects into the registry.
    // Callbacks must not erase current tree nodes or reorder/remove modules.
    virtual bool load_module(InitializationObjectV1, ObjectInitializationV1&, std::string&) = 0;
    virtual bool make_handle(InitializationObjectV1, InitializationHandleV1&, std::string&) = 0;
    virtual bool get_object(const InitializationHandleV1&, bool required,
                            InitializationObjectV1&, std::string&) = 0;
    virtual bool init_post(InitializationObjectV1, std::string&) = 0;
    virtual bool test_enable_condition(InitializationObjectV1, bool force, std::string&) = 0;
    virtual bool type_name(InitializationObjectV1, std::string&, std::string&) = 0;
    virtual bool is_updatable(InitializationObjectV1, bool&, std::string&) = 0;
    virtual bool room_init_object_list(InitializationObjectV1, std::string&) = 0;
    virtual bool fields(InitializationObjectV1, InitializationFieldsV1&, std::string&) = 0;
    // Notification before native list storage is cleared, in original order.
    virtual bool clear_list(std::uint32_t offset,
                            const std::vector<InitializationObjectV1>&, std::string&) = 0;
};
enum class ObjectInitializationStepV1 { pending, complete, failed };
// Original resumable InitPost dispatcher at 0x34552c. Services are required,
// never replaced by fabricated objects/condition evaluation. Failure latches
// this candidate with its partial state: destruction/unwind belongs to the
// owning integration. Stable repeated completion is an adapter guard; the
// original is only compared through its first true return.
ObjectInitializationStepV1 step_object_initialization_v1(
    ObjectInitializationV1&, ObjectInitializationServicesV1&);
}
