#include "../app/src/main/cpp/native_quest_cursor.hpp"
#include <array>
#include <cstdio>
#include <stdexcept>
#include <vector>
unsigned checks=0;
void check(bool value){if(!value)throw std::runtime_error("Quest cursor check "+std::to_string(checks+1));++checks;}
int main(){try{
 namespace d=dh2::data;namespace n=dh2::native::quests;
 std::unique_ptr<n::Cursor> cursor;d::PlayerProfileIndexV1::Borrow backing;
 {d::PlayerProfileIndexV1 profile;std::string error;
  const std::vector<std::uint8_t> bytes={3,0,0,0,4,0,0,0,'P','N','A','M',1,2,3,4,
   3,0,0,0,'Q','E','S','T',5,6,7,2,0,0,0,'F','T','V','L',8,9};
  check(profile.load({bytes.data(),bytes.size()},error));
  backing=profile.borrow();
  cursor=std::make_unique<n::Cursor>(profile.borrow(),"QEST");
  check(!profile.load({bytes.data(),bytes.size()},error));
 }
 // Destroying the profile index retains exactly the borrowed immutable file.
 auto& stream=cursor->stream();std::uint64_t position=0,count=0;
 check(cursor->tell(stream,&position)&&position==24);
 std::array<std::uint8_t,16> buffer{};
 check(cursor->read(stream,buffer.data(),3,&count)&&count==3&&buffer[0]==5&&buffer[2]==7);
 check(cursor->tell(stream,&position)&&position==27);
 // Source stream spans the whole campaign, including the next section header.
 check(cursor->read(stream,buffer.data(),8,&count)&&count==8&&buffer[0]==2&&buffer[4]=='F'&&buffer[7]=='L');
 check(cursor->read(stream,buffer.data(),UINT64_MAX,&count)&&count==2&&buffer[0]==8&&buffer[1]==9);
 check(cursor->tell(stream,&position)&&position==37);
 check(cursor->read(stream,nullptr,4,&count)&&count==0);
 check(cursor->seek(stream,24));check(cursor->tell(stream,&position)&&position==24);
 check(cursor->read(stream,buffer.data(),1,&count)&&count==1&&buffer[0]==5);
 check(!cursor->seek(stream,UINT64_C(0x100000018)));check(cursor->tell(stream,&position)&&position==25);
 auto forged=stream;check(!cursor->tell(forged,&position));check(!cursor->read(forged,buffer.data(),1,&count));
 check(!cursor->read(stream,nullptr,1,&count));check(cursor->tell(stream,&position)&&position==25);
 count=0xabcdef;check(!cursor->read(stream,&count,4,&count));check(count==0xabcdef);
 check(!cursor->tell(stream,&stream.identity));check(cursor->tell(stream,&position)&&position==25);
 check(!cursor->read(stream,const_cast<std::uint8_t*>(backing.bytes().data()),1,&count));
 check(!cursor->tell(stream,reinterpret_cast<std::uint64_t*>(const_cast<d::ProfileSection12V1*>(backing.section("PNAM")))));
 check(!cursor->read(stream,const_cast<d::ProfileSection12V1*>(backing.section("QEST")),1,&count));
 check(!cursor->read(stream,const_cast<std::vector<std::uint8_t>*>(&backing.bytes()),1,&count));
 check(cursor->tell(stream,&position)&&position==25&&backing.section("QEST")->offset==24);
 check(cursor->seek(stream,37));check(cursor->read(stream,nullptr,0,&count)&&count==0);
 bool rejected=false;try{n::Cursor absent({},"QEST");}catch(const std::invalid_argument&){rejected=true;}check(rejected);
 std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"absolute_campaign_cursor\":true,\"retained_whole_profile\":true,\"source_typed_readers\":false}\n",checks);return 0;
 }catch(const std::exception& error){std::fprintf(stderr,"%s\n",error.what());return 1;}}
