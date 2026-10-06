#include "loot_audiovisual_v8.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::data {
namespace {
struct Reader {Bytes b;std::size_t at{};std::uint32_t word(){if(!b.data||b.size-at<4)throw std::runtime_error("Truncated source loot audiovisual cache");auto* p=b.data+at;at+=4;return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}std::string text(){auto n=word();if(n>b.size-at)throw std::runtime_error("Truncated source loot audiovisual string");std::string s(reinterpret_cast<const char*>(b.data+at),n);at+=n;return s;}std::vector<std::string> strings(){auto n=word();if(n>65536)throw std::runtime_error("Loot audiovisual string count exceeds native bound");std::vector<std::string> out;while(n--)out.push_back(text());return out;}};
std::int32_t signed_word(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}
}
struct LootAudioVisualV8::Snapshot {std::vector<LootAudioVisualRowV8> rows;std::vector<std::string> names;};
bool LootAudioVisualV8::load(Bytes records,Bytes names,Bytes schema,std::string& e){try{Reader r{records},n{names},s{schema};auto next=std::make_shared<Snapshot>();auto count=r.word();if(count>65536)throw std::runtime_error("Loot audiovisual row count exceeds native bound");while(count--)next->rows.push_back({signed_word(r.word()),signed_word(r.word()),r.text()});next->names=n.strings();if(s.strings()!=std::vector<std::string>{"AudioDrop","AudioPickup","Visual"}||next->names.size()!=next->rows.size()||r.at!=records.size||n.at!=names.size||s.at!=schema.size)throw std::runtime_error("Source loot audiovisual schema/count/tail mismatch");snapshot_=std::move(next);e.clear();return true;}catch(const std::exception& x){e=x.what();return false;}}
const std::vector<LootAudioVisualRowV8>& LootAudioVisualV8::Borrow::rows()const{if(!p_)throw std::logic_error("Unbound source loot audiovisual rows");return p_->rows;}
const std::vector<std::string>& LootAudioVisualV8::Borrow::names()const{if(!p_)throw std::logic_error("Unbound source loot audiovisual names");return p_->names;}
}
