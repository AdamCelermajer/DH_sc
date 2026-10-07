#include "../item_text_varargs_v5.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
using Raw=std::vector<std::uint8_t>;
extern "C" std::uint32_t dh2_item_text_varargs_fixture_v5(const std::uint8_t*,std::uint8_t*);
static unsigned checks;
static void check(bool b){if(!b)throw std::runtime_error("Item varargs check "+std::to_string(checks));++checks;}
static Raw file(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct Reader{const Raw& bytes;std::size_t at{};std::uint32_t word(){check(bytes.size()-at>=4);std::uint32_t x;std::memcpy(&x,bytes.data()+at,4);at+=4;return x;}Raw block(){auto n=word();check(bytes.size()-at>=n);Raw out(bytes.begin()+at,bytes.begin()+at+n);at+=n;return out;}};
int main(int argc,char** argv){try{check(argc==2);auto gold=file(argv[1]);Reader r{gold};check(r.word()==0x35465649);auto cases=r.word();Raw out(1048576);for(unsigned i=0;i<cases;++i){auto input=r.block(),expected=r.block();auto n=dh2_item_text_varargs_fixture_v5(input.data(),out.data());check(n==expected.size()&&std::equal(expected.begin(),expected.end(),out.begin()));}check(r.at==gold.size());
using namespace dh2;std::string text="retained",error;bool changed=true;unsigned calls=0;ui::HudTextServicesV1 svc{&calls,[](void* p,const ui::HudTextRequestV1&,ui::HudTextResponseV1&,std::string&){++*static_cast<unsigned*>(p);return false;}};
alignas(data::ItemTextArgumentV5) unsigned char bad[32]{};
check(!ui::item_text_varargs_v5("^d",reinterpret_cast<data::ItemTextArgumentV5*>(bad+1),1,svc,text,changed,error)&&calls==0&&text=="retained");
check(!ui::item_text_varargs_v5("^d",nullptr,1,svc,text,changed,error)&&calls==0&&text=="retained");
check(!ui::item_text_varargs_v5("^d",nullptr,65537,svc,text,changed,error)&&calls==0&&text=="retained");
check(ui::item_text_varargs_v5(nullptr,nullptr,0,{},text,changed,error)&&!changed&&text=="retained");
check(!ui::item_text_varargs_v5("plain",nullptr,0,{},text,changed,error)&&text=="retained");
std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"atomic_and_required_guards\":5,\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
