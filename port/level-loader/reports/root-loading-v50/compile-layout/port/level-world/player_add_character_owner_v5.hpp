#pragma once
#include "player_manager_owner_v1.hpp"
#include "canonical_player_facet_v3.hpp"
#include <functional>
namespace dh2::player {
struct PlayerAddNetworkBorrowV5 {
 const std::int32_t* player_class380{};
 const std::uint8_t* visible4e5{};
};
struct PlayerAddCharacterBorrowV5 {
 std::uintptr_t identity{};
 std::shared_ptr<void> receiver;
 world::CanonicalPlayerFacetV3* same_facet{};
 std::int32_t* controller1f88{};std::int32_t* internal1f8c{};
 const float* position160{};const float* rotation16c{};
 const float* initial_position1450{};float* initial_rotation145c{};
 const std::uintptr_t* room_zone2f4{};std::uint8_t* zoned2ef{};
};
enum class PlayerAddOperationV5:std::uint32_t {
 spawn=0x34b724,initialize_save=0x3b36b0,is_local=0x80f1ec,
 set_slot=0x3bb740,set_class=0x3bb814,online=0x7fd794,
 init_all=0x3b35f0,is_active=0x36d48c,init_camera=0x3b4bc4,
 set_idle=0x3c1a00,get_slots=0x36d730,set_skill_slot=0x3bbe54,
 skill_count=0x14e8,get_levels=0x36d76c,set_skill_level=0x3bbebc,
 set_visible=0x38b0f0,current_level=0x31f594,quick_save=0x3f059c,
 attach_controller=0x36f0dc,attach_light=0x371050,is_host=0x80f23c,
 get_host=0x36e09c,set_position=0x393db4,set_rotation=0x3938a0,
 set_initial_position=0x3a58f4,room_add=0x396a90,no_room_add=0x344184,
 zone_entered=0x38c710,remove_character=0x371d80,
 assert_spawn=0x3725fc,assert_skill_count=0x3723d0
};
struct PlayerAddRequestV5 {
 PlayerAddOperationV5 operation;
 std::uintptr_t subject{};std::int32_t argument{},secondary{};
 const char* name{};void* buffer{};std::size_t size{};
};
struct PlayerAddResponseV5 {std::int32_t value{};std::uintptr_t identity{};};
struct PlayerAddServicesV5 {
 std::shared_ptr<void> provider_lease;
 std::function<bool(const PlayerAddRequestV5&,PlayerAddResponseV5&,std::string&)> invoke;
 std::function<bool(PlayerInfoFieldsV1&,PlayerAddNetworkBorrowV5&,std::string&)> network_fields;
 std::function<bool(std::uintptr_t,PlayerAddCharacterBorrowV5&,std::string&)> character;
};
enum class PlayerAddPhaseV5 {not_started,guard,spawn,published,save,ids,placement,
 initialized,slots,levels,visible,quicksave,count,controller,light,complete};
struct PlayerAddResultV5 {PlayerAddPhaseV5 phase{};std::uint32_t calls{};bool count_written{},completed{},development_continuation{};};
// One owner per SAME PlayerInfo/Add invocation. Count is the actual manager
// field passed to character_initialization, never another count authority.
class PlayerAddCharacterOwnerV5 {
 PlayerManagerOwnerV1& manager_;PlayerAddServicesV5 services_;
 PlayerInfoFieldsV1* record_{};std::int32_t source_internal_input_{};bool attempted_{},busy_{},failed_{};
 PlayerAddResultV5 result_;
 bool call(PlayerAddRequestV5,PlayerAddResponseV5&,std::string&);
 bool continuation(PlayerInfoFieldsV1&,std::int32_t*,std::string&);
public:
 PlayerAddCharacterOwnerV5(PlayerManagerOwnerV1& m,PlayerAddServicesV5 s):manager_(m),services_(std::move(s)){}
 // Whole372220 after its actual manager lookup supplied this SAME record.
 bool add(PlayerInfoFieldsV1&,std::int32_t* same_count6c4,std::int32_t actual_internal_input,std::string&);
 // Explicit development adoption entry AFTER already-completed source660
 // publication. Does not clear660 or claim whole Spawn/original launch.
 bool continue_development_published(PlayerInfoFieldsV1&,std::int32_t*,std::int32_t actual_internal_input,std::uintptr_t expected_same_character,std::string&);
 bool failed()const noexcept{return failed_;}
 const PlayerAddResultV5& result()const noexcept{return result_;}
};
}
