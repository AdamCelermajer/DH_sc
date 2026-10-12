#include "../player_gear_effects_v5.hpp"
#include <cstring>
using namespace dh2::data;using Raw=std::vector<std::uint8_t>;
namespace {
std::uint32_t read(const std::uint8_t*& p){std::uint32_t v;std::memcpy(&v,p,4);p+=4;return v;}
void word(Raw& b,std::uint32_t v){auto* p=reinterpret_cast<const std::uint8_t*>(&v);b.insert(b.end(),p,p+4);}
void text(Raw& b,const char* p){auto n=p?std::strlen(p):0;word(b,n);if(n)b.insert(b.end(),p,p+n);}
std::uint32_t hash(const char* s){std::uint32_t h=0;for(;*s;++s)h=h*31+static_cast<unsigned char>(*s);return h&0x7fff;}
struct Context{Raw trace;std::uintptr_t visual=0x100000001;bool fallback{},mutation{};};
bool skin(void* p,FreshInventoryOwnedV4&,const GearSkinRequestV5& q,std::int32_t& result,std::string&){auto& c=*static_cast<Context*>(p);auto op=q.operation;word(c.trace,std::uint32_t(op));word(c.trace,std::uint32_t(q.visual));word(c.trace,std::uint32_t(q.visual>>32));text(c.trace,q.name);
 if(op==GearSkinOperationV5::category_id)result=hash(q.name);
 if(op==GearSkinOperationV5::module_id){word(c.trace,q.category);result=c.fallback&&std::strstr(q.name,"__naked")==nullptr&&std::strstr(q.name,"__placeholder")==nullptr?-1:std::int32_t(hash(q.name));}
 if(op==GearSkinOperationV5::set_modular){word(c.trace,q.category);word(c.trace,q.module);}
 if(op==GearSkinOperationV5::set_weapon){word(c.trace,q.slot);word(c.trace,q.mode);}
 if(q.visual&&c.mutation)++c.visual;return true;
}
bool effect(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&,OwnedInventoryResponseV4& r,std::string&){r={};return true;}
}
extern "C" std::uint32_t dh2_player_skin_fixture_v5(const std::uint8_t* input,std::uint8_t* output){try{auto* p=input;Bytes parts[3];for(auto& x:parts){auto n=read(p);x={p,n};p+=n;}LootTablesV2 tables;std::string error;if(!tables.load(parts[0],parts[1],parts[2],error))return UINT32_MAX;auto loot=read(p),selected=read(p),fallback=read(p),mutation=read(p),count=read(p);auto properties=std::make_shared<PropertyState>();LootRandom8V2 random{1,0};FreshInventoryOwnedV4 inventory(0x100000001,tables.borrow(),random,12,properties);if(selected)inventory.swap_equipment();OwnedInventoryServicesV4 effects{nullptr,effect};if(!inventory.add_fixed_loot(loot,effects,error))return UINT32_MAX;
PropertyView view{};PlayerGearEffectsV5 gear(inventory,view,nullptr,0,{});Context c;c.fallback=fallback;c.mutation=mutation;Raw out;for(unsigned i=0;i<count;++i){auto op=read(p),index=read(p);std::int32_t ignored;if(op==2){if(!inventory.auto_equip(index,ignored,effects,error))return UINT32_MAX;}else if(op==5)inventory.swap_equipment();else if(op!=15)return UINT32_MAX;c.trace.clear();if(!gear.update_skin(&c.visual,{&c,skin},error))return UINT32_MAX;word(out,c.trace.size());out.insert(out.end(),c.trace.begin(),c.trace.end());}std::memcpy(output,out.data(),out.size());return out.size();}catch(...){return UINT32_MAX;}}
