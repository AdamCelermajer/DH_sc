#include "native_quest_cursor.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh2::native::quests {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(an&&bn&&x<y+bn&&y<x+an);
}
}
Cursor::Cursor(data::PlayerProfileIndexV1::Borrow profile,const char* section):
 profile_(std::move(profile)),stream_{reinterpret_cast<std::uintptr_t>(this)}{
 const auto* entry=profile_.section(section);
 if(!profile_||!entry)throw std::invalid_argument("Retained campaign section required");
 position_=entry->offset;
}
bool Cursor::matches(const data::player_saved_quests_v1::StreamRef& stream) const noexcept{
 return &stream==&stream_&&stream.identity==reinterpret_cast<std::uintptr_t>(this);
}
bool Cursor::protected_storage(const void* pointer,std::size_t size) const noexcept{
 const auto& bytes=profile_.bytes();const auto& sections=profile_.source_sections();
 return overlap(pointer,size,this,sizeof(*this))||overlap(pointer,size,&bytes,sizeof(bytes))||
  overlap(pointer,size,bytes.data(),bytes.size())||overlap(pointer,size,&sections,sizeof(sections))||
  overlap(pointer,size,sections.data(),sections.size()*sizeof(sections[0]));
}
bool Cursor::tell(const data::player_saved_quests_v1::StreamRef& stream,std::uint64_t* out) const noexcept{
 if(!matches(stream)||!out||reinterpret_cast<std::uintptr_t>(out)%alignof(std::uint64_t)||
    protected_storage(out,sizeof(*out)))return false;
 *out=position_;return true;
}
bool Cursor::seek(const data::player_saved_quests_v1::StreamRef& stream,std::uint64_t absolute) noexcept{
 if(!matches(stream)||absolute>profile_.bytes().size())return false;
 position_=absolute;return true;
}
bool Cursor::read(const data::player_saved_quests_v1::StreamRef& stream,void* destination,std::uint64_t requested,std::uint64_t* delivered) noexcept{
 if(!matches(stream)||!delivered||reinterpret_cast<std::uintptr_t>(delivered)%alignof(std::uint64_t))return false;
 const auto& bytes=profile_.bytes();
 if(position_>bytes.size())return false;
 const auto count=std::size_t(std::min(requested,std::uint64_t(bytes.size())-position_));
 if((count&&!destination)||protected_storage(delivered,sizeof(*delivered))||
    overlap(delivered,sizeof(*delivered),destination,count)||
    protected_storage(destination,count))return false;
 if(count)std::memcpy(destination,bytes.data()+std::size_t(position_),count);
 position_+=count;*delivered=count;return true;
}
}
