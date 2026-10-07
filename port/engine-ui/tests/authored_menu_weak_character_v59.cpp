#include "authored_menu_weak_character_v59.hpp"
#include "swf_movie.hpp"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#pragma clang diagnostic ignored "-Wnew-returns-null"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif
#include "gameswf/gameswf_sprite.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <cassert>
#include <iostream>
using namespace dh2::ui;
namespace {
// Minimal real two-frame SWF. No game art/audio/UI callback substitutes and no
// display assertions: this isolates the native character/weak/movie lifecycle.
std::vector<std::uint8_t> movie_bytes(){
 std::vector<std::uint8_t> bytes{'F','W','S',6,0,0,0,0};unsigned bit=0;
 auto bits=[&](unsigned value,unsigned count){for(unsigned i=count;i-->0;){
  if(!(bit%8))bytes.push_back(0);bytes.back()|=((value>>i)&1u)<<(7-bit%8);++bit;}};
 bits(12,5);bits(0,12);bits(2000,12);bits(0,12);bits(2000,12);
 for(auto byte:{0,12,2,0,0x40,0,0x40,0,0,0})bytes.push_back(byte);
 const auto size=static_cast<std::uint32_t>(bytes.size());
 for(unsigned i=0;i<4;++i)bytes[4+i]=std::uint8_t(size>>(8*i));return bytes;
}
SwfServices services(){SwfServices out;
 out.read=[](void*,const char*,auto& bytes,auto&){bytes=movie_bytes();return true;};
 out.draw=[](void*,const auto&,auto&){return true;};return out;
}
struct Batch {
 AuthoredMenuWeakCharacterV59& weak;AuthoredMenuFieldsV1& fields;
 unsigned mode;std::uintptr_t original{};
 static bool run(void* raw,SwfAsGraph& graph,std::string& error){auto& t=*static_cast<Batch*>(raw);
  if(t.mode==1){SwfAsValue out;bool live{};assert(!t.weak.borrow(graph,out,live,error));return true;}
  if(t.mode==2){assert(!t.weak.update(graph,t.fields,error));return true;}
  SwfAsValue root;gameswf::as_object* object{};
  assert(graph.root_value(root,error)&&graph.borrow_object(root,object,error));
  auto* sprite=static_cast<gameswf::sprite_instance*>(object);
  auto* child=sprite->add_empty_movieclip("menu_Weak",123);assert(child);
  t.original=reinterpret_cast<std::uintptr_t>(child);
  {
   SwfAsValue value;assert(graph.retain_object(child,value,error));
   assert(t.weak.bind(graph,value,error));
   assert(!t.weak.bind(graph,value,error)); // no RegisterState replay
  }
  const auto before=t.fields.counter78;
  assert(t.weak.update(graph,t.fields,error));
  assert(t.fields.counter78==before+1u); // stopped/one-frame clip still over
  sprite->remove_display_object(child);child=nullptr;
  auto* replacement=sprite->add_empty_movieclip("menu_Weak",123);assert(replacement);
  SwfAsValue value;bool live{};assert(t.weak.borrow(graph,value,live,error)&&!live);
  const auto expired_counter=t.fields.counter78;
  assert(t.weak.update(graph,t.fields,error)&&t.fields.counter78==expired_counter);
  // Name/depth (or recycled pointer address) cannot substitute the old proxy.
  sprite->remove_display_object(replacement);return true;
 }
};
}
int main(){
 std::string error;SwfMovie first,second;auto io=services();
 assert(first.load({},"weak.swf",io,error)&&second.load({},"other.swf",io,error));
 AuthoredMenuWeakCharacterV59 weak;AuthoredMenuFieldsV1 fields;
 fields.render=reinterpret_cast<std::uintptr_t>(&first);fields.counter78=0xffffffffu;
 Batch batch{weak,fields,0};assert(first.menu_action_script(&batch,Batch::run,error));
 assert(fields.counter78==0); // actual uint32 counter wrap
 batch.mode=1;assert(second.menu_action_script(&batch,Batch::run,error));
 assert(first.load({},"replacement.swf",io,error));batch.mode=2;
 assert(first.menu_action_script(&batch,Batch::run,error)); // same facade/new binding rejected
 std::cout<<"Native registered weak identity/expiry, path replacement, counter and graph generation checks passed\n";
}
