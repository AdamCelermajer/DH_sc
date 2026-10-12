#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
using Raw=std::vector<std::uint8_t>;
extern "C" std::uint32_t dh2_player_skin_fixture_v5(const std::uint8_t*,std::uint8_t*);
static unsigned checks;
static void check(bool b){if(!b)throw std::runtime_error("Skin fixture check "+std::to_string(checks));++checks;}
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
static void word(Raw& b,std::uint32_t v){auto* p=reinterpret_cast<const std::uint8_t*>(&v);b.insert(b.end(),p,p+4);}
struct Reader{const Raw& b;std::size_t at{};std::uint32_t word(){check(b.size()-at>=4);std::uint32_t v;std::memcpy(&v,b.data()+at,4);at+=4;return v;}Raw block(){auto n=word();check(b.size()-at>=n);Raw v(b.begin()+at,b.begin()+at+n);at+=n;return v;}};
int main(int argc,char** argv){try{check(argc==3);Raw cache;for(auto name:{"loot_table_pyarray.bin","loot_table_pyarraynames.bin","loot_table_pystructnames.bin"}){auto b=file(std::string(argv[2])+"/"+name);word(cache,b.size());cache.insert(cache.end(),b.begin(),b.end());}auto gold=file(argv[1]);Reader r{gold};check(r.word()==0x35564b53);auto count=r.word();Raw output(1048576);for(unsigned i=0;i<count;++i){auto input=r.block(),expected=r.block();auto args=cache;args.insert(args.end(),input.begin(),input.end());auto n=dh2_player_skin_fixture_v5(args.data(),output.data());check(n==expected.size()&&std::equal(expected.begin(),expected.end(),output.begin()));}check(r.at==gold.size());std::cout<<"{\"validation\":\"PASS\",\"original_skin_cases\":"<<count<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
