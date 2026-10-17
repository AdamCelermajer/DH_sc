#include "character_net_state_owner_v1.hpp"

#include <cmath>
#include <cstring>
#include <limits>

namespace dh2::character_net_state_owner_v1 {
namespace {

bool valid_component(const SourceComponent& component, Identity character,
                     std::uint32_t offset) noexcept {
    if (component.identity == 0 || component.character_identity != character ||
        component.component_offset != offset ||
        component.schema_id != character_netstruct_schema_v1 ||
        component.declared_member_count != source_fields.size() ||
        !component.constructor_complete || !component.members_ready ||
        component.change_counter == nullptr) return false;
    for (const auto& field : component.fields)
        if (!field.constructed) return false;
    return true;
}

bool valid_field(const SourceComponent* component, std::size_t field) noexcept {
    return component && component->members_ready && component->change_counter &&
        field < source_fields.size() && component->fields[field].constructed;
}

void changed(SourceComponent& component, std::size_t field) noexcept {
    ++*component.change_counter;
    component.fields[field].revision = *component.change_counter;
}

std::uint32_t max_code_for_width(std::uint8_t width) noexcept {
    return width == 18 ? 262140u : (width == 16 ? 65535u : 0u);
}

std::uint32_t encode_ranged(float value, std::uint32_t max_code,
                            float minimum, float maximum) noexcept {
    const float normalized = (value - minimum) / (maximum - minimum);
    return static_cast<std::uint32_t>(normalized * static_cast<float>(max_code));
}

float decode_ranged(std::uint32_t raw, std::uint32_t max_code,
                    float minimum, float maximum) noexcept {
    return (static_cast<float>(raw) / static_cast<float>(max_code)) *
        (maximum - minimum) + minimum;
}

Status retire_one(SourceComponent& component, const Services& services,
                  std::uint32_t& retired, std::string& error) noexcept {
    if (!component.identity) return Status::complete;
    if (!services.destroy) return Status::service_unavailable;
    int result = 1;
    try { result = services.destroy(services.context, component, error); }
    catch (...) { result = 1; }
    if (result != 0) return Status::cleanup_incomplete;
    component = {};
    ++retired;
    return Status::complete;
}

void reset(Owner& owner) noexcept { owner = {}; }

} // namespace

Status initialize_members(SourceComponent* component,
                          std::uint64_t* change_counter) noexcept {
    if (!component || !component->identity || !change_counter ||
        component->members_ready) return Status::invalid_argument;
    component->change_counter = change_counter;
    for (std::size_t i = 0; i < source_fields.size(); ++i) {
        component->fields[i] = {};
        component->fields[i].constructed = true;
    }
    for (auto& interpolation : component->interpolation) {
        interpolation = {};
        interpolation.capacity = 40;
    }
    component->members_ready = true;
    return Status::complete;
}

Status construct_component(Owner* owner, Role role, Identity character,
                           std::uint64_t* change_counter) noexcept {
    if (!owner || !character || !change_counter || owner->constructed ||
        (owner->character_identity && owner->character_identity != character))
        return Status::invalid_argument;
    auto& component = role == Role::outgoing_primary ? owner->primary : owner->secondary;
    const auto offset = role == Role::outgoing_primary
        ? primary_component_offset : secondary_component_offset;
    if (component.identity) return Status::invalid_argument;
    if (role == Role::incoming_secondary && !owner->primary.identity)
        return Status::invalid_argument;
    owner->character_identity = character;
    component.identity = reinterpret_cast<Identity>(&component);
    component.character_identity = character;
    component.component_offset = offset;
    component.schema_id = character_netstruct_schema_v1;
    component.declared_member_count = static_cast<std::uint8_t>(source_fields.size());
    component.constructor_complete = true;
    if (initialize_members(&component, change_counter) != Status::complete) {
        component = {};
        if (!owner->primary.identity && !owner->secondary.identity)
            owner->character_identity = 0;
        return Status::invalid_component;
    }
    if (owner->primary.identity && owner->secondary.identity)
        owner->constructed = true;
    return Status::complete;
}

Status destroy_component(Owner* owner, Role role, Identity character) noexcept {
    if (!owner || !character || owner->character_identity != character)
        return Status::invalid_argument;
    auto& component = role == Role::outgoing_primary ? owner->primary : owner->secondary;
    if (!component.identity) return Status::invalid_argument;
    // Character::~Character destroys the secondary NetStruct before primary.
    if (role == Role::outgoing_primary && owner->secondary.identity)
        return Status::invalid_argument;
    component = {};
    owner->constructed = owner->primary.identity && owner->secondary.identity;
    if (!owner->primary.identity && !owner->secondary.identity) {
        owner->character_identity = 0;
        owner->constructed = false;
    }
    return Status::complete;
}

Status set_float(SourceComponent* component, std::size_t field,
                 float value) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    const auto kind = source_fields[field].kind;
    if (kind != FieldKind::interpolated_float && kind != FieldKind::float_value)
        return Status::invalid_field;
    if (!std::isfinite(value)) return Status::invalid_value;
    auto& state = component->fields[field];
    if (state.float_value != value) {
        state.float_value = value;
        changed(*component, field);
    }
    return Status::complete;
}

