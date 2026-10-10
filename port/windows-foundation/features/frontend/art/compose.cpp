#include "runtime_source.hpp"
#include "../flow/menu_flow.hpp"
#include <algorithm>
#include <cctype>
#include <functional>

namespace dh::foundation::frontend::art {
std::string normalize_source_path(std::string_view path) {
    std::string result;
    for (std::size_t begin=0;begin<path.size();) {
        auto end=path.find_first_of("/.",begin);if(end==std::string_view::npos)end=path.size();
        auto part=path.substr(begin,end-begin);
        bool numeric=!part.empty();for(char c:part)numeric=numeric&&c>='0'&&c<='9';
        if(!part.empty()&&!numeric){if(!result.empty())result+='.';result.append(part);}
        begin=end+1;
    }
    return result;
}
std::string source_english_symbol(std::string_view symbol) {
    TextField field;field.path=std::string(symbol);return source_english_label(field);
}
namespace {
source::Matrix multiply(const source::Matrix& a,const source::Matrix& b){
    return {a[0]*b[0]+a[2]*b[1],a[1]*b[0]+a[3]*b[1],a[0]*b[2]+a[2]*b[3],a[1]*b[2]+a[3]*b[3],a[0]*b[4]+a[2]*b[5]+a[4],a[1]*b[4]+a[3]*b[5]+a[5]};
}
std::array<float,2> point(const source::Matrix& m,float x,float y){return {m[0]*x+m[2]*y+m[4],m[1]*x+m[3]*y+m[5]};}
bool equal_case(std::string_view a,std::string_view b){if(a.size()!=b.size())return false;for(std::size_t i=0;i<a.size();++i)if(std::tolower(static_cast<unsigned char>(a[i]))!=std::tolower(static_cast<unsigned char>(b[i])))return false;return true;}
}
bool compose(Screen screen,const flow::PresentationState& state,ScreenArt& output,std::string& error){
    const auto& movie=source::movie();ScreenArt next;
    const char* root_name=screen==Screen::main_menu?"menu_MainMenu":screen==Screen::enter_name?"menu_EnterName":screen==Screen::select_class?"menu_SelectClass":"menu_StartGame";
    auto hidden=[&](const std::string& path){const auto normalized=normalize_source_path(path);for(const auto& candidate:state.hidden_paths){const auto prefix=normalize_source_path(candidate);if(normalized==prefix||(normalized.size()>prefix.size()&&normalized.compare(0,prefix.size(),prefix)==0&&normalized[prefix.size()]=='.'))return true;}return false;};
    std::map<std::string,std::vector<HudGeometryVertex>> hits;
    std::map<std::string,std::array<float,4>> regions;
    std::function<bool(unsigned,const source::Matrix&,const std::string&,const std::string&,std::array<float,4>,std::array<float,4>,unsigned)> walk;
    walk=[&](unsigned character,const source::Matrix& matrix,const std::string& path,const std::string& button,std::array<float,4> mult,std::array<float,4> add,unsigned depth){
        if(depth>40){error="Original frontend nesting bound";return false;}
        if(hidden(path))return true;
        auto shape=movie.shapes.find(character);
        if(shape!=movie.shapes.end()){
            const bool input_only=path.find("hitzone")!=std::string::npos||path.find("btn_blocker")!=std::string::npos||path.find("btnPrevent")!=std::string::npos||path.find("class_select")!=std::string::npos;
            for(const auto& part:shape->second.parts){
                std::vector<HudGeometryVertex> vertices;vertices.reserve(part.vertices.size());
                for(const auto& v:part.vertices){const auto p=point(matrix,v.x,v.y);vertices.push_back({p[0],p[1],v.u,v.v});}
                if(path.find("class_select")!=std::string::npos&&!vertices.empty()){
                    const std::string key="menu_SelectClass.class_select";
                    const auto& v=vertices.front();auto inserted=regions.emplace(key,std::array<float,4>{v.x,v.x,v.y,v.y});
                    auto& b=inserted.first->second;for(const auto& p:vertices){b[0]=std::min(b[0],p.x);b[1]=std::max(b[1],p.x);b[2]=std::min(b[2],p.y);b[3]=std::max(b[3],p.y);}
                }
                if(!button.empty())hits[button].insert(hits[button].end(),vertices.begin(),vertices.end());
                std::array<float,4> color{};for(unsigned i=0;i<4;++i)color[i]=std::clamp(part.color[i]*mult[i]+add[i],0.f,1.f);
                if(!input_only&&color[3]>0){next.batches.push_back({path,character,std::move(vertices)});next.bitmap_ids.push_back(part.bitmap);next.batch_colors.push_back(color);}
            }
            return true;
        }
        auto field=movie.fields.find(character);
        if(field!=movie.fields.end()){
            auto text=field->second;text.path=path;text.matrix=matrix;
            const auto& b=text.local_bounds;const auto a=point(matrix,b[0],b[2]),c=point(matrix,b[1],b[3]);
            text.bounds={std::min(a[0],c[0]),std::max(a[0],c[0]),std::min(a[1],c[1]),std::max(a[1],c[1])};
            for(unsigned i=0;i<4;++i)text.rgba[i]=static_cast<std::uint8_t>(std::clamp(text.rgba[i]/255.f*mult[i]+add[i],0.f,1.f)*255.f);
            bool bound=false;for(const auto& value:state.text)if(normalize_source_path(value.path)==normalize_source_path(path)){text.initial_text=value.localization_symbol?source_english_symbol(value.value):value.value;bound=true;break;}
            if(!bound)text.initial_text=source_english_label(text);
            if(text.rgba[3]>0)next.text_fields.push_back(std::move(text));return true;
        }
        auto clip=movie.clips.find(character);if(clip==movie.clips.end()||clip->second.frames.empty())return true;
        unsigned frame=0;
        auto settle_idle=[&](){
            unsigned end=static_cast<unsigned>(clip->second.frames.size());
            for(const auto& label:clip->second.labels)if(label.second>frame)end=std::min(end,label.second);
            unsigned stop_at=end;for(auto stop:clip->second.stop_frames)if(stop>=frame&&stop<end)stop_at=std::min(stop_at,stop);
            if(stop_at<end)frame=stop_at;
        };
        bool idle=false;for(const auto& label:clip->second.labels)if(equal_case(label.first,"idle")&&label.second==0){settle_idle();idle=true;break;}
        if(!idle&&!clip->second.stop_frames.empty())frame=*std::min_element(clip->second.stop_frames.begin(),clip->second.stop_frames.end());
        const auto final_separator=path.find_last_of('/');
        const bool named_clip=final_separator==std::string::npos||!std::all_of(path.begin()+static_cast<std::ptrdiff_t>(final_separator+1),path.end(),[](char c){return c>='0'&&c<='9';});
        for(const auto& choice:state.timeline_labels)if(named_clip&&normalize_source_path(choice.first)==normalize_source_path(path)){
            bool found=false;for(const auto& label:clip->second.labels)if(equal_case(label.first,choice.second)){frame=label.second;found=true;break;}
            if(!found){error="Unknown original timeline label "+choice.second+" at "+path;return false;}
            if(equal_case(choice.second,"idle"))settle_idle();
            // Settled endpoint of original show tween. Source frame0 and endpoint
            // share authored control placement; endpoint has visible source alpha.
            if(equal_case(choice.second,"show"))frame=static_cast<unsigned>(clip->second.frames.size()-1);
            if(equal_case(choice.second,"released")){
                unsigned end=static_cast<unsigned>(clip->second.frames.size());
                for(const auto& label:clip->second.labels)if(label.second>frame)end=std::min(end,label.second);
                frame=end-1;
            }
            break;
        }
        for(std::size_t index=0;index<clip->second.frames[frame].size();++index){
            const auto& p=clip->second.frames[frame][index];
            const auto name=p.name.empty()?std::to_string(index):p.name;
            if(name=="flush_text")continue;
            auto childpath=path+'/'+name;
            std::string childbutton=button;if(name.rfind("btn",0)==0)childbutton=childpath;
            std::array<float,4> childmult{},childadd{};for(unsigned i=0;i<4;++i){childmult[i]=mult[i]*p.multiply[i];childadd[i]=add[i]+mult[i]*p.add[i];}
            if(!walk(p.character,multiply(matrix,p.matrix),childpath,childbutton,childmult,childadd,depth+1))return false;
        }
        return true;
    };
    for(const auto& root:movie.roots){
        if(root.name!=root_name&&!(root.name=="menu_bg"&&screen==Screen::enter_name))continue;
        const source::Matrix stage{480.f/1024.f,0,0,320.f/768.f,0,0};
        if(!walk(root.character,multiply(stage,root.matrix),root.name,"",root.multiply,root.add,0))return false;
    }
    for(auto& region:hits)next.hit_regions.push_back({region.first,std::move(region.second)});
    for(const auto& region:regions)next.render_regions.push_back({region.first,region.second});
    output=std::move(next);error.clear();return true;
}
bool project(const ScreenArt& art,int width,int height,HudGeometry& output,std::string& error){
    if(width<=0||height<=0){error="Invalid original frontend viewport";return false;}
    HudGeometry next;next.width=static_cast<float>(width);next.height=static_cast<float>(height);next.batches=art.batches;
    // Original retained movie logical rectangle covers the complete viewport.
    const float sx=width/480.f,sy=height/320.f;
    for(auto& batch:next.batches)for(auto& vertex:batch.triangles){vertex.x*=sx;vertex.y*=sy;}
    output=std::move(next);error.clear();return true;
}
bool source_point(float x,float y,int width,int height,std::array<float,2>& out) noexcept{
    if(width<=0||height<=0)return false;out={x*480.f/width,y*320.f/height};return true;
}
bool class_scene_bounds(const ScreenArt& art,std::array<float,4>& out) noexcept{
    for(const auto& region:art.render_regions)if(region.path=="menu_SelectClass.class_select"){out=region.bounds;return true;}return false;
}
}
