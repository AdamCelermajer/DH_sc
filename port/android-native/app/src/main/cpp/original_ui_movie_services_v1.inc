// Included inside OriginalUiSession::Impl. A separate movie gets a separate
// source font platform; the APK cache, actual GPU images and font readers are
// shared. Every callback preserves its original application context.
struct MoviePlatformLease {
    std::shared_ptr<Impl> resources;
    ui::SwfServices application;
    std::unique_ptr<ui::GfntTextBackendV1> bitmap;
    MoviePlatformLease(std::shared_ptr<Impl> owner,const ui::SwfServices& app)
        :resources(std::move(owner)),application(app){
        const ui::GfntTextServicesV1 source{resources.get(),&resources->source_bitmap_cache_width_v1,
            &resources->source_bitmap_cache_height_v1,gfnt_resolve_v1,gfnt_read_v1,nullptr};
        bitmap=std::make_unique<ui::GfntTextBackendV1>(source,resources);
        ++resources->external_movies;
    }
    ~MoviePlatformLease(){--resources->external_movies;}
    static Impl* resource(void* p){return static_cast<MoviePlatformLease*>(p)->resources.get();}
    static bool read(void* p,const char* uri,std::vector<std::uint8_t>& b,std::string& e){return movie_read(resource(p),uri,b,e);}
    static bool tex(void* p,const char* name,int w,int h,ui::SwfTexture& t,std::string& e){return texture(resource(p),name,w,h,t,e);}
    static bool upload(void* p,int w,int h,unsigned c,const std::uint8_t* b,int pitch,ui::SwfTexture& t,std::string& e){return image(resource(p),w,h,c,b,pitch,t,e);}
    static bool submit(void* p,const ui::SwfDraw& d,std::string& e){return draw(resource(p),d,e);}
    static bool mask(void* p,const float b[4],std::uint8_t n,bool& v,std::string& e){return stencil(resource(p),b,n,v,e);}
    static bool primitive(void* p,const char* name,const std::vector<ui::SwfValue>& args,ui::SwfValue& out,std::string& e){
        auto& lease=*static_cast<MoviePlatformLease*>(p);
        if(lease.application.native_call)return lease.application.native_call(lease.application.context,name,args,out,e);
        return native(lease.resources.get(),name,args,out,e);
    }
    static bool action(void* p,const char* name,const gameswf::fn_call& call,std::string& e){
        auto& lease=*static_cast<MoviePlatformLease*>(p);
        return lease.application.native_action(lease.application.context,name,call,e);
    }
    static bool start(void* p,const ui::SwfAsLease& graph,std::string& e){
        auto& lease=*static_cast<MoviePlatformLease*>(p);
        return !lease.application.graph_start||lease.application.graph_start(lease.application.context,graph,e);
    }
    static void report(void* p,bool is_error,const char* message){
        auto& lease=*static_cast<MoviePlatformLease*>(p);diagnostic(lease.resources.get(),is_error,message);
        if(lease.application.diagnostic)lease.application.diagnostic(lease.application.context,is_error,message);
    }
    ui::SwfServices services(){
        ui::SwfServices out;out.context=this;out.read=read;out.texture=tex;out.image=upload;
        out.draw=submit;out.stencil=mask;out.native_call=primitive;out.native_action=action;
        out.graph_start=start;out.diagnostic=report;out.native_actions=application.native_actions;
        // The wrapping source platform retains this lease as platform_owner.
        out.native_owner=application.native_owner;return out;
    }
};
