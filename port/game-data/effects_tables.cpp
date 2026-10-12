#include "effects_tables.hpp"
#include <cstring>
#include <stdexcept>
namespace {
using namespace dh2::data;
constexpr std::size_t max_bytes=16u*1024u*1024u,max_count=65536,max_string=1048576;
struct Reader {
 Bytes bytes;std::size_t at=0,budget=0;
 explicit Reader(Bytes b):bytes(b){
  if(!b.data||b.size<4||b.size>max_bytes)throw std::runtime_error("Effects input outside native bounds");
 }
 void require(std::size_t n){if(n>bytes.size-at)throw std::runtime_error("Truncated effects input");}
 std::uint32_t word(){require(4);const auto* p=bytes.data+at;at+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::int32_t integer(){auto bits=word();std::int32_t v;std::memcpy(&v,&bits,4);return v;}
 std::uint8_t byte(){require(1);return bytes.data[at++];}
 std::uint32_t count(){auto n=word();if(n>max_count||budget+n>max_count)throw std::runtime_error("Effects count outside native bounds at "+std::to_string(at-4)+": "+std::to_string(n));budget+=n;return n;}
 std::string text(){auto n=word();if(n>max_string)throw std::runtime_error("Effects text outside native bounds");require(n);std::string value(reinterpret_cast<const char*>(bytes.data+at),n);at+=n;return value;}
 std::vector<std::string> names(){auto n=count();std::vector<std::string> values;values.reserve(n);while(n--)values.push_back(text());return values;}
 std::vector<std::int32_t> integers(){auto n=count();require(std::size_t(n)*4);std::vector<std::int32_t> values;values.reserve(n);while(n--)values.push_back(integer());return values;}
 void end(){if(at!=bytes.size)throw std::runtime_error("Unexpected effects suffix");}
};
void schema(Reader& reader,std::initializer_list<const char*> expected){
 auto names=reader.names();if(names.size()!=expected.size())throw std::runtime_error("Effects schema differs");
 unsigned i=0;for(auto name:expected)if(names[i++]!=name)throw std::runtime_error("Effects schema differs");
}
}
namespace dh2::data {
struct EffectsTables::Snapshot {
 std::vector<EffectSet> sets;
 std::vector<CharacterEffects> characters;
 std::vector<FootstepEffects> footsteps;
 std::vector<std::string> set_names,character_names,footstep_names;
 Dictionary dictionary;
 std::size_t set_end=0,character_end=0,data_end=0,names_end=0,schema_end=0;
};
bool EffectsTables::load(Bytes records,Bytes names,Bytes layout,Bytes keys,Bytes paths,std::string& error){
 error.clear();if(snapshot_&&snapshot_.use_count()>1){error="Effects snapshot pinned";return false;}
 try{
  auto next=std::make_shared<Snapshot>();Reader r(records),n(names),s(layout);
  next->set_names=n.names();next->character_names=n.names();next->footstep_names=n.names();n.end();next->names_end=n.at;
  schema(s,{"File","ForceCancel","Loop","OrientOnce","OrientWithAnchor","PlayTime","PoolSize","Redir","ScaleWithAnchor","SelfIllum","Speed","SubObject"});
  schema(s,{"ForceCache","LoopAFX","Steps","Type"});
  schema(s,{"BloodDeathEffect","BloodEffect","FootprintEffect","SwooshEffect","TriggerFloorFX"});
  schema(s,{"Effect","Floortype","RunSound","WalkSound"});s.end();next->schema_end=s.at;
  if(!load_dictionary(keys,paths,next->dictionary,error))return false;
  auto count=r.count();if(count!=next->set_names.size())throw std::runtime_error("Effects set dimensions differ");next->sets.reserve(count);
  while(count--){EffectSet set;set.force_cache=r.byte();set.loop=r.integer();auto steps=r.count();set.steps.reserve(steps);
   while(steps--){EffectStep step;step.file=r.integer();step.force_cancel=r.byte();step.loop=r.integer();step.orient_once=r.byte();step.orient_with_anchor=r.byte();step.play_time=r.integer();step.pool_size=r.integer();step.redir=r.integer();step.scale_with_anchor=r.byte();step.self_illum=r.byte();step.speed_bits=r.word();step.subobject=r.text();set.steps.push_back(std::move(step));}
   set.type=r.integer();next->sets.push_back(std::move(set));
  }next->set_end=r.at;
  count=r.count();if(count!=next->character_names.size())throw std::runtime_error("Character effects dimensions differ");next->characters.reserve(count);
  while(count--){CharacterEffects row;row.blood_death=r.integer();row.blood=r.integer();row.footprint=r.integer();row.swoosh=r.integer();row.trigger_floor_fx=r.byte();next->characters.push_back(row);}next->character_end=r.at;
  count=r.count();if(count!=next->footstep_names.size())throw std::runtime_error("Footstep effects dimensions differ");next->footsteps.reserve(count);
  while(count--){FootstepEffects row;row.effect=r.integer();row.floor_type=r.text();row.run_sounds=r.integers();row.walk_sounds=r.integers();next->footsteps.push_back(std::move(row));}r.end();next->data_end=r.at;
  snapshot_=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
#define EFFECT_VIEW(method,member,type) const type& EffectsTables::Borrow::method()const{if(!snapshot_)throw std::logic_error("Missing effects snapshot");return snapshot_->member;}
EFFECT_VIEW(sets,sets,std::vector<EffectSet>)
EFFECT_VIEW(characters,characters,std::vector<CharacterEffects>)
EFFECT_VIEW(footsteps,footsteps,std::vector<FootstepEffects>)
EFFECT_VIEW(set_names,set_names,std::vector<std::string>)
EFFECT_VIEW(character_names,character_names,std::vector<std::string>)
EFFECT_VIEW(footstep_names,footstep_names,std::vector<std::string>)
EFFECT_VIEW(dictionary,dictionary,Dictionary)
#undef EFFECT_VIEW
#define EFFECT_SIZE(method,member) std::size_t EffectsTables::Borrow::method()const{return snapshot_?snapshot_->member:0;}
EFFECT_SIZE(set_end,set_end)
EFFECT_SIZE(character_end,character_end)
EFFECT_SIZE(data_consumed,data_end)
EFFECT_SIZE(names_consumed,names_end)
EFFECT_SIZE(schema_consumed,schema_end)
#undef EFFECT_SIZE
}
