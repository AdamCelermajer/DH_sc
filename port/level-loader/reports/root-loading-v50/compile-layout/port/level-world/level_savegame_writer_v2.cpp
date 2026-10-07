#include "level_savegame_writer_v2.hpp"
#include <array>
#include <map>
namespace dh2::level {
bool savegame_build_frame_v2(const data::PlayerProfileIndexV1::Borrow& snapshot,const std::vector<SavegameSectionWriterV2>& writers,std::unique_ptr<SavegameStreamV2>& output,std::string& e){
 struct Section{std::array<std::uint8_t,4>tag{};std::vector<std::uint8_t>payload;const SavegameSectionWriterV2* writer{};};std::map<std::string,Section> directory;
 if(snapshot)for(const auto& entry:snapshot.source_sections()){std::size_t n=0;while(n<4&&entry.tag[n])++n;std::string name(reinterpret_cast<const char*>(entry.tag),n);Section section;std::copy(entry.tag,entry.tag+4,section.tag.begin());auto bytes=snapshot.payload(name.c_str());if(bytes.size)section.payload.assign(bytes.data,bytes.data+bytes.size);directory[name]=std::move(section);}
 for(const auto& writer:writers){std::size_t n=0;while(n<4&&writer.tag[n])++n;std::string name(reinterpret_cast<const char*>(writer.tag.data()),n);auto& section=directory[name];section.tag=writer.tag;section.writer=&writer;}
 auto stream=std::make_unique<SavegameStreamV2>();if(!stream->write_u32(0xffffffffu,e))return false;
 for(auto& pair:directory){auto length_at=stream->tell_write();if(!stream->write_u32(0,e)||!stream->write({pair.second.tag.data(),4},e))return false;auto start=stream->tell_write();
  if(pair.second.writer&&pair.second.writer->write){if(!pair.second.writer->write(pair.second.writer->context,*stream,e))return false;}
  else if(!stream->write({pair.second.payload.data(),pair.second.payload.size()},e))return false;
  auto end=stream->tell_write();if(end<start||end-start>UINT32_MAX){e="Source save section payload-size domain exceeded";return false;}stream->seek_write(length_at);if(!stream->write_u32(static_cast<std::uint32_t>(end-start),e))return false;stream->seek_write(end);
 }
 auto end=stream->tell_write();stream->seek_write(0);if(!stream->write_u32(static_cast<std::uint32_t>(directory.size()),e))return false;stream->seek_write(end);
 output=std::move(stream);return true;
}
bool level_savegame_save_all_v2(LevelSavegameCacheV1& cache,LevelSavegameFieldsV1& fields,const LevelSaveObjectsServicesV2& objects,SavegameJobsOwnerV2& jobs,std::string& e){
 e.clear();if(!cache.ready()||!fields.level8){e="Required same ready Level Savegame cache/Level";return false;}
 if(!jobs.add_backup(cache.filename(),e))return false;
 auto info=[](void* p,SavegameStreamV2& stream,std::string& err){return stream.write_u32(static_cast<std::uint32_t>(static_cast<LevelSavegameFieldsV1*>(p)->row28),err);};
 struct ObjectsBorrow{const LevelSaveObjectsServicesV2* services;};ObjectsBorrow borrowed{&objects};
 auto obs=[](void* p,SavegameStreamV2& stream,std::string& err){return level_savegame_save_objects_v2(stream,*static_cast<ObjectsBorrow*>(p)->services,err);};
 std::vector<SavegameSectionWriterV2> writers={{{'I','N','F','O'},&fields,info},{{'O','B','J','S'},&borrowed,obs}};
 std::unique_ptr<SavegameStreamV2> stream;{auto snapshot=cache.profile();if(!savegame_build_frame_v2(snapshot,writers,stream,e))return false;}
 const auto& bytes=stream->bytes();if(!cache.recache_v2({bytes.data(),bytes.size()},e))return false;
 return jobs.add_write(cache.filename(),std::move(stream),e);
}
}
