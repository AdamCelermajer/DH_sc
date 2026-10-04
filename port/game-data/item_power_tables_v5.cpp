#include "item_power_tables_v5.hpp"
#include <cstring>
#include <stdexcept>
namespace {
constexpr std::size_t budget=8*1024*1024;
bool span(const void* p,std::size_t n,std::size_t align=1){auto x=reinterpret_cast<std::uintptr_t>(p);return p&&x%align==0&&n<=UINTPTR_MAX-x;}
std::uint32_t word(const std::uint8_t* p){return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
std::int32_t sint(const std::uint8_t* p){auto x=word(p);std::int32_t v;std::memcpy(&v,&x,4);return v;}
struct Reader {dh2::data::Bytes b;std::size_t at{};explicit Reader(dh2::data::Bytes x):b(x){if(!span(b.data,b.size)||b.size>budget)throw std::runtime_error("Invalid ItemPower input span");}
 void take(std::size_t n){if(n>b.size-at)throw std::runtime_error("Truncated ItemPower cache");at+=n;}
 std::uint32_t count(){take(4);auto x=word(b.data+at-4);if(x>65536)throw std::runtime_error("ItemPower count exceeds native budget");return x;}
 std::vector<std::string> strings(){auto n=count();std::vector<std::string> v;while(n--){auto k=count();take(k);v.emplace_back(reinterpret_cast<const char*>(b.data+at-k),k);}return v;}
};
}
extern "C" int dh2_item_power_decode_v5(dh2::data::ItemPowerDecoded48V5* out,const std::uint8_t* b,std::uint32_t n) noexcept{
 if(!span(out,sizeof(*out),alignof(dh2::data::ItemPowerDecoded48V5))||!span(b,n)||n>budget||n<29)return -1;
 auto x=reinterpret_cast<std::uintptr_t>(out),y=reinterpret_cast<std::uintptr_t>(b);if(x<y+n&&y<x+sizeof(*out))return -1;
 auto count=word(b+5);if(count>65536||count>(n-29)/12)return -1;auto at=9+12*count;dh2::data::ItemPowerDecoded48V5 v{};
 v.scalars={b[0],sint(b+1),sint(b+at),sint(b+at+4),sint(b+at+8),sint(b+at+12),sint(b+at+16)};
 // Five scalar fields follow the list: Description, GoldBonusMultiplier,
 // SortingOrder, GoldBonus, AttrMonopoly. Five words, plus preceding Special.
 v.count=count;v.entries=b+9;v.consumed=at+20;*out=v;return 0;
}
namespace dh2::data {
struct ItemPowerTablesV5::Snapshot {std::vector<ItemPowerRecordV5> rows;std::vector<std::string> names;};
bool ItemPowerTablesV5::load(Bytes bytes,Bytes names,Bytes schema,std::string& e){try{
 if(snapshot_&&snapshot_.use_count()!=1)throw std::runtime_error("ItemPower snapshot has live borrowers");auto next=std::make_shared<Snapshot>();Reader r(bytes),n(names),s(schema);
 auto lists=r.count();for(unsigned i=0;i<lists;++i){auto k=r.count();r.take(std::size_t(k)*5);}auto count=r.count();next->rows.reserve(count);
 for(unsigned i=0;i<count;++i){ItemPowerDecoded48V5 d{};if(dh2_item_power_decode_v5(&d,r.b.data+r.at,std::uint32_t(r.b.size-r.at)))throw std::runtime_error("Invalid ItemPower record");ItemPowerRecordV5 row;row.scalars=d.scalars;row.properties.reserve(d.count);
  for(unsigned j=0;j<d.count;++j){auto p=d.entries+12*j;row.properties.push_back({sint(p),sint(p+4),sint(p+8)});}next->rows.push_back(std::move(row));r.take(d.consumed);
 }
 if(r.at!=bytes.size)throw std::runtime_error("Unexpected ItemPower trailing bytes");auto list_names=n.strings();next->names=n.strings();if(n.at!=names.size||list_names.size()!=lists||next->names.size()!=count)throw std::runtime_error("ItemPower names differ");
 if(s.strings()!=std::vector<std::string>{"Power","Prob"}||s.strings()!=std::vector<std::string>{"list_entries"}||s.strings()!=std::vector<std::string>{"Attr","Bonus","Flags"})throw std::runtime_error("ItemPower schema prefix differs");
 const std::vector<std::string> expected{"Palette","SpecialEffect","AttrBonusList","Description","GoldBonusMultiplier","SortingOrder","GoldBonus","AttrMonopoly"};unsigned blocks=0;while(s.at<schema.size){if(s.strings()!=expected)throw std::runtime_error("ItemPower subclass schema differs");++blocks;}if(!blocks)throw std::runtime_error("Missing ItemPower schema");
 snapshot_=std::move(next);e.clear();return true;
 }catch(const std::exception& x){e=x.what();return false;}}
const std::vector<ItemPowerRecordV5>& ItemPowerTablesV5::Borrow::rows()const{if(!snapshot_)throw std::logic_error("Unbound ItemPower snapshot");return snapshot_->rows;}
const std::vector<std::string>& ItemPowerTablesV5::Borrow::names()const{if(!snapshot_)throw std::logic_error("Unbound ItemPower snapshot");return snapshot_->names;}
}

