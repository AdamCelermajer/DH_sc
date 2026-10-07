#include "inventory_gathering_ids_v11.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
using namespace dh2::data;
int main(int argc,char** argv){try{if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> b{std::istreambuf_iterator<char>(f),{}};std::size_t pos=4;unsigned checks{};auto check=[&](bool v){++checks;if(!v)throw std::runtime_error("check "+std::to_string(checks));};check(b.size()>=8&&!std::memcmp(b.data(),"IG11",4));auto word=[&](){check(b.size()-pos>=4);std::uint32_t value=b[pos]|std::uint32_t(b[pos+1])<<8|std::uint32_t(b[pos+2])<<16|std::uint32_t(b[pos+3])<<24;pos+=4;return value;};auto n=word();InventoryGatheringIdsV11 owner;unsigned assertions{};InventoryGatheringAssertServicesV11 s{&assertions,[](void* p,std::int32_t,std::string&){++*static_cast<unsigned*>(p);return true;}};std::string e;check(owner.entries().empty());
 for(unsigned i=0;i<n;++i){auto op=word();auto id=static_cast<std::int32_t>(word());auto enabled=word();auto count=word();if(enabled){if(!op)owner.register_id(id);else check(owner.unregister_id(id,s,e));}check(owner.entries().size()==count);auto at=owner.entries().begin();for(unsigned j=0;j<count;++j,++at){auto expected=static_cast<std::int32_t>(word());auto refs=word();check(at->id==expected&&at->references==refs&&owner.contains(expected));}}
 check(pos==b.size()&&assertions>0);check(!owner.unregister_id(999,{},e)&&!e.empty());
 std::cout<<"PASS original gathering ordered list/register/unregister/byte wrap differential cases="<<n<<" checks="<<checks<<"\n";return 0;
 }catch(const std::exception& ex){std::cerr<<ex.what();return 1;}}
