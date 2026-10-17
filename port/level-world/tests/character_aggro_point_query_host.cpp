#include "../character_aggro_character_list.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace as = dh2::character::aggro_search;
namespace al = dh2::character::aggro_character_list;

namespace {

constexpr std::uint32_t kActorBase = 100;
constexpr std::uint32_t kFlagCharacter = 1U << 0;
constexpr std::uint32_t kFlagVisible = 1U << 1;
constexpr std::uint32_t kFlagZonable = 1U << 2;
constexpr std::uint32_t kFlagZoned = 1U << 3;
constexpr std::uint32_t kFlagInZone = 1U << 4;
constexpr std::uint32_t kFlagInteractive = 1U << 5;
constexpr std::uint32_t kFlagTargetPosition = 1U << 6;

struct DiskCase {
    std::uint32_t name_bytes;
    std::uint32_t actor_count;
    std::uint32_t list_count;
    std::uint32_t owner_index;
    std::uint32_t mutate_index;
    float center[3];
    float forward[3];
    float radius;
    float cone;
    float melee_radius;
};

struct DiskActor {
    std::int32_t index;
    std::uint32_t flags;
    std::uint32_t word1310;
    std::uint32_t word1314;
    float interaction_radius;
    float position[3];
    float target_position[3];
};

struct DiskResult {
    std::uint32_t index;
    std::uint32_t distance_bits;
    std::uint32_t angle_bits;
    std::uint32_t flags;
    std::uint32_t reserved;
};

struct DiskTrace {
    std::uint32_t operation;
    std::uint32_t index;
};

struct Actor {
    as::GameObject object{};
    as::Character character{};
    std::int32_t source_index = -1;
    bool zonable = false;
    bool interactive = true;
    float interaction_radius = 0.0f;
};

struct Node {
    Node* next = nullptr;
    as::GameObject* object = nullptr;
    as::Character* character = nullptr;
};

struct Cursor {
    Node* sentinel = nullptr;
    Node* current = nullptr;
    std::vector<Node*> by_actor;
};

struct ServiceCall { std::uint32_t operation; std::uint32_t index; };

struct ServicesContext {
    std::vector<Actor>* actors = nullptr;
    Cursor* cursor = nullptr;
    std::vector<ServiceCall> trace;
    std::uint32_t owner_index = 0;
    std::uint32_t mutate_index = std::numeric_limits<std::uint32_t>::max();
    float melee_radius = 0.0f;
};

template<class T>
bool read(std::istream& in, T& value) {
    return static_cast<bool>(in.read(reinterpret_cast<char*>(&value), sizeof(value)));
}

bool read_bytes(std::istream& in, void* out, std::size_t count) {
    return static_cast<bool>(in.read(static_cast<char*>(out),
                                     static_cast<std::streamsize>(count)));
}

int cursor_reset(void* raw) {
    auto* cursor = static_cast<Cursor*>(raw);
    if (!cursor || !cursor->sentinel || !cursor->sentinel->next) return -1;
    cursor->current = cursor->sentinel->next;
    return as::complete;
}

int cursor_at_end(void* raw, std::uint32_t* output) {
    auto* cursor = static_cast<Cursor*>(raw);
    if (!cursor || !cursor->sentinel || !cursor->current || !output) return -1;
    *output = cursor->current == cursor->sentinel;
    return as::complete;
}

int cursor_get(void* raw, as::GameObject** output) {
    auto* cursor = static_cast<Cursor*>(raw);
    if (!cursor || !cursor->current || cursor->current == cursor->sentinel || !output) return -1;
    *output = cursor->current->object;
    return as::complete;
}

int cursor_get_char(void* raw, as::Character** output) {
    auto* cursor = static_cast<Cursor*>(raw);
    if (!cursor || !cursor->current || cursor->current == cursor->sentinel || !output) return -1;
    *output = cursor->current->character;
    return as::complete;
}

int cursor_next(void* raw) {
    auto* cursor = static_cast<Cursor*>(raw);
    if (!cursor || !cursor->current || cursor->current == cursor->sentinel ||
        !cursor->current->next) return -1;
    cursor->current = cursor->current->next;
    return as::complete;
}

int invoke(void* raw, const as::Request* request, as::Response* response) {
    auto* context = static_cast<ServicesContext*>(raw);
    if (!context || !request || !response) return -1;
    *response = {};
    if (request->operation == as::ai_melee_radius) {
        if (request->subject != context->actors->at(context->owner_index).character.identity) return -1;
        context->trace.push_back({request->operation,
            static_cast<std::uint32_t>(context->actors->at(context->owner_index).source_index)});
        response->number = context->melee_radius;
        return as::complete;
    }
    if (request->subject < kActorBase) return -1;
    const auto index = static_cast<std::uint32_t>(request->subject - kActorBase);
    if (index >= context->actors->size()) return -1;
    auto& actor = context->actors->at(index);
    context->trace.push_back({request->operation, index});
    switch (request->operation) {
        case as::is_zonable:
            response->word = actor.zonable;
            return as::complete;
        case as::is_interactive:
            if (request->other != context->actors->at(context->owner_index).object.identity) return -1;
            response->word = actor.interactive;
            if (index == context->mutate_index && context->cursor &&
                index < context->cursor->by_actor.size() && context->cursor->by_actor[index]) {
                context->cursor->by_actor[index]->next = context->cursor->sentinel;
                context->trace.push_back({UINT32_MAX,
                    static_cast<std::uint32_t>(actor.source_index)});
            }
            return as::complete;
        case as::interaction_radius:
            response->number = actor.interaction_radius;
            return as::complete;
        default:
            return -1;
    }
}

bool write_u32(std::ostream& out, std::uint32_t value) {
    return static_cast<bool>(out.write(reinterpret_cast<const char*>(&value), sizeof(value)));
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) return 2;
    std::ifstream input(argv[1], std::ios::binary);
    std::ofstream output(argv[2], std::ios::binary | std::ios::trunc);
    if (!input || !output) return 3;
    std::array<char, 4> magic{};
    std::uint32_t count = 0;
    if (!read_bytes(input, magic.data(), magic.size()) || magic != std::array<char, 4>{'C','P','Q','1'} ||
        !read(input, count) || count > 100) return 4;
    output.write("CPO1", 4);
    if (!write_u32(output, count)) return 5;

