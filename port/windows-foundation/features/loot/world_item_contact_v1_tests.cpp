// P16 CONTEXT: walk-over pickup contact rule (ItemObject::OnCollisionBegins moving gate + GameObject::Update consume).
#include "world_item_contact_v1.hpp"

#include <cstdio>
#include <set>

using namespace dh::foundation::loot;

namespace {
int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}
constexpr std::uint64_t hero = 1;
constexpr WorldItemContactTrackerV1::ItemId potion = 10;
constexpr WorldItemContactTrackerV1::ItemId gold = 11;
constexpr WorldItemContactTrackerV1::ItemId other_item = 12;

std::function<bool(WorldItemContactTrackerV1::ItemId)> all_available() {
    return [](WorldItemContactTrackerV1::ItemId) { return true; };
}
} // namespace

int main() {
    {
        WorldItemContactTrackerV1 t;
        const auto first = t.advance(hero, true, {potion}, all_available());
        const auto next = t.advance(hero, true, {potion}, all_available());
        check(first.empty() && next.size() == 1 && next[0] == potion,
              "moving contact picks up on the following update, not the same frame");
    }
    {
        WorldItemContactTrackerV1 t;
        t.advance(hero, false, {potion}, all_available());
        const auto next = t.advance(hero, false, {}, all_available());
        check(next.empty(), "standing still on the item (contact while not moving) does not pick it up");
    }
    {
        WorldItemContactTrackerV1 t;
        t.advance(hero, false, {potion}, all_available());   // stopped on it
        t.advance(hero, true, {potion}, all_available());     // walks while still on it: no new begin
        const auto next = t.advance(hero, true, {potion}, all_available());
        check(next.empty(), "contact that did not begin while moving is not picked up");
    }
    {
        WorldItemContactTrackerV1 t;
        t.advance(hero, true, {potion}, all_available());                    // contact begins while moving
        const auto consumed = t.advance(hero, true, {}, all_available());    // next update runs Interact
        check(consumed.size() == 1 && consumed[0] == potion, "moving contact is consumed on the next update");
        t.advance(hero, true, {potion}, all_available());                    // walks back on while moving
        const auto back = t.advance(hero, true, {potion}, all_available());
        check(back.size() == 1 && back[0] == potion, "re-entering while moving is a new contact");
        const auto after = t.advance(hero, true, {potion}, all_available());
        check(after.empty(), "one pickup per contact, not repeated while still touching");
    }
    {
        WorldItemContactTrackerV1 t;
        t.advance(hero, true, {potion}, all_available());
        const auto due = t.advance(hero, true, {}, [](WorldItemContactTrackerV1::ItemId id) { return id != potion; });
        check(due.empty(), "item looted or removed before its update is not picked up");
    }
    {
        WorldItemContactTrackerV1 t;
        const auto a = t.advance(hero, true, {potion, gold}, all_available());
        const auto b = t.advance(hero, true, {potion, gold, other_item}, all_available());
        const std::set<WorldItemContactTrackerV1::ItemId> got(b.begin(), b.end());
        check(a.empty() && got == std::set<WorldItemContactTrackerV1::ItemId>{potion, gold},
              "two items touched together are both collected; the third new contact is stored");
    }
    {
        WorldItemContactTrackerV1 t;
        t.advance(hero, true, {potion}, all_available());
        const auto other = t.advance(2, true, {}, all_available());
        check(other.empty(), "a different character clears the stored contact");
    }

    std::printf("%s\n", failures == 0 ? "world item contact tests passed" : "world item contact tests FAILED");
    return failures == 0 ? 0 : 1;
}
