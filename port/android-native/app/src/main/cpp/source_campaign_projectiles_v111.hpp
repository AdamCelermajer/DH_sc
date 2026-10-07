#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <projectile_manager_owner_v108.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Typed loan from the genuine loader-owned Projectile/Laser constructor.
// Native providers never allocate or replace that source class receiver.
struct SourceCampaignProjectileClassLoanV111 {
 std::shared_ptr<void> receiver;
 dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
 std::function<bool(std::uintptr_t,std::string&)> set_manager3e4fe4;
};
struct SourceCampaignProjectileClassServicesV111 {
 std::shared_ptr<void> owner; // independent actual canonical family authority
 std::function<bool(const SourceCampaignCandidateBorrowV55&,
  const dh2::world::CanonicalObjectBorrowV1&,bool,
  SourceCampaignProjectileClassLoanV111&,std::string&)> borrow;
};
// Called after genuine class catalog/resource enrollment and before first
// loading tick. Supplies only native manager/Create/DeSpawn; root composes
// SAME process Arrays.ProjectileTable and App FX registration at Stage29.
bool install_source_campaign_projectile_backend_v111(
 const SourceCampaignCandidateBorrowV55&,SourceCampaignProjectileClassServicesV111,
 std::string&);
}
