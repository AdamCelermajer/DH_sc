#pragma once
#include "character_combat_sound_v1.hpp"
#include <string>
namespace dh2::sound {
enum class VoxPlay3DOperationV2 {disabled,current_level,online,network_muted,platform_route,platform_play,sound_row,bank_info,trace,emit,native_play};
struct VoxPlay3DRequestV2 {VoxPlay3DOperationV2 operation{};const character::CombatSoundPlayV1* play{};std::int32_t selected_sound{};};
struct VoxPlay3DResponseV2 {std::uintptr_t identity{};std::int32_t value{},type{};std::int32_t bank_fields[5]{};};
struct VoxPlay3DServicesV2 {void* context{};int(*invoke)(void*,const VoxPlay3DRequestV2&,VoxPlay3DResponseV2&){};};
// Full Play3D36b5d8 ordered branch. Native bank/platform leaves remain genuine
// required endpoints, including source bank-info parameters and trace query.
class VoxPlay3DOwnerV2 {
 VoxPlay3DServicesV2 services_;std::string error_;std::uint32_t phase_{};
 int ask(VoxPlay3DOperationV2,const character::CombatSoundPlayV1&,VoxPlay3DResponseV2&,std::int32_t selected=0);
public:
 explicit VoxPlay3DOwnerV2(VoxPlay3DServicesV2 s):services_(s){}
 int play(const character::CombatSoundPlayV1&);
 const std::string& error()const noexcept{return error_;}
 std::uint32_t phase()const noexcept{return phase_;}
};
}
