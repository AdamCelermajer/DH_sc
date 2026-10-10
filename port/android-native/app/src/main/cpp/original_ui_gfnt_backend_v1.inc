// Included inside shared OriginalUiSession::Impl; needs gfnt_text_backend_v1.hpp.
// Real source MenuManagerC1 InitializationParameters at431d24..431d80.
// FT512x512 is separate. Bitmap dimensions+14/+18 are constructor zero.
std::int32_t source_bitmap_cache_width_v1{},source_bitmap_cache_height_v1{};
std::unique_ptr<ui::GfntTextBackendV1> gfnt_text_backend_v1;
static bool gfnt_resolve_v1(void* raw,const ui::text_v1::Font& font,
 std::string& uri,bool& found,std::string& error){
 auto& self=*static_cast<Impl*>(raw);char bytes[4096]{};
 const ui::FontResolveInput24 input{font.name.c_str(),"",font.bold?1u:0u,font.italic?1u:0u};
 ui::FontResolveOutput32 output{bytes,sizeof(bytes),0,0,0};
 const ui::FontResolveServices16 services{raw,font_service};
 if(dh2_swf_font_resolve(&output,&input,&services)){
  error=self.provider_failure.empty()?"Required original GFNT resolver":self.provider_failure;return false;
 }
 uri=bytes;found=output.found&&(std::strstr(bytes,".fnt")||std::strstr(bytes,".gfnt"));return true;
}
static bool gfnt_read_v1(void* raw,const char* uri,std::vector<std::uint8_t>& out,std::string& error){
 return static_cast<Impl*>(raw)->assets.read(uri,out,error);
}
ui::TextFontBackendsV2 initialize_gfnt_backend_v1(){
 // Shared/main font platforms retain backends() callbacks to this object.
 // Loading another movie must not delete the backend of a retained renderer.
 if(gfnt_text_backend_v1)return gfnt_text_backend_v1->backends();
 const ui::GfntTextServicesV1 services{this,&source_bitmap_cache_width_v1,
  &source_bitmap_cache_height_v1,gfnt_resolve_v1,gfnt_read_v1,nullptr};
 gfnt_text_backend_v1=std::make_unique<ui::GfntTextBackendV1>(services,shared_from_this());
 return gfnt_text_backend_v1->backends();
}
// Use this as the platform's SwfFontServices.read; requires
// hud_freetype_font_v2.hpp. Original FT get_face_entity7d113c delivers a null
// face (and logs) when FT_New_Memory_Face7d1344 leaves face null7d134c..50.
// Preserve actual resolver/resource failures; only real FT format rejection
// is a delivered no-face, enabling SAME core embedded-font continuation.
static bool source_font_read_gfnt_v1(void* raw,const char* name,bool bold,bool italic,
 std::vector<std::uint8_t>& out,std::string& error){
 auto& self=*static_cast<Impl*>(raw);ui::text_v1::Font descriptor;
 descriptor.name=name;descriptor.bold=bold;descriptor.italic=italic;
 std::string uri;bool bitmap{};
 if(!gfnt_resolve_v1(raw,descriptor,uri,bitmap,error))return false;
 if(!bitmap)return font_read(raw,name,bold,italic,out,error);
 std::vector<std::uint8_t> bytes;if(!self.assets.read(uri.c_str(),bytes,error))return false;
 ui::GfntFont gfnt;if(!gfnt.load(bytes.data(),bytes.size(),error))return false;
 ui::HudFreetypeFontV2 face;std::string rejection;
 if(face.load(bytes.data(),bytes.size(),rejection)){out=std::move(bytes);return true;}
 __android_log_print(ANDROID_LOG_WARN,tag,"Original FT delivered no face | GFNT %s | %s",uri.c_str(),rejection.c_str());
 error.clear();return false;
}
