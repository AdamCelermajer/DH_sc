#include "player_save_inventory_v1.hpp"

#include <cstring>
#include <exception>
#include <limits>

namespace dh2::player_save_inventory_v1 {
namespace {

struct AddressRange {
    std::uintptr_t begin{};
    std::uintptr_t end{};
};

bool make_range(const void* pointer, std::size_t size, AddressRange& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if ((!pointer && size) || size > UINTPTR_MAX - begin) return false;
    out = {begin, begin + size};
    return true;
}

bool overlaps(AddressRange left, AddressRange right) {
    return left.begin < right.end && right.begin < left.end;
}

class OutputGuard {
    AddressRange output_{};
    AddressRange result_{};
    AddressRange error_{};

public:
    bool initialize(MutableBytes output, Result* result, std::string& error) {
        return result &&
               reinterpret_cast<std::uintptr_t>(result) % alignof(Result) == 0 &&
               make_range(output.data, output.size, output_) &&
               make_range(result, sizeof(*result), result_) &&
               make_range(&error, sizeof(error), error_) &&
               !overlaps(output_, result_) && !overlaps(output_, error_) &&
               !overlaps(result_, error_);
    }

    bool separates(const void* pointer, std::size_t size) const {
        AddressRange source{};
        return make_range(pointer, size, source) && !overlaps(source, output_) &&
               !overlaps(source, result_) && !overlaps(source, error_);
    }
};

bool protect_string(const std::string& value, const OutputGuard& guard) {
    return guard.separates(&value, sizeof(value)) &&
           guard.separates(value.c_str(), value.size() + 1);
}

bool protect_item(const ItemRef& reference, const OutputGuard& guard) {
    if (!reference.item || !reference.slot ||
        !guard.separates(reference.item, sizeof(*reference.item)) ||
        !guard.separates(reference.slot, sizeof(*reference.slot)))
        return false;
    const auto& item = *reference.item;
    return guard.separates(&item.powers, sizeof(item.powers)) &&
           guard.separates(item.powers.data(),
                           item.powers.size() * sizeof(item.powers[0])) &&
           protect_string(item.name, guard) &&
           protect_string(item.description, guard) &&
           protect_string(item.requirements, guard);
}

bool protect_names(const std::vector<std::string>& names,
                   const OutputGuard& guard) {
    if (!guard.separates(&names, sizeof(names)) ||
        !guard.separates(names.data(), names.size() * sizeof(names[0])))
        return false;
    for (const auto& name : names)
        if (!protect_string(name, guard)) return false;
    return true;
}

class Writer {
    MutableBytes output_;
    Result& result_;
    std::string& error_;

public:
    Writer(MutableBytes output, Result& result, std::string& error)
        : output_(output), result_(result), error_(error) {}

    void stage(Stage stage, std::uint32_t caller) {
        result_.stage = stage;
        result_.source_caller = caller;
    }

    bool fail(const char* message) {
        if (error_.empty()) error_ = message;
        return false;
    }

    bool bytes(const void* value, std::size_t size) {
        ++result_.stream_writes;
        if (result_.written > output_.size || size > output_.size - result_.written)
            return fail("GEAR output buffer ended at source stream write");
        if (size) {
            std::memcpy(output_.data + result_.written, value, size);
            result_.written += size;
        }
        return true;
    }

    bool word(std::uint32_t value) {
        const std::uint8_t raw[4]{std::uint8_t(value),
                                  std::uint8_t(value >> 8),
                                  std::uint8_t(value >> 16),
                                  std::uint8_t(value >> 24)};
        return bytes(raw, sizeof(raw));
    }

    bool signed_word(std::int32_t value) {
        std::uint32_t bits{};
        std::memcpy(&bits, &value, sizeof(bits));
        return word(bits);
    }

