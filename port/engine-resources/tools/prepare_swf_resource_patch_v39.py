"""Stage surgical SwfGpu admission patch; never writes shared live sources."""
from pathlib import Path
import difflib,hashlib,json
root=Path(__file__).resolve().parents[3]
out=root/'port/engine-resources/reports/swf-resource-v39';out.mkdir(parents=True,exist_ok=True)
native=root/'port/android-native/app/src/main/cpp'
header=(native/'swf_gpu.hpp').read_text();source=(native/'swf_gpu.cpp').read_text()
baseline={'swf_gpu.hpp':header,'swf_gpu.cpp':source}
header=header.replace('#include "swf_vertex_cache_v36.hpp"','#include "swf_vertex_cache_v36.hpp"\n#include "native_resource_budget_v38.hpp"\n#include "../../../../../engine-resources/retained_bytes_v39.hpp"\n#include <array>\n#include <stdexcept>')
header=header.replace('void initialize(AAssetManager*);','void initialize(AAssetManager*,resources::ResourceScopeV37);')
header=header.replace('std::vector<std::uint8_t> pixels;','resources::RetainedBytesV39 pixels;resources::ResourceTokenV37 gpu_budget_v39;std::uint64_t generation_v39{};')
header=header.replace('    bool reset_images(std::string&);','    std::shared_ptr<resources::ContextResourceBudgetV37> resource_budget_lease_v39()const noexcept{return budget_v39_;}\n    bool reset_images(std::string&);')
header=header.replace('    void upload(Texture&);','    std::shared_ptr<resources::ContextResourceBudgetV37> budget_v39_;\n    resources::ResourceScopeV37 scope_v39_=resources::ResourceScopeV37::other;\n    std::array<resources::ResourceTokenV37,5> target_budget_v39_{};\n    std::uint64_t target_generation_v39_{};\n    void release_charge_v39(resources::ResourceTokenV37& token){if(token){std::string error;if(!budget_v39_||!budget_v39_->release(token,error))throw std::runtime_error(error);}}\n    void release_texture_v39(Texture&,bool);\n    void release_targets_v39(bool);\n    void upload(Texture&);')
source=source.replace('void SwfGpu::initialize(AAssetManager* assets){','''void SwfGpu::initialize(AAssetManager* assets,resources::ResourceScopeV37 scope){
    if(!budget_v39_)budget_v39_=android_resources::budget_lease_v39();
    if(scope!=resources::ResourceScopeV37::swf_front&&scope!=resources::ResourceScopeV37::swf_gameplay)
        throw std::runtime_error("V39 explicit front/gameplay UI resource scope required");
    if(!textures_.empty()&&scope_v39_!=scope)throw std::runtime_error("V39 retained UI scope cannot change");
    scope_v39_=scope;
    release_targets_v39(true);
    for(auto& pair:textures_)release_texture_v39(pair.second,true);''')
