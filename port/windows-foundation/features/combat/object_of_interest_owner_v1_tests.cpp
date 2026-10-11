// P16 CONTEXT: priority matrix for the source OOI loop (UpdateObjectOfInterest 0x3abb9c) and marker policy.
#include "object_of_interest_owner_v1.hpp"

#include <cmath>
#include <cstdio>
#include <set>

using namespace dh::foundation;

namespace {
constexpr ActorId player = 1;
constexpr ActorId enemy = 7;      // Character, type 8
constexpr ActorId npc = 8;        // Character, type 3
constexpr ActorId chest = 100;    // object, type 0
constexpr ActorId barrel = 101;   // object, type 8
constexpr ActorId item = 102;     // object, type -1 (not admitted)
constexpr ActorId trigger = 103;  // object, type 1

int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}

ObjectOfInterestOwnerStateV1 owner_at_origin() {
    ObjectOfInterestOwnerStateV1 s;
    s.owner = player;
    s.position = {0.0f, 0.0f, 0.0f};
    s.look = {0.0f, 1.0f, 0.0f};   // facing +Y
    s.melee_radius = 0.0f;
    return s;
}

// Object placed at a bearing (degrees from the look vector) and centre distance.
ObjectOfInterestCandidateV1 at(ActorId id, float degrees, float distance, bool character, int type,
                               bool eligible = true) {
    ObjectOfInterestCandidateV1 c;
    c.id = id;
    c.is_character = character;
    const float r = degrees * 3.14159265f / 180.0f;
    c.position = {distance * std::sin(r), distance * std::cos(r), 0.0f};
    c.interaction_type = type;
    c.eligible = eligible;
    return c;
}

ObjectOfInterestSelectionV1 select(const std::vector<ObjectOfInterestCandidateV1>& all) {
    return select_object_of_interest_v1(player, order_object_of_interest_candidates_v1(owner_at_origin(), all));
}

} // namespace

