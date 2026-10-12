#pragma once
#include "model_renderer.hpp"
#include <retained_batch_visual_borrow_v100.hpp>
#include <canonical_character_candidate_v60.hpp>
#include <retained_character_family_visual_v6.hpp>
namespace model_renderer {
//Add only a missing visual_root leaf to Main's existing native batch bag.
//names optionally lends a genuinely produced native Root CString24; no defaults.
bool bind_source_retained_batch_visual_v100(const SourceCampaignCandidateBorrowV55&,
 dh2::loader::BatchNativeServicesV96&,dh2::loader::BatchRootNameBorrowV100,std::string&);
}