start=source.index('void SwfGpu::upload(Texture& texture){');end=source.index('void SwfGpu::primitive(',start)
source=source[:start]+r'''void SwfGpu::release_texture_v39(Texture& texture,bool discard_context){
    const auto current=android_resources::budget_v38().snapshot();
    const bool same=current.context_ready&&current.context_generation==texture.generation_v39;
    if(texture.name&&discard_context&&same)throw std::runtime_error("V39 live texture context cannot be abandoned");
    if(texture.name&&!discard_context&&same)glDeleteTextures(1,&texture.name);
    texture.name=0;texture.generation_v39=0;
    android_resources::release_v38(texture.gpu_budget_v39);
}
void SwfGpu::release_targets_v39(bool discard_context){
    const auto current=android_resources::budget_v38().snapshot();
    const bool same=current.context_ready&&current.context_generation==target_generation_v39_;
    if((target_||query_target_||target_color_||query_color_||stencil_buffer_)&&discard_context&&same)
        throw std::runtime_error("V39 live target context cannot be abandoned");
    if(!discard_context&&same){
        if(target_)glDeleteFramebuffers(1,&target_);
        if(query_target_)glDeleteFramebuffers(1,&query_target_);
        if(target_color_)glDeleteTextures(1,&target_color_);
        if(query_color_)glDeleteTextures(1,&query_color_);
        if(stencil_buffer_)glDeleteRenderbuffers(1,&stencil_buffer_);
    }
    target_=query_target_=target_color_=query_color_=stencil_buffer_=0;
    target_width_=target_height_=0;target_generation_v39_=0;
    for(auto& token:target_budget_v39_)android_resources::release_v38(token);
}
void SwfGpu::upload(Texture& texture){
    barrier_v37("SWF bitmap resource entry");
    if(texture.width>max_texture_size_v37_||texture.height>max_texture_size_v37_)throw std::runtime_error("SWF bitmap exceeds GPU dimensions");
    std::uint64_t bytes;std::string error;
    if(!resources::swf_bitmap_bytes_v39(texture.width,texture.height,texture.channels,bytes,error))throw std::runtime_error(error);
    if(texture.name||texture.gpu_budget_v39)throw std::runtime_error("V39 bitmap upload requires unpublished GPU owner");
    auto& ledger=android_resources::budget_v38();resources::ResourceReservationV37 reservation;
    if(!ledger.reserve_create({resources::ResourceKindV37::texture,scope_v39_,bytes,0},reservation,error))throw std::runtime_error(error);
    const auto generation=ledger.snapshot().context_generation;
    const GLenum format=texture.channels==1?GL_ALPHA:texture.channels==3?GL_RGB:GL_RGBA;
    GLuint candidate=0;
    try{
        glGenTextures(1,&candidate);if(!candidate)throw std::runtime_error("SWF bitmap GL name unavailable");
        glBindTexture(GL_TEXTURE_2D,candidate);glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D,0,format,texture.width,texture.height,0,format,GL_UNSIGNED_BYTE,texture.pixels.data());
        barrier_v37("SWF bitmap upload");
        if(!reservation.commit(texture.gpu_budget_v39,error))throw std::runtime_error(error);
    }catch(...){
        const auto current=ledger.snapshot();
        if(candidate&&current.context_ready&&current.context_generation==generation)glDeleteTextures(1,&candidate);
        throw;
    }
    texture.name=candidate;texture.generation_v39=generation;texture.wrap=GL_CLAMP_TO_EDGE;
}
void SwfGpu::target(int width,int height){
    if(target_width_==width&&target_height_==height)return;
    barrier_v37("SWF framebuffer resource entry");
    if(width<=0||height<=0||width>max_texture_size_v37_||height>max_texture_size_v37_)throw std::runtime_error("Invalid SWF framebuffer size");
    std::uint64_t color_bytes;std::string error;
    if(!resources::rgba_texture_bytes_v37(width,height,false,color_bytes,error))throw std::runtime_error(error);
    auto& ledger=android_resources::budget_v38();
    using Kind=resources::ResourceKindV37;
    const std::array<Kind,5> kinds{Kind::texture,Kind::texture,Kind::framebuffer,Kind::framebuffer,Kind::renderbuffer};
    std::array<resources::ResourceReservationV37,5> admissions;
    std::array<resources::ResourceTokenV37,5> tokens{};
    // Full separate candidate is admitted while the old target remains valid.
    // Packed DEPTH24_STENCIL8 uses four bytes per pixel, same as each RGBA color.
    for(std::size_t i=0;i<admissions.size();++i)
        if(!ledger.reserve_create({kinds[i],scope_v39_,i==2||i==3?0:color_bytes,0},admissions[i],error))throw std::runtime_error(error);
    const auto generation=ledger.snapshot().context_generation;
    GLuint colors[2]{},framebuffers[2]{},stencil=0;
    try{
        glGenRenderbuffers(1,&stencil);if(!stencil)throw std::runtime_error("SWF stencil GL name unavailable");
        glBindRenderbuffer(GL_RENDERBUFFER,stencil);glRenderbufferStorage(GL_RENDERBUFFER,0x88f0,width,height);
        for(unsigned i=0;i<2;++i){
            glGenTextures(1,&colors[i]);if(!colors[i])throw std::runtime_error("SWF target color GL name unavailable");
            glBindTexture(GL_TEXTURE_2D,colors[i]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
            glGenFramebuffers(1,&framebuffers[i]);if(!framebuffers[i])throw std::runtime_error("SWF framebuffer GL name unavailable");
            glBindFramebuffer(GL_FRAMEBUFFER,framebuffers[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colors[i],0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,stencil);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,stencil);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)throw std::runtime_error("SWF shared-stencil framebuffer incomplete");
        }
        if(!stencil_bits_v37_){glGetIntegerv(GL_STENCIL_BITS,&stencil_bits_v37_);++capability_queries_v37_;}
        if(stencil_bits_v37_<8)throw std::runtime_error("SWF masks require an eight-bit stencil framebuffer");
        barrier_v37("SWF framebuffer allocation");
        for(std::size_t i=0;i<tokens.size();++i)if(!admissions[i].commit(tokens[i],error))throw std::runtime_error(error);
        release_targets_v39(false);
    }catch(...){
        const auto current=ledger.snapshot();
        if(current.context_ready&&current.context_generation==generation){
            for(auto name:framebuffers)if(name)glDeleteFramebuffers(1,&name);
            for(auto name:colors)if(name)glDeleteTextures(1,&name);
            if(stencil)glDeleteRenderbuffers(1,&stencil);
        }
        for(auto& token:tokens)android_resources::release_v38(token);
        throw;
    }
    target_=framebuffers[0];query_target_=framebuffers[1];target_color_=colors[0];query_color_=colors[1];stencil_buffer_=stencil;
    target_budget_v39_=tokens;target_generation_v39_=generation;target_width_=width;target_height_=height;
}
bool SwfGpu::image(std::int32_t width,std::int32_t height,unsigned channels,const std::uint8_t* pixels,
                   std::size_t pitch,ui::SwfTexture& out,std::string& error){
    bool inserted=false;std::uintptr_t id=0;
    try{
        ++command_serial_v37_;
        if(!pixels||width<=0||height<=0||width>16384||height>16384||(channels!=1&&channels!=3&&channels!=4))throw std::runtime_error("Invalid SWF bitmap request");
        const auto row=std::size_t(width)*channels;
        if(pitch<row||pitch>64*1024*1024||std::size_t(height)>64*1024*1024/row)throw std::runtime_error("SWF bitmap storage exceeds bounds");
        if(next_identity_==std::numeric_limits<std::uintptr_t>::max())throw std::runtime_error("SWF texture identity exhausted");
        if(scope_v39_!=resources::ResourceScopeV37::swf_front&&scope_v39_!=resources::ResourceScopeV37::swf_gameplay)throw std::runtime_error("V39 UI resource scope is unavailable");
        if(max_texture_size_v37_>0&&(width>max_texture_size_v37_||height>max_texture_size_v37_))throw std::runtime_error("SWF bitmap exceeds GPU dimensions");
        Texture candidate;candidate.width=width;candidate.height=height;candidate.channels=channels;
        if(!candidate.pixels.allocate(android_resources::budget_lease_v39(),scope_v39_,row*height,error))throw std::runtime_error(error);
        for(int y=0;y<height;++y)std::memcpy(candidate.pixels.data()+row*y,pixels+pitch*y,row);
        // Allocate the map node BEFORE creating a GL owner: map allocation failure
        // cannot lose an uploaded GLuint through a moved temporary's destruction.
        id=next_identity_;auto entry=textures_.try_emplace(id);
        if(!entry.second)throw std::runtime_error("V39 duplicate UI bitmap identity");
        inserted=true;entry.first->second=std::move(candidate);
        if(normal_.name)upload(entry.first->second);
        ++next_identity_;out={id,width,height};error.clear();return true;
    }catch(const std::exception& e){
        if(inserted){release_texture_v39(textures_.at(id),false);textures_.erase(id);}
        error=e.what();return false;
    }
}
''' +source[end:]
source=source.replace('for(auto& pair:textures_)if(pair.second.name)glDeleteTextures(1,&pair.second.name);','for(auto& pair:textures_)release_texture_v39(pair.second,false);')
source=source.replace('std::vector<std::uint8_t> pixels(std::size_t(x1-x0)*(y1-y0)*4);','''resources::RetainedBytesV39 pixels;std::uint64_t readback_bytes;
            if(!resources::swf_bitmap_bytes_v39(x1-x0,y1-y0,4,readback_bytes,error)||
               !pixels.allocate(android_resources::budget_lease_v39(),scope_v39_,static_cast<std::size_t>(readback_bytes),error))throw std::runtime_error(error);''')
