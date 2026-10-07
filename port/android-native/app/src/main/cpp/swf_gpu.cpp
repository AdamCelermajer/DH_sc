#include "swf_gpu.hpp"
#include "swf_grid_snap_v1.hpp"
#include "../scene-materials/swf_texture.hpp"
#include "frame_perf_v35.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <optional>

namespace dh2::android_ui {
namespace {
void finite(float x){if(!std::isfinite(x))throw std::runtime_error("Nonfinite SWF render value");}
}
void SwfGpu::barrier_v37(const char* action){
    // Primitive submission is asynchronous. Check at ownership transitions,
    // retaining the complete source command range since the previous barrier.
    ++error_queries_v37_;const auto code=glGetError();
    if(code!=GL_NO_ERROR)throw std::runtime_error(std::string(action)+" GL error "+std::to_string(code)+
        " commands ["+std::to_string(std::min(checked_serial_v37_+1,command_serial_v37_))+","+
        std::to_string(command_serial_v37_)+"]");
    checked_serial_v37_=command_serial_v37_;
}
void SwfGpu::initialize(AAssetManager* assets,resources::ResourceScopeV37 scope){
    if(!budget_v39_)budget_v39_=android_resources::budget_lease_v39();
    if(scope!=resources::ResourceScopeV37::swf_front&&scope!=resources::ResourceScopeV37::swf_gameplay)
        throw std::runtime_error("V39 explicit front/gameplay UI resource scope required");
    if(!textures_.empty()&&scope_v39_!=scope)throw std::runtime_error("V39 retained UI scope cannot change");
    scope_v39_=scope;
    std::string cache_error_v43;
    if(!vertices_capacity_v43_.same_owner(budget_v39_,scope))throw std::runtime_error("V43 SWF stream CPU scope cannot change");
    if(!vertex_cache_v36_.bind_budget_v43(budget_v39_,scope,cache_error_v43))throw std::runtime_error(cache_error_v43);
    // Strong owners release stale ledger records without deleting reused names.
    if(stream_owner_v43_.live())throw std::runtime_error("V43 live SWF stream context cannot be abandoned");
    for(const auto& owner:retained_owners_v43_)if(owner.second.live())throw std::runtime_error("V43 live SWF retained context cannot be abandoned");
    retained_owners_v43_.clear();stream_owner_v43_.reset();vertex_cache_v36_.abandon_context();
    release_targets_v39(true);
    for(auto& pair:textures_)release_texture_v39(pair.second,true);
    // Retained CPU texture identities survive GL recreation; old names must not
    // be deleted in a new context whose names may have already been recycled.
    normal_={};premultiplied_={};buffer_=0;frame_=false;mask_level_=0;submitting_mask_=false;begin_pending_=false;
    materialized_v36_=false;
    max_texture_size_v37_=stencil_bits_v37_=0;line_width_range_v37_[0]=line_width_range_v37_[1]=0;
    checked_serial_v37_=command_serial_v37_;
    target_=query_target_=target_color_=query_color_=stencil_buffer_=0;target_width_=target_height_=0;
    for(auto& pair:textures_)pair.second.name=0;
    barrier_v37("SWF context entry");
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max_texture_size_v37_);glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE,line_width_range_v37_);
    capability_queries_v37_+=2;barrier_v37("SWF context capabilities");
    if(max_texture_size_v37_<=0||!std::isfinite(line_width_range_v37_[0])||!std::isfinite(line_width_range_v37_[1])||
       line_width_range_v37_[0]<=0||line_width_range_v37_[1]<line_width_range_v37_[0])
        throw std::runtime_error("Invalid SWF context capabilities");
    auto normal_candidate_v43=create(assets,false,scope_v39_);auto premultiplied_candidate_v43=create(assets,true,scope_v39_);
    normal_=std::move(normal_candidate_v43);premultiplied_=std::move(premultiplied_candidate_v43);
    buffer_=0; // Stream object is admitted lazily on actual fallback.
    if(!white_){ui::SwfTexture out;std::string error;const std::uint8_t white[4]{255,255,255,255};
        if(!image(1,1,4,white,4,out,error))throw std::runtime_error(error);white_=out.identity;}
    for(auto& pair:textures_)if(!pair.second.name)upload(pair.second);
    barrier_v37("SWF context initialization");
}
void SwfGpu::release_texture_v39(Texture& texture,bool discard_context){
    const auto current=budget_v39_->snapshot();
    const bool same=current.context_ready&&current.context_generation==texture.generation_v39;
    if(texture.name&&discard_context&&same)throw std::runtime_error("V39 live texture context cannot be abandoned");
    if(texture.name&&!discard_context&&same)glDeleteTextures(1,&texture.name);
    texture.name=0;texture.generation_v39=0;
    release_charge_v39(texture.gpu_budget_v39);
}
void SwfGpu::release_targets_v39(bool discard_context){
    const auto current=budget_v39_->snapshot();
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
    for(auto& token:target_budget_v39_)release_charge_v39(token);
}
void SwfGpu::upload(Texture& texture){
    barrier_v37("SWF bitmap resource entry");
    if(texture.width>max_texture_size_v37_||texture.height>max_texture_size_v37_)throw std::runtime_error("SWF bitmap exceeds GPU dimensions");
    std::uint64_t bytes;std::string error;
    if(!resources::swf_bitmap_bytes_v39(texture.width,texture.height,texture.channels,bytes,error))throw std::runtime_error(error);
    if(texture.name||texture.gpu_budget_v39)throw std::runtime_error("V39 bitmap upload requires unpublished GPU owner");
    auto& ledger=*budget_v39_;resources::ResourceReservationV37 reservation;
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
    auto& ledger=*budget_v39_;
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
        for(auto& token:tokens)release_charge_v39(token);
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
        if(!candidate.pixels.allocate(budget_v39_,scope_v39_,row*height,error))throw std::runtime_error(error);
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
void SwfGpu::primitive(const ui::SwfDraw& command,const ui::SwfFill& style,GLenum mode,float line_width){
    if(style.kind==ui::SwfFill::disabled&&!submitting_mask_)return;
    // Retained cache uploads and stream fallback create their own admitted VBO
    // lazily. A cold first primitive therefore has no stream buffer yet.
    if(!normal_.name)throw std::runtime_error("SWF GPU context unavailable");
    std::optional<dh2::perf::Scope> geometry_scope;geometry_scope.emplace(dh2::perf::Phase::ui_geometry);
    const float* xy=command.xy.data();std::size_t xy_size=command.xy.size();
    std::array<float,8> quad_xy{};
    const bool quad=command.kind==ui::SwfDraw::bitmap_quad;
    if(quad){quad_xy={command.rect[0],command.rect[2],command.rect[1],command.rect[2],
                command.rect[0],command.rect[3],command.rect[1],command.rect[3]};xy=quad_xy.data();xy_size=quad_xy.size();}
    if(!xy_size)return;
    if(xy_size%2||xy_size>2*1024*1024)throw std::runtime_error("Invalid SWF vertex span");
    ++primitives_v36_;
    const auto identity=style.kind==ui::SwfFill::bitmap?style.texture.identity:white_;
    auto found=textures_.find(identity);
    if(found==textures_.end())throw std::runtime_error("Required SWF texture owner missing");
    auto& texture=found->second;
    if(!texture.name)throw std::runtime_error("Required SWF texture upload missing");
    if(style.kind==ui::SwfFill::bitmap&&(style.texture.width!=texture.width||style.texture.height!=texture.height))
        throw std::runtime_error("SWF texture dimensions disagree with owner");
    for(auto value:command.matrix.value)finite(value);
    for(auto value:style.uv.value)finite(value);
    const float sx=2.f/(bounds_[1]-bounds_[0]),sy=-2.f/(bounds_[3]-bounds_[2]);
    const auto& m=command.matrix.value;
    std::array<float,16> matrix{
        sx*m[0],sy*m[3],0,0,sx*m[1],sy*m[4],0,0,0,0,1,0,
        sx*(m[2]-bounds_[0])-1.f,sy*(m[5]-bounds_[2])+1.f,0,1};
    if(quad&&grid_fit_){matrix={sx,0,0,0,0,sy,0,0,0,0,1,0,-sx*bounds_[0]-1.f,-sy*bounds_[2]+1.f,0,1};}
    auto& vertices=vertices_v36_;vertices.clear();
    if(xy_size>std::numeric_limits<std::size_t>::max()/2)throw std::runtime_error("SWF packed vertex span overflow");
    if(vertices.capacity()<xy_size*2){std::string error;
        if(!resources::reserve_cpu_vector_v41(vertices,vertices_capacity_v43_,budget_v39_,scope_v39_,xy_size*2,error))throw std::runtime_error(error);
        ++scratch_growths_v36_;}
    for(std::size_t i=0;i<xy_size;i+=2){
        const float x=xy[i],y=xy[i+1];finite(x);finite(y);
        float u=0,v=0;
        if(style.kind==ui::SwfFill::bitmap){
            if(quad){const unsigned vertex=static_cast<unsigned>(i/2);u=command.uv_rect[vertex%2];v=command.uv_rect[2+vertex/2];}
            else{const auto& uv=style.uv.value;u=(uv[0]*x+uv[1]*y+uv[2])/texture.width;v=(uv[3]*x+uv[4]*y+uv[5])/texture.height;}
        }
        float px=x,py=y;
        if(quad&&grid_fit_){
            // Original draw_bitmap7d90fc snaps after the full world matrix:
            // signed ((f2iz(twips)+10)/20)*20, with negative asymmetry.
            auto snap=[](float value){
                std::uint32_t bits;std::memcpy(&bits,&value,4);
                bits=dh2_swf_grid_snap_v1(bits);std::memcpy(&value,&bits,4);
                return value;
            };
            px=snap(m[0]*x+m[1]*y+m[2]);py=snap(m[3]*x+m[4]*y+m[5]);
        }
        finite(u);finite(v);vertices.insert(vertices.end(),{px,py,u,v});
    }
    // These four pass selections and GL blend factors were executed against the
    // original GameSWF material. Unknown authored modes must be implemented.
    const Program* program=&normal_;GLenum source=GL_SRC_ALPHA,destination=GL_ONE_MINUS_SRC_ALPHA;
    const auto blend=style.kind==ui::SwfFill::bitmap&&!quad?style.blend:0;
    switch(blend){
        case 0:case 1:case 15:case 16:break;
        case 3:program=&premultiplied_;source=GL_DST_COLOR;break;
        case 4:program=&premultiplied_;source=GL_ONE;destination=GL_ONE_MINUS_SRC_COLOR;break;
        case 13:program=&premultiplied_;source=GL_DST_COLOR;destination=GL_ONE;break;
        default:throw std::runtime_error("Required SWF blend mode unsupported: "+std::to_string(blend));
    }
    // Recovered immediate draw: solid color applies full cxform and truncates;
    // bitmap fills use multiplicative terms only; glyphs use caller byte RGBA.
    // Cached render-list playback is a separate source path, not this adapter.
    std::array<float,4> color{};
    scene::SwfCxform32 cx{};
    for(unsigned i=0;i<8;++i){finite(command.color_transform.value[i]);cx.terms[i]=command.color_transform.value[i];}
    std::array<std::uint8_t,4> bytes{};
    if(quad)std::copy(std::begin(style.rgba),std::end(style.rgba),bytes.begin());
    else if(style.kind==ui::SwfFill::bitmap)bytes=scene::swf_bitmap_color(cx).rgba;
    else {std::array<std::uint8_t,4> raw{};std::copy(std::begin(style.rgba),std::end(style.rgba),raw.begin());
          bytes=scene::swf_solid_color(cx,raw);}
    for(unsigned i=0;i<4;++i){
        color[i]=bytes[i]/255.f;
    }
    // Modern GL_ALPHA sampling supplies zero RGB; this offset yields white RGB
    // with the authored coverage alpha. It is an adapter representation, not a
    // claim that original font image format12 was texture format2.
    const std::array<float,4> diffuse=texture.channels==1?std::array<float,4>{1,1,1,0}:std::array<float,4>{0,0,0,0};
    std::uint32_t wrap=GL_CLAMP_TO_EDGE;
    if(!scene::swf_texture_gl_wrap(style.kind==ui::SwfFill::bitmap&&!quad?(style.wrap==0?0:2):1,wrap))
        throw std::runtime_error("Invalid SWF texture wrap producer");
    // A fully validated normal pass with exact final byte alpha zero changes
    // neither RGB nor coverage alpha. Stencil writers and non-normal passes
    // must still execute; their output is not determined by alpha alone.
    if(!submitting_mask_&&!mask_level_&&program==&normal_&&source==GL_SRC_ALPHA&&destination==GL_ONE_MINUS_SRC_ALPHA&&bytes[3]==0){
        ++transparent_noops_v37_;return;
    }
    geometry_scope.reset();dh2::perf::Scope submit_scope(dh2::perf::Phase::ui_submit);
    materialize_v36();
    if(mode==GL_LINE_STRIP)glLineWidth(std::clamp(line_width,line_width_range_v37_[0],line_width_range_v37_[1]));
    glEnable(GL_BLEND);glBlendEquation(GL_FUNC_ADD);
    // The private color target accumulates premultiplied color for compositing;
    // preserve the recovered source RGB factors while retaining coverage alpha.
    glBlendFuncSeparate(source,destination,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program->name);glUniformMatrix4fv(program->matrix,1,GL_FALSE,matrix.data());
    glUniform4fv(program->diffuse,1,diffuse.data());glUniform1i(program->sampler,0);
    glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture.name);
    if(texture.wrap!=wrap){glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);texture.wrap=wrap;}
    glDisableVertexAttribArray(program->color);glVertexAttrib4fv(program->color,color.data());
    std::uintptr_t cached_buffer{};bool retained{};std::string cache_error;
    const auto upload=[this](const float* data,std::size_t count,std::uintptr_t& out,std::string& error){
        barrier_v37("SWF retained vertex resource entry");
        std::uint64_t bytes;if(!resources::checked_resource_bytes_v37(count,sizeof(float),bytes,error))return false;
        if(bytes>std::uint64_t(std::numeric_limits<GLsizeiptr>::max()))throw std::runtime_error("SWF vertex GL span overflow");
        resources::GpuObjectOwnerV43 candidate;
        if(!candidate.create(budget_v39_,resources::ResourceKindV37::vertex_buffer,scope_v39_,bytes,
          [&](std::uint32_t& name){glGenBuffers(1,&name);if(!name)return;
            glBindBuffer(GL_ARRAY_BUFFER,name);glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(bytes),data,GL_STATIC_DRAW);
            barrier_v37("SWF retained vertex upload");},
          [](std::uint32_t name){glDeleteBuffers(1,&name);},error))return false;
        const auto name=candidate.name();
        const auto inserted=retained_owners_v43_.try_emplace(name,std::move(candidate));
        if(!inserted.second)throw std::runtime_error("SWF retained buffer name collision");
        out=name;dh2::perf::state.uploads+=bytes;return true;
    };
    const auto release=[this](std::uintptr_t value){release_vertices_v43(value);};
    if(!vertex_cache_v36_.acquire(vertices,mode,upload,release,cached_buffer,retained,cache_error))throw std::runtime_error(cache_error);
    if(!retained){stream_storage_v43(vertices);
        ++stream_uploads_v36_;stream_bytes_v36_+=vertices.size()*sizeof(float);dh2::perf::state.uploads+=vertices.size()*sizeof(float);}
    glBindBuffer(GL_ARRAY_BUFFER,retained?GLuint(cached_buffer):buffer_);
    glEnableVertexAttribArray(program->position);glEnableVertexAttribArray(program->uv);
    glVertexAttribPointer(program->position,2,GL_FLOAT,GL_FALSE,4*sizeof(float),nullptr);
    glVertexAttribPointer(program->uv,2,GL_FLOAT,GL_FALSE,4*sizeof(float),reinterpret_cast<void*>(2*sizeof(float)));
    const auto vertex_count=static_cast<GLsizei>(xy_size/2);
    if(ui::swf_renderer_wireframe_v62().draw_wireframe()&&!submitting_mask_&&
       (mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP)){
        // GLES2 has no polygon mode. Draw the edges of the SAME triangles,
        // with their exact current vertices/UV/color/blend and stencil test.
        // Source mask submission stays filled; there is no new geometry or
        // resource allocation, no hidden callbacks and no overlay rectangle.
        const GLsizei step=mode==GL_TRIANGLES?3:1;
        for(GLsizei first=0;first+2<vertex_count;first+=step){glDrawArrays(GL_LINE_LOOP,first,3);++dh2::perf::state.draws;}
    }else{glDrawArrays(mode,0,vertex_count);++dh2::perf::state.draws;}
    glDisableVertexAttribArray(program->position);glDisableVertexAttribArray(program->uv);glBindBuffer(GL_ARRAY_BUFFER,0);
}
void SwfGpu::release_vertices_v43(std::uintptr_t value){
    const auto found=retained_owners_v43_.find(std::uint32_t(value));
    if(found==retained_owners_v43_.end())throw std::runtime_error("SWF retained buffer ledger owner missing");
    retained_owners_v43_.erase(found);
}
void SwfGpu::stream_storage_v43(const std::vector<float>& vertices){
    std::string error;std::uint64_t bytes;
    if(!resources::checked_resource_bytes_v37(vertices.size(),sizeof(float),bytes,error))throw std::runtime_error(error);
    if(bytes>std::uint64_t(std::numeric_limits<GLsizeiptr>::max()))throw std::runtime_error("SWF stream GL span overflow");
    barrier_v37("SWF stream vertex resource entry");
    const auto upload=[&](std::uint32_t name){
      glBindBuffer(GL_ARRAY_BUFFER,name);glBufferData(GL_ARRAY_BUFFER,GLsizeiptr(bytes),vertices.data(),GL_STREAM_DRAW);
      barrier_v37("SWF stream vertex upload");};
    try{
      if(!stream_owner_v43_.name()){
        if(!stream_owner_v43_.create(budget_v39_,resources::ResourceKindV37::vertex_buffer,scope_v39_,bytes,
          [&](std::uint32_t& name){glGenBuffers(1,&name);if(!name)return;barrier_v37("SWF stream buffer allocation");upload(name);},
          [](std::uint32_t name){glDeleteBuffers(1,&name);},error))throw std::runtime_error(error);
      }else if(!stream_owner_v43_.replace_storage(bytes,upload,error))throw std::runtime_error(error);
    }catch(...){buffer_=stream_owner_v43_.name();throw;}
    buffer_=stream_owner_v43_.name();
}
void SwfGpu::mask_rectangle(){
    ui::SwfDraw command;command.kind=ui::SwfDraw::triangle_strip;
    command.xy={bounds_[0],bounds_[2],bounds_[1],bounds_[2],bounds_[0],bounds_[3],bounds_[1],bounds_[3]};
    command.fill.kind=ui::SwfFill::color;
    primitive(command,command.fill,GL_TRIANGLE_STRIP);
}
void SwfGpu::abort() noexcept {
    if(materialized_v36_||begin_pending_){glBindFramebuffer(GL_FRAMEBUFFER,destination_);glViewport(viewport_[0],viewport_[1],viewport_[2],viewport_[3]);
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glDepthMask(GL_TRUE);glDisable(GL_STENCIL_TEST);}
    frame_=false;begin_pending_=false;mask_level_=0;submitting_mask_=false;
    materialized_v36_=false;
}
void SwfGpu::materialize_v36(){
    if(materialized_v36_)return;
    if(!frame_)throw std::runtime_error("SWF target requested outside logical display");
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&destination_);begin_pending_=true;target(viewport_[2],viewport_[3]);
    glBindFramebuffer(GL_FRAMEBUFFER,target_);
    glViewport(0,0,viewport_[2],viewport_[3]);glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);glDisable(GL_STENCIL_TEST);glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glStencilMask(0xff);
    glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);
    materialized_v36_=true;begin_pending_=false;++materializations_v36_;
}
bool SwfGpu::source_remove_image_v119(const ui::SwfTexture& image,std::string& error){
 if(frame_){error="TextureManager.removeTexture during actual SWF display";return false;}
 const auto at=textures_.find(image.identity);
 if(at==textures_.end()||image.identity==white_||at->second.width!=image.width||at->second.height!=image.height){error="TextureManager.removeTexture requires SAME positive native image";return false;}
 try{barrier_v37("source removeTexture");release_texture_v39(at->second,false);textures_.erase(at);error.clear();return true;}
 catch(const std::exception& ex){error=ex.what();return false;}
}
bool SwfGpu::reset_images(std::string& error){
    abort();
    try{barrier_v37("SWF image reset entry");}catch(const std::exception& e){error=e.what();return false;}
    vertex_cache_v36_.clear([this](std::uintptr_t value){release_vertices_v43(value);});
    for(auto& pair:textures_)release_texture_v39(pair.second,false);
    textures_.clear();white_=0;next_identity_=1;
    ui::SwfTexture out;const std::uint8_t white[4]{255,255,255,255};
    if(!image(1,1,4,white,4,out,error))return false;
    white_=out.identity;return true;
}
bool SwfGpu::draw(const ui::SwfDraw& command,std::string& error){
    const bool entered_frame=frame_;
    const int entered_mask=mask_level_;
    try{
        using Draw=ui::SwfDraw;
        ++command_serial_v37_;
        if(command.kind!=Draw::begin&&!frame_)throw std::runtime_error("SWF command outside display lifetime");
        const bool boundary=command.kind==Draw::begin||command.kind==Draw::end||command.kind==Draw::mask_begin||
            command.kind==Draw::mask_end||command.kind==Draw::mask_disable;
        if(boundary){dh2::perf::Scope control_scope(dh2::perf::Phase::ui_submit);barrier_v37("SWF display/mask entry");}
        switch(command.kind){
        case Draw::begin:{
            dh2::perf::Scope control_scope(dh2::perf::Phase::ui_submit);
            if(frame_)throw std::runtime_error("Nested SWF display begin");
            for(unsigned i=0;i<4;++i){finite(command.bounds[i]);bounds_[i]=command.bounds[i];viewport_[i]=command.viewport[i];}
            if(bounds_[1]<=bounds_[0]||bounds_[3]<=bounds_[2]||viewport_[2]<=0||viewport_[3]<=0)
                throw std::runtime_error("Invalid SWF display extent");
            // Preserve every source command/callback, but do not allocate,
            // clear or composite a fullscreen target for an empty display.
            frame_=true;materialized_v36_=false;begin_pending_=false;mask_level_=0;submitting_mask_=false;++displays_v36_;break;}
        case Draw::end:{
            dh2::perf::Scope control_scope(dh2::perf::Phase::ui_submit);
            if(mask_level_||submitting_mask_)throw std::runtime_error("Unbalanced SWF mask lifetime");
            if(!materialized_v36_){frame_=false;++empty_displays_v36_;break;}
            glDisable(GL_STENCIL_TEST);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBindFramebuffer(GL_FRAMEBUFFER,destination_);glViewport(viewport_[0],viewport_[1],viewport_[2],viewport_[3]);
            glEnable(GL_BLEND);glBlendEquation(GL_FUNC_ADD);glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
            {const std::array<float,16> flipped{1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1};
             draw_quad(normal_,target_color_,flipped,{1,1,1,1},{0,0,0,0});}
            glDepthMask(GL_TRUE);frame_=false;materialized_v36_=false;break;}
        case Draw::triangles:primitive(command,command.fill,GL_TRIANGLES);break;
        case Draw::triangle_strip:primitive(command,command.fill,GL_TRIANGLE_STRIP);break;
        case Draw::bitmap_quad:primitive(command,command.fill,GL_TRIANGLE_STRIP);break;
        case Draw::line_strip:
            finite(command.line_width);if(command.line_width<=0)break;
            // Source widths are twips; actual affine scale and viewport turn them
            // into pixel widths. GLES2 hardware clamps to its supported range.
            {const float x=std::hypot(command.matrix.value[0],command.matrix.value[3]);
             const float y=std::hypot(command.matrix.value[1],command.matrix.value[4]);
             const float scale=(viewport_[2]/(bounds_[1]-bounds_[0])+viewport_[3]/(bounds_[3]-bounds_[2]))*.5f;
             const float width=command.line_width*(x+y)*.5f*scale;finite(width);
             primitive(command,command.line,GL_LINE_STRIP,width);}break;
        case Draw::mask_begin:
            if(mask_level_>=255)throw std::runtime_error("Invalid SWF mask begin");
            materialize_v36();
            if(!mask_level_){glEnable(GL_STENCIL_TEST);glClearStencil(0);glClear(GL_STENCIL_BUFFER_BIT);}
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glStencilFunc(GL_EQUAL,mask_level_++,0xff);glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);submitting_mask_=true;break;
        case Draw::mask_end:
            if(!submitting_mask_||!mask_level_)throw std::runtime_error("Invalid SWF mask end");
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glStencilFunc(GL_EQUAL,mask_level_,0xff);
            glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);submitting_mask_=false;break;
        case Draw::mask_disable:
            if(!mask_level_)throw std::runtime_error("Invalid SWF mask disable");
            submitting_mask_=false;
            if(!--mask_level_)glDisable(GL_STENCIL_TEST);
            else{glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);glStencilFunc(GL_EQUAL,mask_level_+1,0xff);
                 glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);mask_rectangle();
                 glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glStencilFunc(GL_EQUAL,mask_level_,0xff);glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);}
            break;
        case Draw::antialias:break; // frame multisampling belongs to the EGL config
        }
        if(boundary){dh2::perf::Scope control_scope(dh2::perf::Phase::ui_submit);barrier_v37("SWF display/mask exit");}error.clear();return true;
    }catch(const std::exception& e){
        abort();
        error=std::string(e.what())+" [GPU "+std::to_string(reinterpret_cast<std::uintptr_t>(this))+
            ", command "+std::to_string(unsigned(command.kind))+", serial "+std::to_string(command_serial_v37_)+
            ", entered frame "+std::to_string(entered_frame)+", mask "+std::to_string(entered_mask)+"]";
        return false;
    }
}
bool SwfGpu::scene_pane(const float r[4],void* context,
 bool (*render)(void*,int,int,std::string&),std::string& error){
 try{
 ++command_serial_v37_;
 if(!frame_||submitting_mask_||!render){error="Native scene pane outside color display";return false;}
 barrier_v37("SWF scene pane entry");materialize_v36();
 for(unsigned i=0;i<4;++i)if(!std::isfinite(r[i])){error="Invalid scene pane bounds";return false;}
 const float sx=float(viewport_[2])/(bounds_[1]-bounds_[0]),sy=float(viewport_[3])/(bounds_[3]-bounds_[2]);
 const int x=int((r[0]-bounds_[0])*sx),right=int((r[1]-bounds_[0])*sx);
 const int top=int((r[2]-bounds_[2])*sy),bottom=int((r[3]-bounds_[2])*sy);
 const int w=right-x,h=bottom-top;
 if(w<=0||h<=0){error="Empty native scene pane";return false;}
 // Source callback preserves/restores the driver viewport. The private SWF
 // coverage FBO uses the same stage mapping with an OpenGL bottom origin.
 GLint old[4];glGetIntegerv(GL_VIEWPORT,old);glViewport(x,viewport_[3]-bottom,w,h);
 glEnable(GL_SCISSOR_TEST);glScissor(std::max(0,x),std::max(0,viewport_[3]-bottom),w,h);
 glDepthMask(GL_TRUE);glClear(GL_DEPTH_BUFFER_BIT);
 bool ok=false;try{ok=render(context,w,h,error);}catch(const std::exception& e){error=e.what();}
 glViewport(old[0],old[1],old[2],old[3]);glDisable(GL_SCISSOR_TEST);
 glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glDepthMask(GL_FALSE);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);glActiveTexture(GL_TEXTURE0);
 if(mask_level_)glEnable(GL_STENCIL_TEST);else glDisable(GL_STENCIL_TEST);
 barrier_v37("SWF scene pane exit");
 return ok;
 }catch(const std::exception& e){abort();error=e.what();return false;}
}
bool SwfGpu::stencil(const float bounds[4],std::uint8_t pattern,bool& out,std::string& error){
    try{
        ++command_serial_v37_;
        if(!frame_)throw std::runtime_error("SWF stencil query outside display lifetime");
        barrier_v37("SWF shared-stencil query entry");
        materialize_v36();
        if(!query_target_)throw std::runtime_error("SWF stencil query target unavailable");
        for(unsigned i=0;i<4;++i)finite(bounds[i]);
        // Upstream sprite hitTest supplies world pixels after twips_to_pixels.
        // Convert them back through the same authored stage/viewport projection.
        const float sx=target_width_/(bounds_[1]-bounds_[0]),sy=target_height_/(bounds_[3]-bounds_[2]);
        auto coordinate=[](float value,int size,bool upper){
            return static_cast<int>(std::clamp(upper?std::ceil(value):std::floor(value),0.f,float(size)));};
        const int x0=coordinate((bounds[0]*20.f-bounds_[0])*sx,target_width_,false);
        const int x1=coordinate((bounds[1]*20.f-bounds_[0])*sx,target_width_,true);
        const int y0=coordinate((bounds[2]*20.f-bounds_[2])*sy,target_height_,false);
        const int y1=coordinate((bounds[3]*20.f-bounds_[2])*sy,target_height_,true);
        bool result=false;
        if(x1>x0&&y1>y0){
            glBindFramebuffer(GL_FRAMEBUFFER,query_target_);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);
            glEnable(GL_STENCIL_TEST);glStencilFunc(GL_EQUAL,pattern,0xff);glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
            mask_rectangle();
            resources::RetainedBytesV39 pixels;std::uint64_t readback_bytes;
            if(!resources::swf_bitmap_bytes_v39(x1-x0,y1-y0,4,readback_bytes,error)||
               !pixels.allocate(budget_v39_,scope_v39_,static_cast<std::size_t>(readback_bytes),error))throw std::runtime_error(error);
            glPixelStorei(GL_PACK_ALIGNMENT,1);
            glReadPixels(x0,target_height_-y1,x1-x0,y1-y0,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
            for(std::size_t i=3;i<pixels.size();i+=4)if(pixels[i]){result=true;break;}
            glBindFramebuffer(GL_FRAMEBUFFER,target_);
            if(mask_level_){glEnable(GL_STENCIL_TEST);
                glStencilFunc(GL_EQUAL,submitting_mask_?mask_level_-1:mask_level_,0xff);
                glStencilOp(GL_KEEP,GL_KEEP,submitting_mask_?GL_INCR:GL_KEEP);}
            else glDisable(GL_STENCIL_TEST);
            glColorMask(!submitting_mask_,!submitting_mask_,!submitting_mask_,!submitting_mask_);
        }
        barrier_v37("SWF shared-stencil query exit");out=result;error.clear();return true;
    }catch(const std::exception& e){
        abort();
        error=e.what();return false;
    }
}
}
