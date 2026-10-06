#include "authored_fx_nonrender_geometry_v32.hpp"
#include <cstring>
namespace dh2::fx {
bool authored_fx_nonrender_geometry_v32(const resources::BresView& image,unsigned index,
 AuthoredFxNonrenderGeometryV32& out,bool& nonrender,std::string& error){
 nonrender=false;
 const auto* record=dh2_bres_library_item(&image,resources::Library::geometry,index);
 if(!record){error="Required source geometry declaration";return false;}
 std::uint32_t kind,payload;std::memcpy(&kind,record+8,4);std::memcpy(&payload,record+12,4);
 if(!kind)return true;
 if(kind!=1){error="Required original authored geometry family "+std::to_string(kind);return false;}
 if(!payload||payload>image.size||20>image.size-payload){error="Required original kind1 declaration payload";return false;}
 AuthoredFxNonrenderGeometryV32 candidate{index,payload,{}};
 std::memcpy(candidate.fields,image.bytes+payload,20);
 // Actual five-cache source domain is the raw five-word tuple (0,15,3,
 // 0,0). Retain it without inventing render geometry or field semantics.
 if(candidate.fields[0]!=0||candidate.fields[1]!=15||candidate.fields[2]!=3||candidate.fields[3]||candidate.fields[4]){
  error="Required additional original kind1 declaration layout";return false;
 }
 out=candidate;nonrender=true;return true;
}
}

