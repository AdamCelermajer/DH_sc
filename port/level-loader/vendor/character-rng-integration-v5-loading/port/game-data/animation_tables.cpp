#include "animation_tables.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <set>
#include <stdexcept>
namespace dh2::data {
namespace {
constexpr const char* step_fields[]={"AnchorFX","Anim","BlendOut","Cam","CamDir","FX","MoveGO","RandomCam","Redir","Sound","Speed","Swoosh"};
constexpr const char* sequence_fields[]={"Loop","Steps","Type"};
constexpr const char* camera_fields[]={"CamAnims","Crit","Idle","Shake","Template"};
constexpr const char* state_fields[]={"Attack","AttackStatic","Blocking","DeadlyGreatKB","Despawn","DespawnGreatKB","Died","Dodging","GreatKnockedBack","Idle","IdleFromOOC","IdleOOC","IdleSneak","IdleToOOC","Injured","Interact","KnockedBack","LiftDrop","LiftIdle","LiftMove","Limbus","MenuIdle","MenuOnSelect","PreSpawn","Revived","Reviving","Run","Run180","RunSneak","Scared","Spawn","Spells","Stunned","Template","Walk","Walk180","WalkSneak"};
struct Reader {
 Bytes bytes;std::size_t offset=0,budget=0;
 explicit Reader(Bytes b):bytes(b){if(!b.data||b.size<4||b.size>8*1024*1024)throw std::runtime_error("Animation table input outside size limit");}
 void require(std::size_t size){if(offset>bytes.size||size>bytes.size-offset)throw std::runtime_error("Truncated animation table");}
 std::uint32_t word(){require(4);auto* p=bytes.data+offset;offset+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::int32_t integer(){auto bits=word();std::int32_t value;std::memcpy(&value,&bits,4);return value;}
 float real(){auto bits=word();float value;std::memcpy(&value,&bits,4);if(!std::isfinite(value)||value<=0||value>100)throw std::runtime_error("Animation speed outside limit");return value;}
 bool boolean(){require(1);auto value=bytes.data[offset++];if(value>1)throw std::runtime_error("Invalid animation boolean");return value!=0;}
 unsigned count(){auto n=word();if(n>10000||budget+n>100000)throw std::runtime_error("Animation table count outside limit");budget+=n;return n;}
 std::vector<std::int32_t> integers(){auto n=count();require(std::size_t(n)*4);std::vector<std::int32_t> out;out.reserve(n);for(unsigned i=0;i<n;++i)out.push_back(integer());return out;}
 std::vector<std::string> strings(){auto n=count();std::vector<std::string> out;out.reserve(n);std::set<std::string> seen;
  for(unsigned i=0;i<n;++i){auto size=word();if(!size||size>4096)throw std::runtime_error("Animation identifier length outside limit");require(size);std::string value(reinterpret_cast<const char*>(bytes.data+offset),size);offset+=size;
   if(std::any_of(value.begin(),value.end(),[](unsigned char c){return c<32||c>126;})||!seen.insert(value).second)throw std::runtime_error("Invalid or duplicate animation identifier");
   out.push_back(std::move(value));
  }return out;
 }
 void end(){if(offset!=bytes.size)throw std::runtime_error("Unexpected animation table suffix");}
};
template<std::size_t N> void schema(Reader& r,const char* const (&expected)[N]){auto names=r.strings();if(names.size()!=N||!std::equal(names.begin(),names.end(),expected))throw std::runtime_error("Animation field schema differs");}
void reference(std::int32_t id,std::size_t count){if(id< -1||(id>=0&&std::size_t(id)>=count))throw std::runtime_error("Animation reference outside table");}
}
bool load_animation_tables(Bytes records,Bytes names,Bytes fields,const Dictionary& clips,AnimationTables& out,std::string& error){
 out={};error.clear();try{
  if(clips.names.empty()||clips.names.size()!=clips.values.size()||clips.values.size()>10000)throw std::runtime_error("Invalid animation dictionary dimensions");
  Reader data(records),keys(names),layout(fields);AnimationTables next;
  next.sequence_names=keys.strings();next.camera_names=keys.strings();next.character_names=keys.strings();keys.end();
  schema(layout,step_fields);schema(layout,sequence_fields);schema(layout,camera_fields);schema(layout,state_fields);layout.end();next.state_names.assign(std::begin(state_fields),std::end(state_fields));
  auto count=data.count();if(count!=next.sequence_names.size()||!count)throw std::runtime_error("Animation sequence dimensions differ");next.sequences.reserve(count);
  for(unsigned i=0;i<count;++i){AnimationSequence sequence;sequence.loop=data.integer();auto steps=data.count();sequence.steps.reserve(steps);
   for(unsigned j=0;j<steps;++j){AnimationStep step;step.anchor_fx=data.boolean();step.anim=data.integer();step.blend_out=data.integer();step.cam=data.integer();step.cam_dir=data.boolean();step.fx=data.integer();step.move_go=data.boolean();step.random_cam=data.integers();step.redir=data.integer();step.sound=data.integer();step.speed=data.real();step.swoosh=data.boolean();
    if(step.redir!=0&&step.redir!=1)throw std::runtime_error("Invalid animation redirect flag");
    reference(step.anim,step.redir?count:clips.values.size());sequence.steps.push_back(std::move(step));
   }sequence.type=data.integer();next.sequences.push_back(std::move(sequence));
  }next.sequence_end=data.offset;
  count=data.count();if(count!=next.camera_names.size())throw std::runtime_error("Camera animation dimensions differ");next.cameras.reserve(count);
  for(unsigned i=0;i<count;++i){CameraAnimationSet camera;camera.cam_anims=data.integers();camera.crit=data.integer();camera.idle=data.integer();camera.shake=data.integer();camera.template_id=data.integer();next.cameras.push_back(std::move(camera));}next.camera_end=data.offset;
  count=data.count();if(count!=next.character_names.size()||!count)throw std::runtime_error("Character animation dimensions differ");next.characters.resize(count);
  for(auto& character:next.characters)for(unsigned i=0;i<37;++i){auto& values=character.fields[i];if(i==15||i==31)values=data.integers();else values.push_back(data.integer());for(auto value:values)reference(value,next.sequences.size());}
  data.end();next.data_consumed=data.offset;out=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const AnimationSequence* animation_state(const AnimationTables& table,std::int32_t character,const std::string& state,std::size_t variant){
 if(character<0||std::size_t(character)>=table.characters.size())return nullptr;
 auto field=std::find(table.state_names.begin(),table.state_names.end(),state);
 if(field==table.state_names.end()||std::size_t(field-table.state_names.begin())>=37)return nullptr;
 const auto& values=table.characters[character].fields[field-table.state_names.begin()];
 if(variant>=values.size()||values[variant]<0||std::size_t(values[variant])>=table.sequences.size())return nullptr;
 return &table.sequences[values[variant]];
}
const std::string* animation_clip(const AnimationStep& step,const Dictionary& clips){if(step.redir!=0||step.anim<0||std::size_t(step.anim)>=clips.values.size())return nullptr;return &clips.values[step.anim];}
}
