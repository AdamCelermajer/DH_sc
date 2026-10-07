#pragma once
#include "canonical_level_config_module_v1.hpp"
#include "gameplay_camera_load_v19.hpp"
#include "../level-loader/xml_document_v1.hpp"
namespace dh2::world {
struct SelectedWorldCameraServicesV20 {
 std::shared_ptr<void> world;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 // Actual source/adoption registration policy supplied by this World; helper
 // never guesses room/network flags. Must use the supplied SAME manager.
 std::function<bool(CanonicalObjectBorrowV1,const std::string&,const std::string&,std::string&)> register_object;
 // Must publish this SAME config into the actual selected Level/world context.
 // A development context must explicitly retain it, rather than claim GSLevel.
 std::function<bool(std::shared_ptr<CanonicalLevelConfigV1>,std::string&)> publish;
};
class SelectedWorldCameraConfigV20 {
 loader::XmlDocumentV1 document_;std::shared_ptr<CanonicalLevelConfigV1> config_;
 bool attempted_{},ready_{};std::uint32_t element_{};
public:
 bool load(std::string uri,std::vector<std::uint8_t> actual_mlx,CanonicalPropertyMapV1&,CanonicalObjectManagerV1&,SelectedWorldCameraServicesV20,std::string&);
 bool camera(camera::CameraLevelConfigV19&,std::string&)const;
 const std::shared_ptr<CanonicalLevelConfigV1>& config()const noexcept{return config_;}
 const loader::XmlDocumentV1& document()const noexcept{return document_;}
};
}
