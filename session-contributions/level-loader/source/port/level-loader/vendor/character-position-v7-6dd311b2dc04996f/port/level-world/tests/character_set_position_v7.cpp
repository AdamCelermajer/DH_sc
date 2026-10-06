#include "../character_set_position_v7.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2;
static unsigned checks;
static void check(bool b,const char* e){++checks;if(!b)throw std::runtime_error(e);}
static std::uint32_t word(std::istream& stream){std::uint32_t v;stream.read(reinterpret_cast<char*>(&v),4);if(!stream)throw std::runtime_error("gold truncated");return v;}
static void floats(std::istream& stream,float* out,unsigned count){for(unsigned i=0;i<count;++i){auto v=word(stream);std::memcpy(out+i,&v,4);}}
int main(int argc,char** argv){try{
 check(argc==2,"original gold input");std::ifstream gold(argv[1],std::ios::binary);const auto count=word(gold);auto lease=std::make_shared<int>(1);
 for(unsigned n=0;n<count;++n){float current[3],relative[6],attached_xyz[3],destination[3],input[3];floats(gold,current,3);floats(gold,relative,6);floats(gold,attached_xyz,3);floats(gold,destination,3);floats(gold,input,3);
  auto flags=word(gold),alias=word(gold),reentry=word(gold);float expected_current[3],expected_relative[6],expected_attached[3],expected_destination[3];floats(gold,expected_current,3);floats(gold,expected_relative,6);floats(gold,expected_attached,3);floats(gold,expected_destination,3);auto expected_calls=word(gold);float expected_physical[2];floats(gold,expected_physical,2);
  std::uintptr_t attached=(flags&1)?0x10:0,physical=(flags&2)?0x20:0,visual=(flags&4)?0x30:0;float absolute[6]{};
  character::CharacterPositionBorrowV7 b{lease,0x40,current,relative,absolute,destination,&attached,&physical,&visual,{}};
  character::CharacterPositionServicesV7 services;services.world=lease;unsigned calls=0;float physical_xy[2]{};
  services.attached_position=[&](auto id,float*& p,auto&){check(id==0x10,"actual attached identity");calls=calls*10+1;p=attached_xyz;return true;};
  float* p=alias==1?current:alias==2?attached_xyz:alias==3?destination:input;
  services.physical_position=[&](auto id,float x,float y,auto&){check(id==0x20,"actual physical identity");calls=calls*10+2;physical_xy[0]=x;physical_xy[1]=y;if(reentry){visual=0;p[0]+=7.f;p[1]-=3.f;}return true;};
  services.visual_sync_position=[&](auto id,auto&){check(id==0x30,"actual visual identity");calls=calls*10+3;return true;};
  std::string error;character::CharacterPositionResultV7 result;check(character::character_set_position_v7(result,b,p,(flags&8)!=0,services,error),error.c_str());
  check(std::memcmp(current,expected_current,12)==0,"original current words");check(std::memcmp(absolute,expected_relative,24)==0,"original absolute bounds words");check(std::memcmp(attached_xyz,expected_attached,12)==0,"original attached words");check(std::memcmp(destination,expected_destination,12)==0,"original destination words");check(calls==expected_calls,"original callback order/pointer reread");check(std::memcmp(physical_xy,expected_physical,8)==0,"original physical XY arguments");
 }
 // Constructor-null visual/body/attachment reaches no external backend.
 actor::RuntimeState runtime{};character::CharacterPositionFieldsV7 fields;std::string error;check(fields.construct(runtime,error),"genuine inherited ctor fields");check(!fields.physical2dc&&!fields.attached2e0&&runtime.object.motion.floor==~0u,"original null/PF constructor fields");check(!fields.construct(runtime,error),"constructor cannot overwrite live fields twice");
 float position[3]{},p[3]{12,24,36};std::uintptr_t visual=0;character::CharacterPositionBorrowV7 b{lease,0x40,position,runtime.subobjects.local_bounds,runtime.subobjects.absolute_bounds,runtime.controller.destination,&fields.attached2e0,&fields.physical2dc,&visual,{}};character::CharacterPositionResultV7 result;
 const auto floor=runtime.object.motion.floor;runtime.controller.path_requested=1;check(character::character_set_position_v7(result,b,p,true,{},error),"null branches complete without fake providers");check(runtime.object.motion.floor==floor&&runtime.controller.path_requested==1,"SetPosition preserves original PF/path state");
 // Mandatory backend error leaves position/bounds and does not write destination.
 fields.physical2dc=0x20;float next[3]{100,200,300};auto before=runtime.controller.destination[0];character::CharacterPositionServicesV7 s;s.world=lease;s.physical_position=[](auto,float,float,std::string& e){e="actual missing physical tail";return false;};check(!character::character_set_position_v7(result,b,next,true,s,error)&&result.phase==character::CharacterPositionPhaseV7::bounds&&error=="actual missing physical tail","failure prefix diagnostic");check(position[0]==100&&runtime.controller.destination[0]==before,"no rollback/no false destination");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"null_prefix_and_failure\":true}"<<std::endl;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
