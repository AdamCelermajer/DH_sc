#include "player_save_metadata_writer_v45.hpp"
#include <cstring>
namespace dh2::level {
PlayerSaveMetadataWriterV45::PlayerSaveMetadataWriterV45(std::shared_ptr<data::PlayerSaveLoadOwnerV1> actual,PlayerSaveMetadataServicesV45 s):authority_(std::move(actual)),services_(std::move(s)){}
bool PlayerSaveMetadataWriterV45::write(const char* section,SavegameStreamV2& stream,std::string& e){
 if(writing_||!authority_||!section){e="Required SAME nonrecursive metadata Save owner/section";return false;}
 writing_=true;struct End{bool& v;~End(){v=false;}}end{writing_};auto& save=authority_->save();
 // Whole original metadata writers captured source-writers.asm.
 if(!std::strcmp(section,"PCLS")){
  const auto id=save.class_id();if(id<0)return true; // original signed-negative empty payload gate
  if(!services_.tables||!services_.characters||std::size_t(id)>=services_.characters->names.size()){
   e="Required actual Arrays CharacterTable name/assertion domain for PCLS";return false;
  }
  return stream.write_string(services_.characters->names[std::size_t(id)],e); // 4698e4 ->461668
 }
 if(!std::strcmp(section,"PDFL")){
  std::int32_t actual{};if(!services_.current_difficulty||!services_.current_difficulty(actual,e)){if(e.empty())e="Required SAME source CurrentDifficulty global";return false;}
  if(!stream.write_u32(std::uint32_t(actual),e))return false;
  return stream.write_u32(std::uint32_t(save.unlocked_difficulty()),e); // source second field reload
 }
 if(!std::strcmp(section,"LNAM")){
  if(!stream.write_u32(save.location().save_date,e))return false;
  for(unsigned i=0;i<3;++i)if(!stream.write_u32(std::uint32_t(save.location().levels[i]),e)||
   !stream.write_u32(std::uint32_t(save.location().seeds[i]),e)||
   !stream.write_u32(std::uint32_t(save.location().current_acts[i]),e))return false;
  return true;
 }
 if(!std::strcmp(section,"LEPT")){for(unsigned i=0;i<3;++i)if(!stream.write_u32(std::uint32_t(save.location().entry_points[i]),e))return false;return true;}
 if(!std::strcmp(section,"LUSP")){for(unsigned i=0;i<3;++i){const auto value=save.location().use_spawn_point[i];if(!stream.write({&value,1},e))return false;}return true;}
 e="Required original metadata section writer "+std::string(section);return false;
}
}
