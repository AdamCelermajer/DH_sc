// Test-only projection wrapper. It constructs the actual owned production
// table/inventory, executes its services and serializes source-normalized
// identities. libc allocation is an explicit oracle storage boundary.
#define main unused_fresh_inventory_host_main
#include "fresh_inventory_v2.cpp"
#undef main
struct FreshFixture64V2 {const std::uint8_t* records;const std::uint8_t* names;const std::uint8_t* schema;std::uint32_t record_size,name_size,schema_size,seed,calls;std::int32_t capacity;std::uint32_t minimal,mutation;std::int32_t loot;};
static_assert(sizeof(FreshFixture64V2)==64);
extern "C" int dh2_fresh_fixture_v2(const FreshFixture64V2* f,std::uint8_t* output,std::uint32_t capacity,std::uint32_t* written){if(!f||!output||!written)return -1;try{LootTablesV2 table;std::string error;if(!table.load({f->records,f->record_size},{f->names,f->name_size},{f->schema,f->schema_size},error))return -2;LootRandom8V2 random{f->seed,f->calls};FreshInventoryV2 owner(UINT64_C(0x123400005678),table.borrow(),random,std::int8_t(f->capacity));Context c;c.minimal=f->minimal;c.mutation=f->mutation;if(!owner.add_fixed_loot(f->loot,{&c,service},error))return -3;auto state=snap(owner);Raw result;w(result,random.seed);w(result,random.calls);w(result,state.size());result.insert(result.end(),state.begin(),state.end());w(result,c.requests.size()/28);result.insert(result.end(),c.requests.begin(),c.requests.end());if(result.size()>capacity)return -4;std::memcpy(output,result.data(),result.size());*written=result.size();return 0;}catch(...){return -5;}}
