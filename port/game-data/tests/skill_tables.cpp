#include "../skill_tables.hpp"
#include "../../level-world/character_cancel_sneaking.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;using Raw=std::vector<std::uint8_t>;
unsigned checks=0,records=0,lists=0,tables=0,queries=0,guards=0,owned_rows=0;
void check_at(bool b,int line){++checks;if(!b)throw std::runtime_error("Skill tables audit failed line "+std::to_string(line));}
#define check(x) check_at((x),__LINE__)
unsigned word(std::istream& f){unsigned v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
Raw blob(std::istream& f){Raw b(word(f));f.read(reinterpret_cast<char*>(b.data()),b.size());check(bool(f));return b;}
Raw file(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f));return Raw(std::istreambuf_iterator<char>(f),{});}
void push(Raw& b,unsigned w){for(unsigned i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(w>>(i*8)));}
void span(Raw& b,const Raw& s){push(b,s.size());b.insert(b.end(),s.begin(),s.end());}
Bytes bytes(const Raw& b){return {b.data(),b.size()};}
Raw name_blocks(unsigned nl,unsigned ns){Raw b;push(b,nl);for(unsigned i=0;i<nl;++i)span(b,{'d','u','p',0,'a'});push(b,ns);for(unsigned i=0;i<ns;++i)span(b,{'d','u','p',0,'b'});return b;}
void equal(const SkillRecord& r,const SkillProjection76& p,const Raw& display,const Raw& script,const Raw& icon){
 check(!std::memcmp(&r.scalar,&p,76));check(r.display_props.size()*4==display.size());if(!display.empty())check(!std::memcmp(r.display_props.data(),display.data(),display.size()));check(r.script==std::string(script.begin(),script.end()));check(r.icon==std::string(icon.begin(),icon.end()));
}
int main(int argc,char** argv){try{
 check(argc==5);auto data=file(argv[2]),names=file(argv[3]),schema=file(argv[4]);SkillTables owner;std::string error;check(owner.load(bytes(data),bytes(names),bytes(schema),error));auto pinned=owner.borrow();check(pinned.lists().size()==36&&pinned.skills().size()==127&&pinned.list_table_consumed()==1000&&pinned.records_consumed()==11862&&pinned.names_consumed()==2757&&pinned.schema_consumed()==233);
 std::ifstream gold(argv[1],std::ios::binary);check(word(gold)==0x31544b53);auto nr=word(gold),nl=word(gold),nt=word(gold),nq=word(gold);check(nr==383&&nl==164&&nt==65&&nq==195);
 for(unsigned i=0;i<nr;++i){auto b=blob(gold);SkillProjection76 want{};gold.read(reinterpret_cast<char*>(&want),76);check(bool(gold));auto display=blob(gold),script=blob(gold),icon=blob(gold);SkillProjection76 out{};SkillSpans48 view{};unsigned used=0;check(!dh2_skill_decode_record(&out,&view,&used,b.data(),b.size())&&used==b.size()&&!std::memcmp(&out,&want,76));Raw* payload[]={&display,&script,&icon};SkillSpan16* spans[]={&view.display,&view.script,&view.icon};for(unsigned j=0;j<3;++j){check(!spans[j]->reserved&&spans[j]->bytes==payload[j]->size());if(spans[j]->bytes)check(!std::memcmp(spans[j]->data,payload[j]->data(),spans[j]->bytes));}
  if(i<127)equal(pinned.skills()[i],want,display,script,icon);
  else{Raw synthetic;push(synthetic,0);push(synthetic,1);synthetic.insert(synthetic.end(),b.begin(),b.end());auto ns=name_blocks(0,1);SkillTables test;check(test.load(bytes(synthetic),bytes(ns),bytes(schema),error));equal(test.borrow().skills()[0],want,display,script,icon);++owned_rows;}++records;
 }
 for(unsigned i=0;i<nl;++i){auto b=blob(gold),want=blob(gold);SkillSpan16 out{};unsigned used=0;check(!dh2_skill_decode_list(&out,&used,b.data(),b.size())&&used==b.size()&&out.bytes==want.size());if(out.bytes)check(!std::memcmp(out.data,want.data(),want.size()));if(i<36){check(pinned.lists()[i].size()*4==want.size());if(!want.empty())check(!std::memcmp(pinned.lists()[i].data(),want.data(),want.size()));}++lists;}
 for(unsigned i=0;i<nt;++i){auto b=blob(gold);SkillSummary16 want{};gold.read(reinterpret_cast<char*>(&want),16);check(bool(gold));SkillSummary16 out{};check(!dh2_skill_tables_measure(&out,b.data(),b.size())&&!std::memcmp(&out,&want,16));if(i){auto ns=name_blocks(want.lists,want.skills);SkillTables test;check(test.load(bytes(b),bytes(ns),bytes(schema),error));auto v=test.borrow();check(v.lists().size()==want.lists&&v.skills().size()==want.skills&&v.records_consumed()==want.records_end);if(want.lists)check(v.list_index("dup")==0);if(want.skills)check(v.skill_index("dup")==0);}++tables;}
 for(unsigned i=0;i<nq;++i){auto kind=word(gold);auto key=blob(gold);key.push_back(0);auto want=word(gold);const char* k=reinterpret_cast<const char*>(key.data());auto result=kind==0?pinned.list_index(k):kind==1?pinned.skill_index(k):kind==2?pinned.list_field(k):pinned.skill_field(k);check(static_cast<unsigned>(result)==want);++queries;}check(gold.peek()==EOF);check(pinned.skill_field("Flags")==5&&pinned.list_field("List")==0);
 check(!owner.load(bytes(data),bytes(names),bytes(schema),error)&&error.find("pinned")!=std::string::npos);++guards;pinned={};
 for(unsigned i=0;i<data.size();++i){check(!owner.load({data.data(),i},bytes(names),bytes(schema),error));check(owner.borrow().skills().size()==127);++guards;}
 for(unsigned i=0;i<names.size();++i){check(!owner.load(bytes(data),{names.data(),i},bytes(schema),error));check(owner.borrow().lists().size()==36);++guards;}
 for(unsigned i=0;i<schema.size();++i){check(!owner.load(bytes(data),bytes(names),{schema.data(),i},error));check(owner.borrow().skill_fields().size()==15);++guards;}
 auto wrong=schema;wrong[8]^=1;check(!owner.load(bytes(data),bytes(names),bytes(wrong),error));++guards;
 SkillProjection76 out{};SkillSpans48 views{};unsigned used=0xa5a5a5a5;std::memset(&out,0xa5,76);SkillProjection76 before=out;Raw truncated(50);check(dh2_skill_decode_record(&out,&views,&used,truncated.data(),truncated.size())==1&&used==0xa5a5a5a5&&!std::memcmp(&out,&before,76));++guards;
 alignas(SkillProjection76) std::uint8_t unaligned[77]{};check(dh2_skill_decode_record(reinterpret_cast<SkillProjection76*>(unaligned+1),&views,&used,data.data(),data.size())==1);++guards;
 check(dh2_skill_tables_measure(reinterpret_cast<SkillSummary16*>(data.data()),data.data(),data.size())==1);++guards;
 Raw zero;push(zero,0);push(zero,0);SkillTables empty;check(empty.load(bytes(zero),bytes(zero),bytes(schema),error)&&empty.borrow().skills().empty());
 for(unsigned i=0;i<8;++i){check(!empty.load(bytes(zero),{zero.data(),i},bytes(schema),error));++guards;}
 Raw oversized;push(oversized,65537);SkillSummary16 summary{};check(dh2_skill_tables_measure(&summary,oversized.data(),oversized.size())==1);++guards;
 SkillSpan16 list_span{};check(dh2_skill_decode_list(&list_span,&used,oversized.data(),oversized.size())==1);++guards;
 check(dh2_skill_decode_record(&out,reinterpret_cast<SkillSpans48*>(&out),&used,truncated.data(),truncated.size())==1);++guards;
 // Source references are raw values; loading does not synthesize/rewrite them.
 Raw references;push(references,1);push(references,2);push(references,0xffffffff);push(references,0x80000000);push(references,0);auto ns=name_blocks(1,0);SkillTables raw;check(raw.load(bytes(references),bytes(ns),bytes(schema),error));check(raw.borrow().lists()[0][0]==-1&&raw.borrow().lists()[0][1]==static_cast<std::int32_t>(0x80000000u));
 pinned=owner.borrow();auto retained_script=pinned.skills()[1].script;auto retained_display=pinned.skills()[1].display_props;owner=SkillTables{};data.clear();data.shrink_to_fit();names.clear();names.shrink_to_fit();schema.clear();schema.shrink_to_fit();check(pinned.skills()[1].script==retained_script&&pinned.skills()[1].display_props==retained_display&&pinned.skill_index("AAA_FAKE_DONT_DELETE")==0);
 // Actual owned backing supplies frozen CancelSneaking: no required buff/skill
 // body is fabricated. Nonplayer query delivered, actual cache has noSneakbit.
 using namespace dh2::character::sneaking;std::vector<Skill76> rows(pinned.skills().size());std::vector<List16> projected(pinned.lists().size());for(unsigned i=0;i<rows.size();++i)std::memcpy(rows[i].words,pinned.skills()[i].scalar.words,76);for(unsigned i=0;i<projected.size();++i)projected[i]={pinned.lists()[i].data(),static_cast<unsigned>(pinned.lists()[i].size()),0};Tables32 tables_view{projected.data(),static_cast<unsigned>(projected.size()),0,rows.data(),static_cast<unsigned>(rows.size()),0};std::array<std::int32_t,224> properties{};properties[28]=14;properties[198]=1;Character48 character{0x100000001,{properties.data(),224,0},&tables_view,nullptr,0,{}};unsigned calls=0;Services16 service{&calls,[](void* p,const Request24* r,std::uint32_t* result){if(r->operation!=is_player)return 1;++*static_cast<unsigned*>(p);*result=0;return 0;}};check(!dh2_character_cancel_sneaking(&character,&service)&&calls==1&&character.changed415==0);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"record_comparisons\":"<<records<<",\"list_comparisons\":"<<lists<<",\"table_comparisons\":"<<tables<<",\"lookup_comparisons\":"<<queries<<",\"owned_synthetic_rows\":"<<owned_rows<<",\"actual_skills\":127,\"actual_lists\":36,\"native_guards\":"<<guards<<",\"pin_after_owner_and_inputs_destroyed\":true,\"owned_CancelSneaking_calls\":"<<calls<<",\"whole_Application_registration\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
