#include "../character_group_limbus_blur.hpp"

#include <cstdio>
#include <cstring>
#include <limits>
#include <new>
#include <stdexcept>
#include <vector>

using namespace dh2::character_group_limbus_blur;
using dh2::character_group::GroupState;
using dh2::character_group::MemberRef;

namespace {
constexpr std::uintptr_t owner = 0x100;

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Answer { std::uintptr_t identity; std::uint32_t value; };
struct Fixture {
    GroupState* group = nullptr;
    std::vector<Answer> answers;
    std::vector<std::uintptr_t> queries;
    std::uintptr_t fail_identity = 0;
    std::uintptr_t throw_identity = 0;
    std::uintptr_t redirect_at = 0;
    const MemberRef* redirected_members = nullptr;
    std::uint32_t redirected_count = 0;
};

std::int32_t query(void* context, std::uintptr_t identity,
                   std::uint32_t* in_limbus) {
    auto& fixture = *static_cast<Fixture*>(context);
    require(identity != owner, "owner must be skipped");
    require(fixture.group->respawn_status == 7,
            "group status write must follow every source query");
    fixture.queries.push_back(identity);
    if (identity == fixture.throw_identity) throw std::runtime_error("query failure");
    if (identity == fixture.fail_identity) return 1;
    if (identity == fixture.redirect_at) {
        fixture.group->members = fixture.redirected_members;
        fixture.group->member_count = fixture.redirected_count;
    }
    *in_limbus = 0;
    for (const auto& answer : fixture.answers)
        if (answer.identity == identity) *in_limbus = answer.value;
    return 0;
}

dh2::character_group::Services bind(Fixture& fixture) {
    return {&fixture, query};
}

Result sentinel() { return {71, 72, 73, 74, 75, 76}; }
bool unchanged(const Result& result) {
    const auto previous = sentinel();
    return std::memcmp(&result, &previous, sizeof(result)) == 0;
}
}  // namespace

