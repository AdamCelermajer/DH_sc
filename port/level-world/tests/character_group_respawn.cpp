#include "../character_group_respawn.hpp"

#include <cstdio>
#include <stdexcept>
#include <vector>

using namespace dh2::character_group;

namespace {

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct QueryCall {
    std::uintptr_t character_identity;
    std::uint32_t answer;
};

struct Fixture {
    GroupState* group = nullptr;
    std::int32_t expected_group_status_during_queries = 0;
    std::vector<QueryCall> answers;
    std::vector<std::uintptr_t> calls;
    std::uintptr_t fail_on_identity = 0;
};

std::int32_t query_limbus(void* context, std::uintptr_t identity,
                          std::uint32_t* result) {
    auto& fixture = *static_cast<Fixture*>(context);
    require(fixture.group != nullptr, "query fixture group is bound");
    require(fixture.group->respawn_status ==
                fixture.expected_group_status_during_queries,
            "group status remains unchanged until all source queries finish");
    fixture.calls.push_back(identity);
    if (identity == fixture.fail_on_identity) return 1;
    for (const auto& answer : fixture.answers) {
        if (answer.character_identity == identity) {
            *result = answer.answer;
            return 0;
        }
    }
    return 1;
}

void expect_calls(const Fixture& fixture,
                  std::initializer_list<std::uintptr_t> expected) {
    require(fixture.calls.size() == expected.size(), "member query count");
    std::size_t index = 0;
    for (const auto identity : expected) {
        require(fixture.calls[index++] == identity, "source member order");
    }
}

}  // namespace

int main() {
    try {
        MemberRef members[] = {{0x100}, {0x200}, {0x100}};
        GroupState group{1, 0, {0, 0, 0}, members, 3};
        Fixture fixture;
        fixture.group = &group;
        fixture.expected_group_status_during_queries = 1;
        fixture.answers = {{0x100, 1}, {0x200, 1}};
        Services services{&fixture, query_limbus};
        Result result{77, 88, 99, 100};

        // State 0 consults GroupInfo +0x28 only; it does not inspect members.
        auto status = can_respawn(0, &group, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 1 &&
                    result.members_queried == 0 && result.group_status_observed == 0 &&
                    group.respawn_status == 1,
                "Limbus state passes an open source gate");
        group.limbus_respawn_gate = 1;
        status = can_respawn(0, &group, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 0 &&
                    result.members_queried == 0 && result.group_status_observed == 0 &&
                    group.respawn_status == 1,
                "Limbus state respects closed source gate");
        group.limbus_respawn_gate = 0;
        group.members = nullptr;
        status = can_respawn(0, &group, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 1 &&
                    result.group_status_observed == 0 && group.respawn_status == 1,
                "Limbus state does not inspect the unused member vector or status");
        group.members = members;

        // Non-Limbus/non-Idle actor states do not inspect group members.
        fixture.calls.clear();
        status = can_respawn(17, &group, &services, &result);
        require(status == Status::complete && result.can_respawn == 0 &&
                    result.members_queried == 0 && result.group_status_observed == 0 &&
                    fixture.calls.empty() &&
                    group.respawn_status == 1,
                "unsupported Character state is denied without mutation");
        status = can_respawn(17, nullptr, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 0 &&
                    result.group_status_observed == 0,
                "unsupported Character state returns before GroupInfo access");

        // Idle with status 1 visits every member in vector order, including a
        // repeated identity. Only after all queries succeed does +0x24 change.
        fixture.calls.clear();
        status = can_respawn(3, &group, &services, &result);
        require(status == Status::complete && result.can_respawn == 1 &&
                    result.members_queried == 3 && result.group_status_after == 2 &&
                    result.group_status_observed == 1 &&
                    group.respawn_status == 2,
                "all-Limbus group advances to respawn-ready status");
        expect_calls(fixture, {0x100, 0x200, 0x100});

        // Status 2 is ready without more member queries. Other states deny.
        fixture.calls.clear();
        group.members = nullptr;
        status = can_respawn(3, &group, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 1 &&
                    result.members_queried == 0 && result.group_status_observed == 1 &&
                    fixture.calls.empty(),
                "status 2 is immediately respawn-ready");
        group.members = members;
        group.respawn_status = 0;
        status = can_respawn(3, &group, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 0 &&
                    result.group_status_after == 0 && fixture.calls.empty(),
                "other group status denies respawn");

        // One active peer keeps status 1; the native loop still queries the
        // remaining members rather than short-circuiting after the first miss.
        group.respawn_status = 1;
        fixture.answers = {{0x100, 1}, {0x200, 0}};
        fixture.calls.clear();
        status = can_respawn(3, &group, &services, &result);
        require(status == Status::complete && result.can_respawn == 0 &&
                    result.members_queried == 3 && group.respawn_status == 1,
                "any non-Limbus peer blocks and preserves status 1");
        expect_calls(fixture, {0x100, 0x200, 0x100});

        // Empty status-1 group has a vacuously complete source loop and stores
        // status 2. No callback is required because there are no members.
        group.members = nullptr;
        group.member_count = 0;
        fixture.calls.clear();
        status = can_respawn(3, &group, nullptr, &result);
        require(status == Status::complete && result.can_respawn == 1 &&
                    result.members_queried == 0 && group.respawn_status == 2 &&
                    fixture.calls.empty(),
                "empty group follows native all-members result");

        // The source member predicate is a pure query. If its host adapter
        // fails, do not publish a partial group status or output result.
        group.respawn_status = 1;
        group.members = members;
        group.member_count = 3;
        fixture.expected_group_status_during_queries = 1;
        fixture.fail_on_identity = 0x200;
        fixture.calls.clear();
        result = {71, 72, 73, 74};
        status = can_respawn(3, &group, &services, &result);
        require(status == Status::member_query_failed && group.respawn_status == 1 &&
                    result.can_respawn == 71 && result.members_queried == 72 &&
                    result.group_status_after == 73 && result.group_status_observed == 74,
                "failed query leaves group and output untouched");
        expect_calls(fixture, {0x100, 0x200});

        // Missing member-query binding is reported before any source state or
        // result mutation because the original SM_IsInLimbus leaf is required.
        fixture.fail_on_identity = 0;
        result = {81, 82, 83, 84};
        status = can_respawn(3, &group, nullptr, &result);
        require(status == Status::invalid_argument && group.respawn_status == 1 &&
                    result.can_respawn == 81 && result.members_queried == 82 &&
                    result.group_status_after == 83 && result.group_status_observed == 84,
                "missing member query rejects atomically");

        // Malformed borrowed membership is rejected before queries/mutation.
        MemberRef malformed[] = {{0}, {0x100}};
        group.members = malformed;
        group.respawn_status = 1;
        fixture.calls.clear();
        status = can_respawn(3, &group, &services, &result);
        require(status == Status::invalid_argument && fixture.calls.empty() &&
                    group.respawn_status == 1 && result.can_respawn == 81 &&
                    result.members_queried == 82 && result.group_status_after == 83 &&
                    result.group_status_observed == 84,
                "null Character identity rejects atomically");

        std::printf("{\"can_respawn_cases\":%u,\"ordered_member_queries\":%u,\"mismatches\":0}\n",
                    13u, 3u);
        return 0;
    } catch (const std::exception& failure) {
        std::fprintf(stderr, "character group respawn: %s\n", failure.what());
        return 1;
    }
}
