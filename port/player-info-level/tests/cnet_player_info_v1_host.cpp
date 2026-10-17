#include "../cnet_player_info_v1.hpp"
#include <cstdio>
#include <cstring>
#include <string>
namespace n=dh2::netstruct_members_v1;
namespace c=dh2::cnet_player_info_v1;
constexpr unsigned offsets[]={0x130,0x158,0x180,0x1a8,0x1d0,0x1f8,0x220,0x248};
constexpr unsigned bases[]={0x10001000,0x10002000};
static unsigned word(const unsigned char* p,unsigned offset) {unsigned v;std::memcpy(&v,p+offset,4);return v;}
static bool read_text(std::FILE* f,std::string& text) {
    unsigned size;if(std::fread(&size,4,1,f)!=1 || size>4096) return false;
    text.resize(size);return std::fread(text.data(),1,size,f)==size;
}
static void seed(c::Record& r,const unsigned char* data,bool ready,std::uint64_t* serial,const std::string& text) {
    r.constructed=ready;r.serial=serial;r.field_280=word(data,0x280);
    auto& net=r.network;net.constructed=ready;net.source_vtable=word(data,0);net.count=word(data,0x104);
    net.enabled_108=data[0x108];net.field_124=data[0x124];net.field_125=data[0x125];net.field_128=word(data,0x128);
    for(unsigned i=0;i<8;++i) {
        auto& m=r.members[i];std::memcpy(&m.header,data+offsets[i],i==7?32:36);
        m.constructed=ready;m.serial=serial;
        m.kind=i<4?n::Kind::integer:i<7?n::Kind::unsigned_integer:n::Kind::string32;
    }
    r.members[7].text=text;
}
static void pointers(c::Record& r,const unsigned char* data,c::Record& a,c::Record& b) {
    for(unsigned i=0;i<64;++i) {
        const auto v=word(data,4+i*4);auto* m=reinterpret_cast<n::Member*>(static_cast<std::uintptr_t>(v));
        for(unsigned j=0;j<8;++j) {
            if(v==bases[0]+offsets[j]) m=&a.members[j];
            if(v==bases[1]+offsets[j]) m=&b.members[j];
        }
        r.network.members[i]=m;
    }
}
static void project(std::FILE* out,c::Record& r,c::Record& a,c::Record& b) {
    auto& net=r.network;
    unsigned words[]={net.source_vtable,net.count,net.enabled_108,net.field_124,net.field_125,net.field_128,r.field_280};
    std::fwrite(words,sizeof(words),1,out);
    for(auto* m:net.members) {
        auto v=static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(m));
        for(unsigned j=0;j<8;++j) {
            if(m==&a.members[j]) v=bases[0]+offsets[j];
            if(m==&b.members[j]) v=bases[1]+offsets[j];
        }
        std::fwrite(&v,4,1,out);
    }
    for(unsigned i=0;i<8;++i) std::fwrite(&r.members[i].header,i==7?32:36,1,out);
    const auto& text=r.members[7].text;auto size=static_cast<unsigned>(text.size());
    std::fwrite(&size,4,1,out);std::fwrite(text.data(),1,size,out);
}
int main(int argc,char** argv) {
    if(argc!=3) return 2;
    auto* in=std::fopen(argv[1],"rb");auto* out=std::fopen(argv[2],"wb");
    if(!in||!out) return 3;
    unsigned count;if(std::fread(&count,4,1,in)!=1) return 4;
    for(unsigned i=0;i<count;++i) {
        unsigned op;std::uint64_t serial;unsigned char source[0x284],dest[0x284];std::string sa,sb;
        if(std::fread(&op,4,1,in)!=1 || std::fread(&serial,8,1,in)!=1 ||
           std::fread(source,sizeof(source),1,in)!=1 || std::fread(dest,sizeof(dest),1,in)!=1 || !read_text(in,sa) || !read_text(in,sb)) return 5;
        c::Record a,b;seed(a,source,true,&serial,sa);seed(b,dest,op==2||op>=5,&serial,sb);
        pointers(a,source,a,b);pointers(b,dest,a,b);n::Status status;
        switch(op) {
        case 0:status=c::construct(b,&serial);break;
        case 1:status=c::construct_c2(b,&serial);break;
        case 2:status=c::reset(b);break;
        case 3:case 4:status=c::copy_construct(b,a);break;
        case 5:status=c::assign(b,a);break;
        case 6:status=c::assign(b,b);break;
        case 7:case 8:status=c::destroy(b);break;
        default:return 6;
        }
        if(status!=n::Status::complete) return 7;
        std::fwrite(&serial,8,1,out);project(out,a,a,b);project(out,b,a,b);
        // Source pointer-table copies deliberately remain borrowed aliases.
        if((op==3||op==4||op==5) && b.network.members[0]!=a.network.members[0]) return 8;
    }
    std::fclose(in);std::fclose(out);return 0;
}
