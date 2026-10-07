#pragma once
#include "renderer_menu_preview_domain_v121.hpp"
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace model_renderer {
// Fills only the process resource/visual/physical portion. The caller binds
// the SAME Character's Save, script, FSM and authored animation consumers.
bool compose_native_menu_preview_character_resources_v122(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const MenuPreviewProcessDomainV121&,
 dh2::world::CanonicalCharacterCandidateServicesV60&,std::string&);
bool initialize_native_menu_preview_character_light_material_v122(
 dh2::world::CanonicalCharacterCandidateRecordV60&,const MenuPreviewProcessDomainV121&,std::string&);
}
