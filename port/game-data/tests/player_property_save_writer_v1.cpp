#include "../player_property_save_writer_v1.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace {
using namespace dh2::data;
using namespace dh2::data::player_save_section_writers_v1;
using dh2::data::player_property_save_writer_v1::write_properties_v1;

struct Sink {
    std::vector<std::uint8_t> bytes;
    std::uint32_t calls{};
    std::uint32_t fail_on_call{};
};

bool write_bytes(void* context, Bytes bytes, std::string& error) {
    auto& sink = *static_cast<Sink*>(context);
    ++sink.calls;
    if (sink.fail_on_call && sink.calls == sink.fail_on_call) {
        error = "injected stream failure";
        return false;
    }
    sink.bytes.insert(sink.bytes.end(), bytes.data, bytes.data + bytes.size);
    return true;
}

std::uint32_t read_word(const std::vector<std::uint8_t>& bytes,
                        std::size_t offset) {
    assert(offset + 4 <= bytes.size());
    return std::uint32_t(bytes[offset]) |
           (std::uint32_t(bytes[offset + 1]) << 8) |
           (std::uint32_t(bytes[offset + 2]) << 16) |
           (std::uint32_t(bytes[offset + 3]) << 24);
}

void fill_properties(std::array<std::int32_t, 224>& values) {
    for (std::uint32_t i = 0; i < values.size(); ++i) {
        const std::uint32_t bits = 0x9e3779b9u * (i + 1u);
        std::memcpy(&values[i], &bits, sizeof(bits));
    }
}

void test_source_layout_and_values() {
    PlayerSavegameV1 save;
    save.set_character(0x1234);
    save.set_saved_properties_byte_194(1);

    std::array<std::int32_t, 224> values{};
    fill_properties(values);
    PropertyView view{};
    view.resolved = values.data();

    Sink sink;
    WriteServicesV1 services{&sink, write_bytes};
    std::string error;
    const auto status = write_properties_v1(save, view, services, error);
    assert(status == Status::complete);
    assert(error.empty());
    assert(sink.calls == 226);  // count + 224 int32 values + bool byte
    assert(sink.bytes.size() == 4 + 224 * 4 + 1);
    assert(read_word(sink.bytes, 0) == 224);
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::uint32_t expected = 0;
        std::memcpy(&expected, &values[i], sizeof(expected));
        assert(read_word(sink.bytes, 4 + i * 4) == expected);
    }
    assert(sink.bytes.back() == 1);
}

void test_missing_character_writes_no_prefix() {
    PlayerSavegameV1 save;
    std::array<std::int32_t, 224> values{};
    PropertyView view{};
    view.resolved = values.data();
    Sink sink;
    WriteServicesV1 services{&sink, write_bytes};
    std::string error;
    const auto status = write_properties_v1(save, view, services, error);
    assert(status == Status::source_assertion_boundary);
    assert(!error.empty());
    assert(sink.calls == 0 && sink.bytes.empty());
}

void test_missing_live_property_sheet_writes_no_prefix() {
    PlayerSavegameV1 save;
    save.set_character(1);
    PropertyView view{};
    Sink sink;
    WriteServicesV1 services{&sink, write_bytes};
    std::string error;
    const auto status = write_properties_v1(save, view, services, error);
    assert(status == Status::source_assertion_boundary);
    assert(!error.empty());
    assert(sink.calls == 0 && sink.bytes.empty());
}

void test_stream_failure_retains_accepted_prefix() {
    PlayerSavegameV1 save;
    save.set_character(1);
    std::array<std::int32_t, 224> values{};
    fill_properties(values);
    PropertyView view{};
    view.resolved = values.data();
    Sink sink;
    // Accept the count and four values, then reject the fifth call.
    sink.fail_on_call = 6;
    WriteServicesV1 services{&sink, write_bytes};
    std::string error;
    const auto status = write_properties_v1(save, view, services, error);
    assert(status == Status::failed);
    assert(error == "injected stream failure");
    assert(sink.calls == 6);
    assert(sink.bytes.size() == 4 + 4 * 4);
    assert(read_word(sink.bytes, 0) == 224);
    for (std::size_t i = 0; i < 4; ++i) {
        std::uint32_t expected = 0;
        std::memcpy(&expected, &values[i], sizeof(expected));
        assert(read_word(sink.bytes, 4 + i * 4) == expected);
    }
}
}  // namespace

int main() {
    test_source_layout_and_values();
    test_missing_character_writes_no_prefix();
    test_missing_live_property_sheet_writes_no_prefix();
    test_stream_failure_retains_accepted_prefix();
}
