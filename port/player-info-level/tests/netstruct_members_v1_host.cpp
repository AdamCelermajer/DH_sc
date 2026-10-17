#include "../netstruct_members_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <array>
#include <map>
#include <vector>
namespace n=dh2::netstruct_members_v1;
struct Memory {
    std::map<void*,unsigned> ids;unsigned next=1;std::vector<std::array<unsigned,4>> trace;
    bool fail_allocate=false,fail_release=false;n::Member* reenter=nullptr;bool reentry_rejected=false;
};
static void* allocate(void* context,std::size_t size,int mode) {
    auto& m=*static_cast<Memory*>(context);
    if(m.reenter) m.reentry_rejected=n::set_bytes(*m.reenter,{})==n::Status::invalid_state;
    if(m.fail_allocate) return nullptr;
    auto* p=std::malloc(size?size:1);if(!p) throw 1;
    m.ids[p]=m.next++;m.trace.push_back({1,static_cast<unsigned>(size),static_cast<unsigned>(mode),m.ids[p]});return p;
}
static void release(void* context,void* p) {
    auto& m=*static_cast<Memory*>(context);
    if(m.fail_release) throw 1;
    m.trace.push_back({2,m.ids.at(p),0,0});m.ids.erase(p);std::free(p);
}
static bool read_bytes(std::FILE* f,std::vector<unsigned char>& data) {
    unsigned size;if(std::fread(&size,4,1,f)!=1 || size>4096) return false;
    data.resize(size);return std::fread(data.data(),1,size,f)==size;
}
static bool policies() {
    std::uint64_t serial=91;const unsigned char a[]={1,2},b[]={3,4,5};
    {
        n::Member m;
        if(n::construct_scalar(m,n::Kind::integer,16,1,nullptr)!=n::Status::invalid_argument || m.constructed) return false;
    }
    {
        Memory memory;n::Member m;memory.fail_allocate=true;
        if(n::construct_bytes(m,3,{a,2},&serial,{&memory,allocate,release})!=n::Status::allocation_failed || m.constructed || serial!=91) return false;
    }
    {
        Memory memory;n::Member m;
        if(n::construct_bytes(m,3,{a,2},&serial,{&memory,allocate,release})!=n::Status::complete) return false;
        const auto before=serial;memory.fail_allocate=true;
        if(n::set_bytes(m,{b,3})!=n::Status::allocation_failed || m.bytes.data || m.bytes.size!=3 || serial!=before) return false;
        if(n::set_bytes(m,{b,3})!=n::Status::invalid_state || serial!=before) return false;
        if(n::destroy(m)!=n::Status::complete || !memory.ids.empty()) return false;
    }
    {
        Memory memory;n::Member m;
        if(n::construct_bytes(m,3,{a,2},&serial,{&memory,allocate,release})!=n::Status::complete) return false;
        auto* pointer=m.bytes.data;const auto before=serial;memory.fail_release=true;
        if(n::set_bytes(m,{b,3})!=n::Status::provider_failed || m.bytes.data!=pointer || m.bytes.size!=2 || serial!=before || std::memcmp(pointer,a,2)) return false;
        memory.fail_release=false;memory.reenter=&m;
        if(n::set_bytes(m,{b,3})!=n::Status::complete || !memory.reentry_rejected || serial!=before+1 || std::memcmp(m.bytes.data,b,3)) return false;
        if(n::destroy(m)!=n::Status::complete || !memory.ids.empty()) return false;
    }
    return true;
}
int main(int argc,char** argv) {
    if(argc!=3) return 2;
    if(!policies()) return 10;
    auto* in=std::fopen(argv[1],"rb");auto* out=std::fopen(argv[2],"wb");if(!in||!out) return 3;
    unsigned count;if(std::fread(&count,4,1,in)!=1) return 4;
    for(unsigned i=0;i<count;++i) {
        unsigned op,width,value;std::uint64_t serial;Memory memory;n::Member m;std::vector<unsigned char> old,bytes;
        if(std::fread(&op,4,1,in)!=1 || std::fread(&width,4,1,in)!=1 || std::fread(&value,4,1,in)!=1 ||
           std::fread(&serial,8,1,in)!=1 || std::fread(&m.header,36,1,in)!=1 || !read_bytes(in,old) || !read_bytes(in,bytes)) return 5;
        m.kind=op<3?n::Kind::byte_array:op==3?n::Kind::integer:op==4?n::Kind::unsigned_integer:n::Kind::boolean;
        m.serial=&serial;m.constructed=op!=0;m.memory={&memory,allocate,release};
        if(!old.empty()) {
            m.bytes.data=static_cast<unsigned char*>(std::malloc(old.size()));m.bytes.size=static_cast<int>(old.size());
            if(!m.bytes.data) return 6;
            std::memcpy(m.bytes.data,old.data(),old.size());memory.ids[m.bytes.data]=0;
        }
        n::Status status;const n::BytesView view{bytes.data(),static_cast<int>(bytes.size())};
        switch(op) {
        case 0:status=n::construct_bytes(m,width,view,&serial,m.memory);break;
        case 1:status=n::set_bytes(m,view);break;
        case 2:status=n::set_buffer(m,view);break;
        case 3:{std::int32_t v;std::memcpy(&v,&value,4);status=n::set_int(m,v);break;}
        case 4:status=n::set_uint(m,value);break;
        case 5:status=n::set_bool(m,static_cast<unsigned char>(value));break;
        default:return 7;
        }
        if(status!=n::Status::complete) return 8;
        std::fwrite(&serial,8,1,out);std::fwrite(&m.header,32,1,out);
        if(op<3) {
            unsigned info[]={static_cast<unsigned>(m.bytes.size),m.bytes.data?1u:0u};std::fwrite(info,sizeof(info),1,out);
            if(m.bytes.data && m.bytes.size>0) std::fwrite(m.bytes.data,1,m.bytes.size,out);
        } else std::fwrite(&m.header.value,4,1,out);
        auto traces=static_cast<unsigned>(memory.trace.size());std::fwrite(&traces,4,1,out);
        for(const auto& t:memory.trace) std::fwrite(t.data(),16,1,out);
        // Capture precedes retirement; the fixture context outlives the member.
        n::destroy(m);if(!memory.ids.empty()) return 9;
    }
    std::fclose(in);std::fclose(out);std::puts("6");return 0;
}
