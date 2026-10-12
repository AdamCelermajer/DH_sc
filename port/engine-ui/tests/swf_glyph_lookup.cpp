#include "../swf_glyph_lookup.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
static void check(bool b,const char* m){if(!b)throw std::runtime_error(m);}
template<class T>static T read(std::ifstream& f){T v{};check(bool(f.read(reinterpret_cast<char*>(&v),sizeof v)),"truncated gold");return v;}
struct Context {std::uint32_t cfg[5]{};std::vector<unsigned> calls;unsigned fail{},nest{},nested{};bool inside{};
    static int invoke(void* p,GlyphLookupRequest48* r){auto& c=*static_cast<Context*>(p);check(r&&r->reserved==0&&r->reserved_tail==0,"malformed request");auto k=std::uint32_t(r->kind);c.calls.push_back(k);
        if(k==1)r->identity=c.cfg[0]&2?0x1111:0;
        else if(k==2||k==3){const std::uint32_t bounds[]={0x80000000,0x3f000000,0xbf800000,0x7fc01234};std::memcpy(r->glyph->bounds,bounds,16);std::memcpy(&r->glyph->advance,c.cfg+4,4);r->identity=c.cfg[0]&(k==2?4:16)?(k==2?0x2222:0x3333):0;}
        else if(k==4)r->identity=c.cfg[0]&32?0x4444:0;
        else{r->found=c.cfg[1]!=0;r->index=r->found?2:-1;}
        if(c.nest&&!c.inside&&k==1){c.inside=true;GlyphLookup48 inner{};GlyphLookupServices16 services{p,invoke};check(dh2_swf_glyph_lookup(&inner,r->font,&services)==1,"nested lookup failed");++c.nested;c.inside=false;}
        return c.fail==k?-3:0;
    }
};
static std::vector<std::uint32_t> words(const GlyphLookup48& g){std::vector<std::uint32_t> v(9);std::memcpy(v.data(),&g.advance,4);std::memcpy(v.data()+1,g.bounds,16);v[5]=std::uint32_t(g.index);v[6]=g.bitmap;v[7]=g.face;v[8]=g.bitmap_flag;return v;}
int main(int argc,char**argv){try{if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);check(bool(f),"missing original gold");check(read<std::uint32_t>(f)==0x31594c47,"wrong gold magic");auto n=read<std::uint32_t>(f);check(n==6912,"wrong gold count");unsigned services=0,guards=0,prefixes=0;
    Context c;GlyphLookupServices16 s{&c,Context::invoke};float advances[4]{};GlyphLookupFont48 font{"Fontin SmallCaps",65,16,0,1,0,0,0,0,advances};
    for(unsigned i=0;i<n;++i){for(auto& a:c.cfg)a=read<std::uint32_t>(f);auto rc=read<std::uint32_t>(f);std::vector<std::uint32_t> expected(9);for(auto& a:expected)a=read<std::uint32_t>(f);auto nc=read<std::uint32_t>(f);std::vector<unsigned> calls;for(unsigned j=0;j<nc;++j)calls.push_back(read<std::uint32_t>(f));font.bitmap_provider=bool(c.cfg[0]&1);font.freetype_provider=bool(c.cfg[0]&8);font.zone_count=c.cfg[2];font.advance_count=c.cfg[3];std::memcpy(advances+2,c.cfg+4,4);
        GlyphLookup48 g;std::memset(&g,0xa5,sizeof g);g.bitmap=0x5555;g.face=0x6666;g.bitmap_flag=0xa5;g.reserved=0;c.calls.clear();check(dh2_swf_glyph_lookup(&g,&font,&s)==int(rc)&&c.calls==calls,"source return/callback mismatch");auto actual=words(g);for(unsigned j=0;j<9;++j){if(j==0&&(expected[j]&0x7fffffff)>0x7f800000&&(actual[j]&0x7fffffff)>0x7f800000&&font.zone_count&&rc&&!(expected[5]!=0xffffffff&&font.advance_count))continue;check(actual[j]==expected[j],"source glyph word mismatch");}services+=nc;
    }
    check(f.peek()==std::char_traits<char>::eof(),"trailing corpus");c.cfg[0]=1|2|8;c.cfg[1]=1;c.cfg[2]=0;c.cfg[3]=4;c.cfg[4]=0x3f800000;font.bitmap_provider=font.freetype_provider=1;font.zone_count=0;font.advance_count=4;std::memcpy(advances+2,c.cfg+4,4);
    for(unsigned k=1;k<=5;++k){GlyphLookup48 g{};c.calls.clear();c.fail=k;check(dh2_swf_glyph_lookup(&g,&font,&s)==-2&&c.calls.back()==k,"failure prefix lost");++prefixes;}c.fail=0;
    for(unsigned k=0;k<6;++k){auto bad=font;GlyphLookup48 g{};auto saved=g;auto cb=s;if(k==0)bad.code=65536;if(k==1)bad.bold=2;if(k==2)bad.name=nullptr;if(k==3)bad.advances=nullptr;if(k==4)g.reserved=saved.reserved=1;if(k==5)cb.invoke=nullptr;c.calls.clear();check(dh2_swf_glyph_lookup(&g,&bad,&cb)==-1&&c.calls.empty()&&std::memcmp(&g,&saved,sizeof g)==0,"malformed prefix mutated");++guards;}
    GlyphLookup48 g{};c.calls.clear();c.nest=1;check(dh2_swf_glyph_lookup(&g,&font,&s)==1&&c.nested==1&&c.calls.size()==10,"nested lookup lost outer state");
    std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"ordered_services\":"<<services<<",\"failure_prefixes\":"<<prefixes<<",\"atomic_guards\":"<<guards<<",\"nested_calls\":"<<c.nested<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
