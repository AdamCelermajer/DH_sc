#include "../character_interaction_spot_owner_v1.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace k = dh2::character_interaction_spot_owner_v1;

struct Fixture {
    static constexpr std::uintptr_t object_id = 0x10001000;
    static constexpr std::uintptr_t visual_id = 0x10002000;
    static constexpr std::uintptr_t node_id = 0x10003000;

    std::uintptr_t visual = visual_id;
    std::uint8_t lookup_started = 0;
    std::uintptr_t cached_node = 0;
    k::Point absolute{{0x3f800000, 0x40000000, 0x40400000}};
    k::Point target{{0x40800000, 0x40a00000, 0x40c00000}};
    std::uintptr_t resolved_node = 0;
    std::uint32_t lookups = 0, absolute_queries = 0, target_queries = 0;
    bool fail_lookup = false;
    bool ordering_observed = false;

    k::ObjectView object{object_id, &visual, &lookup_started, &cached_node};
    k::Services services{this, lookup, absolute_position, target_position};

    static std::int32_t lookup(void* context, std::uintptr_t visual_id_arg,
                               const char* name, std::uintptr_t* node_out) {
        auto& self = *static_cast<Fixture*>(context);
        ++self.lookups;
        self.ordering_observed = self.lookup_started == 1;
        assert(visual_id_arg == visual_id);
        assert(std::strcmp(name, "interaction_position") == 0);
        if (self.fail_lookup) return 1;
        *node_out = self.resolved_node;
        return 0;
    }

    static std::int32_t absolute_position(void* context, std::uintptr_t node,
                                         const k::Point** borrowed) {
        auto& self = *static_cast<Fixture*>(context);
        ++self.absolute_queries;
        assert(node == node_id);
        *borrowed = &self.absolute;
        return 0;
    }

    static std::int32_t target_position(void* context, std::uintptr_t object_id_arg,
                                        const k::Point** borrowed) {
        auto& self = *static_cast<Fixture*>(context);
        ++self.target_queries;
        assert(object_id_arg == object_id);
        *borrowed = &self.target;
        return 0;
    }

    k::Status run(k::Result& result) {
        return k::get_interaction_spot(&object, &services, &result);
    }
};

int main() {
    std::uint32_t cases = 0;

    // A successful null node is cached, then the source target position is
    // used. A later call must not repeat GetSpecificNode.
    {
        Fixture f;
        f.resolved_node = 0;
        k::Result result{};
        assert(f.run(result) == k::Status::complete);
        assert(f.lookup_started == 1 && f.cached_node == 0);
        assert(result.node_lookup_performed && result.node_lookup_resolved);
        assert(result.used_target_position && !result.used_visual_node);
        assert(result.point.words[0] == f.target.words[0]);
        assert(f.lookups == 1 && f.target_queries == 1 && f.absolute_queries == 0);
        assert(f.run(result) == k::Status::complete);
        assert(f.lookups == 1 && !result.node_lookup_performed);
        ++cases;
    }

    // A missing named-node provider is unsupported, not a successful null
    // lookup; the source's pre-call lazy flag write is still visible.
    {
        Fixture f;
        f.services.get_specific_node = nullptr;
        k::Result result{};
        assert(f.run(result) == k::Status::node_lookup_unsupported);
        assert(f.lookup_started == 1 && f.cached_node == 0);
        assert(!result.node_lookup_resolved && f.lookups == 0);
        ++cases;
    }

    // A resolved visual node supplies the copied absolute point.
    {
        Fixture f;
        f.resolved_node = Fixture::node_id;
        k::Result result{};
        assert(f.run(result) == k::Status::complete);
        assert(f.cached_node == Fixture::node_id);
        assert(result.node == Fixture::node_id && result.used_visual_node);
        assert(!result.used_target_position && f.absolute_queries == 1);
        assert(result.point.words[0] == f.absolute.words[0]);
        ++cases;
    }

    // No visual skips lookup and returns the borrowed live target position by
    // value, matching GetTargetPosition's fallback selection.
    {
        Fixture f;
        f.visual = 0;
        f.lookup_started = 0;
        k::Result result{};
        assert(f.run(result) == k::Status::complete);
        assert(f.lookup_started == 1 && f.cached_node == 0);
        assert(f.lookups == 0 && f.target_queries == 1);
        assert(result.node_lookup_resolved && result.used_target_position);
        assert(result.point.words[2] == f.target.words[2]);
        ++cases;
    }

    // The lazy flag is already set inside the named-node callback. Failed
    // lookup retains that completed source write and does not cache a result.
    {
        Fixture f;
        f.cached_node = 0x10004000;
        f.fail_lookup = true;
        k::Result result{};
        assert(f.run(result) == k::Status::service_failed);
        assert(f.ordering_observed && f.lookup_started == 1);
        assert(f.cached_node == 0x10004000);
        assert(result.node_lookup_performed && !result.node_lookup_resolved);
        ++cases;
    }

    std::cout << "{\"validation\":\"PASS\",\"host_cases\":" << cases
              << ",\"null_node_cached\":true,\"node_point_copied\":true"
                 ",\"target_fallback\":true,\"lookup_mutation_order\":true}\n";
}
