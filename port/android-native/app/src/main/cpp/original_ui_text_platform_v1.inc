// Include inside OriginalUiSession::Impl. Same actual FontResolve services;
// TTF selection is a delivered bitmap-font miss, not an empty fixture.
bool source_bitmap_face_v1(const ui::text_v1::Font& font,
 ui::TextBitmapFaceV2& out,std::string& error){
 char uri[4096]{};
 const ui::FontResolveInput24 input{font.name.c_str(),"",font.bold?1u:0u,font.italic?1u:0u};
 ui::FontResolveOutput32 resolved{uri,sizeof(uri),0,0,0};
 const ui::FontResolveServices16 services{this,font_service};
 const int status=dh2_swf_font_resolve(&resolved,&input,&services);
 if(status){error=provider_failure.empty()?"Required original bitmap font resolution":provider_failure;return false;}
 if(std::strstr(uri,".fnt")){
  error=std::string("Required original GFNT font/image connection: ")+uri;return false;
 }
 // The source resolver has actually selected a non-bitmap face, including a
 // delivered missing TTF. The existing font_read remains its next producer.
 out={};return true;
}
ui::TextFontBackendsV2 source_text_backends_v1(){
 ui::TextFontBackendsV2 backends;
 backends.bitmap_face=[this](const auto& font,auto& out,std::string& error){
  return source_bitmap_face_v1(font,out,error);
 };
 return backends; // platform supplies real embedded core font tables itself
}
