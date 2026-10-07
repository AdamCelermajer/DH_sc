#include "../swf_movie.hpp"
#include "gameswf/gameswf.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <cmath>
using namespace dh2::ui;
struct Sentinel:gameswf::glyph_provider {
 bool*destroyed;explicit Sentinel(bool*d):destroyed(d){}~Sentinel(){*destroyed=true;}
 gameswf::bitmap_info*get_char_image(gameswf::character_def*,Uint16,const tu_string&,bool,bool,int,gameswf::rect*,float*)override{return nullptr;}
};
struct Test {
 std::string base;std::vector<std::string> exports,native_arguments;std::map<unsigned,unsigned> draw_counts;
 unsigned vertices{},native_calls{},images{},errors{},reject_reentry{};SwfMovie*movie{};
 static bool read(void*c,const char*p,std::vector<std::uint8_t>&b,std::string&e){auto&t=*static_cast<Test*>(c);std::string path=p;if(path.find('/')==std::string::npos)path=t.base+"/"+path;std::ifstream f(path,std::ios::binary);if(!f){e="fixture file missing: "+path;return false;}b.assign(std::istreambuf_iterator<char>(f),{});return true;}
 static bool texture(void*c,const char*n,int w,int h,SwfTexture&o,std::string&){auto&t=*static_cast<Test*>(c);t.exports.emplace_back(n);o.identity=100+t.exports.size();o.width=w?w:1024;o.height=h?h:1024;return true;}
 static bool image(void*c,int w,int h,unsigned,const std::uint8_t*,int,SwfTexture&o,std::string&){auto&t=*static_cast<Test*>(c);o.identity=1000+ ++t.images;o.width=w;o.height=h;return true;}
 static bool draw(void*c,const SwfDraw&d,std::string&){auto&t=*static_cast<Test*>(c);++t.draw_counts[d.kind];t.vertices+=d.xy.size()/2;if(t.movie&&t.reject_reentry==0){std::string e;if(!t.movie->advance(0,e)&&e=="SWF core busy")++t.reject_reentry;}return true;}
 static bool native(void*c,const char*n,const std::vector<SwfValue>&a,SwfValue&o,std::string&e){auto&t=*static_cast<Test*>(c);if(std::string(n)!="NativeGetStringFromSymbol"){e="unowned native function";return false;}++t.native_calls;for(const auto&v:a)t.native_arguments.push_back(v.kind==SwfValue::text?v.string:std::to_string(v.numeric));o.kind=SwfValue::text;o.string=a.empty()?"":a[0].string;return true;}
 static bool stencil(void*,const float*,std::uint8_t,bool&r,std::string&){r=false;return true;}
 static void diagnostic(void*c,bool error,const char*){if(error)++static_cast<Test*>(c)->errors;}
 SwfServices services(){SwfServices s;s.context=this;s.read=read;s.texture=texture;s.image=image;s.draw=draw;s.native_call=native;s.stencil=stencil;s.diagnostic=diagnostic;return s;}
};
int main(int argc,char**argv){if(argc!=2)return 2;Test t;t.base=argv[1];SwfMovie movie;t.movie=&movie;std::string error;
 if(!movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",t.services(),error)){std::cerr<<error<<'\n';{std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}}
 if(!movie.advance(0,error)||!movie.display(0,0,480,320,error)){std::cerr<<error<<'\n';{std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}}
 SwfClipInfo hp,mp,player;const std::string prefix="_root.menu_HUD_0.HUDelements.HealthBars.player";
 if(!movie.clip(prefix.c_str(),player,error)||!movie.clip((prefix+".bar_hp").c_str(),hp,error)||!movie.clip((prefix+".bar_mp").c_str(),mp,error)){std::cerr<<error;{std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}}
 if(hp.id!=90||mp.id!=147||player.id!=157||std::abs(hp.world.value[0]-.949895501f)>1e-7f||std::abs(mp.world.value[2]+742.829041f)>.0001f){std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}
 if(!movie.set_number((prefix+".bar_hp._xscale").c_str(),37.5,error)||!movie.clip((prefix+".bar_hp").c_str(),hp,error)||std::abs(hp.local.value[0]-.375f)>1e-7f){std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}
 if(!movie.advance(1.f/30,error)||!movie.display(0,0,854,480,error)){std::cerr<<error;{std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}}
 if(!t.draw_counts[SwfDraw::triangle_strip]||!t.draw_counts[SwfDraw::mask_begin]||!t.native_calls||!t.reject_reentry){std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}
 const auto prior=t.draw_counts[SwfDraw::triangle_strip];if(!movie.display_clip(prefix.c_str(),0,0,854,480,error)||t.draw_counts[SwfDraw::triangle_strip]<=prior){std::cerr<<error;return 1;}
 bool destroyed=false;auto*sentinel=new Sentinel(&destroyed);gameswf::set_glyph_provider(sentinel);
 if(!movie.advance(0,error)||destroyed||gameswf::get_glyph_provider()!=sentinel){std::cerr<<"Borrowed global provider ownership changed";return 1;}
 gameswf::set_glyph_provider(nullptr);delete sentinel;if(!destroyed)return 1;
 Test missing;missing.base=t.base;auto s=missing.services();s.texture=nullptr;SwfMovie rejected;if(rejected.load({},"dqhud_droid.swf",s,error)||error.find("External SWF texture unavailable")==std::string::npos){std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}
 if(movie.advance(-1,error)||movie.display(0,0,0,1,error)||movie.clip("_root.nonexistent",hp,error)){std::cerr << "audit failure at line " << __LINE__ << "\n";return 1;}
 std::cout<<"{\"validation\":\"PASS\",\"triangle_strips\":"<<t.draw_counts[SwfDraw::triangle_strip]<<",\"line_strips\":"<<t.draw_counts[SwfDraw::line_strip]<<",\"vertices\":"<<t.vertices<<",\"mask_submissions\":"<<t.draw_counts[SwfDraw::mask_begin]<<",\"export_requests\":"<<t.exports.size()<<",\"native_calls\":"<<t.native_calls<<",\"native_arguments\":[";for(unsigned i=0;i<t.native_arguments.size();++i){if(i)std::cout<<',';std::cout<<'"'<<t.native_arguments[i]<<'"';}std::cout<<"],\"core_error_diagnostics\":"<<t.errors<<",\"reentry_rejections\":"<<t.reject_reentry<<",\"borrowed_provider_checks\":2,\"limits\":{\"texture_uploads_fixture\":true,\"localization_fixture\":true,\"full_actions_parity\":false,\"font_provider_installed\":false}}\n";
}



