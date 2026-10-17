#include "../camera_animset_bank_v1.hpp"
#include "../../game-data/data.hpp"

#include <array>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using Bytes=std::vector<std::uint8_t>;
Bytes read_file(const std::string& path){
    std::ifstream file(path,std::ios::binary);
    if(!file)throw std::runtime_error("cannot open "+path);
    return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
void require(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
struct Assets {std::string root;};
bool read_asset(void* context,const std::string& path,Bytes& output,std::string& error){
    const auto& assets=*static_cast<Assets*>(context);
    try{output=read_file(assets.root+"/original-cache/"+path);return true;}
    catch(const std::exception& original_error){
        // Some extracted source animation paths are staged in the Android
        // animations/ overlay instead of the original-cache tree.
        const auto slash=path.find_last_of("/\\");
        const auto leaf=slash==std::string::npos?path:path.substr(slash+1);
        try{output=read_file(assets.root+"/animations/"+leaf);return true;}
        catch(const std::exception&){error=original_error.what();return false;}
    }
}
}

int main(int argc,char** argv){
    try{
        if(argc!=2)throw std::runtime_error("usage: camera_animset_bank_v1 <android-assets-root>");
        const std::string root=argv[1]+std::string("/data/");
        const auto records=read_file(root+"animations_pyarray.bin");
        const auto names=read_file(root+"animations_pyarraynames.bin");
        const auto fields=read_file(root+"animations_pystructnames.bin");
        const auto clip_names=read_file(root+"animations_dictionary_pyarraynames.bin");
        const auto clip_values=read_file(root+"animations_dictionary_pyarray.bin");
        const auto view=[](const Bytes& bytes){return dh2::data::Bytes{bytes.data(),bytes.size()};};
        dh2::data::Dictionary clips;dh2::data::AnimationTables tables;std::string error;
        require(dh2::data::load_dictionary(view(clip_names),view(clip_values),clips,error),error);
        require(dh2::data::load_animation_tables(view(records),view(names),view(fields),clips,tables,error),error);

        Assets assets{argv[1]};
        for(const auto& set_name:{std::string("Default"),std::string("SwampCam")}){
            dh2::camera_animset_v1::Selection selected;
            require(dh2::camera_animset_v1::select(tables,clips,set_name,selected,error),error);
            dh2::camera_animset_bank_v1::Owner bank;
            const bool loaded=bank.load(selected,read_asset,&assets,error);
            require(loaded,set_name+": "+error);
            require(bank.set_name()==set_name&&bank.size()==selected.resources.size(),
                    set_name+" did not retain every selected resource");
            for(const auto& resource:selected.resources){
                dh2::camera_animset_v1::PlayRequest request;
                request.present=true;request.resource=resource;
                const auto* clip=bank.resolve(request);
                require(clip&&clip->resource.clip_id==resource.clip_id&&
                            clip->resource.registration_order==resource.registration_order&&
                            clip->resource.path==resource.path&&clip->bdae.size()>=4&&
                            clip->bdae[0]=='B'&&clip->bdae[1]=='R'&&
                            clip->bdae[2]=='E'&&clip->bdae[3]=='S',
                        set_name+" resource failed global-ID/path BDAE resolution");
            }
            dh2::camera_animset_v1::PlayRequest wrong;
            wrong.present=true;wrong.resource=selected.resources.front();
            wrong.resource.clip_id+=100000;
            require(!bank.resolve(wrong),"unregistered global AnimDict ID resolved in bank");

            wrong.resource=selected.resources.front();
            wrong.resource.path+=".wrong";
            require(!bank.resolve(wrong),"same global ID with a different BDAE path resolved");
        }

        dh2::camera_animset_v1::Selection invalid{};
        invalid.name="invalid";
        invalid.resources.push_back({dh2::camera_animset_v1::Resource::Slot::idle,0,55,
                                     "missing/camera_clip.bdae"});
        dh2::camera_animset_bank_v1::Owner bank;
        require(!bank.load(invalid,read_asset,&assets,error),
                "missing selected BDAE unexpectedly loaded");
        require(bank.size()==0&&bank.set_name().empty(),
                "failed load left a partial selected clip bank");
        return 0;
    }catch(const std::exception& e){
        fprintf(stderr,"FAIL: %s\n",e.what());return 1;
    }
}