int main() {
    // --- Priority matrix (source queue: Characters first, then frontal angle; accept rules) ---
    {
        const auto s = select({at(chest, 0, 80, false, 0), at(enemy, 40, 60, true, 8)});
        check(s.object == enemy && s.interaction_type == 8, "enemy in range beats a frontal chest");
    }
    {
        const auto s = select({at(chest, 10, 80, false, 0), at(enemy, 0, 260, true, 8)});
        check(s.object == chest && s.interaction_type == 0, "enemy beyond 200 does not block chest");
    }
    {
        const auto s = select({at(chest, 10, 80, false, 0), at(enemy, 0, 60, true, 8, false)});
        check(s.object == chest, "ineligible enemy is not admitted; chest wins");
    }
    {
        const auto s = select({at(chest, 0, 80, false, 0), at(npc, 30, 60, true, 3)});
        check(s.object == npc && s.interaction_type == 3, "friendly NPC character is accepted before a chest");
    }
    {
        const auto s = select({at(chest, 0, 80, false, 0), at(barrel, 30, 60, false, 8)});
        check(s.object == chest && s.interaction_type == 0, "frontal chest accepted before a side barrel");
    }
    {
        const auto s = select({at(barrel, 10, 60, false, 8), at(chest, 30, 80, false, 0)});
        check(s.object == barrel && s.interaction_type == 8, "frontal barrel stays OOI and blocks the chest behind it");
    }
    {
        const auto s = select({at(item, 0, 20, false, -1), at(chest, 20, 80, false, 0)});
        check(s.object == chest, "item (type -1) never enters the queue");
    }
    {
        const auto s = select({at(item, 0, 20, false, -1)});
        check(s.object == invalid_actor_id && s.interaction_type == -1, "lone item gives no OOI");
    }
    {
        // Ranking is by frontal angle, not distance (correction to the B004 nearest rule).
        const auto s = select({at(barrel, 60, 20, false, 8), at(chest, 5, 150, false, 0)});
        check(s.object == chest, "more frontal far chest beats a near side object");
    }
    {
        // Trigger type 1 without owner link does not accept; it stays assigned and blocks the chest.
        const auto s = select({at(trigger, 0, 40, false, 1), at(chest, 10, 60, false, 0)});
        check(s.object == trigger && s.interaction_type == 1, "type 1 without owner link is assigned, not accepted");
    }
    {
        auto t = at(trigger, 0, 40, false, 1);
        t.type1_targets_owner = true;
        const auto s = select({t, at(chest, 10, 60, false, 0)});
        check(s.object == trigger, "type 1 with owner link is accepted");
    }
    {
        const auto s = select({at(enemy, 0, 60, true, 8), at(enemy + 1, 90, 60, true, 8)});
        check(s.object == enemy, "two enemies: the frontal one");
    }
    {
        const auto s = select({at(enemy + 1, 5, 60, true, 8), at(enemy, 5, 60, true, 8)});
        check(s.object == enemy, "equal angle: lower id wins (deterministic)");
    }

    // --- Range: centre distance - candidate radius - owner melee radius <= 200 ---
    {
        auto c = at(enemy, 0, 200.0f, true, 8);
        check(select({c}).object == enemy, "range edge 200 admitted");
        c = at(enemy, 0, 200.5f, true, 8);
        check(select({c}).object == invalid_actor_id, "range 200.5 rejected");
        c = at(enemy, 0, 210.0f, true, 8);
        c.radius = 10.0f;
        check(select({c}).object == enemy, "candidate radius is subtracted");
        auto owner = owner_at_origin();
        owner.melee_radius = 15.0f;
        ObjectOfInterestOwnerV1 o;
        o.update(owner, 0.5, {at(enemy, 0, 210.0f, true, 8)});
        check(o.object() == enemy, "owner melee radius is subtracted");
    }

    // --- Timer: 500 ms refresh ---
    {
        // The source timer starts at 0, so the first update refreshes at once (empty list here).
        ObjectOfInterestOwnerV1 o;
        const auto list = std::vector<ObjectOfInterestCandidateV1>{at(chest, 0, 80, false, 0)};
        o.update(owner_at_origin(), 0.0, {});
        o.update(owner_at_origin(), 0.4, list);
        check(o.object() == invalid_actor_id, "no OOI before the 500 ms refresh");
        o.update(owner_at_origin(), 0.1, list);
        check(o.object() == chest && o.interaction_type() == 0 && o.changed(), "refresh at 500 ms assigns the chest");
        o.update(owner_at_origin(), 0.2, list);
        check(o.object() == chest && !o.changed(), "OOI kept until the next refresh while still eligible");
    }
    {
        // Per-frame validity: an OOI that is no longer eligible is dropped at once; type stays until refresh.
        ObjectOfInterestOwnerV1 o;
        o.update(owner_at_origin(), 0.5, {at(chest, 0, 80, false, 0)});
        o.update(owner_at_origin(), 0.01, {at(chest, 0, 80, false, 0, false)});
        check(o.object() == invalid_actor_id, "OOI cleared when it stops being eligible");
        check(o.interaction_type() == 0, "cached type is kept until refresh (source only clears the OOI)");
    }
    {
        ObjectOfInterestOwnerV1 o;
        o.update(owner_at_origin(), 0.5, {at(chest, 0, 80, false, 0)});
        o.update(owner_at_origin(), 0.5, {at(chest, 0, 80, false, 0)});
        check(!o.changed() && o.object() == chest, "same OOI at refresh does not latch changed");
    }
    {
        ObjectOfInterestOwnerV1 o;
        o.update(owner_at_origin(), 0.5, {at(chest, 0, 80, false, 0)});
        auto other = owner_at_origin();
        other.owner = 2;
        o.update(other, 0.0, {});
        check(o.owner() == 2 && o.object() == invalid_actor_id, "owner change resets the state");
    }

    // --- Marker policy (B004): last target first, OOI fallback, render eligibility gate ---
    {
        const auto live = std::set<ActorId>{enemy};
        const auto gate = [&live](ActorId id) { return live.count(id) != 0; };
        check(resolve_rendered_target_marker_v1(player, true, 50, enemy, gate) == invalid_actor_id,
              "rejected last target hides the marker (no OOI fallback)");
        check(resolve_rendered_target_marker_v1(player, false, invalid_actor_id, enemy, gate) == enemy,
              "unknown last target falls back to the eligible OOI");
    }

    std::printf("%s\n", failures == 0 ? "object of interest owner tests passed" : "object of interest owner tests FAILED");
    return failures == 0 ? 0 : 1;
}
