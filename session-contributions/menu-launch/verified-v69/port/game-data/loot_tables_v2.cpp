#include "loot_tables_v2.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>
namespace {
constexpr std::size_t limit=8*1024*1024;
std::uint32_t word(const std::uint8_t* p){return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
std::int32_t signed_word(const std::uint8_t* p){auto v=word(p);std::int32_t r;std::memcpy(&r,&v,4);return r;}
bool span(const void* p,std::size_t n,std::size_t a=1){auto v=reinterpret_cast<std::uintptr_t>(p);return p&&v%a==0&&n<=UINTPTR_MAX-v;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<y+bn&&y<x+an;}
struct Reader {
 dh2::data::Bytes b;std::size_t at{};
 explicit Reader(dh2::data::Bytes p):b(p){if(!span(b.data,b.size)||b.size>limit)throw std::runtime_error("Malformed LootTable byte span");}
 void take(std::size_t n){if(n>b.size-at)throw std::runtime_error("Truncated LootTable cache");at+=n;}
 std::uint32_t u32(){take(4);return word(b.data+at-4);}
 std::uint32_t count(){auto n=u32();if(n>65536)throw std::runtime_error("LootTable count outside native budget at "+std::to_string(at-4)+": "+std::to_string(n));return n;}
 std::vector<std::string> strings(){auto n=count();std::vector<std::string> out;out.reserve(n);while(n--){auto len=count();take(len);out.emplace_back(reinterpret_cast<const char*>(b.data+at-len),len);}return out;}
};
}
extern "C" int dh2_loot_v2_decode(dh2::data::LootRow64V2* out,const std::uint8_t* b,std::uint32_t size) noexcept {
 if(!span(b,size)||size>limit||!span(out,sizeof(*out),alignof(dh2::data::LootRow64V2))||overlap(out,sizeof(*out),b,size)||size<20)return -1;
 dh2::data::LootRow64V2 next{};next.roll_type=signed_word(b);next.num_random_item_probs=signed_word(b+4);std::uint32_t at=8;
 auto read=[&](dh2::data::LootSpan16V2& s,std::uint32_t stride){if(size-at<4)return false;auto count=word(b+at);at+=4;if(count>65536||count>(size-at)/stride)return false;s={b+at,count,0};at+=count*stride;return true;};
 if(!read(next.random_entries,32)||!read(next.fixed_entries,32)||!read(next.sub_loots,4))return -1;next.consumed=at;*out=next;return 0;
}
extern "C" int dh2_loot_v2_random(dh2::data::LootRandom8V2* s,std::int32_t bound,std::int32_t* out) noexcept {
 if(!span(s,sizeof(*s),4)||!span(out,4,4)||overlap(s,sizeof(*s),out,4))return -1;
 std::int32_t value=0;if(bound){auto x=s->seed*UINT32_C(0xe6ab)+UINT32_C(0x2b3fd);auto q=std::uint32_t((std::uint64_t(x)*UINT32_C(0x2b52db17))>>32);q=(q+((x-q)>>1))>>23;s->seed=x-q*UINT32_C(0xdaf26b);std::int32_t seed;std::memcpy(&seed,&s->seed,4);auto remainder=std::int64_t(seed)%bound;value=std::int32_t(remainder<0?-remainder:remainder);}++s->calls;*out=value;return 0;
}
namespace dh2::data {
struct LootTablesV2::Snapshot {ItemTable items;std::vector<std::vector<LootItemEntryV2>> item_lists;std::vector<LootRecordV2> loots;std::vector<std::string> loot_names,list_names;std::size_t consumed{};};
bool LootTablesV2::load(Bytes records,Bytes names,Bytes schema,std::string& e){try{
 if(snapshot_&&snapshot_.use_count()!=1)throw std::runtime_error("LootTable snapshot has live borrowers");auto next=std::make_shared<Snapshot>();if(!load_items(records,names,schema,next->items,e))return false;
 Reader r(records),n(names),f(schema);auto count=r.count();for(std::uint32_t i=0;i<count;++i){auto k=r.count();r.take(std::size_t(k)*4);}auto k=r.count();r.take(std::size_t(k)*2);count=r.count();next->item_lists.reserve(count);
 for(std::uint32_t i=0;i<count;++i){auto entries=r.count();std::vector<LootItemEntryV2> list;list.reserve(entries);for(std::uint32_t j=0;j<entries;++j){r.take(7);auto p=r.b.data+r.at-7;auto raw=std::uint16_t(p[4])|std::uint16_t(p[5])<<8;std::int16_t prob;std::memcpy(&prob,&raw,2);list.push_back({signed_word(p),prob,p[6]});}next->item_lists.push_back(std::move(list));}
 if(r.at!=next->items.data_begin)throw std::runtime_error("Loot ItemList prefix differs");r.at=next->items.data_consumed;
 // Arrays::ItemTypeList::read 0x4b9fe0 stores a table of variable-length
 // byte-enum lists. It is not a flat signed16 array. In the original cache
 // this section spans 229006..229060, immediately before Arrays::Loot.
 count=r.count();for(std::uint32_t i=0;i<count;++i)r.take(r.count());
 count=r.count();next->loots.reserve(count);
 for(std::uint32_t i=0;i<count;++i){LootRow64V2 row{};if(dh2_loot_v2_decode(&row,records.data+r.at,std::uint32_t(records.size-r.at)))throw std::runtime_error("Malformed original Loot record");LootRecordV2 actual{row.roll_type,row.num_random_item_probs,{},{},{}};
  for(auto pair:{std::make_pair(&row.random_entries,&actual.random_entries),std::make_pair(&row.fixed_entries,&actual.fixed_entries)})for(std::uint32_t j=0;j<pair.first->count;++j){LootEntry32V2 v{};for(unsigned x=0;x<8;++x)v.words[x]=signed_word(pair.first->bytes+32*j+4*x);pair.second->push_back(v);}
  for(std::uint32_t j=0;j<row.sub_loots.count;++j)actual.sub_loots.push_back(signed_word(row.sub_loots.bytes+4*j));next->loots.push_back(std::move(actual));r.take(row.consumed);
 }
 next->consumed=r.at;n.strings();n.strings();next->list_names=n.strings();n.strings();n.strings();next->loot_names=n.strings();if(next->list_names.size()!=next->item_lists.size()||next->loot_names.size()!=next->loots.size())throw std::runtime_error("LootTable names/count mismatch");
 for(unsigned i=0;i<14;++i)f.strings();auto entry=f.strings(),loot=f.strings();if(entry!=std::vector<std::string>{"ItemListID","ItemPowerList","MProb","NumPowerProbs","Pct","Prob","RProb","WProb"}||loot!=std::vector<std::string>{"RollType","NumRandomItemProbs","RandomLootEntries","FixedLootEntries","SubLoots"})throw std::runtime_error("LootTable schema differs");snapshot_=std::move(next);e.clear();return true;
 }catch(const std::exception& x){e=x.what();return false;}}
const ItemTable& LootTablesV2::Borrow::items()const{if(!snapshot_)throw std::logic_error("Unbound LootTable");return snapshot_->items;}
const std::vector<std::vector<LootItemEntryV2>>& LootTablesV2::Borrow::item_lists()const{if(!snapshot_)throw std::logic_error("Unbound LootTable");return snapshot_->item_lists;}
const std::vector<LootRecordV2>& LootTablesV2::Borrow::loots()const{if(!snapshot_)throw std::logic_error("Unbound LootTable");return snapshot_->loots;}
const std::vector<std::string>& LootTablesV2::Borrow::loot_names()const{if(!snapshot_)throw std::logic_error("Unbound LootTable");return snapshot_->loot_names;}
const std::vector<std::string>& LootTablesV2::Borrow::item_list_names()const{if(!snapshot_)throw std::logic_error("Unbound LootTable");return snapshot_->list_names;}
std::size_t LootTablesV2::Borrow::consumed()const{if(!snapshot_)throw std::logic_error("Unbound LootTable");return snapshot_->consumed;}
}
