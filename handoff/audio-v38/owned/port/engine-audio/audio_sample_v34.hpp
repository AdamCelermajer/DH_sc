#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace dh2::audio {
struct AudioSegmentV34 {std::uint32_t byte_offset{},byte_size{},frames{},source_fields[3]{};};
struct AudioStateV34 {std::uint32_t playlist{};std::string name;std::array<std::uint32_t,7> source_name_words{};};
struct AudioPlaylistElementV34 {std::array<std::int32_t,8> source{};};
struct AudioTransitionV34 {std::int32_t from{},to{},rule{},cue{};};
struct AudioSampleV34 {
 std::shared_ptr<const std::vector<std::uint8_t>> bytes;
 std::uint32_t format{},channels{},rate{},block_align{},bits{},frames_per_block{},data_offset{},data_size{};
 bool native{};
 std::uint32_t container_declared_size{};
 std::vector<AudioSegmentV34> segments;
 std::vector<AudioStateV34> states;
 std::vector<std::array<std::uint32_t,9>> rules;
 std::vector<std::array<std::int32_t,2>> playlists;
 std::vector<std::array<std::int32_t,6>> groups;
 std::vector<AudioPlaylistElementV34> elements;
 std::vector<AudioTransitionV34> transitions;
};
// Borrowed actual bytes are retained; no filename or codec substitution.
bool audio_sample_open_v34(std::shared_ptr<const std::vector<std::uint8_t>>,
 AudioSampleV34&,std::string&);
// Same original IMA arithmetic and channel-word interleaving; bounded caller
// output, no allocation. Returns decoded frames, zero on malformed input.
unsigned audio_ima_block_v34(const std::uint8_t*,unsigned bytes,unsigned channels,
 std::int16_t* output,unsigned output_samples) noexcept;
class AudioSampleCursorV34 {
 const AudioSampleV34* sample_{};unsigned segment_{},block_{~0u},decoded_{};
 std::array<std::int16_t,4096> cache_{};
public:
 bool bind(const AudioSampleV34*,unsigned segment) noexcept;
 bool frame(std::uint64_t index,float& left,float& right) noexcept;
 std::uint32_t frames()const noexcept;
};
}
