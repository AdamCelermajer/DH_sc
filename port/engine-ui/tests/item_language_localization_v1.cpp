#include "../item_language_localization_v1.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::data;using namespace dh2::ui;
void check(bool yes,const char* why){if(!yes)throw std::runtime_error(why);}
struct TextFixture {Item row;
 static const Item* metadata(void* p,const ItemInstanceV1&,std::string&){return &static_cast<TextFixture*>(p)->row;}
 // Declared formatting/constant fixture, not a production empty formatter.
 static bool invoke(void*,ItemInstanceV1&,const ItemTextRequestV5&,ItemTextResponseV5& response,std::string&,std::string&){response.value=0;response.text.clear();return true;}
};
int main(){try{
 TextFixture fixture;fixture.row.record.words[17]=-1;ItemTextServicesV5 text{&fixture,TextFixture::metadata,TextFixture::invoke};
 ItemInstanceV1 item;item.name="old";item.description="old";item.requirements="old";ItemLanguageReceiptV1 receipt;std::string error;
 check(item_update_localization_v1(item,text,{},receipt,error)&&receipt.phase==7&&receipt.powers_rebuilt==0,"genuine source zero-power branch");
 check(item.name.empty()&&item.description.empty()&&item.requirements.empty(),"actual text kernels reached");
 item.powers={7,2,7};check(!item_update_localization_v1(item,text,{},receipt,error)&&receipt.phase==4&&item.powers==std::vector<std::int32_t>{7,2,7},"missing power provider preserves reached prefix/IDs");
 auto owner=std::make_shared<int>(1);std::vector<std::int32_t> added;
 ItemLanguagePowerServicesV1 powers{owner,[](ItemInstanceV1& actual,std::string&){actual.powers.clear();return true;},[&](ItemInstanceV1& actual,std::int32_t id,std::int32_t mode,std::string&){check(mode==-1,"original rebuild mode");added.push_back(id);actual.powers.push_back(id);return true;}};
 check(item_update_localization_v1(item,text,powers,receipt,error)&&receipt.phase==7&&receipt.powers_rebuilt==3&&added==std::vector<std::int32_t>{7,2,7},"captured source IDs/order including duplicates");
 unsigned calls=0;powers.add=[&](ItemInstanceV1& actual,std::int32_t id,std::int32_t,std::string&){if(++calls==2)return false;actual.powers.push_back(id);return true;};
 check(!item_update_localization_v1(item,text,powers,receipt,error)&&receipt.phase==6&&receipt.powers_rebuilt==1&&item.powers==std::vector<std::int32_t>{7},"source powered failure prefix");
 std::cout<<"PASS full item localization coordinator and required power prefixes; formatting/power providers declared fixtures\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
