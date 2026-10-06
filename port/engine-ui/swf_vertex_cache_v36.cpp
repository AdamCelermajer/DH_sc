#include "swf_vertex_cache_v36.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh2::ui {
std::uint64_t SwfVertexCacheV36::fingerprint(const float* vertices,std::size_t count,std::uint32_t mode)noexcept{
 std::uint64_t hash=14695981039346656037ull;
 auto add=[&](const void* value,std::size_t n){auto* bytes=static_cast<const unsigned char*>(value);for(std::size_t i=0;i<n;++i){hash^=bytes[i];hash*=1099511628211ull;}};
 add(&mode,sizeof mode);add(&count,sizeof count);add(vertices,count*sizeof(float));return hash;
}
void SwfVertexCacheV36::erase(std::list<Entry>::iterator entry,const std::function<void(std::uintptr_t)>& release){
 auto range=index_.equal_range(entry->hash);for(auto i=range.first;i!=range.second;++i)if(i->second==entry){index_.erase(i);break;}
 if(entry->buffer&&release)release(entry->buffer);
 stats_.resident_bytes-=entry->vertices.size()*sizeof(float);--stats_.resident_entries;entries_.erase(entry);
}
bool SwfVertexCacheV36::acquire(const std::vector<float>& vertices,std::uint32_t mode,const Upload& upload,
 const std::function<void(std::uintptr_t)>& release,std::uintptr_t& out,bool& retained,std::string& error){
 out=0;retained=false;
 if(!upload||vertices.empty()||vertices.size()>std::numeric_limits<std::size_t>::max()/sizeof(float)){
  error="Invalid actual SWF vertex-cache upload request";return false;
 }
 const auto bytes=vertices.size()*sizeof(float);
 // Oversized source primitives still render through the caller's stream VBO.
 // The bounded cache is a resource policy, never a drawing-size reduction.
 if(!maximum_entries_||bytes>maximum_bytes_){error.clear();return true;}
 const auto hash=fingerprint(vertices.data(),vertices.size(),mode);
 auto range=index_.equal_range(hash);
 for(auto i=range.first;i!=range.second;++i){auto entry=i->second;
  if(entry->mode!=mode||entry->vertices.size()!=vertices.size()||std::memcmp(entry->vertices.data(),vertices.data(),bytes))continue;
  if(!entry->buffer){
   if(!upload(entry->vertices.data(),entry->vertices.size(),entry->buffer,error)||!entry->buffer){if(entry->buffer&&release)release(entry->buffer);entry->buffer=0;return false;}
   ++stats_.uploads;++stats_.context_reuploads;stats_.uploaded_bytes+=bytes;
  }
  ++stats_.hits;entries_.splice(entries_.end(),entries_,entry);out=entry->buffer;retained=true;error.clear();return true;
 }
 ++stats_.misses;
 while(!entries_.empty()&&(stats_.resident_entries>=maximum_entries_||stats_.resident_bytes>maximum_bytes_-bytes)){
  erase(entries_.begin(),release);++stats_.evictions;
 }
 Entry candidate;candidate.hash=hash;candidate.mode=mode;candidate.vertices=vertices;
 if(!upload(candidate.vertices.data(),candidate.vertices.size(),candidate.buffer,error)||!candidate.buffer){if(candidate.buffer&&release)release(candidate.buffer);return false;}
 auto unowned=candidate.buffer;
 try{entries_.push_back(std::move(candidate));auto entry=std::prev(entries_.end());
  unowned=0;
  try{index_.emplace(hash,entry);}catch(...){if(entry->buffer&&release)release(entry->buffer);entries_.erase(entry);throw;}
 }catch(const std::exception& e){if(unowned&&release)release(unowned);error=e.what();return false;}
 ++stats_.uploads;stats_.uploaded_bytes+=bytes;stats_.resident_bytes+=bytes;++stats_.resident_entries;
 out=entries_.back().buffer;retained=true;error.clear();return true;
}
void SwfVertexCacheV36::clear(const std::function<void(std::uintptr_t)>& release){
 while(!entries_.empty())erase(entries_.begin(),release);
}
void SwfVertexCacheV36::abandon_context()noexcept{for(auto& entry:entries_)entry.buffer=0;}
bool swf_append_triangles_v36(std::vector<float>& out,const std::vector<float>& source,bool strip,std::string& error){
 if(source.size()%4){error="Invalid packed source SWF vertex stride";return false;}
 const auto count=source.size()/4;
 if(!strip){const auto accepted=count-count%3;out.insert(out.end(),source.begin(),source.begin()+accepted*4);error.clear();return true;}
 if(count<3){error.clear();return true;}
 if(count-2>(std::numeric_limits<std::size_t>::max()-out.size())/12){error="SWF triangle batch span overflow";return false;}
 out.reserve(out.size()+(count-2)*12);
 for(std::size_t i=2;i<count;++i){const std::size_t indices[]{i&1?i-1:i-2,i&1?i-2:i-1,i};
  for(auto vertex:indices)out.insert(out.end(),source.begin()+vertex*4,source.begin()+vertex*4+4);
 }
 error.clear();return true;
}
}
