#pragma once
#include "renderer_menu_preview_domain_v121.hpp"
namespace model_renderer {
//Bind LAST, after the actual instance providers. The source block3b51e8
//tests the retained manager's1c/20 vector, not authored Arrays row counts.
bool bind_native_menu_preview_fx_initialization_v122(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const MenuPreviewProcessDomainV121&,
 dh2::world::CanonicalCharacterCandidateServicesV60&,std::string&);
}
