#include "../hud_freetype_font.hpp"
#include "../freetype_bitmap_alpha.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
using namespace dh2::ui;
static void check(bool b,const char* s){if(!b)throw std::runtime_error(s);}
static std::vector<std::uint8_t> bytes(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f),"missing actual font");return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char**argv){try{if(argc!=3)return 2;auto fontin=bytes(argv[1]),wqy=bytes(argv[2]);FreetypeFont old;HudFreetypeFont font;std::string e;check(old.load(fontin.data(),fontin.size(),e)&&font.load(fontin.data(),fontin.size(),e),e.c_str());unsigned oldcases=0,actual=0,packed=0,guards=0;std::map<unsigned,unsigned> modes;
    for(int size:{8,12,16,24,32,48})for(unsigned code=32;code<127;++code){FreetypeGlyph a,b;check(old.raster(a,code,size,1,e)&&font.raster(b,code,size,1,e),e.c_str());check(a.alpha==b.alpha&&a.width==b.width&&a.height==b.height&&a.bitmap_width==b.bitmap_width&&a.bitmap_height==b.bitmap_height&&a.advance==b.advance&&std::memcmp(a.bounds,b.bounds,16)==0,"GRAY regression changed");++oldcases;}
    check(font.load(wqy.data(),wqy.size(),e),e.c_str());FT_Library lib{};FT_Face face{};check(FT_Init_FreeType(&lib)==0&&FT_New_Memory_Face(lib,wqy.data(),wqy.size(),0,&face)==0,"actual reference face failed");
    std::cout<<"{\"PLAYLIST_P_probes\":[";bool first=true;
    for(int size=8;size<=64;++size)for(unsigned code=32;code<127;++code){FreetypeGlyph a;HudBitmapInfo32 info;check(font.raster(a,code,size,1,e,&info),e.c_str());check(FT_Set_Pixel_Sizes(face,0,size)==0&&FT_Load_Char(face,code,FT_LOAD_RENDER)==0,"actual reference raster failed");auto& b=face->glyph->bitmap;check(info.pixel_mode==unsigned(b.pixel_mode)&&info.width==b.width&&info.rows==b.rows&&info.pitch==b.pitch,"bitmap provenance mismatch");++modes[info.pixel_mode];packed+=info.packed_conversion;++actual;
        FT_Bitmap converted{};check(FT_Bitmap_Convert(lib,&b,&converted,1)==0,"genuine FT pixel conversion failed");for(int y=0;y<b.rows;++y)for(int x=0;x<b.width;++x){auto v=converted.buffer[y*converted.pitch+x];if(b.pixel_mode!=FT_PIXEL_MODE_GRAY)v=std::uint8_t(v*(255/(converted.num_grays-1)));check(a.alpha[y*a.width+x]==v,"genuine FT conversion pixel mismatch");}FT_Bitmap_Done(lib,&converted);
        if(code=='P'&&(size==8||size==12||size==16||size==24||size==32)){if(!first)std::cout<<',';first=false;std::cout<<"{\"size\":"<<size<<",\"mode\":"<<info.pixel_mode<<",\"width\":"<<info.width<<",\"rows\":"<<info.rows<<",\"pitch\":"<<info.pitch<<",\"num_grays\":"<<info.num_grays<<"}";}
    }
    FT_Done_Face(face);FT_Done_FreeType(lib);check(packed>0&&modes[1]>0&&modes[2]>0,"actual embedded MONO coverage missing");
    std::uint8_t source[]={0x81,0x7e},output[64];FreetypeBitmap40 in{source,2,8,2,1,1,2,0};FreetypeAlpha32 out{output,sizeof output,123,123,123,0};for(unsigned k=0;k<5;++k){auto bad=in;auto target=out;std::memset(output,0xa5,sizeof output);if(k==0)bad.size=1;if(k==1)bad.pitch=0;if(k==2)bad.pixel_mode=5;if(k==3)target.capacity=1;if(k==4)target.reserved=1;check(dh2_freetype_bitmap_alpha(&target,&bad)==-1&&output[0]==0xa5&&target.width==123,"malformed conversion changed output");++guards;}
    std::cout<<"],\"validation\":\"PASS\",\"gray_regression_cases\":"<<oldcases<<",\"actual_wqy_glyphs\":"<<actual<<",\"packed_wqy_glyphs\":"<<packed<<",\"mode1_glyphs\":"<<modes[1]<<",\"mode2_glyphs\":"<<modes[2]<<",\"atomic_guards\":"<<guards<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
