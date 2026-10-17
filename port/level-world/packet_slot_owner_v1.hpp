#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace dh2::packet_slot_owner_v1 {

// CPacketManager static registry, shared across Level instances. Native
// callbacks remain pointer-width; this is not an ARM32 memory overlay.
struct Callbacks {
    std::uintptr_t write{}, read{}, acknowledged{}, lost{};
};
struct Slot {
    std::uint8_t registered{};
    Callbacks callbacks{};
};
class Owner {
public:
    // RegisterPacketSlot 0x815258: registering an occupied slot leaves its
    // callbacks unchanged. No packet transport is implied by registration.
    bool register_slot(std::size_t index, Callbacks callbacks) noexcept {
        if (index >= slots_.size()) return false;
        auto& slot = slots_[index];
        if (!slot.registered) {
            slot.registered = 1;
            slot.callbacks = callbacks;
            bitmap_ = static_cast<std::uint8_t>(bitmap_ | (1u << index));
        }
        return true;
    }
    // UnregisterPacketSlot 0x8152c0 clears the registration byte, four
    // callbacks, and only this slot's bit, even if already unregistered.
    bool unregister_slot(std::size_t index) noexcept {
        if (index >= slots_.size()) return false;
        slots_[index] = {};
        bitmap_ = static_cast<std::uint8_t>(bitmap_ & ~(1u << index));
        return true;
    }
    const Slot* slot(std::size_t index) const noexcept {
        return index < slots_.size() ? &slots_[index] : nullptr;
    }
    std::uint8_t bitmap() const noexcept { return bitmap_; }
private:
    // Original static table: 0xa33540..0xa33820, eight 92-byte slots.
    std::array<Slot, 8> slots_{};
    std::uint8_t bitmap_{};
};

} // namespace dh2::packet_slot_owner_v1
