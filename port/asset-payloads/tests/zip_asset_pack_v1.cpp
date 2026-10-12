#include "../zip_asset_pack_v1.hpp"
#include "../sha256.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::assets;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
struct File {
    int fd;std::uint64_t bytes;
    explicit File(const char* path):fd(open(path,O_RDONLY)),bytes(0){require(fd>=0,"Open input ZIP");struct stat st{};require(!fstat(fd,&st)&&st.st_size>=0,"ZIP stat");bytes=st.st_size;}
    ~File(){close(fd);}
};
static ZipBackingV1 source(const char* path){
    auto owner=std::make_shared<File>(path);ZipBackingV1 b;b.owner=owner;b.bytes=owner->bytes;
    b.read=[owner](std::uint64_t at,void* out,std::size_t n,std::string& e){auto*p=static_cast<unsigned char*>(out);std::size_t done=0;while(done<n){auto v=pread(owner->fd,p+done,n-done,static_cast<off_t>(at+done));if(v<0&&errno==EINTR)continue;if(v<=0){e="Short test backing read";return false;}done+=v;}e.clear();return true;};return b;
}
static std::string hex(const std::vector<std::uint8_t>& b){Sha256Digest d{};require(sha256(b.data(),b.size(),d),"Native SHA256");std::string h;for(auto x:d){char t[3];std::snprintf(t,sizeof(t),"%02x",x);h+=t;}return h;}
int main(int argc,char** argv){try{
    require(argc==4,"zip_audit archive expected.tsv guards.tsv");std::string e;ZipAssetPackV1 pack;
    require(pack.mount(source(argv[1]),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);
    std::ifstream expected(argv[2]);require(bool(expected),"Expected manifest");std::string row;std::size_t cases=0;std::uint64_t bytes=0;
    while(std::getline(expected,row)){auto tab=row.find('\t'),second=row.find('\t',tab+1);require(tab!=row.npos&&second!=row.npos,"Expected manifest row");auto uri=row.substr(0,tab);auto n=std::stoull(row.substr(tab+1,second-tab-1));bool found=false;std::vector<std::uint8_t> out{17};require(pack.read(uri,found,out,e)&&found,e);require(out.size()==n&&hex(out)==row.substr(second+1),"Complete resource SHA/length mismatch: "+uri);bytes+=n;++cases;}
    require(pack.entries().size()==cases&&cases==6833,"Complete ZIP directory coverage");unsigned guards=0;
    for(const auto* uri:{"../data/a","/data/a","C:\\data\\a","data//a","data/../a"}){bool found=true;std::vector<std::uint8_t> out{19};require(!pack.read(uri,found,out,e)&&out==std::vector<std::uint8_t>{19}&&found,"Unsafe URI mutated output");++guards;}
    bool found=true;std::vector<std::uint8_t> out{19};require(pack.read("not-an-authored-resource",found,out,e)&&!found&&out==std::vector<std::uint8_t>{19},"Genuine missing resource");++guards;
    std::ifstream fixtures(argv[3]);require(bool(fixtures),"Guard manifest");while(std::getline(fixtures,row)){
        auto tab=row.find('\t');require(tab!=row.npos,"Guard manifest row");auto mode=row.substr(0,tab),path=row.substr(tab+1);ZipAssetPackV1 candidate;auto backing=source(path.c_str());
        if(mode=="mount_fail")require(!candidate.mount(backing,"root/",e),"Malformed ZIP accepted: "+path);
        else {
            require(candidate.mount(backing,"root/",e),e);bool hit=false;std::vector<std::uint8_t> b{23};const bool ok=candidate.read("data/a",hit,b,e);
            if(mode=="read_fail")require(!ok&&b==std::vector<std::uint8_t>{23}&&!hit,"Corrupt ZIP resource accepted: "+path);
            else require(mode=="read_pass"&&ok&&hit&&std::string(b.begin(),b.end())=="native cache payload","Valid fixture read: "+path);
        }
        ++guards;
    }
    // Failed replacement preserves the mounted authoritative directory.
    ZipBackingV1 missing;require(!pack.mount(missing,"root/",e)&&pack.entries().size()==cases,"Failed mount replaced cache");++guards;
    std::string key;require(ZipAssetPackV1::key(".\\DATA\\MENUS\\LOADANIMS_I9000.SWF",key,e)&&key=="data/menus/loadanims_i9000.swf","Original ASCII resource identity");++guards;
    std::cout<<"{\"validation\":\"PASS\",\"files\":"<<cases<<",\"uncompressed_bytes\":"<<bytes<<",\"guards\":"<<guards<<",\"mismatches\":0}"<<std::endl;return 0;
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
