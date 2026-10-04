#include "../item_presentation_v5.hpp"
#include <cstring>
using namespace dh2::data;
namespace {
void word(std::vector<std::uint8_t>& b,std::uint32_t x){auto* p=reinterpret_cast<const std::uint8_t*>(&x);b.insert(b.end(),p,p+4);}
std::uint32_t read(const std::uint8_t*& p){std::uint32_t n;std::memcpy(&n,p,4);p+=4;return n;}
Bytes block(const std::uint8_t*& p){auto n=read(p);auto* b=p;p+=n;return {b,n};}
bool service(void*,ItemInstanceV1&,const ItemTextRequestV5& q,ItemTextResponseV5& r,std::string& out,std::string& e){if(q.operation==ItemTextOperationV5::integer_string){r.text="OID"+std::to_string(q.value);return true;}if(q.operation==ItemTextOperationV5::parse_ex){out+=q.input;for(unsigned i=0;i<q.count;++i){std::uint32_t bits;std::memcpy(&bits,&q.arguments[i].number,4);out+=':'+std::to_string(bits)+'/'+std::to_string(q.arguments[i].integer);}return true;}e="Missing required Power formatter fixture";return false;}
}
extern "C" std::uint32_t dh2_item_power_instance_fixture_v5(const std::uint8_t* input,std::uint8_t* output){auto* p=input;auto records=block(p),names=block(p),schema=block(p);ItemPowerTablesV5 tables;std::string e;if(!tables.load(records,names,schema,e))return UINT32_MAX;ItemPresentationOwnerV5 owner(tables.borrow());ItemInstanceV1 item;ItemTextServicesV5 svc{nullptr,nullptr,service};auto count=read(p);std::vector<std::uint8_t> result;while(count--){auto id=read(p),mode=read(p);if(!owner.add_power(item,std::int32_t(id),std::int32_t(mode),svc,e))return UINT32_MAX;auto* state=owner.powers(item);word(result,state->size());for(const auto& x:*state){word(result,std::uint32_t(x.id));word(result,std::uint32_t(x.sorting_order));word(result,x.description.size());result.insert(result.end(),x.description.begin(),x.description.end());}}std::memcpy(output,result.data(),result.size());return std::uint32_t(result.size());}
