namespace dh2::android_ui {
void SwfGpu::barrier_v37(const char* action){
    // Primitive submission is asynchronous. Check at ownership transitions,
    // retaining the complete source command range since the previous barrier.
    ++error_queries_v37_;const auto code=glGetError();
    if(code!=GL_NO_ERROR)throw std::runtime_error(std::string(action)+" GL error "+std::to_string(code)+
        " commands ["+std::to_string(std::min(checked_serial_v37_+1,command_serial_v37_))+","+
        std::to_string(command_serial_v37_)+"]");
    checked_serial_v37_=command_serial_v37_;
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

}
