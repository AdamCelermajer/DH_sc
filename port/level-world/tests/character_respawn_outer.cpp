#include "../character_respawn_outer.hpp"

#include <cstdio>
#include <cstring>
#include <limits>
#include <new>
#include <stdexcept>
#include <vector>

using namespace dh2::character_respawn;
using dh2::character_group::GroupState;
using dh2::character_group::MemberRef;

namespace {

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    std::int32_t raw_property = 0;
    std::uint32_t property_calls = 0;
    std::int32_t property_id = -1;
    bool fail_property = false;
    std::uint32_t member_calls = 0;
    std::uintptr_t fail_member = 0;
    std::uint32_t member_answer = 1;
    std::vector<std::uintptr_t> queried_members;
};

std::int32_t read_property(void* context, std::int32_t property_id,
                           std::int32_t* raw) {
    auto& fixture = *static_cast<Fixture*>(context);
    ++fixture.property_calls;
    fixture.property_id = property_id;
    if (fixture.fail_property) return 1;
    *raw = fixture.raw_property;
    return 0;
}

std::int32_t is_in_limbus(void* context, std::uintptr_t identity,
                          std::uint32_t* result) {
    auto& fixture = *static_cast<Fixture*>(context);
    ++fixture.member_calls;
    fixture.queried_members.push_back(identity);
    if (identity == fixture.fail_member) return 1;
    *result = fixture.member_answer;
    return 0;
}

Services services_for(Fixture& fixture) {
    return {&fixture, read_property, {&fixture, is_in_limbus}};
}

}  // namespace

