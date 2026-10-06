#include "game_design_tables.hpp"
#include <cstring>
#include <new>
namespace {
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool names(const dh2::data::DesignNames16* n){
 if(!aligned(n,alignof(dh2::data::DesignNames16))||n->reserved||n->count>65536||(n->count&&!aligned(n->names,alignof(const char*))))return false;
 for(std::uint32_t i=0;i<n->count;++i)if(!n->names[i])return false;
 return true;
}
bool overlaps(const void* out,const void* in,std::size_t size){auto a=reinterpret_cast<std::uintptr_t>(out),b=reinterpret_cast<std::uintptr_t>(in);return a<=b?b-a<4:a-b<size;}
}
extern "C" int dh2_game_design_find(std::int32_t* out,const dh2::data::DesignNames16* n,const char* key){
 if(!aligned(out,alignof(std::int32_t))||!key||!names(n)||overlaps(out,n,sizeof(*n))||(n->count&&overlaps(out,n->names,std::size_t(n->count)*sizeof(char*))))return 1;
 std::int32_t result=-1;
 for(std::uint32_t i=0;i<n->count;++i)if(!std::strcmp(key,n->names[i])){result=static_cast<std::int32_t>(i);break;}
 *out=result;return 0;
}
extern "C" int dh2_game_design_tables_lookup(void* opaque,std::uint32_t kind,const char* group,const char* key,std::int32_t* out){
 auto* r=static_cast<const dh2::data::DesignRegistry16*>(opaque);
 if(kind!=1||!aligned(r,alignof(dh2::data::DesignRegistry16))||r->reserved||r->count>65536||!group||!key||!aligned(out,alignof(std::int32_t))||overlaps(out,r,sizeof(*r))||(r->count&&(!aligned(r->registrations,alignof(dh2::data::DesignRegistration24))||overlaps(out,r->registrations,std::size_t(r->count)*sizeof(*r->registrations)))))return 1;
 for(std::uint32_t i=0;i<r->count;++i){auto* n=r->registrations[i].members;if(!r->registrations[i].group||r->registrations[i].reserved||!names(n)||overlaps(out,n,sizeof(*n))||(n->count&&overlaps(out,n->names,std::size_t(n->count)*sizeof(char*))))return 1;}
 for(std::uint32_t i=r->count;i;--i){auto& entry=r->registrations[i-1];if(!std::strcmp(group,entry.group))return dh2_game_design_find(out,entry.members,key);}
 *out=-1;return 0;
}
namespace dh2::data {
bool GameDesignTables::register_table(const char* group,const DesignNames16* members,std::string& error){
 error.clear();if(!group||!names(members)||entries_.size()>=65536){error="Invalid design registration";return false;}
 try{
  // Allocate every fallible object before changing the observable ordered list.
  auto next=entries_;next.push_back({std::string(group),members});
  std::vector<DesignRegistration24> projection;projection.reserve(next.size());
  for(auto& entry:next)projection.push_back({entry.group.c_str(),entry.members,0});
  entries_.swap(next);projection_.swap(projection);
  return true;
 }catch(const std::bad_alloc&){error="Design registration allocation failure";return false;}
}
DesignRegistry16 GameDesignTables::view() const{return {projection_.data(),static_cast<std::uint32_t>(projection_.size()),0};}
}
