#include "../character_aggro_target_search.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>

namespace {
using namespace dh2::character::aggro_search;

struct ActorFixture {
    GameObject object{};
    Character character{};
    bool has_character = true;
    bool zonable = false;
    bool can_interact = true;
    float interaction_radius = 0.0f;
    std::uint32_t relation_event = 0;
};

struct RoomFixture {
    Room room{};
    ObjectEntry sentinel{};
    std::array<ObjectEntry, 16> entries{};
};

struct Call { Operation operation; std::uintptr_t subject; std::uintptr_t other; };

struct Fixture {
    Character* owner = nullptr;
    float owner_melee_radius = 0.0f;
    std::vector<ActorFixture*> actors;
    std::vector<Call> calls;
    Operation fail_operation = static_cast<Operation>(0);
    std::uintptr_t fail_subject = 0;
};

bool check(bool ok, const char* message) {
    if (ok) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

ActorFixture* find(Fixture* fixture, std::uintptr_t identity) {
    for (auto* actor : fixture->actors) {
        if (actor->object.identity == identity) return actor;
    }
    return nullptr;
}

int invoke(void* context, const Request* request, Response* response) {
    auto* fixture = static_cast<Fixture*>(context);
    fixture->calls.push_back({static_cast<Operation>(request->operation),
                              request->subject, request->other});
    *response = {};
    const auto operation = static_cast<Operation>(request->operation);
    if (operation == fixture->fail_operation &&
        (!fixture->fail_subject || fixture->fail_subject == request->subject)) {
        return -1;
    }
    switch (operation) {
        case resolve_character: {
            auto* actor = find(fixture, request->subject);
            response->word = actor && actor->has_character
                                 ? reinterpret_cast<std::uintptr_t>(&actor->character)
                                 : 0;
            return 0;
        }
        case is_zonable: {
            auto* actor = find(fixture, request->subject);
            response->word = actor && actor->zonable;
            return 0;
        }
        case is_interactive: {
            auto* actor = find(fixture, request->subject);
            response->word = actor && actor->can_interact &&
                             request->other == fixture->owner->object->identity;
            return 0;
        }
        case interaction_radius: {
            auto* actor = find(fixture, request->subject);
            if (!actor) return -1;
            response->number = actor->interaction_radius;
            return 0;
        }
        case ai_melee_radius:
            if (request->subject != fixture->owner->identity) return -1;
            response->number = fixture->owner_melee_radius;
            return 0;
    }
    return -1;
}

void set_actor(ActorFixture& actor, std::uintptr_t identity, float x, float y,
               std::int32_t property_word, std::uint32_t relation_event,
               float interaction_radius = 0.0f) {
    actor.object.identity = identity;
    actor.object.position[0] = x;
    actor.object.position[1] = y;
    actor.object.position[2] = 0.0f;
    actor.object.forward[0] = 0.0f;
    actor.object.forward[1] = 1.0f;
    actor.object.forward[2] = 0.0f;
    actor.object.visible = 1;
    actor.character.identity = identity;  // Character is the GameObject base address.
    actor.character.object = &actor.object;
    actor.character.source_word_1310 = property_word;
    actor.character.source_word_1314 = 0;
    actor.interaction_radius = interaction_radius;
    actor.relation_event = relation_event;
}

void set_room(RoomFixture& room, const std::vector<ActorFixture*>& actors) {
    room.sentinel = {&room.sentinel, nullptr};
    room.room.objects = &room.sentinel;
    ObjectEntry* tail = &room.sentinel;
    for (std::size_t i = 0; i < actors.size(); ++i) {
        room.entries[i] = {&room.sentinel, &actors[i]->object};
        tail->next = &room.entries[i];
        tail = &room.entries[i];
    }
}

bool contains_call(const std::vector<Call>& calls, Operation op,
                   std::uintptr_t subject) {
    for (const auto& call : calls) {
        if (call.operation == op && call.subject == subject) return true;
    }
    return false;
}

bool source_query_and_event_order() {
    ActorFixture owner{}, enemy{}, friend_actor{}, neutral{}, gated{},
        higher_property{}, noncharacter{}, invisible{}, noninteractive{},
        outside{}, boundary{};
    set_actor(owner, 100, 0, 0, 0, 0);
    owner.character.source_word_1314 = 20;
    owner.object.visible = 1;
    owner.object.has_target_position = 0;
    set_actor(enemy, 2, 0, 12, 10, 9, 3);
    set_actor(friend_actor, 3, 0, 22, 20, 7, 1);
    set_actor(neutral, 4, 0, -32, 0, 8, 2);  // 2π admits the rear hemisphere.
    set_actor(gated, 5, 0, 4, 0, 9);
    gated.zonable = true;
    gated.object.character_2ee = 1;
    gated.object.character_2f0 = 0;
    set_actor(higher_property, 6, 0, 5, 21, 9);
    set_actor(noncharacter, 7, 0, 6, 0, 9);
    noncharacter.has_character = false;
    set_actor(invisible, 8, 0, 7, 0, 9);
    invisible.object.visible = 0;
    set_actor(noninteractive, 9, 0, 8, 0, 9);
    noninteractive.can_interact = false;
    set_actor(outside, 10, 0, 250, 0, 9);
    set_actor(boundary, 11, 0, 103, 0, 7, 1);

    RoomFixture first{}, second{};
    set_room(first, {&neutral, &gated, &invisible, &enemy, &owner});
    set_room(second, {&noncharacter, &friend_actor, &higher_property,
                      &noninteractive, &outside, &boundary});
    first.room.next = &second.room;
    Room sentinel{&first.room, nullptr};
    second.room.next = &sentinel;
    RoomRegistry registry{&sentinel};

    Fixture fixture{};
    fixture.owner = &owner.character;
    fixture.owner_melee_radius = 2.0f;
    fixture.actors = {&owner, &enemy, &friend_actor, &neutral, &gated,
                      &higher_property, &noncharacter, &invisible,
                      &noninteractive, &outside, &boundary};
    Services services{&fixture, invoke};
    std::array<TargetInfo, 16> heap{};
    TargetList list{};
    bool ok = true;
    ok &= check(dh2_aggro_target_list_init(&list, heap.data(), heap.size(),
                                           &owner.character) == complete,
                "initialize source aggro target list");
    ok &= check(dh2_aggro_target_search(&list, &registry, 100.0f,
                                       6.2831853071795864769f, &services) == complete,
                "run filter-2/all-flags/full-cone source search");
    ok &= check(list.count == 4,
                "view radius rejects distant candidate but includes exact boundary");

    const std::array<std::uintptr_t, 4> expected_ids{2, 3, 4, 11};
    const std::array<std::uint32_t, 4> expected_events{9, 7, 8, 7};
    std::array<std::uintptr_t, 4> actual_ids{};
    std::array<std::uint32_t, 4> actual_events{};
    std::array<float, 4> expected_distances{7.0f, 19.0f, 28.0f, 100.0f};
    for (std::size_t i = 0; i < expected_ids.size(); ++i) {
        TargetInfo result{};
        ok &= check(dh2_aggro_target_pop(&list, &result) == complete,
                    "pop an ordered source candidate");
        actual_ids[i] = result.object_identity;
        auto* actor = find(&fixture, result.object_identity);
        actual_events[i] = actor ? actor->relation_event : 0;
        ok &= check(result.character_identity == expected_ids[i],
                    "candidate keeps original Character/GameObject base identity");
        ok &= check(std::fabs(result.distance - expected_distances[i]) < 0.0001f,
                    "distance subtracts both source radii and includes boundary");
        ok &= check(result.flags == 1U,
                    "filter-2 results are Character TargetInfo entries");
    }
    ok &= check(actual_ids == expected_ids,
                "closest sort orders candidates independent of room order");
    ok &= check(actual_events == expected_events,
                "friend/neutral/enemy dispatch remains in popped search order");
    ok &= check(list.count == 0 && dh2_aggro_target_pop(&list, &heap[0]) == 1,
                "empty pop reports no candidate");

    // Source GetChar runs for every room entry, including invisible, self and
    // non-character objects. No dead/enemy/player/group service exists here:
    // the 0x7fffffff mask bypasses those narrower _IsCharacterValid gates.
    const std::array<std::uintptr_t, 11> resolved{4, 5, 8, 2, 100, 7,
                                                 3, 6, 9, 10, 11};
    std::vector<std::uintptr_t> resolved_actual;
    for (const auto& call : fixture.calls) {
        if (call.operation == resolve_character) resolved_actual.push_back(call.subject);
    }
    ok &= check(resolved_actual.size() == resolved.size(),
                "source resolver called once per registry entry");
    for (std::size_t i = 0; i < resolved.size(); ++i) {
        ok &= check(resolved_actual[i] == resolved[i],
                    "room iteration and resolver order are preserved");
    }
    ok &= check(!contains_call(fixture.calls, is_interactive, 5),
                "source gate rejects its matching 2ee/2f0 branch before interact");
    ok &= check(!contains_call(fixture.calls, interaction_radius, 6),
                "all-flags query still rejects property word above owner threshold");
    ok &= check(!contains_call(fixture.calls, interaction_radius, 7),
                "filter 2 excludes non-character game objects");
    ok &= check(!contains_call(fixture.calls, interaction_radius, 9),
                "source IsInteractive false excludes candidate");
    return ok;
}

bool service_failure_and_invalid_inputs() {
    ActorFixture owner{}, candidate{};
    set_actor(owner, 100, 0, 0, 0, 0);
    owner.character.source_word_1314 = 20;
    set_actor(candidate, 2, 0, 5, 0, 9);
    RoomFixture room{};
    set_room(room, {&candidate});
    Room sentinel{&room.room, nullptr};
    room.room.next = &sentinel;
    RoomRegistry registry{&sentinel};
    Fixture fixture{};
    fixture.owner = &owner.character;
    fixture.actors = {&owner, &candidate};
    fixture.fail_operation = interaction_radius;
    Services services{&fixture, invoke};
    std::array<TargetInfo, 4> heap{};
    TargetList list{};
    bool ok = dh2_aggro_target_list_init(&list, heap.data(), heap.size(),
                                         &owner.character) == complete;
    ok &= check(dh2_aggro_target_search(&list, &registry, 100.0f, 6.2831855f,
                                       &services) == source_service_failed,
                "service failure is surfaced, never fabricated as a candidate");

    TargetList invalid{};
    std::array<TargetInfo, 4> bad_heap{};
    ok &= check(dh2_aggro_target_list_init(&invalid, bad_heap.data(), 0,
                                          &owner.character) == invalid_argument,
                "zero-capacity output is rejected");
    ok &= check(dh2_aggro_target_search(&list, &registry,
                                       std::numeric_limits<float>::quiet_NaN(),
                                       6.2831855f, &services) == invalid_argument,
                "non-finite range rejected before service calls");
    return ok;
}

}  // namespace

int main() {
    bool ok = true;
    ok &= source_query_and_event_order();
    ok &= service_failure_and_invalid_inputs();
    if (!ok) return 1;
    std::puts("character aggro target search host checks passed: 2 suites; 4 ordered source candidates");
    return 0;
}
