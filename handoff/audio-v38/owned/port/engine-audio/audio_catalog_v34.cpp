#include "audio_catalog_v34.hpp"
#include <algorithm>
#include <charconv>
#include <cctype>
#include <cstring>
#include <stdexcept>
namespace dh2::audio {namespace {
using Attributes=std::map<std::string,std::string>;
struct Xml {
 const std::string&text;std::size_t pos{};
 void space(){while(pos<text.size()&&std::isspace(static_cast<unsigned char>(text[pos])))++pos;}
 std::string name(){auto start=pos;while(pos<text.size()&&(std::isalnum(static_cast<unsigned char>(text[pos]))||text[pos]=='_'||text[pos]=='-'||text[pos]==':'))++pos;if(start==pos)throw std::runtime_error("Audio XML name");return text.substr(start,pos-start);}
 static std::string decode(std::string v){const std::pair<const char*,const char*>entities[]{{"&amp;","&"},{"&quot;","\""},{"&apos;","'"},{"&lt;","<"},{"&gt;",">"}};for(const auto&e:entities){std::size_t p=0;while((p=v.find(e.first,p))!=std::string::npos){v.replace(p,std::strlen(e.first),e.second);p+=std::strlen(e.second);}}return v;}
 bool next(std::string&tag,Attributes&attrs,bool&closing,bool&empty){
  for(;;){auto p=text.find('<',pos);if(p==std::string::npos)return false;pos=p;
   if(text.compare(pos,4,"<!--")==0){auto end=text.find("-->",pos+4);if(end==std::string::npos)throw std::runtime_error("Audio XML comment");pos=end+3;continue;}
   if(text.compare(pos,2,"<?")==0){auto end=text.find("?>",pos+2);if(end==std::string::npos)throw std::runtime_error("Audio XML declaration");pos=end+2;continue;}
   if(text.compare(pos,2,"<!")==0)throw std::runtime_error("Required original audio XML declaration family");break;
  }
  ++pos;closing=pos<text.size()&&text[pos]=='/';if(closing)++pos;tag=name();attrs.clear();empty=false;
  for(;;){space();if(pos>=text.size())throw std::runtime_error("Audio XML tag extent");if(text[pos]=='>'){++pos;return true;}if(text[pos]=='/'&&!closing){++pos;if(pos>=text.size()||text[pos++]!='>')throw std::runtime_error("Audio XML empty tag");empty=true;return true;}
   auto key=name();space();if(pos>=text.size()||text[pos++]!='=')throw std::runtime_error("Audio XML attribute assignment");space();if(pos>=text.size()||(text[pos]!='\"'&&text[pos]!='\''))throw std::runtime_error("Audio XML attribute quote");char quote=text[pos++];auto end=text.find(quote,pos);if(end==std::string::npos)throw std::runtime_error("Audio XML attribute extent");auto value=decode(text.substr(pos,end-pos));pos=end+1;if(!attrs.emplace(std::move(key),std::move(value)).second)throw std::runtime_error("Duplicate source XML attribute");
  }
 }
};
const std::string&get(const Attributes&a,const char*key){auto it=a.find(key);if(it==a.end())throw std::runtime_error(std::string("Required source XML field ")+key);return it->second;}
int integer(const std::string&s){int v;auto result=std::from_chars(s.data(),s.data()+s.size(),v);if(result.ec!=std::errc{}||result.ptr!=s.data()+s.size())throw std::runtime_error("Audio XML integer");return v;}
int integer(const Attributes&a,const char*key){return integer(get(a,key));}
int choice(const std::string&s,std::initializer_list<const char*>values){int i=0;for(const auto*v:values){if(s==v)return i;++i;}throw std::runtime_error("Required source audio enum "+s);}
}
bool AudioCatalogV34::load_xml(const std::vector<std::uint8_t>&bytes,std::string&error){try{
 std::string text(bytes.begin(),bytes.end());Xml xml{text};AudioCatalogV34 next;std::string tag;Attributes a;bool closing,empty;std::vector<std::string>stack;std::map<std::string,unsigned>declared;
 while(xml.next(tag,a,closing,empty)){
  if(closing){if(stack.empty()||stack.back()!=tag)throw std::runtime_error("Audio XML topology");stack.pop_back();continue;}
  if(stack.empty()&&tag!="soundpack")throw std::runtime_error("Original soundpack root");
  if(a.count("size")){int n=integer(a,"size");if(n<0||n>65536)throw std::runtime_error("Source catalog count");declared[tag]=unsigned(n);}
  const std::string parent=stack.empty()?"":stack.back();
  if(tag=="sound"&&parent=="sounds"){
   AudioSoundV34 s;s.uid=integer(a,"uid");s.bank=integer(a,"bank");s.group=integer(a,"group");s.priority=integer(a,"priority");s.format=choice(get(a,"format"),{"pcm","adpcm","vxn"});s.loading_flags=choice(get(a,"loadingflags"),{"none","load","load and decode"});s.loop=a.count("loop")?choice(get(a,"loop"),{"no","yes"})!=0:false;s.label=get(a,"label");s.filename=get(a,"filename");
   if(s.uid<0||s.uid>65535||s.filename.empty()||s.filename.find_first_of("/\\:")!=std::string::npos||s.filename.find("..")!=std::string::npos)throw std::runtime_error("Original sound UID/filename domain");next.sound_labels_[s.label]=s.uid;next.sounds_.push_back(std::move(s));
  }else if(tag=="group"&&parent=="groups"){
   AudioGroupV34 g;g.uid=integer(a,"uid");g.name=get(a,"name");g.bus=get(a,"bus");g.position_type=choice(get(a,"pos3d"),{"no","yes","relative"});g.volume_group=choice(get(a,"volumeslider"),{"MUSIC","SFX","VFX"});next.groups_.push_back(std::move(g));
  }else if(tag=="bank"&&parent=="banks"){
   const auto behavior=get(a,"behaviour");int mode=behavior=="steal oldest"?0:behavior=="steal low. prio."?1:behavior=="steal low. prio. or old. same prio"?2:behavior=="do nothing"?3:-1;if(mode<0)throw std::runtime_error("Required source bank behavior "+behavior);next.banks_.push_back({integer(a,"uid"),a.count("threshold")?integer(a,"threshold"):-2147483647,integer(a,"maxplaybacks"),mode});
  }else if(tag=="event"&&parent=="events"){
   AudioEventV34 e;e.uid=integer(a,"uid");e.type=choice(get(a,"type"),{"random","playlist"});e.history_limit=integer(a,"params");e.label=get(a,"label");e.probability=a.count("probability")?integer(a,"probability"):100;const auto&value=get(a,"value");std::size_t p=0;for(;;){auto end=value.find(',',p);e.source.push_back(integer(value.substr(p,end==std::string::npos?end:end-p)));if(end==std::string::npos)break;p=end+1;}if(e.source.empty()||e.source.size()>32767||e.history_limit<0||e.probability<0||e.probability>100)throw std::runtime_error("Source event parameters");e.remaining=e.source;if(!next.event_labels_.emplace(e.label,e.uid).second)throw std::runtime_error("Duplicate original event label");next.events_.push_back(std::move(e));
  }else if(empty&&tag!="groupmask")throw std::runtime_error("Required source XML element "+tag);
  if(!empty)stack.push_back(tag);
 }
 if(!stack.empty()||next.sounds_.empty()||declared["sounds"]!=next.sounds_.size()||declared["groups"]!=next.groups_.size()||declared["banks"]!=next.banks_.size()||declared["events"]!=next.events_.size())throw std::runtime_error("Original catalog row counts");
 auto order=[](const auto&x,const auto&y){return x.uid<y.uid;};std::sort(next.sounds_.begin(),next.sounds_.end(),order);std::sort(next.groups_.begin(),next.groups_.end(),order);std::sort(next.events_.begin(),next.events_.end(),order);
 for(unsigned i=0;i<next.sounds_.size();++i){const auto&s=next.sounds_[i];if(s.uid!=int(i)||!next.group(s.group)||std::none_of(next.banks_.begin(),next.banks_.end(),[&](const auto&b){return b.id==s.bank;}))throw std::runtime_error("Original sound/group/bank identity");}
 for(unsigned i=0;i<next.events_.size();++i){const auto&e=next.events_[i];if(e.uid!=int(i))throw std::runtime_error("Original event identity");for(auto uid:e.source)if(!next.sound(uid))throw std::runtime_error("Original event source sound UID");}
 *this=std::move(next);error.clear();return true;
}catch(const std::exception&e){error=e.what();return false;}}
const AudioSoundV34* AudioCatalogV34::sound(int id)const noexcept{return id>=0&&std::size_t(id)<sounds_.size()&&sounds_[id].uid==id?&sounds_[id]:nullptr;}
const AudioGroupV34* AudioCatalogV34::group(int id)const noexcept{for(const auto&g:groups_)if(g.uid==id)return &g;return nullptr;}
int AudioCatalogV34::sound_uid(const char*name)const noexcept{if(!name)return -1;auto it=sound_labels_.find(name);return it==sound_labels_.end()?-1:it->second;}
int AudioCatalogV34::event_uid(const char*name)const noexcept{if(!name)return -1;auto it=event_labels_.find(name);return it==event_labels_.end()?-1:it->second;}
bool AudioCatalogV34::select_event(int uid,AudioRandomV34 random,int&sound,std::string&error){
 if(uid<0||std::size_t(uid)>=events_.size()||events_[uid].remaining.empty()){error="Required source event UID";return false;}auto&e=events_[uid];int roll;
 if(!random.next||!random.next(random.context,roll)||roll<0){error="Required same Vox rand stream";return false;}
 if(roll%100>=e.probability){sound=-1;return true;}
 if(e.type==1){if(e.cursor>=e.remaining.size())e.cursor=0;sound=e.remaining[e.cursor++];return true;}
 if(!random.next(random.context,roll)||roll<0){error="Required source random-event selection draw";return false;}unsigned index=unsigned(roll)%unsigned(e.remaining.size());sound=e.remaining[index];e.history.push_back(sound);e.remaining[index]=e.remaining.back();e.remaining.pop_back();
 if(e.history.size()>unsigned(e.history_limit)||e.remaining.empty()){e.remaining.push_back(e.history.front());e.history.erase(e.history.begin());}return true;
}
bool AudioCatalogV34::reset_event(int id,std::string&error){if(id<0||std::size_t(id)>=events_.size()){error="Required source reset event";return false;}auto&e=events_[id];e.remaining=e.source;e.history.clear();e.cursor=0;return true;}
bool audio_sound_autogen_rows_v34(const std::uint8_t*bytes,std::size_t size,std::vector<AudioSoundAutoGenV34>&out,std::string&error){if(!bytes||size<4){error="SoundAutoGen source range";return false;}unsigned count;std::memcpy(&count,bytes,4);if(count>65536||size!=4+std::uint64_t(count)*8){error="Required exact original SoundAutoGen UID/event schema";return false;}std::vector<AudioSoundAutoGenV34> rows(count);for(unsigned i=0;i<count;++i){std::memcpy(&rows[i],bytes+4+8*i,8);if(rows[i].event<0||rows[i].event>1||rows[i].uid< -1){error="Required SoundAutoGen source UID/event domain";return false;}}out=std::move(rows);return true;}
}
