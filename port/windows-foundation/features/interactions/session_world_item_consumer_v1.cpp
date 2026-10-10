#include "session_world_item_consumer_v1.hpp"
#include <utility>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool same_record(const loot::RuntimeWorldItemRecordV1& a,
                 const loot::RuntimeWorldItemRecordV1& b) noexcept {
    return a.source_actor == b.source_actor && a.killer_actor == b.killer_actor &&
           a.loot_table == b.loot_table && a.item_id == b.item_id &&
           a.quantity == b.quantity && a.authored_item == b.authored_item &&
           a.authored_entry == b.authored_entry &&
           a.resolved_gold_value == b.resolved_gold_value;
}
}

bool SessionWorldItemConsumerV1::enumerate(
    std::vector<SessionWorldItemPresentationV1>& output,
    std::string& error) const {
    output.clear();
    error.clear();
    if (!audiovisual_) return fail(error, "Actual ItemAudioVisualTable borrow is unavailable");

    std::vector<loot::RuntimeWorldItemRenderV1> rendered;
    if (!items_.render_items(rendered, error)) return false;
    const auto& rows = audiovisual_.rows();
    const auto& names = audiovisual_.names();
    if (rows.size() != names.size())
        return fail(error, "ItemAudioVisualTable row/name identities diverged");

    std::vector<SessionWorldItemPresentationV1> next;
    try {
        next.reserve(rendered.size());
        for (const auto& view : rendered) {
            loot::RuntimeWorldItemEntryV1 stored;
            if (!items_.inspect(view.identity, stored, error)) return false;
            if (!stored.authored_item || stored.source_outcome.item_id != view.item_id ||
                stored.source_outcome.authored_item != stored.authored_item ||
                stored.source_outcome.quantity != stored.quantity ||
                stored.quantity != view.quantity || stored.source_position != view.position)
                return fail(error, "World-item view no longer matches its exact retained source record");
            const auto& item = *stored.authored_item;
            SessionWorldItemPresentationV1 row;
            row.identity = view.identity;
            row.source_record = stored.source_outcome;
            row.item_identifier = view.item_identifier;
            row.exact_icon_name = view.exact_icon_name;
            row.source_name_text_oid = view.source_name_text_oid;
            row.source_audio_visual_id = item.record.words[21];
            row.quantity = view.quantity;
            row.position = view.position;
            row.has_exact_icon = view.has_exact_icon;

            // Item::AudioVisualID is its actual source category index. A
            // missing/out-of-range ID stays explicitly unresolved; no item
            // name, icon, or nearby model is substituted.
            if (row.source_audio_visual_id >= 0 &&
                static_cast<std::size_t>(row.source_audio_visual_id) < rows.size()) {
                const auto index = static_cast<std::size_t>(row.source_audio_visual_id);
                row.source_audio_visual_name = names[index];
                row.source_model_resource_uri = source_item_model_resource_uri_v1;
                row.source_visual_uri = rows[index].visual;
                row.has_source_visual = !row.source_visual_uri.empty();
            }
            next.push_back(std::move(row));
        }
    } catch (...) {
        return fail(error, "World-item source presentation allocation failed");
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool SessionWorldItemConsumerV1::interact_selected(
    ActorId current_player, bool is_local_player, const ActorState* player,
    const SessionWorldItemPresentationV1& selected,
    const loot::RuntimeWorldItemInteractionServicesV1& services,
    loot::RuntimeWorldItemInteractionReceiptV1& receipt,
    std::string& error) const {
    loot::RuntimeWorldItemEntryV1 stored;
    if (!items_.inspect(selected.identity, stored, error)) return false;
    if (!same_record(selected.source_record, stored.source_outcome) ||
        selected.identity == loot::invalid_runtime_world_item_v1)
        return fail(error, "Selected world-item view is stale or belongs to another store");
    loot::RuntimeWorldItemInteractionV1 interaction;
    return interaction.dispatch_live_player(
        current_player, is_local_player, player,
        {current_player, selected.identity}, services, items_, receipt, error);
}

} // namespace dh::foundation::interactions
