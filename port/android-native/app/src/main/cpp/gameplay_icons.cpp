#include "gameplay_icons.hpp"
#include "player_gameplay_binding.hpp"
#include "original_cache_assets_v1.hpp"
#include "textures.hpp"
#include <algorithm>
#include <map>
namespace dh2::android_ui {
namespace {
struct Crop {const char* name;const char* texture;int x,y,w,h;};
const Crop crops[]{
#include "gameplay_icon_catalog.inc"
};
struct Image {int w{},h{};std::vector<std::uint8_t> rgba;};
struct Cache {AAssetManager* manager{};std::map<std::string,Image> images;};
Cache cache;
std::string lower(std::string s){for(auto& c:s)if(c>='A'&&c<='Z')c=char(c+32);return s;}
}
bool equipment_slot_icon_name(const model_renderer::PlayerGameplayBinding& player,int slot,std::string& name,std::string& error){
 name.clear();error.clear();
 auto* design=player.design.design();
 if(slot<0||!design||!design->lookup){error="Actual EquipmentSlots constants required";return false;}
 constexpr const char* labels[]{"Torso","RightHand","LeftHand","Feet","HandArmor","RightHandRingFinger","LeftHandRingFinger","Waist","Head"};
 for(const char* label:labels){std::int32_t value;
  if(design->lookup(design->context,0,"EquipmentSlots",label,&value)){error=std::string("Original EquipmentSlots constant unavailable: ")+label;return false;}
  if(value==slot){name=label;return true;}
 }
 error="Equipment slot has no original authored frame label";return false;
}
std::vector<int> menu_icon_pixels(AAssetManager* manager,const std::string& requested,std::string& error){
 error.clear();if(!manager||requested.empty()||requested.size()>256||requested.find('\0')!=std::string::npos){error="Invalid original icon request";return {};}
 const auto name=lower(requested);const Crop* crop=nullptr;
 for(const auto& c:crops)if(name==c.name){crop=&c;break;}
 std::string uri;
 if(crop)uri=crop->texture;
 else if(name.size()>4&&name.substr(name.size()-4)==".tga"){
  if(name.find("..")!=std::string::npos){error="Invalid original icon URI";return {};}
  auto slash=name.find_last_of("/\\");uri="data/3d/textures/"+name.substr(slash==std::string::npos?0:slash+1);
 }else {error="Original icon label unavailable: "+requested;return {};}
 if(cache.manager!=manager){cache.manager=manager;cache.images.clear();}
 auto found=cache.images.find(uri);
 if(found==cache.images.end()){
  OriginalCacheAssetsV1 assets(manager);bool present=false;std::vector<std::uint8_t> encoded;
  if(!assets.read(uri,present,encoded,error)||!present){if(error.empty())error="Original icon texture absent: "+uri;return {};}
  textures::View view{};auto status=dh2_texture_open(encoded.data(),encoded.size(),&view);
  if(status!=textures::Error::ok){error=dh2_texture_error(status);return {};}
  if(!view.width||!view.height||view.width>2048||view.height>2048){error="Original icon texture dimensions outside bound";return {};}
  Image image{int(view.width),int(view.height),std::vector<std::uint8_t>(std::size_t(view.width)*view.height*4)};
  status=dh2_texture_decode(&view,image.rgba.data(),image.rgba.size());
  if(status!=textures::Error::ok){error=dh2_texture_error(status);return {};}
  found=cache.images.emplace(uri,std::move(image)).first;
 }
 const auto& image=found->second;int x=crop?crop->x:0,y=crop?crop->y:0,w=crop?crop->w:image.w,h=crop?crop->h:image.h;
 if(x<0||y<0||w<=0||h<=0||x+w>image.w||y+h>image.h){error="Authored icon crop outside original texture";return {};}
 std::vector<int> out(std::size_t(w)*h+2);out[0]=w;out[1]=h;
 for(int row=0;row<h;++row)for(int column=0;column<w;++column){auto at=(std::size_t(y+row)*image.w+x+column)*4;
  auto word=(std::uint32_t(image.rgba[at+3])<<24)|(std::uint32_t(image.rgba[at])<<16)|(std::uint32_t(image.rgba[at+1])<<8)|image.rgba[at+2];
  out[2+std::size_t(row)*w+column]=static_cast<int>(word);
 }
 return out;
}
}
