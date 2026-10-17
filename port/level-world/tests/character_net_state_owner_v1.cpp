#include "../character_net_state_owner_v1.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace ns = dh2::character_net_state_owner_v1;

struct Fixture {
    std::vector<ns::Role> made;
    std::vector<ns::Identity> retired;
    ns::Identity next{0x1000};
    ns::Identity outgoing{};
    ns::Identity incoming{};
    bool fail_secondary{};
    bool corrupt_member_count{};
};
struct Bits { std::uint32_t value{}; std::uint8_t width{}; };
static int write_bits(void* opaque, std::uint32_t value, std::uint8_t width) {
    auto& bits = *static_cast<Bits*>(opaque);
    bits = {value, width};
    return 0;
}
static int read_bits(void* opaque, std::uint8_t width, std::uint32_t* value) {
    const auto& bits = *static_cast<Bits*>(opaque);
    if (!value || bits.width != width) return 1;
    *value = bits.value;
    return 0;
}

static int construct(void* opaque, ns::Identity character, ns::Role role,
                     std::uint32_t offset, ns::SourceComponent* output,
                     std::string&) {
    auto& f = *static_cast<Fixture*>(opaque);
    f.made.push_back(role);
    if (role == ns::Role::incoming_secondary && f.fail_secondary) return 1;
    *output = {f.next++, character, offset, ns::character_netstruct_schema_v1,
               static_cast<std::uint8_t>(ns::source_fields.size()), true};
    if (role == ns::Role::incoming_secondary && f.corrupt_member_count)
        --output->declared_member_count;
    return 0;
}
static int outgoing(void* opaque, const ns::SourceComponent& component,
                    std::string&) {
    static_cast<Fixture*>(opaque)->outgoing = component.identity;
    return 0;
}
static int incoming(void* opaque, const ns::SourceComponent& component,
                    std::string&) {
    static_cast<Fixture*>(opaque)->incoming = component.identity;
    return 0;
}
static int destroy(void* opaque, const ns::SourceComponent& component,
                   std::string&) {
    static_cast<Fixture*>(opaque)->retired.push_back(component.identity);
    return 0;
}
static ns::Services services(Fixture& f) {
    return {&f, construct, outgoing, incoming, destroy};
}

