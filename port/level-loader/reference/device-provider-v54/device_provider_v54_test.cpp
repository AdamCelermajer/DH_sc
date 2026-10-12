#include "actual_device_provider_v54.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;
int main(){
 world::ActualDeviceProviderV54 p;std::string e;bool out=true;
 auto owner=std::make_shared<resources::ContextResourceBudgetV37>();
 auto foreign=std::make_shared<resources::ContextResourceBudgetV37>();
 world::CurrentGlesContextV54 c{11,22,33,2,true};int reads=0;
 auto current=[&](auto& x,auto&){++reads;x=c;return true;};
 ui::MenuDeviceFactsV1 facts{};facts.driver_type=0; // menu literal is deliberately ignored
 auto flags=[&](auto& f,auto&){f=facts;return true;};
 assert(!p.high_performance(owner,current,flags,out,e)&&out);
 facts.sharp=255;reads=0;assert(p.high_performance(owner,current,flags,out,e)&&!out&&reads==0);
 facts={};assert(owner->begin_context(e));assert(p.bind_current(owner,current,e));
 assert(p.high_performance(owner,current,flags,out,e)&&out&&e.empty());
 for(auto field:{&facts.sharp,&facts.htc,&facts.multiplayer_mode}){*field=1;reads=0;assert(p.high_performance(owner,current,flags,out,e)&&!out&&reads==0);*field=0;}
 out=true;assert(!p.high_performance(foreign,current,flags,out,e)&&out);
 c.context=23;assert(!p.high_performance(owner,current,flags,out,e)&&out);c.context=22;
 c.thread=34;assert(!p.high_performance(owner,current,flags,out,e)&&out);c.thread=33;
 c.client_version=1;assert(!p.high_performance(owner,current,flags,out,e)&&out);c.client_version=2;
 c.opengl_es_api=false;assert(!p.high_performance(owner,current,flags,out,e)&&out);c.opengl_es_api=true;
 owner->context_lost();assert(!p.high_performance(owner,current,flags,out,e)&&out);
 assert(owner->begin_context(e));assert(!p.high_performance(owner,current,flags,out,e)&&out);
 assert(p.bind_current(owner,current,e));assert(p.high_performance(owner,current,flags,out,e)&&out);
 auto lost=[&](auto& x,auto&){x=c;owner->context_lost();return true;};
 assert(!p.high_performance(owner,lost,flags,out,e)&&out);
 auto badflags=[](auto&,auto& e){e="Required actual multiplayer/Build owner";return false;};
 assert(!p.high_performance(owner,current,badflags,out,e)&&out);
 p.context_lost();assert(!p.high_performance(owner,current,flags,out,e)&&out);
 std::cout<<"PASS source flags3 byte255/short-circuit; actual ledger identity/loss/generation/rebind; current GL/thread/API fixtures; output preserved; no live Android activation claim\n";
}
