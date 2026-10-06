#include "swf_source_movie_v1.hpp"
#include "swf_frame_connection.hpp"
namespace dh2::ui {
namespace {
struct SourceOwner {
 SwfServices original;
 std::shared_ptr<SwfInputHistory> history=std::make_shared<SwfInputHistory>();
 SwfFrameConnection frames;
 static SourceOwner& self(void*p){return *static_cast<SourceOwner*>(p);}
 static bool start(void*p,const SwfAsLease&lease,std::string&e){auto&o=self(p);if(!o.history->bind(lease.player,e)||!o.frames.bind(lease.player,o.history,e))return false;return !o.original.graph_start||o.original.graph_start(o.original.context,lease,e);}
 static bool read(void*p,const char*n,std::vector<std::uint8_t>&b,std::string&e){auto&o=self(p).original;return o.read(o.context,n,b,e);}
 static bool texture(void*p,const char*n,std::int32_t w,std::int32_t h,SwfTexture&t,std::string&e){auto&o=self(p).original;return o.texture(o.context,n,w,h,t,e);}
 static bool image(void*p,std::int32_t w,std::int32_t h,std::uint32_t c,const std::uint8_t*b,std::int32_t pitch,SwfTexture&t,std::string&e){auto&o=self(p).original;return o.image(o.context,w,h,c,b,pitch,t,e);}
 static bool draw(void*p,const SwfDraw&d,std::string&e){auto&o=self(p).original;return o.draw(o.context,d,e);}
 static bool native(void*p,const char*n,const std::vector<SwfValue>&a,SwfValue&r,std::string&e){auto&o=self(p).original;return o.native_call(o.context,n,a,r,e);}
 static bool native_as(void*p,const char*n,const gameswf::fn_call&f,std::string&e){auto&o=self(p).original;return o.native_action(o.context,n,f,e);}
 static bool stencil(void*p,const float*b,std::uint8_t n,bool&r,std::string&e){auto&o=self(p).original;return o.stencil(o.context,b,n,r,e);}
 static void diagnostic(void*p,bool failed,const char*n){auto&o=self(p).original;o.diagnostic(o.context,failed,n);}
};
}
bool source_movie_services_v1(const SwfServices&original,SwfServices&wrapped,std::string&e){
 // Preserve the original facade's native ownership rejection before replacing
 // native_owner with the real observer owner; never legitimize borrowed AS.
 if(!original.native_actions.empty()&&(!original.native_owner||!original.native_action)){e="Required owned native AS callback provider unavailable";return false;}
 if(original.graph_start&&!original.native_owner){e="Required owned SWF graph startup provider unavailable";return false;}
 auto owner=std::make_shared<SourceOwner>();owner->original=original;auto out=original;out.context=owner.get();out.native_owner=owner;out.graph_start=SourceOwner::start;
 if(out.read)out.read=SourceOwner::read;if(out.texture)out.texture=SourceOwner::texture;if(out.image)out.image=SourceOwner::image;if(out.draw)out.draw=SourceOwner::draw;
 if(out.native_call)out.native_call=SourceOwner::native;if(out.native_action)out.native_action=SourceOwner::native_as;if(out.stencil)out.stencil=SourceOwner::stencil;if(out.diagnostic)out.diagnostic=SourceOwner::diagnostic;
 wrapped=std::move(out);e.clear();return true;
}
bool source_movie_frame_borrow_v1(const SwfServices& services,SwfSourceFrameBorrowV1& out,std::string& error){
 if(services.graph_start!=SourceOwner::start||!services.native_owner||services.context!=services.native_owner.get()){
  error="Required actual source movie frame owner";return false;
 }
 auto& owner=SourceOwner::self(services.context);
 out={owner.history,&owner.frames};error.clear();return true;
}
}
