#include "quest_events_v1.hpp"

namespace dh::foundation::quest_runtime {
namespace {
QuestEventSink& active_sink() {
    static QuestEventSink sink;
    return sink;
}
} // namespace

QuestEventSink bind_quest_event_sink(QuestEventSink sink) {
    auto previous = std::move(active_sink());
    active_sink() = std::move(sink);
    return previous;
}

bool raise_quest_event(const QuestEvent& event) {
    const auto& sink = active_sink();
    if (!sink) return false;
    sink(event);
    return true;
}

} // namespace dh::foundation::quest_runtime
