#pragma once
#include <player_manager_owner_v1.hpp>
#include <memory>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace model_renderer {
// Journal owns sequencing only; same PM/Character/World allocate all fields.
// A failed phase resumes only when its owning native lifecycle calls again.
class SourcePlayerCharacterRemovalV114 {
 struct Impl;std::unique_ptr<Impl> p_;
public:
 SourcePlayerCharacterRemovalV114();~SourcePlayerCharacterRemovalV114();
 bool execute(const std::shared_ptr<void>& actual_world,
  const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
  const dh2::player::PlayerManagerRequestV1&,std::string&);
};
}