    bool string(const std::string& value) {
        if (value.size() >= UINT32_MAX)
            return fail("GEAR source string exceeds its 32-bit length field");
        const auto length = static_cast<std::uint32_t>(value.size() + 1);
        if (!word(length)) return false;
        // IStreamBase::writeAs(string) writes the NUL-terminated payload in a
        // second stream operation after the 32-bit length operation.
        return bytes(value.c_str(), length);
    }
};

std::int32_t signed_quantity(std::uint16_t bits) {
    std::int16_t value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

template <class GetItem>
bool write_all(Writer& writer, std::int32_t gold, std::int32_t selected,
               std::size_t item_count,
               const std::vector<std::string>& item_names,
               const std::vector<std::string>& power_names,
               GetItem get_item, Result& result) {
    writer.stage(Stage::gold, 0x46a128);
    if (!writer.signed_word(gold)) return false;

    writer.stage(Stage::selection, 0x46a134);
    if (!writer.signed_word(selected)) return false;

    writer.stage(Stage::item_count, 0x46a140);
    result.declared_items = static_cast<std::uint32_t>(item_count);
    if (!writer.word(result.declared_items)) return false;

    for (std::size_t index = 0; index < item_count; ++index) {
        const ItemRef reference = get_item(index);
        if (!reference.item || !reference.slot)
            return writer.fail("GEAR reached an unavailable inventory Item/slot");
        const auto& item = *reference.item;

        writer.stage(Stage::item_name, 0x46a200);
        if (item.id < 0 || static_cast<std::size_t>(item.id) >= item_names.size())
            return writer.fail("GEAR Item ID has no source identifier row");
        if (!writer.string(item_names[static_cast<std::size_t>(item.id)])) return false;

        writer.stage(Stage::item_slot_first, 0x46a214);
        if (!writer.signed_word(reference.slot->slots[0])) return false;
        writer.stage(Stage::item_slot_second, 0x46a228);
        if (!writer.signed_word(reference.slot->slots[1])) return false;

        writer.stage(Stage::item_quantity, 0x46a23c);
        if (!writer.signed_word(signed_quantity(item.quantity))) return false;
        writer.stage(Stage::item_value, 0x46a250);
        if (!writer.signed_word(item.value)) return false;

        writer.stage(Stage::item_identified, 0x46a264);
        const auto identified = item.identified;
        if (!writer.bytes(&identified, sizeof(identified))) return false;

        writer.stage(Stage::power_count, 0x46a27c);
        if (item.powers.size() > UINT32_MAX)
            return writer.fail("GEAR ItemPower count exceeds its 32-bit field");
        result.declared_powers = static_cast<std::uint32_t>(item.powers.size());
        result.completed_powers = 0;
        if (!writer.word(result.declared_powers)) return false;

        for (const auto power_id : item.powers) {
            writer.stage(Stage::power_name, 0x46a2d0);
            if (power_id < 0 ||
                static_cast<std::size_t>(power_id) >= power_names.size())
                return writer.fail("GEAR ItemPower ID has no source identifier row");
            if (!writer.string(power_names[static_cast<std::size_t>(power_id)]))
                return false;
            ++result.completed_powers;
        }
        ++result.completed_items;
    }
    writer.stage(Stage::complete, 0x46a314);
    return true;
}

bool protect_view(const GearView& view, const OutputGuard& guard) {
    if (!guard.separates(&view, sizeof(view)) ||
        !guard.separates(view.items, view.item_count * sizeof(ItemRef)) ||
        !view.item_names || !view.power_names ||
        !protect_names(*view.item_names, guard) ||
        !protect_names(*view.power_names, guard))
        return false;
    for (std::size_t i = 0; i < view.item_count; ++i) {
        const auto& reference = view.items[i];
        if (reference.item &&
            (!guard.separates(reference.item, sizeof(*reference.item)) ||
             !guard.separates(&reference.item->powers,
                              sizeof(reference.item->powers)) ||
             !guard.separates(reference.item->powers.data(),
                              reference.item->powers.size() *
                                  sizeof(reference.item->powers[0])) ||
             !protect_string(reference.item->name, guard) ||
             !protect_string(reference.item->description, guard) ||
             !protect_string(reference.item->requirements, guard)))
            return false;
        if (reference.slot &&
            !guard.separates(reference.slot, sizeof(*reference.slot)))
            return false;
    }
    return true;
}

bool protect_bound_inventory(const Bindings& bindings,
                             const OutputGuard& guard) {
    if (!guard.separates(bindings.save, sizeof(*bindings.save)) ||
        !guard.separates(bindings.inventory, sizeof(*bindings.inventory)) ||
        !guard.separates(&bindings.powers, sizeof(bindings.powers)))
        return false;

    const auto& slots = bindings.inventory->items();
    const auto& item_names = bindings.inventory->table().identifiers;
    const auto& power_names = bindings.powers.names();
    if (!guard.separates(&slots, sizeof(slots)) ||
        !guard.separates(slots.data(), slots.size() * sizeof(slots[0])) ||
        !protect_names(item_names, guard) || !protect_names(power_names, guard))
        return false;
    for (const auto& slot : slots) {
        if (!slot) continue;
        if (!guard.separates(slot.get(), sizeof(*slot))) return false;
        if (slot->item &&
            !protect_item(ItemRef{slot->item.get(), slot.get()}, guard))
            return false;
    }
    return true;
}

template <class Run>
Status begin_save(MutableBytes output, Result* result, std::string& error,
                  Run run) {
    *result = {};
    error.clear();
    Writer writer(output, *result, error);
    try {
        return run(writer) ? Status::complete : Status::failed;
    } catch (const std::exception& exception) {
        if (error.empty()) error = exception.what();
        return Status::failed;
    } catch (...) {
        if (error.empty()) error = "GEAR source writer provider exception";
        return Status::failed;
    }
}

}  // namespace

Status save(const GearView& view, MutableBytes output, Result* result,
            std::string& error) {
    OutputGuard guard;
    if (!guard.initialize(output, result, error) ||
        view.item_count > UINT32_MAX || !protect_view(view, guard) ||
        (!view.items && view.item_count) || !view.item_names ||
        !view.power_names || view.selected_set < INT8_MIN ||
        view.selected_set > INT8_MAX)
        return Status::invalid_argument;

    return begin_save(output, result, error, [&](Writer& writer) {
        return write_all(writer, view.gold, view.selected_set, view.item_count,
                         *view.item_names, *view.power_names,
                         [&](std::size_t index) { return view.items[index]; },
                         *result);
    });
}

Status save(const Bindings& bindings, MutableBytes output, Result* result,
            std::string& error) {
    OutputGuard guard;
    if (!guard.initialize(output, result, error) ||
        !guard.separates(&bindings, sizeof(bindings)) || !bindings.save ||
        !bindings.inventory || !bindings.powers ||
        !protect_bound_inventory(bindings, guard))
        return Status::invalid_argument;

    const auto character = bindings.save->character();
    if (!character || character != bindings.inventory->character()) {
        *result = {};
        error = "GEAR reached missing or mismatched source Character";
        return Status::failed;
    }
    const auto selected = bindings.inventory->current_equipment();
    // The authoritative V4 projection is a 0/1 byte. Values outside it do not
    // have a source-backed interpretation for this writer.
    if (selected > 1) {
        *result = {};
        error = "GEAR current equipment set is outside the source 0/1 domain";
        return Status::failed;
    }

    const auto& slots = bindings.inventory->items();
    const auto& item_names = bindings.inventory->table().identifiers;
    const auto& power_names = bindings.powers.names();
    if (slots.size() > UINT32_MAX) {
        *result = {};
        error = "GEAR inventory item count exceeds its 32-bit field";
        return Status::failed;
    }

    return begin_save(output, result, error, [&](Writer& writer) {
        return write_all(writer, bindings.inventory->gold(), selected,
                         slots.size(), item_names, power_names,
                         [&](std::size_t index) {
                             const auto* slot = slots[index].get();
                             return slot ? ItemRef{slot->item.get(), slot}
                                         : ItemRef{};
                         },
                         *result);
    });
}

}  // namespace dh2::player_save_inventory_v1