int main() {
    std::uint32_t cases = 0;
    try {
        Result result = sentinel();
        auto status = after_revive(owner, 0,
            reinterpret_cast<GroupState*>(std::uintptr_t{1}), nullptr, &result);
        require(status == Status::complete && result.group_status_written == 0 &&
                    result.members_queried == 0, "non-role3 skips poisoned group");
        ++cases;

        GroupState empty{7, 231, {0, 0, 0},
                         reinterpret_cast<MemberRef*>(std::uintptr_t{1}), 0};
        status = after_revive(owner, 3, &empty, nullptr, &result);
        require(status == Status::complete && result.member_count_snapshot == 0 &&
                    result.group_status_written == 1 && empty.respawn_status == 0,
                "empty role3 group writes zero and ignores unused member pointer/gate");
        ++cases;

        MemberRef only_owner[]{{owner}, {owner}};
        GroupState self{7, 0, {0, 0, 0}, only_owner, 2};
        status = after_revive(owner, 3, &self, nullptr, &result);
        require(status == Status::complete && result.owner_entries_skipped == 2 &&
                    result.members_queried == 0 && self.respawn_status == 0,
                "self-only entries skip queries and write zero");
        ++cases;

        MemberRef mixed[]{{owner}, {0xa1}, {0xb1}, {owner}, {0xc1}};
        GroupState mixed_group{7, 0, {0, 0, 0}, mixed, 5};
        Fixture mixed_fixture;
        mixed_fixture.group = &mixed_group;
        mixed_fixture.answers = {{0xa1, 1}, {0xb1, 0}, {0xc1, 1}};
        auto mixed_services = bind(mixed_fixture);
        status = after_revive(owner, 3, &mixed_group, &mixed_services, &result);
        require(status == Status::complete && result.member_count_snapshot == 5 &&
                    result.members_queried == 3 && result.owner_entries_skipped == 2 &&
                    result.any_other_in_limbus == 1 && mixed_group.respawn_status == 2 &&
                    mixed_fixture.queries == std::vector<std::uintptr_t>{0xa1, 0xb1, 0xc1},
                "any Limbus answer selects2 while every other member is queried in order");
        ++cases;

        MemberRef neither[]{{0xa1}, {0xb1}};
        GroupState neither_group{7, 0, {0, 0, 0}, neither, 2};
        Fixture neither_fixture;
        neither_fixture.group = &neither_group;
        auto neither_services = bind(neither_fixture);
        status = after_revive(owner, 3, &neither_group, &neither_services, &result);
        require(status == Status::complete && result.members_queried == 2 &&
                    result.any_other_in_limbus == 0 && neither_group.respawn_status == 0,
                "no other Limbus member selects zero");
        ++cases;

        MemberRef duplicated[]{{0xa1}, {0xa1}};
        GroupState duplicate_group{7, 0, {0, 0, 0}, duplicated, 2};
        Fixture duplicate_fixture;
        duplicate_fixture.group = &duplicate_group;
        duplicate_fixture.answers = {{0xa1, 1}};
        auto duplicate_services = bind(duplicate_fixture);
        status = after_revive(owner, 3, &duplicate_group, &duplicate_services, &result);
        require(status == Status::complete && result.members_queried == 2 &&
                    duplicate_fixture.queries == std::vector<std::uintptr_t>{0xa1, 0xa1},
                "source vector duplicate peers are queried twice");
        ++cases;

        MemberRef initial[]{{owner}, {0xa1}, {0xb1}, {0xc1}};
        MemberRef redirected[]{{owner}, {0xa1}, {0xd1}, {0xe1}};
        GroupState redirected_group{7, 0, {0, 0, 0}, initial, 4};
        Fixture redirected_fixture;
        redirected_fixture.group = &redirected_group;
        redirected_fixture.answers = {{0xd1, 1}};
        redirected_fixture.redirect_at = 0xa1;
        redirected_fixture.redirected_members = redirected;
        redirected_fixture.redirected_count = 1;
        auto redirected_services = bind(redirected_fixture);
        status = after_revive(owner, 3, &redirected_group, &redirected_services, &result);
        require(status == Status::complete && result.member_count_snapshot == 4 &&
                    result.members_queried == 3 && redirected_group.respawn_status == 2 &&
                    redirected_fixture.queries == std::vector<std::uintptr_t>{0xa1, 0xd1, 0xe1},
                "count is captured while each member pointer is read live");
        ++cases;

        GroupState failed_group{7, 0, {0, 0, 0}, neither, 2};
        Fixture failed_fixture;
        failed_fixture.group = &failed_group;
        failed_fixture.fail_identity = 0xb1;
        auto failed_services = bind(failed_fixture);
        result = sentinel();
        status = after_revive(owner, 3, &failed_group, &failed_services, &result);
        require(status == Status::member_query_failed && unchanged(result) &&
                    failed_group.respawn_status == 7 &&
                    failed_fixture.queries == std::vector<std::uintptr_t>{0xa1, 0xb1},
                "query error preserves result and defers kernel-owned status write");
        ++cases;

        GroupState bool_group{7, 0, {0, 0, 0}, neither, 2};
        Fixture bool_fixture;
        bool_fixture.group = &bool_group;
        bool_fixture.answers = {{0xa1, 2}};
        auto bool_services = bind(bool_fixture);
        status = after_revive(owner, 3, &bool_group, &bool_services, &result);
        require(status == Status::member_query_failed && unchanged(result) &&
                    bool_group.respawn_status == 7,
                "nonnormalized source bool is an adapter error");
        ++cases;

        alignas(GroupState) unsigned char header_alias[sizeof(GroupState)]{};
        auto* aliased_group = new (header_alias) GroupState{7, 0, {0, 0, 0}, neither, 2};
        unsigned char header_before[sizeof(header_alias)];
        std::memcpy(header_before, header_alias, sizeof(header_before));
        status = after_revive(owner, 3, aliased_group, nullptr,
                             reinterpret_cast<Result*>(header_alias));
        require(status == Status::invalid_argument &&
                    std::memcmp(header_before, header_alias, sizeof(header_alias)) == 0,
                "output group-header alias is mutation-free");
        ++cases;

        MemberRef member_alias[]{{0xa1}, {0xb1}, {0xc1}};
        GroupState member_alias_group{7, 0, {0, 0, 0}, member_alias, 3};
        unsigned char member_before[sizeof(member_alias)];
        std::memcpy(member_before, member_alias, sizeof(member_before));
        status = after_revive(owner, 3, &member_alias_group, nullptr,
                             reinterpret_cast<Result*>(member_alias));
        require(status == Status::invalid_argument && member_alias_group.respawn_status == 7 &&
                    std::memcmp(member_before, member_alias, sizeof(member_alias)) == 0,
                "output member-range alias is mutation-free");
        ++cases;

        GroupState wrapped_group{7, 0, {0, 0, 0},
            reinterpret_cast<MemberRef*>(std::numeric_limits<std::uintptr_t>::max() - 3), 1};
        status = after_revive(owner, 3, &wrapped_group, nullptr, &result);
        require(status == Status::invalid_argument && unchanged(result) &&
                    wrapped_group.respawn_status == 7, "wrapped address rejected before reading");
        ++cases;

        GroupState missing_group{7, 0, {0, 0, 0}, neither, 2};
        status = after_revive(owner, 3, &missing_group, nullptr, &result);
        require(status == Status::invalid_argument && unchanged(result) &&
                    missing_group.respawn_status == 7, "required missing query rejected");
        ++cases;

        MemberRef bad_identity[]{{0xa1}, {0}};
        GroupState bad_group{7, 0, {0, 0, 0}, bad_identity, 2};
        Fixture bad_fixture;
        bad_fixture.group = &bad_group;
        auto bad_services = bind(bad_fixture);
        status = after_revive(owner, 3, &bad_group, &bad_services, &result);
        require(status == Status::invalid_argument && unchanged(result) &&
                    bad_group.respawn_status == 7 && bad_fixture.queries.size() == 1,
                "later null identity fails without an invented status write");
        ++cases;

        GroupState throwing_group{7, 0, {0, 0, 0}, neither, 2};
        Fixture throwing_fixture;
        throwing_fixture.group = &throwing_group;
        throwing_fixture.throw_identity = 0xb1;
        auto throwing_services = bind(throwing_fixture);
        bool propagated = false;
        try { (void)after_revive(owner, 3, &throwing_group, &throwing_services, &result); }
        catch (const std::runtime_error&) { propagated = true; }
        require(propagated && unchanged(result) && throwing_group.respawn_status == 7 &&
                    throwing_fixture.queries == std::vector<std::uintptr_t>{0xa1, 0xb1},
                "query exception propagates with no rollback claim or final status write");
        ++cases;

        MemberRef final_redirect[]{{0xa1}};
        GroupState final_group{7, 0, {0, 0, 0}, final_redirect, 1};
        Fixture final_fixture;
        final_fixture.group = &final_group;
        final_fixture.redirect_at = 0xa1;
        final_fixture.redirected_members = reinterpret_cast<MemberRef*>(&result);
        final_fixture.redirected_count = 1;
        auto final_services = bind(final_fixture);
        status = after_revive(owner, 3, &final_group, &final_services, &result);
        require(status == Status::invalid_argument && unchanged(result) &&
                    final_group.respawn_status == 7,
                "final query cannot introduce an output/member alias before commit");
        ++cases;

        std::printf("{\"group_limbus_blur_cases\":%u,\"role3_only\":true,\"any_other_limbus_selects2\":true,\"all_other_queries_in_order\":true,\"count_snapshot_pointer_live\":true,\"owner_skipped\":true,\"aliases_and_errors_guarded\":true,\"native_group_wired\":false,\"mismatches\":0}\n", cases);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "group Limbus Blur: %s\n", error.what());
        return 1;
    }
}
