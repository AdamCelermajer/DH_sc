#include "embedded_scene_clips.hpp"
#include "../engine-resources/resources.hpp"
#include <cstring>
#include <set>
#include <stdexcept>
namespace dh::foundation {
bool decode_embedded_scene_clips(const std::uint8_t* bytes,std::size_t size,
    std::vector<EmbeddedSceneClip>& output,std::string& error){
    error.clear();try{
        dh2::resources::BresView image{};
        if(dh2_bres_open(&image,bytes,size)!=dh2::resources::BresError::ok)throw std::runtime_error("Embedded clip BRES rejected");
        const auto count=dh2_bres_library_count(&image,dh2::resources::Library::animation_clip);
        if(count>4096)throw std::runtime_error("Embedded clip library exceeds limit");
        std::set<std::string> names;std::vector<EmbeddedSceneClip> clips;clips.reserve(count);
        for(std::uint32_t i=0;i<count;++i){
            const auto* record=dh2_bres_library_item(&image,dh2::resources::Library::animation_clip,static_cast<std::int32_t>(i));
            if(!record||std::size_t(record-bytes)>size||size-std::size_t(record-bytes)<12)throw std::runtime_error("Embedded clip record outside image");
            auto word=[](const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);};
            const auto offset=word(record);if(offset>=size)throw std::runtime_error("Embedded clip name outside image");
            const auto* end=static_cast<const std::uint8_t*>(std::memchr(bytes+offset,0,size-offset));
            if(!end||end==bytes+offset||std::size_t(end-(bytes+offset))>4096)throw std::runtime_error("Embedded clip name invalid");
            EmbeddedSceneClip clip;clip.name.assign(reinterpret_cast<const char*>(bytes+offset),end-(bytes+offset));
            const auto start=word(record+4),finish=word(record+8);
            std::memcpy(&clip.start_ms,&start,4);std::memcpy(&clip.end_ms,&finish,4);
            if(clip.end_ms<clip.start_ms||!names.insert(clip.name).second)throw std::runtime_error("Embedded clip range/name invalid");
            clips.push_back(std::move(clip));
        }
        output=std::move(clips);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
