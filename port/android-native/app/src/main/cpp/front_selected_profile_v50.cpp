#include "front_selected_profile_v50.hpp"
namespace dh2::android_ui {
bool retain_front_selected_profile_v50(data::CampaignProfileFileV1 file,
 data::MenuProfileMetadataV1 metadata,const std::string& directory,std::int32_t difficulty,
 std::shared_ptr<const FrontSelectedProfileV50>& out,std::string& error){
 if(directory.empty()||metadata.slot<0||metadata.slot>=4){error="Required actual selected campaign directory/slot";return false;}
 data::PlayerProfileIndexV1 index;
 if(!index.load({file.bytes.data(),file.bytes.size()},error))return false;
 auto value=std::make_shared<FrontSelectedProfileV50>();value->file=std::move(file);
 value->metadata=std::move(metadata);value->private_directory=directory;value->requested_difficulty=difficulty;
 {auto source=index.borrow();value->has_gear=source.section("GEAR")!=nullptr;
  value->has_properties=source.section("PROP")!=nullptr;value->has_quests=source.section("QEST")!=nullptr;
  value->has_skills=source.section("SKIL")!=nullptr;value->has_faeries=source.section("FAES")!=nullptr;
 }
 out=std::move(value);error.clear();return true;
}
}
