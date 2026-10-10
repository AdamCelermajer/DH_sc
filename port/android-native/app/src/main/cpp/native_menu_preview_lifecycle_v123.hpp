#pragma once
#include "renderer_menu_preview_domain_v121.hpp"
#include "canonical_character_candidate_v60.hpp"
namespace model_renderer {
// Implemented by the one menu GPU transport, with same-domain/delivery checks.
// This retires this receiver's submitted loans before its Gear/visual D0.
bool retire_native_menu_character_draws_v123(const std::shared_ptr<void>& process_domain,
 std::uintptr_t character,std::string&);
class NativeMenuPreviewLifecycleV123;
bool acquire_native_menu_preview_lifecycle_v123(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const MenuPreviewProcessDomainV121&,std::shared_ptr<NativeMenuPreviewLifecycleV123>&,std::string&);
// The factory produces the actual record before ObjectManager.Add publication.
// Receiver/factory loans stay pinned through map and native orphan unpublication.
bool register_native_menu_preview_lifecycle_character_v123(
 const std::shared_ptr<NativeMenuPreviewLifecycleV123>&,
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60>&,
 const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,std::string&);
// The real per-Character service owner supplies its retained FX instance backend.
bool detach_native_menu_preview_fx_anchors_v123(
 dh2::world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t provider_character,std::uintptr_t retiring_character,std::string&);
bool borrow_native_menu_preview_fx_owner_v123(
 dh2::world::CanonicalCharacterCandidateRecordV60&,std::string&);
bool retire_native_menu_preview_script_receiver_v123(
 dh2::world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,
 const std::shared_ptr<dh2::character::ScriptCharacterObject>&,std::string&);
void forget_native_menu_preview_character_v123(const std::shared_ptr<void>&,
 std::uintptr_t,const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&);
}
