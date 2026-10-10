#pragma once
#include "../../../android-native/app/src/main/cpp/model_renderer.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_runtime_v61.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../level-world/character_mesh_fx_owner_v4.hpp"
#include "source_current_level_backend_v1.hpp"
namespace dh::foundation::actor_frame {
enum class SourceCampaignBackendModeV1 : std::uint8_t { menu, campaign };
// The root lends existing owners; no native storage is constructed here.
struct SourceCampaignBackendServicesV1 {
  std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
  std::function<bool(std::uintptr_t, bool, std::string &)> zoning;
  std::function<bool(std::uintptr_t, std::uintptr_t, std::string &)> interact,
      noncharacter_interact;
  std::function<int(dh2::world::CanonicalCharacterCandidateRecordV60 &,
                    const dh2::character::StateOwnerRequest48 &, std::string &)>
      limbus;
  std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets;
  std::function<bool(std::uintptr_t, bool &, std::string &)> ai_turn;
  std::function<bool(std::uint32_t, std::uintptr_t, std::uintptr_t,
                     const dh2_script_callback_scope *, std::string &)>
      aggro;
  std::function<bool(std::uintptr_t, std::uintptr_t,
                     const dh2_script_callback_scope *, std::string &)>
      clear_aggro;
  // Aliasing shared_ptr must retain the real manager's enclosing owner.
  std::shared_ptr<dh2::fx::CharacterMeshFxOwnerV4> fx;
  dh2::audio::AudioApplicationBorrowV42 audio;
  std::shared_ptr<dh2::audio::AudioCampaignBridgeV46> audio_bridge;
};
struct SourceCampaignBackendGraphV1 {
  SourceCampaignBackendModeV1 mode{SourceCampaignBackendModeV1::campaign};
  model_renderer::SourceCampaignCandidateBorrowV55 candidate;
  std::shared_ptr<model_renderer::SourceWorldBorrowV61> world;
  std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60> characters;
  std::shared_ptr<SourceCurrentLevelBackendV1> current_level;
  SourceCampaignBackendServicesV1 services;
};
class SourceCampaignBackendContextV1 {
  SourceCampaignBackendGraphV1 graph_;

public:
  explicit SourceCampaignBackendContextV1(SourceCampaignBackendGraphV1 graph)
      : graph_(std::move(graph)) {}
  bool validate(std::string &) const;
  const SourceCampaignBackendGraphV1 &graph() const noexcept { return graph_; }
  // Root externally retains this context. Never retain it in a
  // record/FSM/Session. Sequential native runtime thread. Registry is weak;
  // root controls lifetime.
  static bool
  register_current(const std::shared_ptr<SourceCampaignBackendContextV1> &,
                   std::string &);
  static void unregister_current(
      const std::shared_ptr<SourceCampaignBackendContextV1> &) noexcept;
  static std::shared_ptr<SourceCampaignBackendContextV1>
  borrow_current(std::string &);
};
} // namespace dh::foundation::actor_frame
