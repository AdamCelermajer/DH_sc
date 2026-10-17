#include "world_map_tables.hpp"
#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>
namespace dh2::data {
namespace {
bool utf8(const std::string& text) {
    for(std::size_t i=0;i<text.size();) {
        const auto first=static_cast<unsigned char>(text[i++]);
        if(!first)return false;
        if(first<0x80)continue;
        std::uint32_t code=0,minimum=0;unsigned trailing=0;
        if(first>=0xc2&&first<=0xdf){code=first&31;trailing=1;minimum=0x80;}
        else if(first>=0xe0&&first<=0xef){code=first&15;trailing=2;minimum=0x800;}
        else if(first>=0xf0&&first<=0xf4){code=first&7;trailing=3;minimum=0x10000;}
        else return false;
        if(trailing>text.size()-i)return false;
        for(unsigned n=0;n<trailing;++n){const auto byte=static_cast<unsigned char>(text[i++]);if((byte&0xc0)!=0x80)return false;code=(code<<6)|(byte&63);}
        if(code<minimum||code>0x10ffff||(code>=0xd800&&code<=0xdfff))return false;
    }
    return true;
}
struct Reader {
    Bytes bytes;std::size_t cursor=0;
    explicit Reader(Bytes input):bytes(input){if(!input.data||input.size<4||input.size>8*1024*1024)throw std::runtime_error("WorldMap input outside native bound");}
    void require(std::size_t size) const {if(cursor>bytes.size||size>bytes.size-cursor)throw std::runtime_error("truncated WorldMap input");}
    std::uint32_t word(){require(4);const auto* p=bytes.data+cursor;cursor+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
    std::int32_t integer(){auto bits=word();std::int32_t value;std::memcpy(&value,&bits,4);return value;}
    std::uint32_t count(){auto n=word();if(n>4096)throw std::runtime_error("WorldMap count outside native bound");return n;}
    std::string text(){auto size=word();if(size>4096)throw std::runtime_error("WorldMap name outside native bound");require(size);std::string value(reinterpret_cast<const char*>(bytes.data+cursor),size);cursor+=size;if(!utf8(value))throw std::runtime_error("invalid WorldMap UTF-8 name");return value;}
    std::vector<std::string> names(){std::vector<std::string> values;std::set<std::string> unique;auto n=count();for(unsigned i=0;i<n;++i){auto name=text();if(name.empty()||!unique.insert(name).second)throw std::runtime_error("empty or duplicate WorldMap name");values.push_back(std::move(name));}return values;}
    void finish()const{if(cursor!=bytes.size)throw std::runtime_error("unexpected WorldMap suffix");}
};
}
bool load_world_map(Bytes records,Bytes names,Bytes schema,WorldMapTables& output,std::string& error) {
    try {
        Reader data(records),keys(names),layout(schema);
        auto locations=keys.names(),lockers=keys.names();keys.finish();
        const std::vector<std::string> location_schema{"Name","State","LocationLevels"},locker_schema{"OnState","QuestID"};
        if(layout.names()!=location_schema||layout.names()!=locker_schema)throw std::runtime_error("WorldMap schema differs");
        layout.finish();WorldMapTables next;
        if(data.count()!=locations.size())throw std::runtime_error("WorldMap location count differs");
        for(auto& name:locations) {
            WorldMapLocation row;row.name=std::move(name);row.name_id=data.integer();row.state=data.integer();
            auto count=data.count();row.location_levels.reserve(count);
            for(unsigned i=0;i<count;++i)row.location_levels.push_back(data.integer());
            next.locations.push_back(std::move(row));
        }
        if(data.count()!=lockers.size())throw std::runtime_error("WorldMap locker count differs");
        for(auto& name:lockers){WorldMapLocker row;row.name=std::move(name);row.on_state=data.integer();row.quest_id=data.integer();next.lockers.push_back(std::move(row));}
        data.finish();output=std::move(next);error.clear();return true;
    }catch(const std::exception& failure){error=failure.what();return false;}
    catch(...){error="WorldMap allocation/provider failure";return false;}
}
std::int32_t find_world_map_location(const WorldMapTables& tables,const std::string& name) noexcept {
    for(std::size_t i=0;i<tables.locations.size();++i)if(tables.locations[i].name==name)return static_cast<std::int32_t>(i);
    return -1;
}
bool read_world_map_default_word(const WorldMapTables& tables,std::uint32_t row,std::int32_t& output) noexcept {
    if(row>=tables.locations.size())return false;
    output=tables.locations[row].state;return true;
}
} // namespace dh2::data
