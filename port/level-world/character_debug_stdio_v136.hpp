#pragma once
#include "character_design_services.hpp"
#include <cstdio>
#include <limits>
#include <string>
namespace dh2::character {
// Actual native stdio IStream leaves only. The caller supplies its own
// FileManager-authoritative open-write path, and keeps context/FILEs alive.
inline DebugExistingFileServicesV136 debug_stdio_services_v136(
 int(*open_write)(void*,const char*,std::uintptr_t*)){
 DebugExistingFileServicesV136 result{};result.open_write=open_write;
 result.remaining=[](void*,std::uintptr_t raw,std::uint64_t* out){
  auto* file=reinterpret_cast<std::FILE*>(raw);if(!file||!out)return 1;
  const auto at=std::ftell(file);if(at<0||std::fseek(file,0,SEEK_END))return 1;
  const auto end=std::ftell(file);const auto restored=std::fseek(file,at,SEEK_SET);
  if(end<at||restored)return 1;
  *out=static_cast<std::uint64_t>(end-at);return 0;
 };
 result.read=[](void*,std::uintptr_t raw,void* out,std::uint32_t count){
  auto* file=reinterpret_cast<std::FILE*>(raw);return !file||(!out&&count)||std::fread(out,1,count,file)!=count?1:0;
 };
 result.seek_relative=[](void*,std::uintptr_t raw,std::int64_t delta){
  if(!raw||delta<std::numeric_limits<long>::min()||delta>std::numeric_limits<long>::max())return 1;
  return std::fseek(reinterpret_cast<std::FILE*>(raw),static_cast<long>(delta),SEEK_CUR)?1:0;
 };
 result.write=[](void*,std::uintptr_t raw,const void* data,std::uint32_t count){
  auto* file=reinterpret_cast<std::FILE*>(raw);return !file||(!data&&count)||std::fwrite(data,1,count,file)!=count?1:0;
 };
 return result;
}
// Production convenience route. Keep the existing actual FileManager's read
// and close callbacks; the caller supplies its SAME authoritative directory.
// The packet only borrows directory/provider during this synchronous call.
inline int load_debug_stdio_v136(DebugSwitches* debug,const DebugFileServices24* actual,
 const char* directory){
 if(!actual||!actual->open_read||!actual->close_read||!directory||!*directory)return -1;
 struct Delivery {
  const DebugFileServices24* actual;const char* directory;
  static int open(void* raw,const char* name,std::uintptr_t* out){auto& self=*static_cast<Delivery*>(raw);return self.actual->open_read(self.actual->context,name,out);}
  static int close(void* raw,std::uintptr_t handle){auto& self=*static_cast<Delivery*>(raw);return self.actual->close_read(self.actual->context,handle);}
  static int open_write(void* raw,const char* name,std::uintptr_t* out){
   auto& self=*static_cast<Delivery*>(raw);if(!name||!out)return 1;
   *out=0;auto* file=std::fopen((std::string(self.directory)+"/"+name).c_str(),"wb");
   if(!file)return 1;
   *out=reinterpret_cast<std::uintptr_t>(file);return 0;
  }
 } delivery{actual,directory};
 const DebugFileServices24 files{&delivery,Delivery::open,Delivery::close};
 const auto streams=debug_stdio_services_v136(Delivery::open_write);
 return dh2_character_debug_load_stream_v136(debug,&files,&streams);
}
}