    for (std::uint32_t case_index = 0; case_index < count; ++case_index) {
        DiskCase disk{};
        if (!read(input, disk) || disk.actor_count == 0 || disk.actor_count > 128 ||
            disk.list_count > disk.actor_count || disk.owner_index >= disk.actor_count) return 6;
        std::string name(disk.name_bytes, '\0');
        if (!read_bytes(input, name.data(), name.size())) return 7;
        std::vector<DiskActor> actor_inputs(disk.actor_count);
        for (auto& actor : actor_inputs) if (!read(input, actor)) return 8;
        std::vector<std::uint32_t> list_indices(disk.list_count);
        for (auto& item : list_indices) if (!read(input, item) || item >= disk.actor_count) return 9;

        std::vector<Actor> actors(disk.actor_count);
        for (std::uint32_t i = 0; i < disk.actor_count; ++i) {
            const auto& src = actor_inputs[i];
            auto& dst = actors[i];
            const auto identity = static_cast<std::uintptr_t>(kActorBase + i);
            dst.object.identity = identity;
            for (int axis = 0; axis < 3; ++axis) {
                dst.object.position[axis] = src.position[axis];
                dst.object.target_position[axis] = src.target_position[axis];
                dst.object.forward[axis] = disk.forward[axis];
            }
            dst.object.visible = (src.flags & kFlagVisible) != 0;
            dst.object.has_target_position = (src.flags & kFlagTargetPosition) != 0;
            dst.object.character_2ee = (src.flags & kFlagZoned) != 0;
            dst.object.character_2f0 = (src.flags & kFlagInZone) != 0;
            dst.source_index = src.index;
            dst.zonable = (src.flags & kFlagZonable) != 0;
            dst.interactive = (src.flags & kFlagInteractive) != 0;
            dst.interaction_radius = src.interaction_radius;
            dst.character.identity = identity;
            dst.character.object = &dst.object;
            dst.character.source_word_1310 = static_cast<std::int32_t>(src.word1310);
            dst.character.source_word_1314 = static_cast<std::int32_t>(src.word1314);
        }
        auto& owner = actors[disk.owner_index];
        // The original fixture passes query center as an explicit argument.
        // The linked kernel reads the owner's source position, so project that
        // same fact onto the owner while retaining candidate positions verbatim.
        for (int axis = 0; axis < 3; ++axis) {
            owner.object.position[axis] = disk.center[axis];
            owner.object.target_position[axis] = disk.center[axis];
        }
        owner.object.has_target_position = 0;
        owner.character.source_word_1314 = static_cast<std::int32_t>(actor_inputs[disk.owner_index].word1314);
        std::vector<Node> nodes(disk.list_count + 1);
        Cursor cursor{};
        cursor.sentinel = &nodes.back();
        cursor.current = cursor.sentinel;
        cursor.by_actor.resize(disk.actor_count, nullptr);
        for (std::uint32_t i = 0; i < disk.list_count; ++i) {
            const auto actor_index = list_indices[i];
            auto& node = nodes[i];
            node.next = i + 1 < disk.list_count ? &nodes[i + 1] : cursor.sentinel;
            node.object = &actors[actor_index].object;
            node.character = (actor_inputs[actor_index].flags & kFlagCharacter)
                                 ? &actors[actor_index].character : nullptr;
            cursor.by_actor[actor_index] = &node;
        }
        cursor.sentinel->next = disk.list_count ? &nodes[0] : cursor.sentinel;
        std::vector<as::TargetInfo> heap(disk.actor_count + 1);
        as::TargetList list{};
        if (dh2_aggro_target_list_init(&list, heap.data(), static_cast<std::uint32_t>(heap.size()),
                                       &owner.character) != as::complete) return 10;
        al::ObjectListMethods methods{&cursor, cursor_reset, cursor_at_end,
                                      cursor_get, cursor_get_char, cursor_next};
        ServicesContext context{&actors, &cursor, {}, disk.owner_index,
            disk.mutate_index, disk.melee_radius};
        const as::Services services{&context, invoke};
        if (dh2_aggro_target_search_object_list(&list, &methods, disk.radius,
                                                disk.cone, &services) != as::complete) return 11;
        std::vector<DiskResult> results;
        while (list.count) {
            as::TargetInfo row{};
            if (dh2_aggro_target_pop(&list, &row) != as::complete ||
                row.object_identity < kActorBase) return 12;
            const auto actor_index = static_cast<std::uint32_t>(row.object_identity - kActorBase);
            results.push_back({static_cast<std::uint32_t>(actors.at(actor_index).source_index),
                [&] { std::uint32_t v; std::memcpy(&v, &row.distance, sizeof(v)); return v; }(),
                [&] { std::uint32_t v; std::memcpy(&v, &row.angle, sizeof(v)); return v; }(),
                row.flags, row.reserved});
        }
        if (!write_u32(output, static_cast<std::uint32_t>(results.size()))) return 13;
        for (const auto& row : results) if (!output.write(reinterpret_cast<const char*>(&row), sizeof(row))) return 14;
        if (!write_u32(output, static_cast<std::uint32_t>(context.trace.size()))) return 15;
        for (const auto& row : context.trace) {
            const DiskTrace record{row.operation, row.index};
            if (!output.write(reinterpret_cast<const char*>(&record), sizeof(record))) return 16;
        }
    }
    return input.peek() == std::char_traits<char>::eof() && output ? 0 : 17;
}
