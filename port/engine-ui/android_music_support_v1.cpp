#include "android_music_support_v1.hpp"
#include <cstring>
namespace dh2::ui {
int android_music_support_v1(const char* manufacturer,const char* model,std::int32_t& result) noexcept{
 if(!manufacturer||!model)return -1;
 const auto same=[](const char* actual,const char* name){return std::strcmp(actual,name)==0;};
 // Original DEX const/4 v3,-1; every device branch returns that unchanged v3.
 // Preserve the authored classification reads, rather than guessing support.
 if(same(manufacturer,"samsung")||same(manufacturer,"Samsung")){
  if(same(model,"SC-02B")||same(model,"SHW-M110S")||same(model,"GT-I9000")||same(model,"SGH-T959")||same(model,"SHW-M130L")){result=-1;return 0;}
 }
 if(same(manufacturer,"Sony Ericsson")&&same(model,"X10i")){result=-1;return 0;}
 if(same(manufacturer,"HTC")&&same(model,"HTC Desire")){result=-1;return 0;}
 result=-1;return 0;
}
}
