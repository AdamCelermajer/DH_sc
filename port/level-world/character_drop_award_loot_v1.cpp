#include "character_drop_award_loot_v1.hpp"

#include <algorithm>
#include <exception>
#include <stdexcept>

namespace dh2::character_drop_award_loot_v1 {
namespace {

bool fail(bool ok, std::string& error, const char* fallback) {
    if (!ok && error.empty()) error = fallback;
    return !ok;
}

} // namespace

Runtime::Runtime(Request request, Services services)
    : request_(request), services_(services) {
    const auto items_address = reinterpret_cast<std::uintptr_t>(request.items);
    if ((request.item_count && (!request.items ||
         items_address % alignof(StagedItem) != 0 ||
         request.item_count > (UINTPTR_MAX - items_address) / sizeof(StagedItem))) ||
        request.item_count > 65536 || !request.killer_character ||
        !services.scatter || !services.spawn || !services.auto_pickup)
        throw std::invalid_argument("Invalid source DropAndAwardLoot bindings");
    for (std::size_t i = 0; i < request.item_count; ++i) {
        if (!request.items[i].item_identity)
            throw std::invalid_argument("Source DropAndAwardLoot item identity is null");
        for (std::size_t j = 0; j < i; ++j)
            if (request.items[i].item_identity == request.items[j].item_identity)
                throw std::invalid_argument("Source DropAndAwardLoot repeats a staged Item");
    }
    snapshot_.item_count = request.item_count;
}

Status Runtime::run(Result* output, std::string& error) {
    if (busy_) return Status::busy;
    if (consumed_) return Status::consumed;
    if (!output) return Status::invalid_argument;
    *output = {};
    output->item_count = request_.item_count;
    error.clear();

    busy_ = true;
    struct BusyReset { bool& value; ~BusyReset() { value = false; } } reset{busy_};
    consumed_ = true;
    try {
        for (std::size_t i = 0; i < request_.item_count; ++i) {
            output->current_index = i;
            const auto& item = request_.items[i];
            std::array<float, 3> spawn_position{}, destination{};

            ++output->scatter_attempted;
            if (fail(services_.scatter(services_.context, item.item_identity,
                                       spawn_position, destination, error), error,
                     "Source ItemObject::_GetRandomDropPos failed")) {
                snapshot_ = *output;
                return Status::provider_failed;
            }
            ++output->scatter_completed;

            std::uintptr_t world_item = 0;
            ++output->spawn_attempted;
            if (fail(services_.spawn(services_.context, item.item_identity,
                                     spawn_position, destination, world_item, error), error,
                     "Source ItemManager::Spawn failed")) {
                snapshot_ = *output;
                return Status::provider_failed;
            }
            if (!world_item) {
                error = "Source ItemManager::Spawn returned no active ItemObject";
                snapshot_ = *output;
                return Status::provider_failed;
            }
            ++output->spawn_completed;
            output->last_world_item_identity = world_item;

            if (item.pick_up_type == request_.automatic_pickup_type) {
                ++output->auto_pickup_attempted;
                if (fail(services_.auto_pickup(services_.context, world_item,
                                               request_.killer_character, error), error,
                         "Source ItemObject::_DoAutoPickupHack/Interact failed")) {
                    snapshot_ = *output;
                    return Status::provider_failed;
                }
                ++output->auto_pickup_completed;
            }
            ++output->items_completed;
        }
    } catch (const std::exception& exception) {
        error = exception.what();
    } catch (...) {
        error = "Source DropAndAwardLoot provider threw";
    }
    if (!error.empty()) {
        snapshot_ = *output;
        return Status::provider_failed;
    }
    snapshot_ = *output;
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_drop_award_loot_v1
