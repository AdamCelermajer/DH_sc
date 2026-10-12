#include "../sha256.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
using dh2::assets::Sha256Digest;
using dh2::assets::sha256;
namespace {
std::uint64_t checks=0,comparisons=0,alignment_checks=0,overlap_checks=0,rejections=0;
void check(bool ok){++checks;if(!ok)throw std::runtime_error("SHA256 audit failed at check "+std::to_string(checks));}
std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
std::string hex(const Sha256Digest& digest){std::string out;for(auto b:digest){out+="0123456789abcdef"[b>>4];out+="0123456789abcdef"[b&15];}return out;}
class Reader {
 const std::vector<std::uint8_t>& bytes_;std::size_t at_=0;
 public:explicit Reader(const std::vector<std::uint8_t>& b):bytes_(b){}
 const std::uint8_t* take(std::size_t n){check(n<=bytes_.size()-at_);const auto* p=bytes_.data()+at_;at_+=n;return p;}
 unsigned word(){const auto* p=take(4);return unsigned(p[0])|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);}
 std::uint64_t size(){const auto lo=word(),hi=word();return std::uint64_t(lo)|(std::uint64_t(hi)<<32);}
 bool end()const{return at_==bytes_.size();}
};
}
int main(int argc,char** argv){try{
 check(argc==3);const auto gold=read(argv[1]);Reader r(gold);check(!std::memcmp(r.take(4),"SHV1",4));const auto count=r.word();check(count>1000&&count<2000);
 for(unsigned i=0;i<count;++i){const auto label_size=r.word();check(label_size&&label_size<512);r.take(label_size);const auto size=r.size();check(size<=gold.size());Sha256Digest expected{};std::memcpy(expected.data(),r.take(32),32);const auto* input=r.take(std::size_t(size));Sha256Digest out{};check(sha256(input,std::size_t(size),out)&&out==expected);++comparisons;
  for(std::size_t offset:{1u,3u,31u}){std::vector<std::uint8_t> unaligned(std::size_t(size)+offset+1,0xa5);std::memcpy(unaligned.data()+offset,input,std::size_t(size));const auto before=unaligned;check(sha256(unaligned.data()+offset,std::size_t(size),out)&&out==expected&&unaligned==before);++alignment_checks;}
  if(size==32){Sha256Digest alias{};std::memcpy(alias.data(),input,32);check(sha256(alias.data(),alias.size(),alias)&&alias==expected);++overlap_checks;}
 }check(r.end());
 Sha256Digest sentinel{};sentinel.fill(0xa5);auto rejected=[&](const std::uint8_t* p,std::size_t size){auto out=sentinel;check(!sha256(p,size,out)&&out==sentinel);++rejections;};
 rejected(nullptr,1);rejected(nullptr,64);rejected(reinterpret_cast<const std::uint8_t*>(std::uintptr_t(1)),std::numeric_limits<std::size_t>::max());
 rejected(reinterpret_cast<const std::uint8_t*>(std::numeric_limits<std::uintptr_t>::max()-31),64);rejected(reinterpret_cast<const std::uint8_t*>(std::numeric_limits<std::uintptr_t>::max()),1);
 if(sizeof(std::size_t)==8)rejected(reinterpret_cast<const std::uint8_t*>(std::uintptr_t(1)),std::size_t(std::numeric_limits<std::uint64_t>::max()/8+1));
 Sha256Digest empty{};check(sha256(nullptr,0,empty)&&hex(empty)=="e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
 const auto dact=read(argv[2]);Sha256Digest digest{};check(sha256(dact.data(),dact.size(),digest));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"hashlib_fixture_comparisons\":"<<comparisons<<",\"unaligned_readonly_checks\":"<<alignment_checks<<",\"overlapping_output_checks\":"<<overlap_checks<<",\"atomic_span_rejections\":"<<rejections<<",\"null_empty_checks\":1,\"actual_DACT_bytes\":"<<dact.size()<<",\"actual_DACT_sha256\":\""<<hex(digest)<<"\"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
