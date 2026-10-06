#include "audio_sample_v34.hpp"
#include <algorithm>
#include <cstring>
#include <map>
#include <stdexcept>
namespace dh2::audio {namespace {
unsigned word(const std::uint8_t*p){return p[0]|unsigned(p[1])<<8|unsigned(p[2])<<16|unsigned(p[3])<<24;}
unsigned half(const std::uint8_t*p){return p[0]|unsigned(p[1])<<8;}
int signed_half(const std::uint8_t*p){unsigned v=half(p);return v<32768?int(v):int(v)-65536;}
constexpr int steps[]{7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767};
constexpr int indices[]{-1,-1,-1,-1,2,4,6,8};
int nibble(unsigned n,int&predictor,int&index){int step=steps[index],delta=step>>3;if(n&4)delta+=step;if(n&2)delta+=step>>1;if(n&1)delta+=step>>2;predictor=std::clamp(predictor+((n&8)?-delta:delta),-32768,32767);index=std::clamp(index+indices[n&7],0,88);return predictor;}
struct Reader {
 const std::vector<std::uint8_t>& b;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{if(p>b.size()||n>b.size()-p)throw std::runtime_error("Audio source range");return b.data()+p;}
 unsigned w(std::uint64_t p)const{return word(at(p,4));}
};
void format(AudioSampleV34&s){
 if((s.channels!=1&&s.channels!=2)||s.rate<8000||s.rate>192000)throw std::runtime_error("Required source audio rate/channel domain");
 if(s.format==1){if(s.bits!=16||s.block_align!=2*s.channels)throw std::runtime_error("Required source PCM16 stream");s.frames_per_block=1;}
 else if(s.format==17){if(s.bits!=4||s.block_align<4*s.channels||(s.block_align-4*s.channels)%(4*s.channels))throw std::runtime_error("Required source IMA block layout");s.frames_per_block=(s.block_align-4*s.channels)*2/s.channels+1;if(s.frames_per_block*s.channels>4096)throw std::runtime_error("Required source IMA block capacity");}
 else throw std::runtime_error("Required original audio codec "+std::to_string(s.format));
}
template<std::size_t N>void rows(const Reader&r,unsigned p,unsigned size,std::vector<std::array<std::int32_t,N>>&out){unsigned n=r.w(p);if(n>65536||size!=4+std::uint64_t(n)*N*4)throw std::runtime_error("Required exact source audio metadata rows");out.resize(n);for(unsigned i=0;i<n;++i)std::memcpy(out[i].data(),r.at(p+4+std::uint64_t(i)*N*4,N*4),N*4);}
}
unsigned audio_ima_block_v34(const std::uint8_t*in,unsigned bytes,unsigned channels,std::int16_t*out,unsigned capacity)noexcept{
 if(!in||!out||(channels!=1&&channels!=2)||bytes<4*channels||(bytes-4*channels)%(4*channels))return 0;
 const unsigned frames=(bytes-4*channels)*2/channels+1;if(std::uint64_t(frames)*channels>capacity)return 0;
 int predictor[2]{},index[2]{};
 for(unsigned c=0;c<channels;++c){predictor[c]=signed_half(in+4*c);index[c]=in[4*c+2];if(index[c]>88)return 0;out[c]=std::int16_t(predictor[c]);}
 unsigned frame=1;
 for(unsigned p=4*channels;p<bytes;p+=4*channels){for(unsigned c=0;c<channels;++c){unsigned v=word(in+p+4*c);for(unsigned k=0;k<8;++k){out[(frame+k)*channels+c]=std::int16_t(nibble(v&15,predictor[c],index[c]));v>>=4;}}frame+=8;}
 return frames;
}
bool audio_sample_open_v34(std::shared_ptr<const std::vector<std::uint8_t>>bytes,AudioSampleV34&out,std::string&error){try{
 if(!bytes||bytes->size()<12)throw std::runtime_error("Audio source header");Reader r{*bytes};AudioSampleV34 s;s.bytes=std::move(bytes);
 if(!std::memcmp(r.at(0,4),"RIFF",4)){
  if(std::memcmp(r.at(8,4),"WAVE",4))throw std::runtime_error("Original RIFF WAVE family");s.container_declared_size=r.w(4);
  // Actual IMA cache files sometimes retain an uncompressed RIFF size.
  // The stream/chunk extents, rather than that stale container field, bound IO.
  unsigned fact=0;bool fmt=false,data=false;const std::uint64_t end=r.b.size();
  for(std::uint64_t p=12;p+8<=end;){unsigned n=r.w(p+4);r.at(p+8,n);if(std::uint64_t(n)+p+8>end)throw std::runtime_error("RIFF chunk extent");
   if(!std::memcmp(r.at(p,4),"fmt ",4)){if(fmt||n<16)throw std::runtime_error("Source fmt chunk");const auto*f=r.at(p+8,n);s.format=half(f);s.channels=half(f+2);s.rate=word(f+4);s.block_align=half(f+12);s.bits=half(f+14);fmt=true;if(s.format==17&&(n<20||half(f+16)<2))throw std::runtime_error("Source IMA fmt extension");}
   else if(!std::memcmp(r.at(p,4),"fact",4)){if(n<4)throw std::runtime_error("Source fact extent");fact=r.w(p+8);}
   else if(!std::memcmp(r.at(p,4),"data",4)){if(data)throw std::runtime_error("Required multiple source WAVE data chunks");s.data_offset=unsigned(p+8);s.data_size=n;data=true;}
   p+=8+std::uint64_t(n)+(n&1);
  }if(!fmt||!data)throw std::runtime_error("Required original WAVE fmt/data");format(s);
  if(!s.data_size||s.data_size%s.block_align)throw std::runtime_error("Source audio whole block extent");const std::uint64_t frames=std::uint64_t(s.data_size/s.block_align)*s.frames_per_block;
  if(frames>0xffffffffu||(fact&&fact>frames))throw std::runtime_error("Source audio frame extent");s.segments.push_back({0,s.data_size,fact?fact:unsigned(frames),{}});
 }else if(!std::memcmp(r.at(0,4),"VoxN",4)){
  s.native=true;std::map<std::string,std::pair<unsigned,unsigned>> chunks;
  for(std::uint64_t p=0;p+8<=r.b.size();){unsigned n=r.w(p+4);r.at(p+8,n);std::string tag(reinterpret_cast<const char*>(r.at(p,4)),4);if(!chunks.emplace(tag,std::make_pair(unsigned(p+8),n)).second)throw std::runtime_error("Duplicate original VoxN chunk");p+=8+std::uint64_t(n);if(p==r.b.size())break;if(p>r.b.size())throw std::runtime_error("Native chunk extent");}
  auto get=[&](const char*name){auto it=chunks.find(name);if(it==chunks.end())throw std::runtime_error(std::string("Required original VoxN chunk ")+name);return it->second;};
  const auto header=get("VoxN");if(header.second!=16||std::memcmp(r.at(header.first,6),"0.0.1",6)||r.w(header.first+8)!=r.b.size())throw std::runtime_error("Original VoxN header version/size");
  const auto fmt=get("Afmt");if(fmt.second!=12)throw std::runtime_error("Original Afmt size");const auto*f=r.at(fmt.first,12);s.format=half(f);s.channels=half(f+2);s.rate=word(f+4);s.block_align=half(f+8);s.bits=half(f+10);format(s);
  const auto data=get("Data");s.data_offset=data.first;s.data_size=data.second;
  const auto seg=get("Segm");const unsigned n=r.w(seg.first);if(!n||n>256||seg.second!=4+24*n)throw std::runtime_error("Original Segm rows");
  for(unsigned i=0;i<n;++i){const auto*p=r.at(seg.first+4+24*i,24);AudioSegmentV34 v{word(p),word(p+4),word(p+8),{word(p+12),word(p+16),word(p+20)}};if(v.byte_offset>s.data_size||v.byte_size>s.data_size-v.byte_offset||v.byte_size%s.block_align||v.frames>std::uint64_t(v.byte_size/s.block_align)*s.frames_per_block||!v.frames)throw std::runtime_error("Original segment stream extent");if(v.source_fields[0]||v.source_fields[1]||v.source_fields[2])throw std::runtime_error("Required additional authored segment cue continuation");s.segments.push_back(v);}
  const auto stat=get("Stat");unsigned states=r.w(stat.first);if(!states||states>256||stat.second!=4+states*32)throw std::runtime_error("Original Stat rows");
  for(unsigned i=0;i<states;++i){const auto*p=r.at(stat.first+4+32*i,32);const auto*end=static_cast<const std::uint8_t*>(std::memchr(p+4,0,28));if(!end)throw std::runtime_error("Original state name range");AudioStateV34 state{word(p),std::string(reinterpret_cast<const char*>(p+4),reinterpret_cast<const char*>(end)),{}};std::memcpy(state.source_name_words.data(),p+4,28);s.states.push_back(std::move(state));}
  auto playlist=get("Plst");rows(r,playlist.first,playlist.second,s.playlists);auto group=get("Grps");rows(r,group.first,group.second,s.groups);
  auto element=get("Grpe");std::vector<std::array<std::int32_t,8>>elements;rows(r,element.first,element.second,elements);for(const auto&v:elements)s.elements.push_back({v});
  auto rules=get("Rule");std::vector<std::array<std::int32_t,9>>rule;rows(r,rules.first,rules.second,rule);for(const auto&v:rule){std::array<unsigned,9> raw;std::memcpy(raw.data(),v.data(),36);s.rules.push_back(raw);}
  auto transitions=get("Trsn");std::vector<std::array<std::int32_t,4>>trs;rows(r,transitions.first,transitions.second,trs);for(const auto&v:trs)s.transitions.push_back({v[0],v[1],v[2],v[3]});
  for(const auto&state:s.states)if(state.playlist>=s.playlists.size())throw std::runtime_error("Original state playlist");
  if(chunks.count("Cues"))throw std::runtime_error("Required authored explicit cues");
 }else throw std::runtime_error("Required original audio file family");
 out=std::move(s);error.clear();return true;
}catch(const std::exception&e){error=e.what();return false;}}
bool AudioSampleCursorV34::bind(const AudioSampleV34*s,unsigned segment)noexcept{if(!s||segment>=s->segments.size())return false;sample_=s;segment_=segment;block_=~0u;decoded_=0;return true;}
std::uint32_t AudioSampleCursorV34::frames()const noexcept{return sample_?sample_->segments[segment_].frames:0;}
bool AudioSampleCursorV34::frame(std::uint64_t index,float&left,float&right)noexcept{
 if(!sample_||index>=frames())return false;const auto&s=*sample_;const auto&segment=s.segments[segment_];const auto*data=s.bytes->data()+s.data_offset+segment.byte_offset;
 if(s.format==1){const auto*p=data+index*s.block_align;left=float(signed_half(p))/32768.f;right=s.channels==2?float(signed_half(p+2))/32768.f:left;return true;}
 const unsigned block=unsigned(index/s.frames_per_block),frame=unsigned(index%s.frames_per_block);
 if(block!=block_){if(std::uint64_t(block)*s.block_align+s.block_align>segment.byte_size)return false;decoded_=audio_ima_block_v34(data+std::uint64_t(block)*s.block_align,s.block_align,s.channels,cache_.data(),unsigned(cache_.size()));block_=block;}
 if(frame>=decoded_)return false;left=float(cache_[frame*s.channels])/32768.f;right=s.channels==2?float(cache_[frame*s.channels+1])/32768.f:left;return true;
}
}
