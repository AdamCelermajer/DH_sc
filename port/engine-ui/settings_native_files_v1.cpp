#include "settings_native_files_v1.hpp"
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <stdexcept>
namespace dh2::ui {
SettingsNativeFilesV1::SettingsNativeFilesV1(std::string directory):directory_(std::move(directory)){if(directory_.empty()||directory_.find('\0')!=std::string::npos)throw std::invalid_argument("Settings private directory missing");}
bool SettingsNativeFilesV1::open(void* context,const char* name,bool& found,std::vector<std::uint8_t>& bytes,std::uintptr_t& lease,std::string& error){
 error.clear();if(!context||!name||std::strcmp(name,"dh2_settings.savegame")){error="Settings filename outside source contract";return false;}
 auto& self=*static_cast<SettingsNativeFilesV1*>(context);const auto path=self.directory_+"/"+name;
 errno=0;auto* file=std::fopen(path.c_str(),"rb");
 if(!file){if(errno==ENOENT){found=false;bytes.clear();lease=0;return true;}error="Settings fopen failed: "+std::string(std::strerror(errno));return false;}
 if(std::fseek(file,0,SEEK_END)){std::fclose(file);error="Settings file size query failed";return false;}
 const auto size=std::ftell(file);if(size<0||size>16*1024*1024||std::fseek(file,0,SEEK_SET)){std::fclose(file);error="Settings file outside source copy bounds";return false;}
 try{std::vector<std::uint8_t> buffer(static_cast<std::size_t>(size));if(size&&std::fread(buffer.data(),1,buffer.size(),file)!=buffer.size()){std::fclose(file);error="Settings fread failed";return false;}
  found=true;bytes=std::move(buffer);lease=reinterpret_cast<std::uintptr_t>(file);return true;
 }catch(...){std::fclose(file);throw;}
}
bool SettingsNativeFilesV1::close(void*,std::uintptr_t lease,std::string& error){error.clear();if(!lease){error="Settings file lease missing";return false;}if(std::fclose(reinterpret_cast<std::FILE*>(lease))){error="Settings fclose failed";return false;}return true;}
}
