#pragma once
#include <cstdint>
#include <string>
namespace dh2::sound {
// Sole source Vox constructor-backed music fields. This is a music-domain
// projection; it does not assert sound-bank/device construction completed.
struct VoxMusicFieldsV1 {
 std::int32_t current_music_24{-1},field_28{-1},field_2c{-1};
 std::uint8_t enabled_30{1},ambient_31{1},level_music_32{},field_33{};
};
enum VoxMusicServiceV1:std::uint32_t {vox_music_disabled_v1=1,vox_music_event_index_v1,
 vox_music_channel_v1,vox_music_channel_info_v1,vox_music_channel_state_v1,vox_music_channel_info_destroy_v1};
struct VoxMusicRequestV1 {
 std::uint32_t service{};std::uintptr_t owner{},channel{},info{};
 std::int32_t music{},sound_index{};const char* state{};
};
struct VoxMusicResponseV1 {std::int32_t integer{};std::uintptr_t identity{};};
struct VoxMusicServicesV1 {void* context{};int(*invoke)(void*,const VoxMusicRequestV1*,VoxMusicResponseV1*){};};
class VoxMusicStateOwnerV1 {
 VoxMusicFieldsV1& fields_;VoxMusicServicesV1 services_;std::string error_;
 int ask(const VoxMusicRequestV1&,VoxMusicResponseV1&);
public:
 explicit VoxMusicStateOwnerV1(VoxMusicFieldsV1&same_fields,VoxMusicServicesV1);
 int set_music_state(const char* actual_state);
 VoxMusicFieldsV1& fields()noexcept{return fields_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const std::string& error()const noexcept{return error_;}
};
}