Status set_unsigned(SourceComponent* component, std::size_t field,
                    std::uint32_t value) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    if (source_fields[field].kind != FieldKind::unsigned_integer)
        return Status::invalid_field;
    auto& state = component->fields[field];
    if (state.unsigned_value != value) {
        state.unsigned_value = value;
        changed(*component, field);
    }
    return Status::complete;
}

Status set_integer(SourceComponent* component, std::size_t field,
                   std::int32_t value) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    if (source_fields[field].kind != FieldKind::integer)
        return Status::invalid_field;
    if (value < std::numeric_limits<std::int16_t>::min() ||
        value > std::numeric_limits<std::int16_t>::max()) return Status::invalid_value;
    auto& state = component->fields[field];
    if (state.integer_value != value) {
        state.integer_value = value;
        changed(*component, field);
    }
    return Status::complete;
}

Status set_boolean(SourceComponent* component, std::size_t field,
                   bool value) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    if (source_fields[field].kind != FieldKind::boolean) return Status::invalid_field;
    auto& state = component->fields[field];
    if (state.boolean_value != value) {
        state.boolean_value = value;
        changed(*component, field);
    }
    return Status::complete;
}

bool test_float(std::size_t field, float value) noexcept {
    if (field >= source_fields.size() || !std::isfinite(value)) return false;
    const auto kind = source_fields[field].kind;
    if (kind != FieldKind::interpolated_float && kind != FieldKind::float_value)
        return false;
    return value >= static_cast<float>(source_fields[field].range_min) &&
        value <= static_cast<float>(source_fields[field].range_max);
}

Status write_member(const SourceComponent* component, std::size_t field,
                    const BitStream& stream) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    if (!stream.write_u32) return Status::service_unavailable;
    const auto& spec = source_fields[field];
    const auto& value = component->fields[field];
    std::uint32_t raw = 0;
    switch (spec.kind) {
    case FieldKind::interpolated_float:
        raw = encode_ranged(value.float_value,
            max_code_for_width(spec.bit_width),
            static_cast<float>(spec.range_min), static_cast<float>(spec.range_max));
        break;
    case FieldKind::float_value:
        raw = encode_ranged(value.float_value, static_cast<float>(max_code_for_width(spec.bit_width)),
                            static_cast<float>(spec.range_min),
                            static_cast<float>(spec.range_max));
        break;
    case FieldKind::unsigned_integer: raw = value.unsigned_value; break;
    case FieldKind::integer: raw = static_cast<std::uint16_t>(value.integer_value); break;
    case FieldKind::boolean: raw = value.boolean_value ? 1u : 0u; break;
    }
    try {
        return stream.write_u32(stream.context, raw, spec.bit_width) == 0
            ? Status::complete : Status::bitstream_failed;
    } catch (...) { return Status::bitstream_failed; }
}

Status read_member(SourceComponent* component, std::size_t field,
                   const BitStream& stream) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    if (!stream.read_u32) return Status::service_unavailable;
    const auto& spec = source_fields[field];
    std::uint32_t raw = 0;
    try {
        if (stream.read_u32(stream.context, spec.bit_width, &raw) != 0)
            return Status::bitstream_failed;
    } catch (...) { return Status::bitstream_failed; }
    switch (spec.kind) {
    case FieldKind::interpolated_float:
        return set_float(component, field, decode_ranged(raw,
            max_code_for_width(spec.bit_width),
            static_cast<float>(spec.range_min), static_cast<float>(spec.range_max)));
    case FieldKind::float_value:
        return set_float(component, field, decode_ranged(raw, max_code_for_width(spec.bit_width),
            static_cast<float>(spec.range_min), static_cast<float>(spec.range_max)));
    case FieldKind::unsigned_integer:
        return set_unsigned(component, field, raw);
    case FieldKind::integer: {
        const auto bits = static_cast<std::uint16_t>(raw);
        const auto signed_value = (bits & 0x8000u)
            ? static_cast<std::int32_t>(bits) - 0x10000
            : static_cast<std::int32_t>(bits);
        return set_integer(component, field, signed_value);
    }
    case FieldKind::boolean:
        return set_boolean(component, field, raw != 0);
    }
    return Status::invalid_field;
}