int main() {
    static_assert(ns::primary_component_offset == 0x1508);
    static_assert(ns::secondary_component_offset == 0x1a48);
    static_assert(ns::source_fields.size() == 11);
    assert(ns::source_fields[0].byte_offset == 0x130 &&
           ns::source_fields[0].kind == ns::FieldKind::interpolated_float &&
           ns::source_fields[0].bit_width == 18 &&
           ns::source_fields[0].range_min == -200000 &&
           ns::source_fields[0].range_max == 200000);
    assert(ns::source_fields[2].byte_offset == 0x320 &&
           ns::source_fields[2].bit_width == 16 &&
           ns::source_fields[2].range_min == -50000 &&
           ns::source_fields[2].range_max == 50000);
    assert(ns::source_fields[3].byte_offset == 0x418 &&
           ns::source_fields[3].kind == ns::FieldKind::float_value &&
           ns::source_fields[3].range_min == -1 &&
           ns::source_fields[3].range_max == 1);

    Fixture f;
    auto svc = services(f);
    ns::Owner owner;
    ns::Result result;
    std::string error;
    std::uint64_t serial = 4;
    assert(ns::construct(&owner, 0x55, &serial, &svc, &result, error) == ns::Status::complete);
    assert(owner.constructed && result.constructed_components == 2 &&
           owner.primary.identity != owner.secondary.identity &&
           owner.primary.component_offset == ns::primary_component_offset &&
           owner.secondary.component_offset == ns::secondary_component_offset &&
           owner.primary.members_ready && owner.secondary.members_ready);
    assert(ns::test_float(0, -200000.0f) && ns::test_float(0, 200000.0f) &&
           !ns::test_float(0, 200001.0f) && ns::test_float(3, -1.0f) &&
           ns::test_float(3, 1.0f) && !ns::test_float(3, 1.01f));
    assert(ns::set_float(&owner.primary, 0, 12.5f) == ns::Status::complete && serial == 5);
    assert(ns::set_float(&owner.primary, 0, 12.5f) == ns::Status::complete && serial == 5);
    assert(ns::set_unsigned(&owner.primary, 5, 0x12345678) == ns::Status::complete && serial == 6);
    assert(ns::set_integer(&owner.primary, 7, -12) == ns::Status::complete && serial == 7);
    assert(ns::set_boolean(&owner.primary, 8, true) == ns::Status::complete && serial == 8);
    assert(ns::set_boolean(&owner.primary, 8, true) == ns::Status::complete && serial == 8);
    Bits packed;
    ns::BitStream write_stream{&packed, write_bits, nullptr};
    assert(ns::write_member(&owner.primary, 0, write_stream) == ns::Status::complete &&
           packed.width == 18);
    ns::BitStream read_stream{&packed, nullptr, read_bits};
    assert(ns::read_member(&owner.secondary, 0, read_stream) == ns::Status::complete);
    assert(owner.secondary.fields[0].float_value > 10.0f &&
           owner.secondary.fields[0].float_value < 13.0f);
    assert(ns::post_load(&owner.secondary, 0, 90, 100) == ns::Status::complete);
    assert(ns::post_load(&owner.secondary, 0, 90, 100) == ns::Status::complete);
    assert(ns::post_load(&owner.secondary, 0, 90, 100) == ns::Status::complete);
    assert(owner.secondary.interpolation[0].sample_count == 3 &&
           owner.secondary.interpolation[0].average_elapsed == 10 &&
           owner.secondary.interpolation[0].samples[2].source_timestamp == 90);
    auto roundtrip = [&](std::size_t field) {
        assert(ns::write_member(&owner.primary, field, write_stream) == ns::Status::complete);
        assert(packed.width == ns::source_fields[field].bit_width);
        return ns::read_member(&owner.secondary, field, read_stream);
    };
    assert(ns::set_float(&owner.primary, 3, 0.25f) == ns::Status::complete);
    assert(roundtrip(3) == ns::Status::complete &&
           owner.secondary.fields[3].float_value > 0.24f &&
           owner.secondary.fields[3].float_value < 0.27f);
    assert(ns::set_float(&owner.primary, 4, -0.75f) == ns::Status::complete);
    assert(roundtrip(4) == ns::Status::complete &&
           owner.secondary.fields[4].float_value < -0.74f &&
           owner.secondary.fields[4].float_value > -0.78f);
    assert(ns::set_unsigned(&owner.primary, 6, 0xfedcba98u) == ns::Status::complete);
    assert(roundtrip(6) == ns::Status::complete &&
           owner.secondary.fields[6].unsigned_value == 0xfedcba98u);
    assert(ns::set_integer(&owner.primary, 7, -12345) == ns::Status::complete);
    assert(roundtrip(7) == ns::Status::complete &&
           owner.secondary.fields[7].integer_value == -12345);
    assert(ns::set_boolean(&owner.primary, 9, true) == ns::Status::complete);
    assert(roundtrip(9) == ns::Status::complete && owner.secondary.fields[9].boolean_value);
    assert(ns::set_boolean(&owner.primary, 10, false) == ns::Status::complete);
    assert(roundtrip(10) == ns::Status::complete && !owner.secondary.fields[10].boolean_value);
    assert(ns::set_integer(&owner.primary, 7, 32768) == ns::Status::invalid_value);
    assert(ns::set_float(&owner.primary, 5, 1.0f) == ns::Status::invalid_field);
    assert(ns::post_load(&owner.secondary, 3, 90, 100) == ns::Status::invalid_field);
    assert(ns::populate_outgoing(&owner, &svc, error) == ns::Status::complete);
    assert(f.outgoing == owner.primary.identity);
    assert(ns::interpret_incoming(&owner, &svc, error) == ns::Status::complete);
    assert(f.incoming == owner.secondary.identity);
    assert(ns::destroy(&owner, &svc, &result, error) == ns::Status::complete);
    assert(!owner.constructed && result.retired_components == 2 &&
           f.retired.size() == 2 && f.retired[0] == 0x1001 &&
           f.retired[1] == 0x1000);

    ns::Owner ordered;
    std::uint64_t ordered_counter = 0;
    assert(ns::construct_component(&ordered, ns::Role::incoming_secondary, 0x88,
        &ordered_counter) == ns::Status::invalid_argument);
    assert(ns::construct_component(&ordered, ns::Role::outgoing_primary, 0x88,
        &ordered_counter) == ns::Status::complete && ordered.primary.identity &&
        !ordered.secondary.identity && !ordered.constructed);
    assert(ns::construct_component(&ordered, ns::Role::incoming_secondary, 0x88,
        &ordered_counter) == ns::Status::complete && ordered.constructed);
    assert(ns::destroy_component(&ordered, ns::Role::outgoing_primary, 0x88) ==
           ns::Status::invalid_argument);
    assert(ns::destroy_component(&ordered, ns::Role::incoming_secondary, 0x88) ==
           ns::Status::complete && !ordered.constructed && ordered.primary.identity);
    assert(ns::destroy_component(&ordered, ns::Role::outgoing_primary, 0x88) ==
           ns::Status::complete && !ordered.character_identity);

    Fixture failed;
    failed.fail_secondary = true;
    auto failed_services = services(failed);
    ns::Owner rejected;
    assert(ns::construct(&rejected, 0x66, &serial, &failed_services, &result, error) ==
           ns::Status::service_failed);
    assert(!rejected.constructed && !rejected.character_identity &&
           failed.retired.size() == 1 && failed.retired[0] == 0x1000 &&
           result.retired_components == 1 &&
           result.failed_role == ns::Role::incoming_secondary);

    Fixture malformed;
    malformed.corrupt_member_count = true;
    auto malformed_services = services(malformed);
    ns::Owner invalid;
    assert(ns::construct(&invalid, 0x77, &serial, &malformed_services, &result, error) ==
           ns::Status::invalid_component);
    assert(!invalid.constructed && malformed.retired.size() == 2 &&
           malformed.retired[0] == 0x1001 && malformed.retired[1] == 0x1000);

    std::cout << "{\"two_component_schema\":true,\"directional_routing\":true,"
                 "\"reverse_destruction\":true,\"partial_rollback\":true,"
                 "\"incomplete_schema_rejected\":true,\"member_codecs\":true,"
                 "\"interpolation_history\":true,\"source_order_hooks\":true}\n";
}
