#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::camera {class CameraAnchorOwnerV75;}
namespace model_renderer {
struct SourceWorldBorrowV61;
struct SourceCampaignAnchorDirectoryV75;
bool borrow_source_campaign_anchor_v80(const std::shared_ptr<void>& actual_world,std::uintptr_t anchor,std::shared_ptr<dh2::camera::CameraAnchorOwnerV75>&,std::string&);
//Source AddCharacter InitCam3b4bc4, separate from Level._LoadCamera20.
bool source_campaign_character_init_camera_v75(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::string&);
bool source_campaign_anchor_position_v75(const std::shared_ptr<void>& actual_world,std::uintptr_t anchor,const float*&,std::string&);
bool source_campaign_anchor_update_v75(const std::shared_ptr<void>& actual_world,std::uintptr_t anchor,std::string&);
bool source_campaign_anchor_delete_v75(const std::shared_ptr<void>& actual_world,std::uintptr_t anchor,std::string&);
}