Status post_load(SourceComponent* component, std::size_t field,
                 std::uint32_t source_timestamp,
                 std::uint32_t current_frame_time) noexcept {
    if (!valid_field(component, field)) return Status::member_not_ready;
    if (field >= component->interpolation.size() ||
        source_fields[field].kind != FieldKind::interpolated_float)
        return Status::invalid_field;
    auto& history = component->interpolation[field];
    if (history.capacity != 40) return Status::invalid_value;
    if (history.sample_count >= history.capacity) {
        // Source removes old intervals until its 40-frame interpolation window fits.
        while (history.sample_count >= history.capacity) {
            history.elapsed_total -= history.average_elapsed;
            --history.sample_count;
        }
    }
    const std::uint32_t elapsed = current_frame_time - source_timestamp;
    history.elapsed_total += elapsed;
    ++history.sample_count;
    history.average_elapsed = history.elapsed_total / history.sample_count;
    const auto next = (history.current_index + 1u) %
        static_cast<std::uint32_t>(history.samples.size());
    history.samples[next] = {source_timestamp, component->fields[field].float_value};
    history.current_index = next;
    if (next == history.start_index) {
        history.start_index = (history.start_index + 1u) %
            static_cast<std::uint32_t>(history.samples.size());
        history.auxiliary_index = history.start_index;
        history.end_index = history.start_index;
    } else {
        history.end_index = next;
    }
    return Status::complete;
}

Status construct(Owner* owner, Identity character, std::uint64_t* change_counter,
                 const Services* services, Result* result,
                 std::string& error) {
    if (result) *result = {};
    if (!owner || !character || !change_counter || !services || owner->constructed ||
        owner->character_identity || owner->primary.identity || owner->secondary.identity)
        return Status::invalid_argument;
    if (!services->construct || !services->populate_outgoing ||
        !services->interpret_incoming || !services->destroy)
        return Status::service_unavailable;

    owner->character_identity = character;
    auto create = [&](Role role, std::uint32_t offset, SourceComponent& output) {
        int status = 1;
        try { status = services->construct(services->context, character, role, offset,
                                           &output, error); }
        catch (...) { status = 1; }
        if (status != 0) return Status::service_failed;
        if (initialize_members(&output, change_counter) != Status::complete)
            return Status::invalid_component;
        if (!valid_component(output, character, offset)) return Status::invalid_component;
        return Status::complete;
    };

    auto status = create(Role::outgoing_primary, primary_component_offset,
                         owner->primary);
    if (status == Status::complete && result) ++result->constructed_components;
    Role failed_role = Role::outgoing_primary;
    if (status == Status::complete) {
        failed_role = Role::incoming_secondary;
        status = create(Role::incoming_secondary, secondary_component_offset,
                        owner->secondary);
        if (status == Status::complete && result) ++result->constructed_components;
    }
    if (status == Status::complete &&
        owner->primary.identity == owner->secondary.identity) {
        status = Status::invalid_component;
        error = "Character NetState roles must have separate canonical owners";
        owner->secondary = {};
    }
    if (status != Status::complete) {
        if (result) result->failed_role = failed_role;
        std::uint32_t retired = 0;
        const auto second = retire_one(owner->secondary, *services, retired, error);
        const auto first = retire_one(owner->primary, *services, retired, error);
        if (result) result->retired_components = retired;
        if (second != Status::complete || first != Status::complete)
            return Status::cleanup_incomplete;
        reset(*owner);
        return status;
    }
    owner->constructed = true;
    return Status::complete;
}

Status populate_outgoing(Owner* owner, const Services* services,
                         std::string& error) {
    if (!owner || !owner->constructed) return Status::invalid_argument;
    if (!services || !services->populate_outgoing) return Status::service_unavailable;
    int status = 1;
    try { status = services->populate_outgoing(services->context, owner->primary, error); }
    catch (...) { status = 1; }
    return status == 0 ? Status::complete : Status::service_failed;
}

Status interpret_incoming(Owner* owner, const Services* services,
                          std::string& error) {
    if (!owner || !owner->constructed) return Status::invalid_argument;
    if (!services || !services->interpret_incoming) return Status::service_unavailable;
    int status = 1;
    try { status = services->interpret_incoming(services->context,
                                               owner->secondary, error); }
    catch (...) { status = 1; }
    return status == 0 ? Status::complete : Status::service_failed;
}

Status destroy(Owner* owner, const Services* services, Result* result,
               std::string& error) noexcept {
    if (result) *result = {};
    if (!owner) return Status::invalid_argument;
    if (!owner->character_identity && !owner->primary.identity &&
        !owner->secondary.identity) return Status::complete;
    if (!services) return Status::service_unavailable;
    if (result) result->constructed_components =
        static_cast<std::uint32_t>(owner->primary.identity != 0) +
        static_cast<std::uint32_t>(owner->secondary.identity != 0);
    std::uint32_t retired = 0;
    const auto second = retire_one(owner->secondary, *services, retired, error);
    const auto first = retire_one(owner->primary, *services, retired, error);
    if (result) result->retired_components = retired;
    if (second != Status::complete || first != Status::complete)
        return Status::cleanup_incomplete;
    reset(*owner);
    return Status::complete;
}

} // namespace dh2::character_net_state_owner_v1
