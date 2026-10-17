#pragma once

#include <cmath>
#include <cstdlib>
#include <string>
#include <string_view>
#include <utility>

namespace dh2::native::level_camera_config_v1 {

// Level::_LoadCamera passes this CameraBase::SetData FOV on both SWAMP and
// Crypt routes (IDA: 0x3f1008 -> 0x40e9a8; ARM immediate 0x3edbf877).
inline constexpr float kVerticalFovRadians=0.42963001132011414f;

struct ClipPlanes {
    float near_clip=0.0f;
    float far_clip=0.0f;
};

inline bool xml_space(char c) noexcept {
    return c==' '||c=='\t'||c=='\r'||c=='\n';
}

inline bool xml_name_boundary(char c) noexcept {
    return xml_space(c)||c=='<'||c=='/';
}

inline bool attribute_text(std::string_view xml,std::string_view name,std::string* out) {
    if(!out||name.empty()||xml.size()>1024*1024)return false;
    std::size_t at=0;
    while((at=xml.find(name,at))!=std::string_view::npos){
        const bool left=at==0||xml_name_boundary(xml[at-1]);
        std::size_t cursor=at+name.size();
        const bool right=cursor==xml.size()||xml_space(xml[cursor])||xml[cursor]=='=';
        if(left&&right){
            while(cursor<xml.size()&&xml_space(xml[cursor]))++cursor;
            if(cursor==xml.size()||xml[cursor++]!='='){at+=name.size();continue;}
            while(cursor<xml.size()&&xml_space(xml[cursor]))++cursor;
            if(cursor==xml.size()||(xml[cursor]!='\''&&xml[cursor]!='"'))return false;
            const char quote=xml[cursor++];
            const auto end=xml.find(quote,cursor);
            if(end==std::string_view::npos||end-cursor>4096)return false;
            *out=std::string(xml.substr(cursor,end-cursor));
            return true;
        }
        at+=name.size();
    }
    return false;
}

struct CameraRoute {
    std::string camera_file="CameraTests.bdae";
    std::string camera_name="PlayerCamera_Default";
    std::string animset="Default";
};

// LevelConfig::DeclareProperties initializes combat_music_enabled to true;
// generated Crypt's identity LevelConfig omits the property, so that source
// default remains active unless the authored row overrides it with 0 or 1.
inline bool parse_combat_music_enabled(std::string_view xml,bool* out) noexcept {
    if(!out||xml.size()>1024*1024)return false;
    try {
        std::string value;
        bool enabled=true;
        if(attribute_text(xml,"combat_music_enabled",&value)){
            // The source Property<bool> accepts the generated-layout spelling
            // as well as the numeric spelling present in authored MLX rows.
            // Generated Crypt writes this property from kConfigDefaults as
            // `true`; rejecting it previously made the entire camera config
            // unavailable even though its clip planes and route were valid.
            if(value=="1"||value=="true")enabled=true;
            else if(value=="0"||value=="false")enabled=false;
            else return false;
        }
        *out=enabled;
        return true;
    }catch(...){return false;}
}

// LevelConfig::DeclareProperties supplies Default for camera_animset;
// LevelConfig::InitPost supplies CameraTests.bdae for an empty camera_file.
inline bool parse_camera_route(std::string_view xml,CameraRoute* out) noexcept {
    if(!out||xml.size()>1024*1024)return false;
    try {
        CameraRoute value{};
        std::string field;
        if(attribute_text(xml,"camera_file",&field)&&!field.empty())value.camera_file=field;
        field.clear();
        if(attribute_text(xml,"camera_name",&field)&&!field.empty())value.camera_name=field;
        field.clear();
        if(attribute_text(xml,"camera_animset",&field)&&!field.empty())value.animset=field;
        if(value.camera_file.size()>4096||value.camera_name.size()>4096||
           value.animset.empty()||value.animset.size()>4096)return false;
        *out=std::move(value);
        return true;
    }catch(...){return false;}
}

inline bool attribute_number(std::string_view xml,std::string_view name,float* out) {
    if(!out||name.empty()||xml.size()>1024*1024)return false;
    std::size_t at=0;
    while((at=xml.find(name,at))!=std::string_view::npos){
        const bool left=at==0||xml_name_boundary(xml[at-1]);
        std::size_t cursor=at+name.size();
        const bool right=cursor==xml.size()||xml_space(xml[cursor])||xml[cursor]=='=';
        if(left&&right){
            while(cursor<xml.size()&&xml_space(xml[cursor]))++cursor;
            if(cursor==xml.size()||xml[cursor++]!='='){at+=name.size();continue;}
            while(cursor<xml.size()&&xml_space(xml[cursor]))++cursor;
            if(cursor==xml.size()||(xml[cursor]!='\''&&xml[cursor]!='"'))return false;
            const char quote=xml[cursor++];
            const auto end=xml.find(quote,cursor);
            if(end==std::string_view::npos||end==cursor||end-cursor>64)return false;
            const std::string text(xml.substr(cursor,end-cursor));
            char* parsed_end=nullptr;
            const float value=std::strtof(text.c_str(),&parsed_end);
            if(parsed_end!=text.c_str()+text.size()||!std::isfinite(value))return false;
            *out=value;
            return true;
        }
        at+=name.size();
    }
    return false;
}

inline bool parse_clip_planes(std::string_view level_config_xml,
                              ClipPlanes* out) noexcept {
    if(!out)return false;
    try {
        ClipPlanes value{};
        if(!attribute_number(level_config_xml,"camera_znear",&value.near_clip)||
           !attribute_number(level_config_xml,"camera_zfar",&value.far_clip)||
           !(value.near_clip>0.0f&&value.far_clip>value.near_clip))return false;
        *out=value;
        return true;
    }catch(...){return false;}
}

} // namespace dh2::native::level_camera_config_v1
