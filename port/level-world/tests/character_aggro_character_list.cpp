#include "../character_aggro_character_list.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <vector>

namespace {
namespace al = dh2::character::aggro_character_list;
namespace as = dh2::character::aggro_search;

struct Actor {
    as::GameObject object{};
    as::Character character{};
    bool zonable = false;
    bool can_interact = true;
    float interaction_radius = 0.0f;
};

struct Call { as::Operation op; std::uintptr_t subject; std::uintptr_t other; };
struct Fixture {
    as::Character* owner = nullptr;
    std::vector<Actor*> actors;
    std::vector<Call> calls;
    al::Entry* mutate_current = nullptr;
    al::Entry* mutation_next = nullptr;
    as::Operation fail = static_cast<as::Operation>(0);
};

bool check(bool value, const char* message) {
    if (value) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

Actor* find(Fixture* f, std::uintptr_t identity) {
    for (auto* a : f->actors) if (a->object.identity == identity) return a;
    return nullptr;
}

int invoke(void* context, const as::Request* request, as::Response* response) {
    auto* f = static_cast<Fixture*>(context);
    const auto op = static_cast<as::Operation>(request->operation);
    f->calls.push_back({op, request->subject, request->other});
    *response = {};
    if (op == f->fail) return -1;
    switch (op) {
        case as::resolve_character: {
            auto* actor = find(f, request->subject);
            response->word = actor ? reinterpret_cast<std::uintptr_t>(&actor->character) : 0;
            return 0;
        }
        case as::is_zonable: {
            auto* actor = find(f, request->subject);
            response->word = actor && actor->zonable;
            return 0;
        }
        case as::is_interactive: {
            auto* actor = find(f, request->subject);
            response->word = actor && actor->can_interact &&
                             request->other == f->owner->object->identity;
            if (actor && f->mutate_current && request->subject == actor->object.identity) {
                f->mutate_current->next = f->mutation_next;
                f->mutate_current = nullptr;
            }
            return 0;
        }
        case as::interaction_radius: {
            auto* actor = find(f, request->subject);
            if (!actor) return -1;
            response->number = actor->interaction_radius;
            return 0;
        }
        case as::ai_melee_radius:
            if (request->subject != f->owner->identity) return -1;
            response->number = 2.0f;
            return 0;
    }
    return -1;
}

void set_actor(Actor& a, std::uintptr_t id, float x, float y, std::int32_t word) {
    a.object.identity = id;
    a.object.position[0] = x; a.object.position[1] = y; a.object.position[2] = 0.0f;
    a.object.forward[0] = 0.0f; a.object.forward[1] = 1.0f; a.object.forward[2] = 0.0f;
    a.object.visible = 1;
    a.character.identity = id;
    a.character.object = &a.object;
    a.character.source_word_1310 = word;
}

void set_entries(std::array<al::Entry, 8>& entries, al::Entry& sentinel,
                 const std::vector<Actor*>& actors) {
    sentinel = {&sentinel, nullptr};
    auto* tail = &sentinel;
    for (std::size_t i = 0; i < actors.size(); ++i) {
        entries[i] = {&sentinel, &actors[i]->character};
        tail->next = &entries[i];
        tail = &entries[i];
    }
}

void set_room(as::Room& room, as::ObjectEntry& sentinel,
              std::array<as::ObjectEntry, 8>& entries,
              const std::vector<Actor*>& actors) {
    sentinel = {&sentinel, nullptr};
    room.objects = &sentinel;
    auto* tail = &sentinel;
    for (std::size_t i = 0; i < actors.size(); ++i) {
        entries[i] = {&sentinel, &actors[i]->object};
        tail->next = &entries[i];
        tail = &entries[i];
    }
}

bool flat_list_matches_room_query() {
    Actor owner{}, near{}, far{}, gated{}, hidden{};
    set_actor(owner, 100, 0, 0, 0);
    owner.character.source_word_1314 = 20;
    set_actor(near, 2, 0, 8, 10); near.interaction_radius = 1.0f;
    set_actor(far, 3, 0, 18, 0);
    set_actor(gated, 4, 0, 4, 0); gated.zonable = true;
    gated.object.character_2ee = 1;
    set_actor(hidden, 5, 0, 5, 0); hidden.object.visible = 0;

    std::array<al::Entry, 8> nodes{};
    al::Entry sentinel{};
    set_entries(nodes, sentinel, {&owner, &near, &gated, &hidden, &far});
    al::CharacterList chars{};
    bool ok = check(al::dh2_aggro_character_list_init(&chars, &sentinel) == as::complete,
                    "bind flat ObjectManager Character sentinel");

    std::array<as::ObjectEntry, 8> room_nodes{};
    as::ObjectEntry room_sentinel{};
    as::Room room{};
    set_room(room, room_sentinel, room_nodes, {&owner, &near, &gated, &hidden, &far});
    as::Room room_end{&room, nullptr}; room.next = &room_end;
    as::RoomRegistry registry{&room_end};

    Fixture f{}; f.owner = &owner.character; f.actors = {&owner,&near,&far,&gated,&hidden};
    as::Services services{&f, invoke};
    std::array<as::TargetInfo, 8> room_heap{}, flat_heap{};
    as::TargetList room_targets{}, flat_targets{};
    ok &= check(as::dh2_aggro_target_list_init(&room_targets, room_heap.data(),
                                               room_heap.size(), &owner.character) == as::complete,
                "initialize reference room query");
    ok &= check(as::dh2_aggro_target_list_init(&flat_targets, flat_heap.data(),
                                               flat_heap.size(), &owner.character) == as::complete,
                "initialize source CharacterList query");
    ok &= check(as::dh2_aggro_target_search(&room_targets, &registry, 100.0f,
                                            6.2831855f, &services) == as::complete,
                "run historical room registry kernel");
    f.calls.clear();
    const int flat_status = al::dh2_aggro_target_search_character_list(&flat_targets, &chars,
                                                                        100.0f, 6.2831855f,
                                                                        &services);
    if (flat_status != as::complete) std::fprintf(stderr, "flat status=%d\n", flat_status);
    ok &= check(flat_status == as::complete,
                "run flat source CharacterList kernel");
    ok &= check(flat_targets.count == 2 && room_targets.count == 2,
                "CharacterList and one-room candidate totals agree");

    std::array<as::TargetInfo, 2> expected{};
    for (std::size_t i = 0; i < expected.size(); ++i) {
        ok &= check(as::dh2_aggro_target_pop(&room_targets, &expected[i]) == as::complete,
                    "pop reference room result");
        as::TargetInfo actual{};
        ok &= check(as::dh2_aggro_target_pop(&flat_targets, &actual) == as::complete,
                    "pop CharacterList result");
        ok &= check(actual.object_identity == expected[i].object_identity &&
                    actual.character_identity == expected[i].character_identity &&
                    std::fabs(actual.distance - expected[i].distance) < 1e-5f,
                    "candidate identity and adjusted distance match the existing kernel");
    }
    std::uint32_t resolved_calls = 0;
    for (const auto& call : f.calls) if (call.op == as::resolve_character) ++resolved_calls;
    ok &= check(resolved_calls == 0,
                "source CharacterList supplies Character directly; no room resolver fabricated");
    ok &= check(chars.current == &sentinel && chars.end == &sentinel,
                "search resets and consumes the complete source circular list");
    return ok;
}

bool source_next_observes_live_link() {
    Actor owner{}, first{}, skipped{};
    set_actor(owner, 100, 0, 0, 0); owner.character.source_word_1314 = 10;
    set_actor(first, 1, 0, 4, 0);
    set_actor(skipped, 2, 0, 5, 0);
    std::array<al::Entry, 8> nodes{};
    al::Entry sentinel{};
    set_entries(nodes, sentinel, {&first, &skipped});
    al::CharacterList chars{};
    bool ok = al::dh2_aggro_character_list_init(&chars, &sentinel) == as::complete;
    Fixture f{}; f.owner = &owner.character; f.actors = {&owner,&first,&skipped};
    f.mutate_current = &nodes[0]; f.mutation_next = &sentinel;
    as::Services services{&f, invoke};
    std::array<as::TargetInfo, 4> heap{};
    as::TargetList list{};
    ok &= check(as::dh2_aggro_target_list_init(&list, heap.data(), heap.size(),
                                               &owner.character) == as::complete,
                "initialize mutation query");
    const int status = al::dh2_aggro_target_search_character_list(&list, &chars, 100.0f,
                                                                   6.2831855f, &services);
    if (status != as::complete) std::fprintf(stderr, "mutation status=%d\n", status);
    ok &= check(status == as::complete,
                "query advances through the live CharacterList link");
    ok &= check(list.count == 1 && heap[0].object_identity == first.object.identity,
                "callback link mutation is observed by subsequent Next");
    std::uint32_t skipped_interactive = 0;
    for (const auto& call : f.calls) {
        if (call.op == as::is_interactive && call.subject == skipped.object.identity) {
            ++skipped_interactive;
        }
    }
    ok &= check(skipped_interactive == 0,
                "the now-unlinked next Character is not visited from an early snapshot");
    return ok;
}

bool cursor_and_failure_contracts() {
    Actor a{}; set_actor(a, 7, 0, 1, 0);
    std::array<al::Entry, 8> nodes{};
    al::Entry sentinel{}; set_entries(nodes, sentinel, {&a});
    al::CharacterList list{};
    bool ok = check(al::dh2_aggro_character_list_init(&list, &sentinel) == as::complete,
                    "initialize CharacterList cursor");
    std::uint32_t at_end = 1;
    as::GameObject* object = nullptr;
    as::Character* character = nullptr;
    ok &= check(al::dh2_aggro_character_list_at_end(&list, &at_end) == as::complete && !at_end,
                "AtEnd reports the first linked Character");
    ok &= check(al::dh2_aggro_character_list_get(&list, &object) == as::complete &&
                object == &a.object,
                "Get projects the list Character as a GameObject");
    ok &= check(al::dh2_aggro_character_list_get_char(&list, &character) == as::complete &&
                character == &a.character,
                "GetChar returns the source Character projection");
    ok &= check(al::dh2_aggro_character_list_next(&list) == as::complete &&
                al::dh2_aggro_character_list_at_end(&list, &at_end) == as::complete && at_end,
                "Next reaches the sentinel and AtEnd observes it");
    ok &= check(al::dh2_aggro_character_list_get(&list, &object) == as::invalid_argument,
                "Get at sentinel is rejected by the normalized adapter");

    Actor owner{}; set_actor(owner, 99, 0, 0, 0); owner.character.source_word_1314 = 10;
    al::Entry bad_sentinel{nullptr, nullptr};
    al::CharacterList bad{};
    ok &= check(al::dh2_aggro_character_list_init(&bad, &bad_sentinel) == as::invalid_argument,
                "broken sentinel topology is rejected");
    Fixture f{}; f.owner = &owner.character; f.actors = {&owner,&a}; f.fail = as::ai_melee_radius;
    as::Services services{&f, invoke};
    std::array<as::TargetInfo, 4> heap{};
    as::TargetList targets{};
    ok &= check(as::dh2_aggro_target_list_init(&targets, heap.data(), heap.size(),
                                               &owner.character) == as::complete,
                "initialize service failure query");
    ok &= check(al::dh2_aggro_target_search_character_list(&targets, &list, 10.0f,
                                                            6.2831855f, &services) == as::source_service_failed,
                "source virtual failure is surfaced before list mutation");
    return ok;
}

}  // namespace

int main() {
    bool ok = true;
    ok &= flat_list_matches_room_query();
    ok &= source_next_observes_live_link();
    ok &= cursor_and_failure_contracts();
    if (!ok) return 1;
    std::puts("flat CharacterList aggro host checks passed: 3 suites; live one-list traversal");
    return 0;
}
