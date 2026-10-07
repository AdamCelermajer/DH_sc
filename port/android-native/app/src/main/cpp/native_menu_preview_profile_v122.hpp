#pragma once
#include "renderer_menu_preview_domain_v121.hpp"
#include "canonical_character_candidate_v60.hpp"
namespace model_renderer {
bool prepare_native_menu_preview_profile_v122(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const MenuPreviewProcessDomainV121&,dh2::world::CanonicalCharacterCandidateRecordV60&,std::shared_ptr<void>&,std::string&);
bool finish_native_menu_preview_profile_v122(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 dh2::world::CanonicalCharacterCandidateRecordV60&,std::string&);
bool save_native_menu_preview_profile_v122(dh2::world::CanonicalCharacterCandidateRecordV60&,std::string&);
bool destroy_native_menu_preview_profile_v122(dh2::world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&);
}
