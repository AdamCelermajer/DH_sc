#include "character_ai_initialization.hpp"
#include <cstddef>

namespace dh2::character_ai_initialization {
namespace {
bool span(const void* p, std::size_t n, std::size_t alignment) {
    const auto at = reinterpret_cast<std::uintptr_t>(p);
    return p && at % alignment == 0 && at <= UINTPTR_MAX - n;
}
bool overlaps(const void* p, std::size_t n, const void* q, std::size_t m) {
    const auto a = reinterpret_cast<std::uintptr_t>(p), b = reinterpret_cast<std::uintptr_t>(q);
    return a <= b ? b - a < n : a - b < m;
}
void empty_tree(TreeHeader& h) {
    h.color = 0;
    h.parent = 0;
    h.left = h.right = reinterpret_cast<std::uintptr_t>(&h);
    h.count = 0;
}
}
Status construct(State* s, std::uintptr_t table, const Services* c, Result* out) {
    if (!span(s, sizeof(*s), alignof(State)) || !span(c, sizeof(*c), alignof(Services)) ||
        !span(out, sizeof(*out), alignof(Result)) || !table || !s->identity ||
        overlaps(s, sizeof(*s), c, sizeof(*c)) || overlaps(s, sizeof(*s), out, sizeof(*out)) ||
        overlaps(c, sizeof(*c), out, sizeof(*out))) return Status::invalid_argument;
    const auto services = *c;
    *out = {};
    s->dispatch_table = table;
    s->byte_55 = 1;
    s->word_08 = s->word_0c = 0;
    s->paused_18 = 0;
    s->active_ais_1c = s->alternate_ais_20 = 0;
    s->byte_24 = 0;
    s->pointer_28 = 0;
    s->byte_2c = 0;
    s->script_name_30 = 0;
    s->word_34 = 0;
    s->requested_target_3c = s->target_40 = s->last_target_44 = 0;
    s->sight_49 = 0;
    s->byte_4a = s->byte_4b = 1;
    s->sticky_4c = 0;
    s->targetable_4d = 1;
    s->master_50 = 0;
    s->byte_54 = 1;
    s->word_58 = 0;
    s->word_10 = s->word_14 = s->word_38 = UINT32_MAX;
    empty_tree(s->tree_5c);
    empty_tree(s->tree_7c);
    empty_tree(s->tree_94);
    const auto list = reinterpret_cast<std::uintptr_t>(&s->list_ac);
    s->list_ac.next = s->list_ac.previous = list;
    s->word_cc = UINT32_MAX;
    s->byte_d1 = 0;
    s->words_b4_to_c8.fill(0);
    s->byte_d0 = 0;
    s->group_info_identity = 0;
    if (!services.append_queue) return Status::service_unavailable;
    ++out->queue_calls;
    try {
        if (services.append_queue(services.context, s, s->identity)) return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    out->queued = 1;
    return Status::complete;
}
} // namespace dh2::character_ai_initialization
