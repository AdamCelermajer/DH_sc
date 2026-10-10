#pragma once

#include "../loot/runtime_world_item_interaction_v1.hpp"
#include "../../../game-data/loot_audiovisual_v8.hpp"

namespace dh::foundation::interactions {

// ItemObject::InitOnce at 0x3ECE80 loads this original resource, then takes
// the selected ItemAudioVisualTable row's Visual string as its scene xref.
inline constexpr const char* source_item_model_resource_uri_v1 =
    "data/3D/GameObjects/itemdrops.bdae";

// Root-facing view of one exact record already stored by the gameplay world's
// RuntimeWorldItemAdapterV1. Text/icon/resource values are source metadata;
// this projection does not load or invent render assets.
struct SessionWorldItemPresentationV1 {
    loot::RuntimeWorldItemIdV1 identity{loot::invalid_runtime_world_item_v1};
    loot::RuntimeWorldItemRecordV1 source_record;
    std::string item_identifier;
    std::string exact_icon_name;
    std::int32_t source_name_text_oid{};
    std::int32_t source_audio_visual_id{-1};
    std::string source_audio_visual_name;
    std::string source_model_resource_uri;
    std::string source_visual_uri;
    std::uint32_t quantity{};
    std::array<float, 3> position{};
    bool has_exact_icon{};
    bool has_source_visual{};
};

// Consumer over the caller's existing world-item store and exact original
// ItemAudioVisualTable borrow. It owns neither the store nor a second pickup
// authority. Selected interactions require the exact current projection ID.
class SessionWorldItemConsumerV1 {
    loot::RuntimeWorldItemAdapterV1& items_;
    dh2::data::LootAudioVisualV8::Borrow audiovisual_;

public:
    SessionWorldItemConsumerV1(loot::RuntimeWorldItemAdapterV1& items,
                               dh2::data::LootAudioVisualV8::Borrow audiovisual)
        : items_(items), audiovisual_(std::move(audiovisual)) {}

    bool enumerate(std::vector<SessionWorldItemPresentationV1>&,
                   std::string& error) const;

    // Caller must pass the exact selected view produced by enumerate() from
    // this same session/store. This makes no radius or target-selection rule;
    // the source caller remains responsible for the explicit Interact branch.
    bool interact_selected(ActorId current_player, bool is_local_player,
                           const ActorState* player,
                           const SessionWorldItemPresentationV1& selected,
                           const loot::RuntimeWorldItemInteractionServicesV1&,
                           loot::RuntimeWorldItemInteractionReceiptV1&,
                           std::string& error) const;
};

} // namespace dh::foundation::interactions