int main() {
    std::uint32_t cases = 0;
    try {
        Fixture fixture;
        auto services = services_for(fixture);
        Facts facts{3, 1, {0, 0, 0}};
        DelayResult delay{71, 72, 73};
        PredicateResult predicate{71, 72, 73, 74, 75, 76, 77};

        // Both source methods return at the InitSpawned byte gate before the
        // property getter or optional group is read.
        const auto suppressed_facts = facts;
        auto status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.raw_respawn_time == 0 &&
                    delay.delay_ms == 0 && delay.property_read == 0 &&
                    fixture.property_calls == 0,
                "GetRespawnDelay suppression skips property query");
        ++cases;
        auto* unread_group = reinterpret_cast<GroupState*>(std::uintptr_t{1});
        status = can_respawn(&facts, &services, unread_group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 0 &&
                    predicate.property_read == 0 &&
                    predicate.group_predicate_called == 0 &&
                    fixture.property_calls == 0 && fixture.member_calls == 0,
                "CanRespawn suppression skips property and group reads");
        ++cases;
        facts = suppressed_facts;
        facts.init_spawned_suppression = 0;

        // The authored ResurrectionTime value for Crypt_Ghost_RE is 5120.
        fixture.raw_property = 5120;
        status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.raw_respawn_time == 5120 &&
                    delay.delay_ms == 20000 && delay.property_read == 1 &&
                    fixture.property_id == respawn_time_property_id,
                "Q8.8 RespawnTime 5120 yields 20 seconds");
        ++cases;

        fixture.raw_property = 255;
        status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.delay_ms == 0 &&
                    delay.raw_respawn_time == 255,
                "positive sub-256 delay shifts to zero");
        ++cases;
        status = can_respawn(&facts, &services, nullptr, &predicate);
        require(status == Status::complete && predicate.can_respawn == 1 &&
                    predicate.raw_respawn_time == 255 &&
                    predicate.group_predicate_called == 0 &&
                    fixture.member_calls == 0,
                "positive raw time allows no-group actor despite zero ms delay");
        ++cases;

        fixture.raw_property = -1;
        status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.delay_ms == -1000,
                "negative arithmetic shift preserves sign");
        ++cases;
        fixture.raw_property = -257;
        status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.delay_ms == -2000,
                "negative shift rounds toward negative infinity");
        ++cases;
        fixture.raw_property = -257;
        status = can_respawn(&facts, &services, nullptr, &predicate);
        require(status == Status::complete && predicate.can_respawn == 0 &&
                    predicate.raw_respawn_time == -257 &&
                    predicate.group_predicate_called == 0,
                "nonpositive property denies before group access");
        ++cases;

        fixture.raw_property = std::numeric_limits<std::int32_t>::max();
        status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.delay_ms == -201327592,
                "ARM32 multiply wraps positive overflow to signed 32 bits");
        ++cases;
        fixture.raw_property = std::numeric_limits<std::int32_t>::min();
        status = get_delay(&facts, &services, &delay);
        require(status == Status::complete && delay.delay_ms == 201326592,
                "ARM32 multiply wraps negative overflow to signed 32 bits");
        ++cases;

        MemberRef member[] = {{0xabc}};
        GroupState group{1, 0, {0, 0, 0}, member, 1};
        fixture.raw_property = 0;
        const auto member_calls_before = fixture.member_calls;
        status = can_respawn(&facts, &services, &group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 0 &&
                    predicate.group_predicate_called == 0 &&
                    fixture.member_calls == member_calls_before &&
                    group.respawn_status == 1,
                "zero property denies before reading group status or members");
        ++cases;

        fixture.raw_property = 5120;
        fixture.member_answer = 1;
        status = can_respawn(&facts, &services, nullptr, &predicate);
        require(status == Status::complete && predicate.can_respawn == 1 &&
                    predicate.group_predicate_called == 0,
                "no-group outer branch returns true after gates");
        ++cases;

        // Supported group branch delegates to the existing GroupInfo kernel.
        facts.character_state_id = 3;
        group.respawn_status = 1;
        fixture.member_calls = 0;
        fixture.queried_members.clear();
        status = can_respawn(&facts, &services, &group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 1 &&
                    predicate.group_predicate_called == 1 &&
                    predicate.group_members_queried == 1 &&
                    predicate.group_status_observed == 1 &&
                    predicate.group_status_after == 2 &&
                    fixture.member_calls == 1 &&
                    fixture.queried_members == std::vector<std::uintptr_t>{0xabc} &&
                    group.respawn_status == 2,
                "outer predicate reuses and mutates through GroupInfo kernel");
        ++cases;

        // Unsupported state is rejected by GroupInfo before touching its
        // object; Character's outer method still performs the exact delegate.
        facts.character_state_id = 17;
        auto* state_only_group = reinterpret_cast<GroupState*>(std::uintptr_t{1});
        status = can_respawn(&facts, &services, state_only_group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 0 &&
                    predicate.group_predicate_called == 1 &&
                    predicate.group_status_observed == 0 &&
                    predicate.group_members_queried == 0,
                "existing GroupInfo kernel rejects unsupported state before group reads");
        ++cases;

        facts.character_state_id = 3;
        fixture.fail_property = true;
        predicate = {71, 72, 73, 74, 75, 76, 77};
        const auto predicate_before = predicate;
        status = can_respawn(&facts, &services, &group, &predicate);
        require(status == Status::property_read_failed &&
                    std::memcmp(&predicate, &predicate_before, sizeof(predicate)) == 0,
                "property failure preserves output");
        ++cases;
        fixture.fail_property = false;

        fixture.raw_property = 5120;
        fixture.fail_member = 0xabc;
        group.respawn_status = 1;
        predicate = {71, 72, 73, 74, 75, 76, 77};
        const auto group_failure_before = predicate;
        status = can_respawn(&facts, &services, &group, &predicate);
        require(status == Status::group_member_query_failed &&
                    std::memcmp(&predicate, &group_failure_before, sizeof(predicate)) == 0 &&
                    group.respawn_status == 1,
                "member query failure preserves outer output and source group status");
        ++cases;

        // Preserve raw byte gating, including non-normalized nonzero values,
        // without requiring a property provider or reading the poisoned group.
        Facts raw_byte_facts{3, 255, {0, 0, 0}};
        status = get_delay(&raw_byte_facts, nullptr, &delay);
        require(status == Status::complete && delay.delay_ms == 0 &&
                    delay.property_read == 0,
                "raw nonzero suppression byte skips missing property service");
        ++cases;
        status = can_respawn(&raw_byte_facts, nullptr, unread_group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 0 &&
                    predicate.property_read == 0,
                "raw nonzero suppression byte skips poisoned group");
        ++cases;
        fixture.raw_property = 0;
        status = can_respawn(&facts, &services, unread_group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 0 &&
                    predicate.property_read == 1 &&
                    predicate.group_predicate_called == 0,
                "nonpositive raw property never dereferences poisoned group");
        ++cases;

        fixture.raw_property = 5120;
        fixture.fail_member = 0;
        fixture.member_answer = 1;

        // Boundary validation must not inspect member storage on group paths
        // which do not consume it in the original source.
        GroupState ignored_members_group{
            2, 0, {0, 0, 0}, reinterpret_cast<MemberRef*>(std::uintptr_t{1}),
            std::numeric_limits<std::uint32_t>::max()};
        const Facts limbus_facts{0, 0, {0, 0, 0}};
        const auto calls_before_ignored_members = fixture.member_calls;
        status = can_respawn(&limbus_facts, &services, &ignored_members_group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 1 &&
                    predicate.group_status_observed == 0 &&
                    fixture.member_calls == calls_before_ignored_members,
                "Limbus ignores poisoned member range and group status");
        ++cases;
        status = can_respawn(&facts, &services, &ignored_members_group, &predicate);
        require(status == Status::complete && predicate.can_respawn == 1 &&
                    predicate.group_status_observed == 1 &&
                    predicate.group_status_after == 2 &&
                    fixture.member_calls == calls_before_ignored_members,
                "Idle status 2 ignores poisoned member range");
        ++cases;

        // The outer result must not bypass the leaf's alias protection merely
        // because the wrapper supplies a separate local leaf result.
        alignas(GroupState) unsigned char header_storage[sizeof(PredicateResult)]{};
        auto* aliased_group = new (header_storage) GroupState{1, 0, {0, 0, 0}, member, 1};
        unsigned char header_before[sizeof(header_storage)];
        std::memcpy(header_before, header_storage, sizeof(header_before));
        const auto calls_before_alias = fixture.member_calls;
        status = can_respawn(&facts, &services, aliased_group,
                             reinterpret_cast<PredicateResult*>(header_storage));
        require(status == Status::group_input_invalid &&
                    std::memcmp(header_before, header_storage, sizeof(header_before)) == 0 &&
                    fixture.member_calls == calls_before_alias,
                "outer output overlapping group header rejected before group mutation");
        ++cases;

        alignas(MemberRef) MemberRef aliased_members[4]{{0xa1}, {0xa2}, {0xa3}, {0xa4}};
        GroupState member_alias_group{1, 0, {0, 0, 0}, aliased_members, 4};
        unsigned char members_before[sizeof(aliased_members)];
        std::memcpy(members_before, aliased_members, sizeof(members_before));
        status = can_respawn(&facts, &services, &member_alias_group,
                             reinterpret_cast<PredicateResult*>(aliased_members));
        require(status == Status::group_input_invalid &&
                    member_alias_group.respawn_status == 1 &&
                    std::memcmp(members_before, aliased_members, sizeof(members_before)) == 0 &&
                    fixture.member_calls == calls_before_alias,
                "outer output overlapping member range rejected before queries");
        ++cases;

        GroupState wrapped_range_group{
            1, 0, {0, 0, 0},
            reinterpret_cast<MemberRef*>(std::numeric_limits<std::uintptr_t>::max() - 3), 1};
        predicate = predicate_before;
        status = can_respawn(&facts, &services, &wrapped_range_group, &predicate);
        require(status == Status::group_input_invalid &&
                    wrapped_range_group.respawn_status == 1 &&
                    std::memcmp(&predicate, &predicate_before, sizeof(predicate)) == 0 &&
                    fixture.member_calls == calls_before_alias,
                "wrapped member address range rejected without dereference");
        ++cases;

        alignas(DelayResult) unsigned char facts_storage[sizeof(DelayResult)]{};
        auto* alias_facts = new (facts_storage) Facts{3, 0, {0, 0, 0}};
        unsigned char facts_before[sizeof(facts_storage)];
        std::memcpy(facts_before, facts_storage, sizeof(facts_before));
        const auto property_calls_before_alias = fixture.property_calls;
        status = get_delay(alias_facts, &services,
                           reinterpret_cast<DelayResult*>(facts_storage));
        require(status == Status::invalid_argument &&
                    std::memcmp(facts_before, facts_storage, sizeof(facts_before)) == 0 &&
                    fixture.property_calls == property_calls_before_alias,
                "facts output overlap rejected without property access");
        ++cases;
        const auto services_before = services;
        status = can_respawn(&facts, &services, &group,
                             reinterpret_cast<PredicateResult*>(&services));
        require(status == Status::invalid_argument &&
                    std::memcmp(&services, &services_before, sizeof(services)) == 0 &&
                    fixture.property_calls == property_calls_before_alias,
                "services output overlap rejected before callback access");
        ++cases;

        delay = {71, 72, 73};
        const auto delay_before = delay;
        status = get_delay(&facts, nullptr, &delay);
        require(status == Status::invalid_argument &&
                    std::memcmp(&delay, &delay_before, sizeof(delay)) == 0,
                "required missing property service preserves delay output");
        ++cases;
        GroupState invalid_group{1, 0, {1, 0, 0}, member, 1};
        predicate = predicate_before;
        status = can_respawn(&facts, &services, &invalid_group, &predicate);
        require(status == Status::group_input_invalid &&
                    invalid_group.respawn_status == 1 &&
                    std::memcmp(&predicate, &predicate_before, sizeof(predicate)) == 0 &&
                    fixture.member_calls == calls_before_alias,
                "invalid group status maps without partial outer commit");
        ++cases;
        fixture.member_answer = 2;
        group.respawn_status = 1;
        status = can_respawn(&facts, &services, &group, &predicate);
        require(status == Status::group_member_query_failed &&
                    group.respawn_status == 1 &&
                    std::memcmp(&predicate, &predicate_before, sizeof(predicate)) == 0,
                "nonnormalized member answer maps to group query failure");
        ++cases;

        std::printf("{\"character_respawn_outer_cases\":%u,\"property_id\":11,\"group_kernel_reused\":true,\"suppression_skips_property\":true,\"rejected_property_skips_group\":true,\"arm_signed_shift_and_mul\":true,\"outer_alias_bounds_checked\":true,\"hosting_timer_policy_wired\":false,\"mismatches\":0}\n",
                    cases);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "character respawn outer audit: %s\n", error.what());
        return 1;
    }
}
