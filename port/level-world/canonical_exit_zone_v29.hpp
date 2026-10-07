#pragma once
#include "canonical_trigger_zone_v22.hpp"
#include <optional>
namespace dh2::world {
class CanonicalExitZoneV29;
struct ExitZoneServicesV29 {
 std::shared_ptr<void> owner;
 TriggerZoneServicesV22 trigger;
 std::function<bool(const char*,std::int32_t&,std::string&)> level_name_id;
 std::function<bool(CanonicalExitZoneV29&,std::string&)> whole_update,whole_destroy;
};
// Actual derived TriggerZoneExitLevel340ea0/39c9a4. Inherits the same
// TriggerZone/Trigger/ZoneEx/Zone state with source GO14, never a second base.
class CanonicalExitZoneV29:public CanonicalTriggerZoneV22 {
 ExitZoneServicesV29 services_;
 std::int32_t level_list_id7d4_{-1};
 std::optional<std::int32_t> level_id7d8_,entrypoint7f4_,maploc7f8_,question7fc_;
 std::string level_name7dc_,fasttravel800_;
 std::uint8_t byte818_{},byte819_{};
 bool destroyed_{};
public:
 CanonicalExitZoneV29(std::shared_ptr<void>,actor::RuntimeState&,ExitZoneServicesV29);
 CanonicalExitZoneV29(const CanonicalExitZoneV29&)=delete;
 CanonicalPropertyActorV1 properties()noexcept;
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){
  auto object=CanonicalTriggerZoneV22::canonical(std::move(lease));
  object.type14_localization_valid819=&byte819_;return object;
 }
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 std::int32_t* source_integer(std::uint32_t)noexcept;
 std::uint8_t* source_byte(std::uint32_t)noexcept;
 std::string* source_string(std::uint32_t)noexcept;
 bool init_post(std::string&);
 bool update(std::string&);
 bool destroy(std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalExitZoneV29>,std::shared_ptr<const void>);
};
}
