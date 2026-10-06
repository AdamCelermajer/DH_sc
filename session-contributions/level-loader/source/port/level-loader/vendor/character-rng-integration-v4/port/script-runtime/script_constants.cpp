#include "script_constants.hpp"
#include <algorithm>
#include <cstring>
#include <map>
#include <new>
#include <string>
struct dh2_script_constants {
  std::map<std::string,std::map<std::string,int32_t>> groups;
};
namespace {
struct Reader {
  const uint8_t* data;
  uint32_t size, position=0;
  bool word(uint32_t& value) {
    if(size-position<4)return false;
    const uint8_t* p=data+position;position+=4;
    value=uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);
    return true;
  }
  int name(std::string& out) {
    uint32_t length;
    if(!word(length))return -1;
    uint32_t consumed=std::min(length,uint32_t(255));
    if(consumed>size-position)return -1;
    const uint8_t* start=data+position;position+=consumed;
    if(length>=256)return 1;
    uint32_t nul=0;while(nul<length && start[nul])++nul;
    out.assign(reinterpret_cast<const char*>(start),nul);
    return 0;
  }
};
}
extern "C" dh2_script_constants* dh2_script_constants_create() {
  try{return new dh2_script_constants;}catch(...){return nullptr;}
}
extern "C" void dh2_script_constants_destroy(dh2_script_constants* table){delete table;}
extern "C" void dh2_script_constants_clear(dh2_script_constants* table){if(table)table->groups.clear();}
extern "C" int dh2_script_constants_load(dh2_script_constants* table,const uint8_t* data,uint32_t size,dh2_script_constants_reload* out) {
  if(out)*out={};
  if(!table||!data||!out)return -1;
  Reader reader{data,size};
  auto finish=[&](int status){out->consumed=reader.position;out->source_name_stop=status==1;return status;};
  try {
    uint32_t count;if(!reader.word(count))return finish(-1);
    std::string group,name;
    for(uint32_t g=0;g<count;++g) {
      int status=reader.name(group);if(status)return finish(status);
      uint32_t entries;if(!reader.word(entries))return finish(-1);
      for(uint32_t e=0;e<entries;++e) {
        status=reader.name(name);if(status)return finish(status);
        uint32_t word;if(!reader.word(word))return finish(-1);
        int32_t value;std::memcpy(&value,&word,4);
        table->groups[group][name]=value;++out->assignments;
      }
      ++out->groups_complete;
    }
    return finish(0);
  }catch(const std::bad_alloc&){return finish(-2);}
}
extern "C" int dh2_script_constants_get(const dh2_script_constants* table,const char* group,const char* name,int32_t* out) {
  if(!table||!group||!name||!out)return -1;
  *out=0;
  try {
    auto g=table->groups.find(group);if(g==table->groups.end())return 0;
    auto entry=g->second.find(name);if(entry!=g->second.end())*out=entry->second;
    return 0;
  }catch(const std::bad_alloc&){return -2;}
}
extern "C" uint32_t dh2_script_constants_size(const dh2_script_constants* table) {
  if(!table)return 0;
  uint32_t count=0;for(const auto& g:table->groups)count+=static_cast<uint32_t>(g.second.size());
  return count;
}
extern "C" int dh2_script_constants_lookup(void* context,uint32_t kind,const char* group,const char* name,int32_t* out) {
  if(kind)return -1;
  return dh2_script_constants_get(static_cast<const dh2_script_constants*>(context),group,name,out);
}
