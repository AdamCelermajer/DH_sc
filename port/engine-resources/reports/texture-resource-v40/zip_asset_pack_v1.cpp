#include "zip_asset_pack_v1.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <map>
#include <stdexcept>
#include <zlib.h>

namespace dh2::assets {
namespace {
std::uint16_t u16(const unsigned char* p){return std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);}
std::uint32_t u32(const unsigned char* p){return std::uint32_t(u16(p))|(std::uint32_t(u16(p+2))<<16);}
bool fail(std::string& e,const char* text){e=std::string("Original cache ZIP: ")+text;return false;}
constexpr std::uint32_t max_file=256u*1024u*1024u;
constexpr std::uint32_t max_directory=32u*1024u*1024u;
bool safe_name(const std::string& name){
    if(name.empty()||name.size()>4096||name.front()=='/'||name.front()=='\\')return false;
    std::size_t start=0;
    for(std::size_t i=0;i<=name.size();++i){
        if(i<name.size()&&(name[i]=='\0'||name[i]==':'||static_cast<unsigned char>(name[i])<32))return false;
        if(i==name.size()||name[i]=='/'||name[i]=='\\'){
            const auto part=name.substr(start,i-start);
            if(part==".."||part.empty())return false;
            start=i+1;
        }
    }
    return true;
}
}
struct ZipAssetPackV1::Impl {
    struct Item {ZipEntryV1 public_entry;std::string original;std::uint32_t local{};std::uint16_t method{},flags{};};
    ZipBackingV1 backing;
    std::map<std::string,Item> index;
    std::uint32_t directory{};
    bool read_at(std::uint64_t at,void* out,std::size_t n,std::string& e) const{
        if(at>backing.bytes||n>backing.bytes-at)return fail(e,"read outside archive");
        return backing.read(at,out,n,e);
    }
};
bool ZipAssetPackV1::key(const std::string& input,std::string& out,std::string& e){
    std::string next=input;
    std::replace(next.begin(),next.end(),'\\','/');
    while(next.rfind("./",0)==0)next.erase(0,2);
    if(!safe_name(next))return fail(e,"invalid resource URI");
    for(auto& c:next)if(c>='A'&&c<='Z')c=char(c-'A'+'a');
    out=std::move(next);e.clear();return true;
}
bool ZipAssetPackV1::mount(ZipBackingV1 backing,const std::string& prefix,std::string& e){
    try {
        if(!backing.owner||!backing.read||backing.bytes<22||backing.bytes>0xffffffffull)
            return fail(e,"required bounded owned archive unavailable");
        std::string normalized_prefix;
        if(prefix.empty()||prefix.back()!='/'||!key(prefix.substr(0,prefix.size()-1),normalized_prefix,e))
            return fail(e,"required resource root unavailable");
        normalized_prefix+='/';
        auto next=std::make_shared<Impl>();next->backing=std::move(backing);
        const auto tail_size=static_cast<std::size_t>(std::min<std::uint64_t>(next->backing.bytes,65557));
        std::vector<unsigned char> tail(tail_size);
        const auto tail_start=next->backing.bytes-tail_size;
        if(!next->read_at(tail_start,tail.data(),tail.size(),e))return false;
        std::size_t end=tail.size();
        for(std::size_t i=tail.size()-22+1;i-->0;){
            if(u32(tail.data()+i)==0x06054b50&&i+22+u16(tail.data()+i+20)==tail.size()){end=i;break;}
        }
        if(end==tail.size())return fail(e,"end directory missing");
        const auto* footer=tail.data()+end;
        const auto count=u16(footer+10);
        const auto directory_bytes=u32(footer+12),directory=u32(footer+16);
        if(u16(footer+4)||u16(footer+6)||u16(footer+8)!=count||count==65535||
           !count||directory_bytes>max_directory||directory==0xffffffffu||
           std::uint64_t(directory)+directory_bytes!=tail_start+end)
            return fail(e,"unsupported or malformed directory");
        next->directory=directory;
        std::vector<unsigned char> central(directory_bytes);
        if(!next->read_at(directory,central.data(),central.size(),e))return false;
        std::size_t at=0;
        for(unsigned i=0;i<count;++i){
            if(at>central.size()||central.size()-at<46||u32(central.data()+at)!=0x02014b50)
                return fail(e,"central record missing");
            const auto* p=central.data()+at;
            const auto name_bytes=u16(p+28),extra=u16(p+30),comment=u16(p+32);
            const std::size_t total=46u+name_bytes+extra+comment;
            if(total>central.size()-at||!name_bytes)return fail(e,"central record truncated");
            const auto flags=u16(p+8),method=u16(p+10);
            const auto compressed=u32(p+20),bytes=u32(p+24),local=u32(p+42);
            if((flags&~std::uint16_t(0x0808))||(method!=0&&method!=8)||u16(p+34)||
               compressed==0xffffffffu||bytes==0xffffffffu||local==0xffffffffu||
               bytes>max_file||compressed>max_file||std::uint64_t(local)+30>directory||
               (method==0&&compressed!=bytes))return fail(e,"unsupported or oversized entry");
            const std::string original(reinterpret_cast<const char*>(p+46),name_bytes);
            std::string archive_key;
            const bool is_directory=original.back()=='/';
            if(!key(is_directory?original.substr(0,original.size()-1):original,archive_key,e))return false;
            if(!is_directory){
                if(archive_key.rfind(normalized_prefix,0)!=0)return fail(e,"file outside resource root");
                const auto logical=archive_key.substr(normalized_prefix.size());
                if(logical.empty())return fail(e,"empty resource name");
                Impl::Item entry{{logical,compressed,bytes,u32(p+16)},original,local,method,flags};
                if(!next->index.emplace(logical,std::move(entry)).second)return fail(e,"duplicate resource URI");
            }
            at+=total;
        }
        if(at!=central.size()||next->index.empty())return fail(e,"directory suffix or empty resource root");
        impl_=std::move(next);e.clear();return true;
    }catch(const std::exception& x){e=std::string("Original cache ZIP: ")+x.what();return false;}
}
bool ZipAssetPackV1::read(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,std::string& e) const{
    return read_admitted(uri,found,out,{},e);
}
bool ZipAssetPackV1::read_admitted(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,
    const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string& e) const{
    try {
        auto owner=impl_;
        if(!owner)return fail(e,"filesystem not mounted");
        std::string logical;if(!key(uri,logical,e))return false;
        const auto it=owner->index.find(logical);
        if(it==owner->index.end()){found=false;e.clear();return true;}
        const auto& entry=it->second;
        unsigned char local[30];
        if(!owner->read_at(entry.local,local,sizeof(local),e))return false;
        const auto& metadata=entry.public_entry;
        const auto name_bytes=u16(local+26),extra=u16(local+28);
        const std::uint64_t payload=std::uint64_t(entry.local)+30+name_bytes+extra;
        if(u32(local)!=0x04034b50||u16(local+6)!=entry.flags||u16(local+8)!=entry.method||
           name_bytes!=entry.original.size()||payload>owner->directory||metadata.compressed>owner->directory-payload)
            return fail(e,"local entry/header mismatch");
        if(!(entry.flags&8)&&(u32(local+14)!=metadata.crc||u32(local+18)!=metadata.compressed||u32(local+22)!=metadata.bytes))
            return fail(e,"local entry metadata mismatch");
        std::string original(name_bytes,0);
        if(!owner->read_at(std::uint64_t(entry.local)+30,original.data(),original.size(),e))return false;
        if(original!=entry.original)return fail(e,"local entry name mismatch");
        if(before_payload&&!before_payload(metadata.bytes,e))return false;
        std::vector<std::uint8_t> candidate(metadata.bytes);
        if(entry.method==0){if(!owner->read_at(payload,candidate.data(),candidate.size(),e))return false;}
        else {
            z_stream stream{};
            if(inflateInit2(&stream,-MAX_WBITS)!=Z_OK)return fail(e,"deflate initialization failed");
            struct End {z_stream* s;~End(){inflateEnd(s);}} cleanup{&stream};
            // Only the requested output is allocated. Compressed resources are
            // streamed in bounded chunks directly from their APK descriptor.
            unsigned char chunk[65536],empty_output{};
            std::uint64_t consumed=0;
            stream.next_out=candidate.empty()?&empty_output:candidate.data();
            stream.avail_out=candidate.empty()?1:static_cast<uInt>(candidate.size());
            int status=Z_OK;
            while(status==Z_OK){
                if(!stream.avail_in&&consumed<metadata.compressed){
                    const auto n=static_cast<std::size_t>(std::min<std::uint64_t>(sizeof(chunk),metadata.compressed-consumed));
                    if(!owner->read_at(payload+consumed,chunk,n,e))return false;
                    consumed+=n;stream.next_in=chunk;stream.avail_in=static_cast<uInt>(n);
                }
                const auto before_in=stream.total_in,before_out=stream.total_out;
                status=inflate(&stream,Z_NO_FLUSH);
                if(status==Z_OK&&(stream.total_in==before_in&&stream.total_out==before_out))return fail(e,"deflate made no progress");
            }
            if(status!=Z_STREAM_END||stream.total_in!=metadata.compressed||stream.total_out!=metadata.bytes)
                return fail(e,"deflate length or stream mismatch");
        }
        const auto checksum=crc32(0,candidate.data(),static_cast<uInt>(candidate.size()));
        if(checksum!=metadata.crc)return fail(e,"resource CRC mismatch");
        out=std::move(candidate);found=true;e.clear();return true;
    }catch(const std::exception& x){e=std::string("Original cache ZIP: ")+x.what();return false;}
}
std::vector<ZipEntryV1> ZipAssetPackV1::entries() const{
    std::vector<ZipEntryV1> result;auto owner=impl_;if(owner){result.reserve(owner->index.size());for(const auto& item:owner->index)result.push_back(item.second.public_entry);}return result;
}
}
