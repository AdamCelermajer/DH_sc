#pragma once
#include "../swf_movie.hpp"
#include "gameswf/gameswf.h"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
using namespace dh2::ui;
struct Sentinel:gameswf::glyph_provider {
 bool*destroyed;explicit Sentinel(bool*d):destroyed(d){}~Sentinel(){*destroyed=true;}
 gameswf::bitmap_info*get_char_image(gameswf::character_def*,Uint16,const tu_string&,bool,bool,int,gameswf::rect*,float*)override{return nullptr;}
};
struct Test {
 std::string base;std::vector<std::string> exports,native_arguments;std::map<unsigned,unsigned> draw_counts;
 unsigned vertices{},native_calls{},images{},errors{},reject_reentry{};SwfMovie*movie{};void*graph_context{};
 static bool read(void*c,const char*p,std::vector<std::uint8_t>&b,std::string&e){auto&t=*static_cast<Test*>(c);std::string path=p;if(path.find('/')==std::string::npos)path=t.base+"/"+path;std::ifstream f(path,std::ios::binary);if(!f){e="fixture file missing: "+path;return false;}b.assign(std::istreambuf_iterator<char>(f),{});return true;}
 static bool texture(void*c,const char*n,int w,int h,SwfTexture&o,std::string&){auto&t=*static_cast<Test*>(c);t.exports.emplace_back(n);o.identity=100+t.exports.size();o.width=w?w:1024;o.height=h?h:1024;return true;}
 static bool image(void*c,int w,int h,unsigned,const std::uint8_t*,int,SwfTexture&o,std::string&){auto&t=*static_cast<Test*>(c);o.identity=1000+ ++t.images;o.width=w;o.height=h;return true;}
 static bool draw(void*c,const SwfDraw&d,std::string&){auto&t=*static_cast<Test*>(c);++t.draw_counts[d.kind];t.vertices+=d.xy.size()/2;if(t.movie&&t.reject_reentry==0){std::string e;if(!t.movie->advance(0,e)&&e=="SWF core busy")++t.reject_reentry;}return true;}
 static bool native(void*c,const char*n,const std::vector<SwfValue>&a,SwfValue&o,std::string&e){auto&t=*static_cast<Test*>(c);if(std::string(n)!="NativeGetStringFromSymbol"){e="unowned native function";return false;}++t.native_calls;for(const auto&v:a)t.native_arguments.push_back(v.kind==SwfValue::text?v.string:std::to_string(v.numeric));o.kind=SwfValue::text;o.string=a.empty()?"":a[0].string;return true;}
 static bool stencil(void*,const float*,std::uint8_t,bool&r,std::string&){r=false;return true;}
 static void diagnostic(void*c,bool error,const char*){if(error)++static_cast<Test*>(c)->errors;}
 SwfServices services(){SwfServices s;s.context=this;s.read=read;s.texture=texture;s.image=image;s.draw=draw;s.native_call=native;s.stencil=stencil;s.diagnostic=diagnostic;return s;}
};
