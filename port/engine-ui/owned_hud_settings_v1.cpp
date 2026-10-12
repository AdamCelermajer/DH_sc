#include "owned_hud_settings_v1.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::ui {namespace {
std::uint32_t readword(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
std::int32_t signedword(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,4);return out;}
bool fail(std::string& e,const char* s){e=s;return false;}
bool device_valid(const SettingsDeviceFactsV1& d){return d.sharp_devices<=255&&d.htc_devices<=255&&d.no_igp<=255&&d.in_multiplayer_mode<=255&&d.korean_build<=255&&d.japanese_build<=255;}
struct Reader {
 const std::vector<std::uint8_t>& data;std::size_t at{};
 bool word(std::uint32_t& v){if(data.size()-at<4)return false;v=readword(data.data()+at);at+=4;return true;}
 bool key(std::string& value,bool& fits){std::uint32_t n;if(!word(n))return false;auto read=std::min<std::size_t>(n,127);if(read>data.size()-at)return false;value.assign(reinterpret_cast<const char*>(data.data()+at),read);at+=read;value.resize(std::strlen(value.c_str()));fits=n<128;return true;}
};
}
OwnedHudSettingsV1::OwnedHudSettingsV1(GameOptionTableV1::Borrow table):table_(std::move(table)){if(!table_)throw std::invalid_argument("Owned settings need actual GameOption backing");}
bool OwnedHudSettingsV1::has_option(const char* key)const{return key&&options_.find(key)!=options_.end();}
std::int32_t OwnedHudSettingsV1::option(const char* key)const{if(!key)return -1;auto i=options_.find(key);return i==options_.end()?-1:i->second.current;}
std::int32_t OwnedHudSettingsV1::saved_option(const char* key)const{return has_option(key)?option(key):0;}
bool OwnedHudSettingsV1::set_option(const char* key,std::int32_t value){if(!key)return false;auto i=options_.find(key);if(i==options_.end())return false;i->second.current=value;return true;}
const GameOptionRow32V1* OwnedHudSettingsV1::descriptor(const char* key)const{if(!key)return nullptr;auto i=options_.find(key);return i==options_.end()?nullptr:&table_.rows()[i->second.descriptor];}
std::int32_t OwnedHudSettingsV1::language()const{return !has_option("Language")?language_hint_:option("Language")==-1?-1:option("Language");}
bool OwnedHudSettingsV1::set_language(std::int32_t language,const SettingsLanguageServicesV1& services,std::string& error){
 error.clear();set_option("Language",language);
 if(!services.refresh_scene||!services.text)return fail(error,"Required settings language scene/TextManager backend missing");
 if(!services.refresh_scene(services.context,*this,language,error))return false;
 return services.text->switch_pack(language,true,error);
}
bool OwnedHudSettingsV1::load(bool language_only,const SettingsFileServicesV1& files,const SettingsLanguageServicesV1& language_services,const SettingsDeviceFactsV1& device,SettingsLoadReceiptV1& receipt,std::string& error){
 error.clear();if(!files.open_read||!files.close_read||!device_valid(device))return fail(error,"Malformed settings file/device services");
 // Source deletes the previous buffer, clears option nodes, then sets tutorial
 // bytes. Existing load/new flags are deliberately not reset by _initSettings.
 file_.clear();options_.clear();if(!language_only)for(std::size_t i=0;i<table_.rows().size();++i)options_[table_.names()[i].c_str()]={i,table_.rows()[i].default_value};tutorials_.fill(1);
 bool found=false;std::uintptr_t lease=0;std::vector<std::uint8_t> buffer;
 if(!files.open_read(files.context,"dh2_settings.savegame",found,buffer,lease,error)){
  if(lease){std::string close_error;files.close_read(files.context,lease,close_error);}return false;
 }
 if((found&&!lease)||(!found&&(lease||!buffer.empty()))||buffer.size()>16u*1024u*1024u){if(lease){std::string ignored;files.close_read(files.context,lease,ignored);}return fail(error,"Malformed settings file response");}
 if(lease&&!files.close_read(files.context,lease,error))return false;
 file_=std::move(buffer);SettingsLoadReceiptV1 next{found,loaded_,new_settings_,file_.size(),0,0};
 if(!set_language(language_hint_,language_services,error))return false;
 if(language_only){
  bool got_language=false,got_orientation=false;Reader reader{file_};
  if(found){std::uint32_t count;if(!reader.word(count)||count>65536)return fail(error,"Malformed settings option count");
   for(std::uint32_t i=0;i<count&&! (got_language&&got_orientation);++i){std::string key;bool fits;std::uint32_t value;
    if(!reader.key(key,fits))return fail(error,"Truncated settings key");if(!fits)break;
    if(!reader.word(value))return fail(error,"Truncated settings value");
    if(key=="Language"){language_hint_=signedword(value);got_language=true;}else if(key=="AutoOrientation"){orientation_=value!=0;got_orientation=true;}
   }
  }
  next.consumed=reader.at;
  // Source always applies platform language after scanning saved language.
  if(device.korean_build)language_hint_=5;
  else if(device.japanese_build)language_hint_=4;
  else{std::uint32_t value;if(!language_services.platform_language)return fail(error,"Required platform language backend missing");if(!language_services.platform_language(language_services.context,value,error))return false;static constexpr std::int32_t mapped[]={0,2,1,7,3,4,5,6};language_hint_=value<8?mapped[value]:0;}
  if(language_hint_==-1)new_settings_=true;
 }else if(found){
  SettingsParserSpan24V1 parser{file_.data(),static_cast<std::uint32_t>(file_.size()),0,0,0};
  SettingsLookup16V1 lookup{this,[](void* opaque,const char* name)->std::int32_t*{auto& self=*static_cast<OwnedHudSettingsV1*>(opaque);auto item=self.options_.find(name);return item==self.options_.end()?nullptr:&item->second.current;}};
  if(dh2_settings_v1_read_options(&parser,&lookup))return fail(error,"Truncated/malformed settings option stream");
  next.recognized_records=parser.recognized;
  if(file_.size()-parser.cursor<14)return fail(error,"Truncated settings tutorial bytes");std::memcpy(tutorials_.data(),file_.data()+parser.cursor,14);next.consumed=parser.cursor+14;
  auto selected=language();if(selected==-1){if(!set_language(0,language_services,error))return false;new_settings_=true;}
  else if(!set_language(selected,language_services,error))return false;
  loaded_=true;
 }
 next.source_loaded=loaded_;next.source_new_settings=new_settings_;receipt=next;return true;
}
int settings_startup_v1_service(void* opaque,HudStartupState48* state,const HudStartupRequest40* request,HudStartupResponse16* response){
 auto* binding=static_cast<SettingsStartupBindingV1*>(opaque);
 if(!binding||!binding->owner||!binding->files||!binding->language||!binding->device||!state||!request||!response||request->reserved||!device_valid(*binding->device)||!binding->application_identity||!binding->savegame_identity)return 1;
 auto& owner=*binding->owner;auto& e=binding->error;e.clear();
 const auto op=request->operation;
 if((op==HudStartupOperation::load_settings||op==HudStartupOperation::get_language||op==HudStartupOperation::set_language)&&request->subject!=binding->savegame_identity)return 1;
 if(op==HudStartupOperation::get_saved_option&&request->subject!=binding->application_identity)return 1;
 switch(request->operation){
 case HudStartupOperation::load_settings:return request->argument==0&&owner.load(false,*binding->files,*binding->language,*binding->device,binding->last_load,e)?0:1;
 case HudStartupOperation::update_saved_values:(void)owner.saved_option("AutoOrientation");return 0; // genuine ResetOrientation bx lr.
 case HudStartupOperation::get_saved_option:if(!request->name)return 1;response->value=owner.saved_option(request->name);return 0;
 case HudStartupOperation::get_language:response->value=owner.language();return 0;
 case HudStartupOperation::set_language:return owner.set_language(request->argument,*binding->language,e)?0:1;
 case HudStartupOperation::is_high_performance:{if(state->sharp_devices!=binding->device->sharp_devices||state->htc_devices!=binding->device->htc_devices||state->no_igp!=binding->device->no_igp)return 1;HudDevicePipeline16 facts{{binding->device->sharp_devices,binding->device->htc_devices,binding->device->in_multiplayer_mode},binding->device->capabilities};response->value=dh2_hud_device_pipeline(&facts);return response->value<0?1:0;}
 case HudStartupOperation::set_initial_volume:case HudStartupOperation::set_result_bool:if(!binding->downstream.invoke){e="Required settings audio/AS result backend missing";return 1;}return binding->downstream.invoke(binding->downstream.context,state,request,response);
 default:return 1;
 }
}
}
extern "C" int dh2_settings_v1_read_options(dh2::ui::SettingsParserSpan24V1* span,const dh2::ui::SettingsLookup16V1* lookup) noexcept {
 if(!span||!lookup||reinterpret_cast<std::uintptr_t>(span)%alignof(dh2::ui::SettingsParserSpan24V1)||reinterpret_cast<std::uintptr_t>(lookup)%alignof(dh2::ui::SettingsLookup16V1)||!lookup->lookup||span->reserved||(!span->data&&span->size)||span->size>16u*1024u*1024u||span->cursor>span->size)return -1;
 auto read=[&](std::uint32_t& out){if(span->size-span->cursor<4)return false;const auto* p=span->data+span->cursor;out=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);span->cursor+=4;return true;};
 std::uint32_t count;if(!read(count))return -2;if(count>65536)return -1;
 for(std::uint32_t i=0;i<count;++i){
  std::uint32_t length;if(!read(length))return -2;const auto n=std::min<std::uint32_t>(length,127);if(n>span->size-span->cursor)return -2;
  char name[128]{};if(n)std::memcpy(name,span->data+span->cursor,n);span->cursor+=n;
  if(length>=128)return 0;
  std::uint32_t value;if(!read(value))return -2;auto* target=lookup->lookup(lookup->context,name);
  if(target){std::memcpy(target,&value,4);++span->recognized;}
 }
 return 0;
}