source=source.replace('android_resources::budget_v38().snapshot()','budget_v39_->snapshot()').replace('auto& ledger=android_resources::budget_v38();','auto& ledger=*budget_v39_;').replace('android_resources::release_v38(','release_charge_v39(').replace('pixels.allocate(android_resources::budget_lease_v39(),','pixels.allocate(budget_v39_,')
changes={'swf_gpu.hpp':header,'swf_gpu.cpp':source}
diff=[]
for name,text in changes.items():
 (out/name).write_text(text)
 path='port/android-native/app/src/main/cpp/'+name
 diff.extend(difflib.unified_diff(baseline[name].splitlines(True),text.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
# Exact minimal call-site scope and helper linkage hunks for root to apply.
for name,scope in [('original_ui_session.cpp','swf_gameplay'),('front_ui_session_v87.cpp','swf_front')]:
 before=(native/name).read_text();after=before.replace('impl_->gpu.initialize(manager);',f'impl_->gpu.initialize(manager,resources::ResourceScopeV37::{scope});')
 old='std::vector<std::uint8_t> rgba(std::size_t(view.width)*view.height*4);'
 replacement=f'''resources::RetainedBytesV39 rgba;std::uint64_t rgba_bytes;
        if(!resources::swf_bitmap_bytes_v39(view.width,view.height,4,rgba_bytes,error)||
           !rgba.allocate(self.gpu.resource_budget_lease_v39(),resources::ResourceScopeV37::{scope},static_cast<std::size_t>(rgba_bytes),error))return false;'''
 assert after.count(old)==1
 after=after.replace(old,replacement)
 assert after!=before
 path='port/android-native/app/src/main/cpp/'+name
 diff.extend(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
cmake=native/'CMakeLists.txt';before=cmake.read_text()
after=before.replace('target_sources(dh2_scene_materials PRIVATE "${DH2_SOURCE_DIR}/port/engine-resources/resource_budget_v37.cpp")','target_sources(dh2_scene_materials PRIVATE "${DH2_SOURCE_DIR}/port/engine-resources/resource_budget_v37.cpp"\n  "${DH2_SOURCE_DIR}/port/engine-resources/retained_bytes_v39.cpp")')
assert after!=before
path='port/android-native/app/src/main/cpp/CMakeLists.txt'
diff.extend(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
(out/'swf-resource-v39.patch').write_bytes(''.join(diff).encode())
manifest={name:hashlib.sha256((native/name).read_bytes()).hexdigest() for name in [*baseline,'original_ui_session.cpp','front_ui_session_v87.cpp','CMakeLists.txt']}
(out/'baseline-source-sha256.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps({'staged':str(out),'baseline':manifest,'patch_bytes':(out/'swf-resource-v39.patch').stat().st_size}))
