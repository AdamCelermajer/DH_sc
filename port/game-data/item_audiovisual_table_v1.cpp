#include "item_audiovisual_table_v1.hpp"
#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>

namespace dh2::data {
namespace {
constexpr std::size_t byte_limit=8*1024*1024;
constexpr std::uint32_t count_limit=4096;
constexpr std::uint32_t string_limit=65536;

bool valid_utf8(const std::string& text) {
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
        for(unsigned n=0;n<trailing;++n) {
            const auto byte=static_cast<unsigned char>(text[i++]);
            if((byte&0xc0)!=0x80)return false;
            code=(code<<6)|(byte&63);
        }
        if(code<minimum||code>0x10ffff||(code>=0xd800&&code<=0xdfff))return false;
    }
    return true;
}

struct Reader {
    Bytes bytes;std::size_t cursor=0;
    explicit Reader(Bytes input):bytes(input) {
        const auto address=reinterpret_cast<std::uintptr_t>(input.data);
        if(!input.data||!input.size||input.size>byte_limit||input.size>UINTPTR_MAX-address)
            throw std::runtime_error("ItemAudioVisual input outside native bound");
    }
    void require(std::size_t size)const {
        if(cursor>bytes.size||size>bytes.size-cursor)
            throw std::runtime_error("Truncated ItemAudioVisual input");
    }
    std::uint32_t word() {
        require(4);const auto* p=bytes.data+cursor;cursor+=4;
        return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
    }
    std::int32_t integer() {
        const auto bits=word();std::int32_t value;std::memcpy(&value,&bits,sizeof value);return value;
    }
    std::uint32_t count() {
        const auto value=word();
        if(value>count_limit)throw std::runtime_error("ItemAudioVisual count outside native bound");
        return value;
    }
    std::string text() {
        const auto size=word();
        if(size>string_limit)throw std::runtime_error("ItemAudioVisual string outside native bound");
        require(size);
        std::string value(reinterpret_cast<const char*>(bytes.data+cursor),size);cursor+=size;
        if(value.empty()||!valid_utf8(value))throw std::runtime_error("Empty or invalid ItemAudioVisual string");
        return value;
    }
    std::vector<std::string> strings() {
        const auto size=count();std::vector<std::string> result;result.reserve(size);
        std::set<std::string> unique;
        for(std::uint32_t i=0;i<size;++i) {
            auto value=text();
            if(!unique.insert(value).second)throw std::runtime_error("Duplicate ItemAudioVisual name");
            result.push_back(std::move(value));
        }
        return result;
    }
    void finish()const {
        if(cursor!=bytes.size)throw std::runtime_error("Unexpected ItemAudioVisual input suffix");
    }
};
}

struct ItemAudioVisualTableV1::Snapshot {
    std::vector<ItemAudioVisualRowV1> rows;
};

bool ItemAudioVisualTableV1::load(Bytes records,Bytes names,Bytes schema,std::string& error) {
    try {
        Reader data(records),keys(names),layout(schema);
        auto identifiers=keys.strings();keys.finish();
        const std::vector<std::string> expected_schema{"AudioDrop","AudioPickup","Visual"};
        if(layout.strings()!=expected_schema)throw std::runtime_error("ItemAudioVisual schema differs");
        layout.finish();
        const auto count=data.count();
        if(count!=identifiers.size())throw std::runtime_error("ItemAudioVisual row/name count differs");
        auto next=std::make_shared<Snapshot>();next->rows.reserve(count);
        for(std::uint32_t i=0;i<count;++i) {
            ItemAudioVisualRowV1 row;row.identifier=std::move(identifiers[i]);
            row.audio_drop=data.integer();row.audio_pickup=data.integer();row.visual=data.text();
            next->rows.push_back(std::move(row));
        }
        data.finish();snapshot_=std::move(next);error.clear();return true;
    } catch(const std::exception& failure) {
        error=failure.what();return false;
    } catch(...) {
        error="ItemAudioVisual allocation/provider failure";return false;
    }
}

const std::vector<ItemAudioVisualRowV1>& ItemAudioVisualTableV1::Borrow::rows()const {
    if(!snapshot_)throw std::logic_error("Unbound ItemAudioVisual table");
    return snapshot_->rows;
}

const ItemAudioVisualRowV1* ItemAudioVisualTableV1::Borrow::get(std::int32_t id)const noexcept {
    if(!snapshot_||id<0||std::size_t(id)>=snapshot_->rows.size())return nullptr;
    return &snapshot_->rows[std::size_t(id)];
}

std::int32_t ItemAudioVisualTableV1::Borrow::id(const std::string& identifier)const noexcept {
    if(!snapshot_)return -1;
    for(std::size_t i=0;i<snapshot_->rows.size();++i)
        if(snapshot_->rows[i].identifier==identifier)return static_cast<std::int32_t>(i);
    return -1;
}
} // namespace dh2::data
