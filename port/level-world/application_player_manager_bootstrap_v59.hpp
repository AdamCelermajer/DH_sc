#pragma once
#include "player_manager_combat_runtime_v2.hpp"
#include "player_network_local_owner_v4.hpp"
#include "player_info_skill_buffers_v26.hpp"
#include <functional>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::player {
struct FirstLocalControllerBorrowV59 {
 std::uintptr_t identity{};const std::uint8_t* connected758{};
 std::shared_ptr<void> receiver;
};
struct FirstLocalControllerServicesV59 {
 std::shared_ptr<void> provider;void* context{};
 // Actual InputManager.GetNumGamepads34d754 and virtual8 GetGamepad(0).
 // Source clamps count to >=1 and still queries controller0 before GetOnline.
 bool(*count)(void*,std::int32_t&,std::string&){};
 bool(*controller_zero)(void*,FirstLocalControllerBorrowV59&,std::string&){};
};
enum class PlayerManagerBootstrapPhaseV59 {allocated,constructor_failed,constructed,
 adopted,first_local_failed,first_local_added,first_local_retained};
class ApplicationPlayerManagerBootstrapV59 final :public std::enable_shared_from_this<ApplicationPlayerManagerBootstrapV59> {
 struct ProviderLifetime {std::weak_ptr<application::ApplicationServicesOwnerV5> application;};
 std::shared_ptr<ProviderLifetime> lifetime_;
 std::unique_ptr<PlayerNetworkLocalOwnerV4> network_;
 std::unique_ptr<PlayerManagerCombatRuntimeV2> runtime_;
 PlayerManagerCombatRuntimeV2* actual_runtime_{};PlayerNetworkLocalOwnerV4* actual_network_{};
 bool loaned_{};
 std::map<PlayerInfoFieldsV1*,std::shared_ptr<PlayerInfoSkillBuffersV26>> buffers_;
 std::shared_ptr<void> predecessor_provider_;
 std::shared_ptr<void> predecessor_buffers_provider_;
 std::function<bool(PlayerInfoFieldsV1&,std::shared_ptr<PlayerInfoSkillBuffersV26>&,std::string&)> predecessor_buffers_;
 std::shared_ptr<void> remaining_provider_;PlayerManagerServicesV1 remaining_{};
 PlayerManagerBootstrapPhaseV59 phase_{PlayerManagerBootstrapPhaseV59::allocated};
 std::uint32_t service_depth_{};bool busy_{},profile_input_pending_{};std::uint8_t observed_controller_connected_{};
 std::string error_;
 explicit ApplicationPlayerManagerBootstrapV59(const std::shared_ptr<application::ApplicationServicesOwnerV5>&);
 static bool service(void*,const PlayerManagerRequestV1&,PlayerManagerResponseV1&,std::string&);
 bool network_enabled(bool&,std::string&);
 bool available(std::string&)const;
 bool return_loan(std::unique_ptr<PlayerManagerCombatRuntimeV2>&,
  std::unique_ptr<PlayerNetworkLocalOwnerV4>&,std::string&);
public:
 // Caller first inspects Application/legacy owner slots. Never call this
 // fresh branch when a prior manager exists; adopt_existing preserves it.
 static bool create_fresh(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
  MatchingLocalSelectionOwnerV4&,std::shared_ptr<ApplicationPlayerManagerBootstrapV59>&,std::string&);
 static bool adopt_existing(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
  std::unique_ptr<PlayerManagerCombatRuntimeV2>&,std::unique_ptr<PlayerNetworkLocalOwnerV4>&,
  PlayerManagerCombatServicesV2 expected_provider,std::shared_ptr<void> actual_previous_provider,
  std::shared_ptr<ApplicationPlayerManagerBootstrapV59>&,std::string&);
 // Does not emulate the remainder of _CheckLocalControllers (profile prompts,
 // all other controllers, network users or controller removal). This is its
 // original first offline AddPlayer prefix, with mandatory upstream queries.
 bool source_first_local_add_prefix(const FirstLocalControllerServicesV59&,std::string&);
 // Modern front-end transport after the actual menu selection/file read.
 // Publishes only its selected slot into the existing offline PlayerInfo;
 // Character construction and Save::SetSlot remain later source operations.
 bool publish_selected_save_slot_v67(std::int32_t local_index,std::int32_t selected_slot,std::string&);
 bool publish_selected_profile_v68(std::int32_t local_index,std::int32_t selected_slot,std::int32_t actual_character_row,std::string&);
 bool bind_remaining(std::shared_ptr<void>,PlayerManagerServicesV1,std::string&);
 bool bind_existing_buffers(std::shared_ptr<void>,
  std::function<bool(PlayerInfoFieldsV1&,std::shared_ptr<PlayerInfoSkillBuffersV26>&,std::string&)>,std::string&);
 // Native lifetime transport for existing runtime unique_ptr consumers. SAME
 // objects remain App-published via observed pointers; caller runs return
 // callback before destroying its fields. No C1/adoption/source field replay.
 bool lend_to_runtime(std::unique_ptr<PlayerManagerCombatRuntimeV2>&,
  std::unique_ptr<PlayerNetworkLocalOwnerV4>&,std::function<void()>& return_before_fields,std::string&);
 PlayerManagerCombatRuntimeV2* runtime()noexcept{return actual_runtime_;}
 PlayerManagerOwnerV1* manager()noexcept{return actual_runtime_?&actual_runtime_->manager():nullptr;}
 PlayerNetworkLocalOwnerV4* network()noexcept{return actual_network_;}
 bool get_local_player(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 bool get_by_internal(std::int32_t,bool,PlayerInfoFieldsV1*&,std::string&);
 // Whole IsLocalPlayer36effc: NULL returns0; nonnull source lookup then
 // actual selected PlayerInfo virtual50=CNetPlayerInfo.IsLocal80f1ec.
 // Constructor dummy is a real receiver, not a missing-association false.
 bool source_is_local_player_v61(std::uintptr_t,bool&,std::string&);
 bool skill_buffers(PlayerInfoFieldsV1&,std::shared_ptr<PlayerInfoSkillBuffersV26>&,std::string&);
 const std::int32_t* count_field()const noexcept{return actual_runtime_?actual_runtime_->manager().character_count_field():nullptr;}
 PlayerManagerBootstrapPhaseV59 phase()const noexcept{return phase_;}
 std::uint8_t observed_controller_connected()const noexcept{return observed_controller_connected_;}
 // Existing receiver with slot664=-1 reaches the original controller/menu
 // input tail at378e58. Prefix completion never claims that tail succeeded.
 bool profile_input_pending()const noexcept{return profile_input_pending_;}
 const std::string& error()const noexcept{return error_;}
 bool belongs_to_application(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app)const noexcept{
  const auto actual=lifetime_?lifetime_->application.lock():nullptr;
  return actual&&app&&actual.get()==app.get()&&!actual.owner_before(app)&&!app.owner_before(actual);
 }
};
}
