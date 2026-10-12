#pragma once

#include "../../character_state.hpp"
#include <functional>
#include <cstdint>
#include <string>

namespace dh::foundation::equipment_menu::confirmation_v1 {

enum class Action { none, transmute, drop };
enum class Button { other, yes, no };
enum class Outcome { swallowed, cancelled, confirmed, rejected };

// Captured from the currently admitted Details-page command. The snapshot is
// deliberately the canonical inventory row, not a display-list index.
struct Request {
    Action action = Action::none;
    std::string instance_id;
    std::string definition_id;
    std::uint32_t quantity = 0;
    unsigned source_slot = 10;
    std::int32_t transmute_value = 0;
    std::uint64_t source_visit_epoch = 0;
    // Full value copy; in the integration branch this includes optional
    // source metadata (value, identified state and ordered power names).
    InventoryItem admitted_item;
};

struct Release {
    Outcome outcome = Outcome::swallowed;
    Request request;
};

using CanonicalCurrentCheck = std::function<bool(const Request&, const InventoryItem&, std::string&)>;

class Pending {
public:
    bool open(Action, const InventoryItem&, unsigned source_slot,
              std::int32_t transmute_value, std::uint64_t source_visit_epoch,
              std::string& error);
    bool visible() const noexcept { return request_.action != Action::none; }
    const Request& request() const noexcept { return request_; }
    // Every pointer release is swallowed while this modal is open. No clears
    // it; Yes is one-shot and requires exact still-owned canonical instance.
    Release release(Button, const CharacterState&, std::uint64_t current_source_visit_epoch,
                    const CanonicalCurrentCheck&,
                    std::string& error);
    void cancel() noexcept { request_ = {}; }
private:
    Request request_;
};

} // namespace dh::foundation::equipment_menu::confirmation_v1
