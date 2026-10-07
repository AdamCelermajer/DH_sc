#include "world_map_profile_table_v59.hpp"
#include <cstring>
namespace dh2::data {namespace {
struct Reader {
 Bytes b;std::size_t at{};std::string& e;
 bool word(std::uint32_t& v){if(at>b.size||b.size-at<4){e="Truncated actual WorldMap word";return false;}const auto* p=b.data+at;at+=4;
  v=std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;return true;}
 bool integer(std::int32_t& v){std::uint32_t raw;if(!word(raw))return false;std::memcpy(&v,&raw,4);return true;}
 bool count(std::uint32_t& n,std::size_t minimum){if(!word(n))return false;if(n>(b.size-at)/minimum){e="Unsafe actual WorldMap array count";return false;}return true;}
 bool strings(std::vector<std::string>& out){std::uint32_t n;if(!count(n,4))return false;out.resize(n);
  for(auto& s:out){std::uint32_t length;if(!word(length))return false;if(length>b.size-at){e="Truncated actual WorldMap CString";return false;}
   if(length){s.assign(reinterpret_cast<const char*>(b.data+at),length);if(s.back()=='\0')s.pop_back();}at+=length;
  }return true;
 }
};
}
bool WorldMapProfileTableV59::decode(Bytes records,Bytes names,Bytes schema,std::string& e){
 if(ready_){e="Actual immutable WorldMap profile table cannot replay";return false;}
 if((!records.data&&records.size)||(!names.data&&names.size)||(!schema.data&&schema.size)){
  e="Required actual WorldMap cache spans";return false;
 }
 Reader r{records,0,e},n{names,0,e},s{schema,0,e};std::vector<std::string> map_names,locker_names,map_fields,locker_fields;
 if(!n.strings(map_names)||!n.strings(locker_names)||!s.strings(map_fields)||!s.strings(locker_fields))return false;
 if(n.at!=names.size||s.at!=schema.size||map_fields!=std::vector<std::string>{"Name","State","LocationLevels"}||
    locker_fields!=std::vector<std::string>{"OnState","QuestID"}){e="Actual WorldMap source schema/name suffix differs";return false;}
 std::uint32_t count;if(!r.count(count,12))return false;
 if(count!=map_names.size()){e="Actual WorldMap source row/name count differs";return false;}
 std::vector<WorldMapLocationProfileV59> rows(count);std::vector<std::int32_t> defaults;defaults.reserve(count);
 for(auto& row:rows){if(!r.integer(row.name4)||!r.integer(row.state8))return false;
  std::uint32_t size;if(!r.count(size,4))return false;row.location_levels.resize(size);
  for(auto& value:row.location_levels)if(!r.integer(value))return false;
  defaults.push_back(row.state8);
 }
 if(!r.count(count,8))return false;if(count!=locker_names.size()){e="Actual WorldMapLocker source row/name count differs";return false;}
 std::vector<WorldMapLockerProfileV59> lockers(count);
 for(auto& row:lockers)if(!r.integer(row.on_state4)||!r.integer(row.quest_id8))return false;
 if(r.at!=records.size){e="Unconsumed actual WorldMap source records";return false;}
 names_=std::move(map_names);locker_names_=std::move(locker_names);rows_=std::move(rows);lockers_=std::move(lockers);defaults8_=std::move(defaults);
 ready_=true;e.clear();return true;
}
}
