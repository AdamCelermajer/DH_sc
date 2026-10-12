#include "player_profile_index_v1.hpp"
#include <cstring>
#include <map>
#include <stdexcept>
namespace {
std::uint32_t word(const std::uint8_t* p){return std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
std::string key(const std::uint8_t* p){std::size_t n=0;while(n<4&&p[n])++n;return std::string(reinterpret_cast<const char*>(p),n);}
}
extern "C" int dh2_player_profile_v1_index(dh2::data::ProfileIndexSpan24V1* p,const dh2::data::ProfileIndexServices16V1* s) noexcept {
 if(!p||!s||!s->section||p->reserved||p->cursor||(!p->bytes&&p->size))return -1;
 if(p->size<4)return -2;auto count=word(p->bytes);if(count==UINT32_MAX)return -4;p->source_count=count;p->cursor=4;
 for(std::uint32_t i=0;i<count;++i){if(p->size-p->cursor<8)return -2;auto size=word(p->bytes+p->cursor);p->cursor+=4;dh2::data::ProfileSection12V1 section{};std::memcpy(section.tag,p->bytes+p->cursor,4);p->cursor+=4;section.offset=p->cursor;section.size=size;
  // The original indexes before seek; invalid seeks are rejected before
  // storage delivery here to avoid publishing out-of-bounds native spans.
  if(size>p->size-p->cursor)return -2;try{if(!s->section(s->context,&section))return -3;}catch(...){return -3;}p->cursor+=size;
 }return 0;
}
namespace dh2::data {
struct PlayerProfileIndexV1::Snapshot {std::vector<std::uint8_t> bytes;std::vector<ProfileSection12V1> sections;std::map<std::string,std::size_t,std::less<>> last;};
bool PlayerProfileIndexV1::load(Bytes b,std::string& e){if(snapshot_&&snapshot_.use_count()!=1){e="profile snapshot borrowed";return false;}if(b.size>UINT32_MAX||(!b.data&&b.size)){e="invalid profile byte span";return false;}
 try{auto next=std::make_shared<Snapshot>();if(b.size)next->bytes.assign(b.data,b.data+b.size);ProfileIndexSpan24V1 span{next->bytes.data(),static_cast<std::uint32_t>(next->bytes.size()),0,0,0};ProfileIndexServices16V1 s{next.get(),[](void* p,const ProfileSection12V1* section){try{auto& n=*static_cast<Snapshot*>(p);n.last[key(section->tag)]=n.sections.size();n.sections.push_back(*section);return true;}catch(...){return false;}}};
  auto status=dh2_player_profile_v1_index(&span,&s);if(status){e="source campaign section index failed: "+std::to_string(status);return false;}snapshot_=std::move(next);e.clear();return true;
 }catch(...){e="profile allocation failed";return false;}}
const std::vector<std::uint8_t>& PlayerProfileIndexV1::Borrow::bytes()const{if(!snapshot_)throw std::logic_error("missing profile");return snapshot_->bytes;}
const std::vector<ProfileSection12V1>& PlayerProfileIndexV1::Borrow::source_sections()const{if(!snapshot_)throw std::logic_error("missing profile");return snapshot_->sections;}
const ProfileSection12V1* PlayerProfileIndexV1::Borrow::section(const char* tag)const noexcept{if(!snapshot_||!tag)return nullptr;auto i=snapshot_->last.find(tag);return i==snapshot_->last.end()?nullptr:&snapshot_->sections[i->second];}
Bytes PlayerProfileIndexV1::Borrow::payload(const char* tag)const noexcept{auto s=section(tag);return s?Bytes{snapshot_->bytes.data()+s->offset,s->size}:Bytes{nullptr,0};}
}
