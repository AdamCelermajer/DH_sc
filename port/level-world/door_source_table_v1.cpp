#include "door_source_table_v1.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh2::door_source_table_v1 {
namespace {

constexpr std::size_t kMaxBytes = 8u * 1024u * 1024u;
constexpr std::uint32_t kMaxRows = 4096;
constexpr std::uint32_t kMaxString = 4096;

class Reader {
public:
    explicit Reader(Bytes bytes) : bytes_(bytes) {
        if (!bytes.data || bytes.size < 4 || bytes.size > kMaxBytes)
            throw std::runtime_error("Door source table byte span is invalid");
    }
    std::uint32_t word() {
        require(4);
        const auto* p = bytes_.data + cursor_;
        cursor_ += 4;
        return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
            (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
    }
    std::int32_t signed_word() {
        const auto raw = word();
        std::int32_t value{};
        std::memcpy(&value, &raw, sizeof(value));
        return value;
    }
    std::string text(std::uint32_t length) {
        if (length > kMaxString) throw std::runtime_error("Door source string exceeds limit");
        require(length);
        std::string out(reinterpret_cast<const char*>(bytes_.data + cursor_), length);
        cursor_ += length;
        if (out.find('\0') != std::string::npos)
            throw std::runtime_error("Door source string contains an embedded NUL");
        return out;
    }
    std::vector<std::string> strings() {
        const auto count = word();
        if (!count || count > kMaxRows)
            throw std::runtime_error("Door source name count exceeds limit");
        std::vector<std::string> out;
        out.reserve(count);
        for (std::uint32_t i=0;i<count;++i) out.push_back(text(word()));
        return out;
    }
    void skip(std::size_t n) { require(n); cursor_ += n; }
    std::size_t cursor() const noexcept { return cursor_; }
    std::size_t size() const noexcept { return bytes_.size; }
private:
    void require(std::size_t n) const {
        if (cursor_ > bytes_.size || n > bytes_.size-cursor_)
            throw std::runtime_error("Door source table is truncated");
    }
    Bytes bytes_{};
    std::size_t cursor_{};
};

void require_fields(const std::vector<std::string>& fields,
                    const char* const* expected, std::size_t count,
                    const char* label) {
    if (fields.size()!=count) throw std::runtime_error(
        std::string(label)+" source schema width differs");
    for (std::size_t i=0;i<count;++i) if(fields[i]!=expected[i])
        throw std::runtime_error(std::string(label)+" source schema order differs");
}

void skip_container_rows(Reader& data, std::uint32_t count) {
    for(std::uint32_t i=0;i<count;++i) {
        data.skip(7u*4u+1u+2u*4u);
        data.text(data.word());
        data.skip(4u*4u);
    }
}

} // namespace

bool Tables::load(Bytes game_objects, Bytes game_object_names,
                  Bytes game_object_fields, Bytes visual_dictionary,
                  Bytes visual_dictionary_names, std::string& error) {
    try {
        Reader data(game_objects), names(game_object_names), fields(game_object_fields);
        Reader dictionary_values(visual_dictionary), dictionary_names(visual_dictionary_names);
        const auto container_names = names.strings();
        const auto door_names = names.strings();
        const auto container_fields = fields.strings();
        const auto door_fields = fields.strings();
        static constexpr const char* container_schema[] = {
            "AirResist","DamageReduction","EarthResist","Effect","FireResist",
            "HitPoints","InteractSound","KeepSolid","LightningResist","Loot",
            "Script","TrapChance","TrapType","Visual","WaterResist"};
        static constexpr const char* door_schema[] = {
            "Script","SoundClose","SoundOpen","Visual"};
        require_fields(container_fields,container_schema,15,"DestructibleContainer");
        require_fields(door_fields,door_schema,4,"Door");
        if (data.word()!=container_names.size())
            throw std::runtime_error("DestructibleContainer names/count mismatch before Doors");
        skip_container_rows(data,static_cast<std::uint32_t>(container_names.size()));
        const auto count=data.word();
        if (!count||count>kMaxRows||count!=door_names.size())
            throw std::runtime_error("Door names/count mismatch");
        std::vector<Row> rows;
        rows.reserve(count);
        for(std::uint32_t i=0;i<count;++i) {
            Row row{};
            row.script=data.text(data.word());
            row.sound_close=data.signed_word();
            row.sound_open=data.signed_word();
            row.visual=data.signed_word();
            rows.push_back(std::move(row));
        }
        auto next_visual_names=dictionary_names.strings();
        auto next_visual_paths=dictionary_values.strings();
        if(next_visual_names.size()!=next_visual_paths.size()||
           dictionary_names.cursor()!=visual_dictionary_names.size||
           dictionary_values.cursor()!=visual_dictionary.size)
            throw std::runtime_error("GameObjectDict names/values differ or have trailing bytes");
        names_=door_names;
        rows_=std::move(rows);
        visual_names_=std::move(next_visual_names);
        visual_paths_=std::move(next_visual_paths);
        error.clear();
        return true;
    } catch(const std::exception& e) {
        error=e.what(); return false;
    } catch(...) {
        error="Door source table allocation failed"; return false;
    }
}

const Row* Tables::find(std::string_view name,std::int32_t* data_id) const noexcept {
    const auto found=std::find(names_.begin(),names_.end(),name);
    if(found==names_.end())return nullptr;
    const auto index=static_cast<std::size_t>(found-names_.begin());
    if(index>=rows_.size()||index>static_cast<std::size_t>(INT32_MAX))return nullptr;
    if(data_id)*data_id=static_cast<std::int32_t>(index);
    return &rows_[index];
}
const std::string* Tables::visual_path(std::int32_t id) const noexcept {
    return id<0||static_cast<std::size_t>(id)>=visual_paths_.size()?nullptr:
        &visual_paths_[static_cast<std::size_t>(id)];
}
const std::string* Tables::data_name(std::int32_t id) const noexcept {
    return id<0||static_cast<std::size_t>(id)>=names_.size()?nullptr:
        &names_[static_cast<std::size_t>(id)];
}

} // namespace dh2::door_source_table_v1
