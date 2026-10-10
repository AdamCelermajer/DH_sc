#include "runtime_loot_source_owner_v1.hpp"

#include "../../content_paths.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include "../../playable_actor_world.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <exception>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace dh::foundation::loot {
namespace {
using Bytes = dh2::data::Bytes;

constexpr std::array<std::uint32_t,64> sha_round={
 0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
 0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
 0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
 0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
 0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
 0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
 0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
 0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
std::uint32_t rotate(std::uint32_t x,unsigned n){return (x>>n)|(x<<(32-n));}
std::string sha256(const std::vector<std::uint8_t>& source){
    std::vector<std::uint8_t> data(source);const auto bit_count=std::uint64_t(data.size())*8;
    data.push_back(0x80);while(data.size()%64!=56)data.push_back(0);
    for(int shift=56;shift>=0;shift-=8)data.push_back(std::uint8_t(bit_count>>shift));
    std::array<std::uint32_t,8> state={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    for(std::size_t offset=0;offset<data.size();offset+=64){
        std::array<std::uint32_t,64> w{};
        for(unsigned i=0;i<16;++i){const auto p=offset+i*4;w[i]=(std::uint32_t(data[p])<<24)|(std::uint32_t(data[p+1])<<16)|(std::uint32_t(data[p+2])<<8)|data[p+3];}
        for(unsigned i=16;i<64;++i){const auto a=rotate(w[i-15],7)^rotate(w[i-15],18)^(w[i-15]>>3);const auto b=rotate(w[i-2],17)^rotate(w[i-2],19)^(w[i-2]>>10);w[i]=w[i-16]+a+w[i-7]+b;}
        auto a=state[0],b=state[1],c=state[2],d=state[3],e=state[4],f=state[5],g=state[6],h=state[7];
        for(unsigned i=0;i<64;++i){const auto s1=rotate(e,6)^rotate(e,11)^rotate(e,25);const auto ch=(e&f)^(~e&g);const auto t1=h+s1+ch+sha_round[i]+w[i];const auto s0=rotate(a,2)^rotate(a,13)^rotate(a,22);const auto maj=(a&b)^(a&c)^(b&c);const auto t2=s0+maj;h=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;}
        state[0]+=a;state[1]+=b;state[2]+=c;state[3]+=d;state[4]+=e;state[5]+=f;state[6]+=g;state[7]+=h;
    }
    std::ostringstream out;out<<std::hex<<std::setfill('0');for(auto word:state)out<<std::setw(8)<<word;return out.str();
}

struct BoundedReader {
    Bytes bytes;std::size_t at{};
    std::uint32_t word(){if(!bytes.data||at>bytes.size||bytes.size-at<4)throw std::runtime_error("Truncated original LootTables suffix");const auto* p=bytes.data+at;at+=4;return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
    std::uint32_t count(){const auto value=word();if(value>65536)throw std::runtime_error("Original LootTables suffix count exceeds V88 budget");return value;}
    std::int32_t integer(){const auto value=word();std::int32_t result;std::memcpy(&result,&value,4);return result;}
    std::vector<std::string> strings(){const auto n=count();std::vector<std::string> result;result.reserve(n);for(std::uint32_t i=0;i<n;++i){const auto size=count();if(at>bytes.size||size>bytes.size-at)throw std::runtime_error("Truncated original LootTables suffix string");result.emplace_back(reinterpret_cast<const char*>(bytes.data+at),size);at+=size;}return result;}
    void skip(std::size_t n){if(at>bytes.size||n>bytes.size-at)throw std::runtime_error("Truncated original LootTables suffix records");at+=n;}
};

const dh::foundation::frontend::creation::RuntimeCreationSourceHashV1* receipt_for(
    const dh::foundation::frontend::creation::RuntimeCreationSourceOwnerV1& source,const std::string& path){
    for(const auto& receipt:source.source_hashes)if(receipt.asset_path==path)return &receipt;
    return nullptr;
}

bool read_asset(const AssetCatalog& assets,const char* uri,std::vector<std::uint8_t>& bytes,
                std::string& relative,std::string& digest,std::string& error){
    try{const auto resolved=resolve_content_path(assets,uri);bytes=read_content(assets,uri);
        if(bytes.size()>8u*1024u*1024u){error=std::string("Original loot resource exceeds V88 native byte bound: ")+uri;return false;}
        relative=resolved.lexically_relative(assets.root()).generic_string();digest=sha256(bytes);
        error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}

bool read_same_creation_asset(const AssetCatalog& assets,
    const dh::foundation::frontend::creation::RuntimeCreationSourceOwnerV1& creation,const char* uri,
    std::vector<std::uint8_t>& bytes,std::string& error){
    std::string relative,digest;if(!read_asset(assets,uri,bytes,relative,digest,error))return false;
    const auto* receipt=receipt_for(creation,relative);
    if(!receipt||receipt->byte_count!=bytes.size()||receipt->sha256!=digest){error=std::string("Asset differs from the exact RuntimeCreationSourceOwner receipt: ")+uri;return false;}
    return true;
}

bool load_character_caps(const std::vector<std::uint8_t>& bytes,std::array<std::int32_t,3>& output,std::string& error){
    std::unique_ptr<dh2_script_constants,decltype(&dh2_script_constants_destroy)> constants(
        dh2_script_constants_create(),&dh2_script_constants_destroy);
    if(!constants){error="CharacterDesign constants owner allocation failed";return false;}
    dh2_script_constants_reload receipt{};
    if(bytes.size()>UINT32_MAX||dh2_script_constants_load(constants.get(),bytes.data(),std::uint32_t(bytes.size()),&receipt)||receipt.consumed!=bytes.size()){
        error="Original CharacterDesign constants did not load completely";return false;}
    constexpr const char* names[]={"MaxLevelBNormal","MaxLevelCHard","MaxLevelDVeryHard"};
    std::array<std::int32_t,3> next{};
    for(std::size_t i=0;i<next.size();++i)if(dh2_script_constants_get(constants.get(),"CharacterDesign",names[i],&next[i])||next[i]<=0){
        error=std::string("Missing positive original CharacterDesign cap ")+names[i];return false;}
    output=next;error.clear();return true;
}

bool decode_v88_suffix(Bytes records,Bytes names,Bytes schema,std::size_t prefix,
                       std::size_t& merchant_rows,std::vector<std::uint8_t>& quantity_bytes,
                       std::string& error){
    try{
        if(!records.data||prefix>records.size)throw std::runtime_error("LootTables prefix cursor is outside the actual cache");
        BoundedReader r{records,prefix};auto count=r.count();merchant_rows=count;
        for(std::uint32_t i=0;i<count;++i){(void)r.integer();(void)r.integer();const auto n=r.count();for(std::uint32_t j=0;j<n;++j){(void)r.integer();(void)r.integer();}}
        const auto quantity_start=r.at;count=r.count();
        for(std::uint32_t i=0;i<count;++i){const auto choices=r.count();r.skip(std::size_t(choices)*4);}
        if(r.at!=records.size)throw std::runtime_error("Unexpected bytes after original NumProbArray suffix");
        quantity_bytes.assign(records.data+quantity_start,records.data+r.at);
        BoundedReader s{schema};for(unsigned i=0;i<22;++i)(void)s.strings();
        if(s.strings()!=std::vector<std::string>{"ConditionId","Merchandise"}||
           s.strings()!=std::vector<std::string>{"BuyMultiplier","SellMultiplier","MerchandiseList"})
            throw std::runtime_error("Original LootTables merchant schema differs from V88 cursor");
        BoundedReader n{names};for(unsigned i=0;i<6;++i)(void)n.strings();
        const auto merchant_names=n.strings();if(merchant_names.size()!=merchant_rows)throw std::runtime_error("Merchant names/count differ from V88 suffix");
        error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
} // namespace

bool RuntimeLootSourceV1::valid() const noexcept {
    return properties&&loot&&item_power_tables&&power_resources&&audiovisual&&design_settings&&
           character_max_level[0]>0&&character_max_level[1]>0&&character_max_level[2]>0;
}

bool RuntimeLootSourceV1::max_level_for_difficulty(std::int32_t difficulty,std::int32_t& output,
                                                     std::string& error) const {
    if(!valid()){error="Runtime loot source borrow is incomplete";return false;}
    if(difficulty<0||std::size_t(difficulty)>=character_max_level.size()){
        error="Unlocked difficulty is outside original CharacterDesign MaxLevel caps";return false;}
    output=character_max_level[std::size_t(difficulty)];error.clear();return true;
}

bool RuntimeLootSourceOwnerV1::valid() const noexcept {
    return properties_&&loot_&&item_power_tables_&&item_power_tables_->borrow()&&power_resources_&&
           power_resources_->borrow()&&audiovisual_&&audiovisual_->borrow()&&design_owner_&&
           design_owner_->borrow()&&character_max_level_[0]>0&&character_max_level_[1]>0&&
           character_max_level_[2]>0&&source_hashes_.size()==25;
}

RuntimeLootSourceV1 RuntimeLootSourceOwnerV1::borrow() const {
    RuntimeLootSourceV1 result;if(!valid())return result;
    result.properties=properties_;result.loot=loot_;result.item_power_tables=item_power_tables_->borrow();
    result.power_resources=power_resources_->borrow();result.audiovisual=audiovisual_->borrow();
    result.design_settings=design_owner_->borrow();result.character_max_level=character_max_level_;
    result.source_merchant_rows=source_merchant_rows_;return result;
}

bool RuntimeLootSourceOwnerV1::load(const AssetCatalog& assets,
    const dh::foundation::frontend::creation::RuntimeCreationSourceOwnerV1& creation,std::string& error){
    error.clear();
    if(!creation.valid()||!creation.properties||!creation.loot_owner){error="Same RuntimeCreationSourceOwnerV1 is incomplete";return false;}
    try{
        RuntimeLootSourceOwnerV1 next;next.properties_=creation.properties;
        next.loot_=creation.loot_owner->borrow();
        auto read_extra=[&](const char* uri,std::vector<std::uint8_t>& bytes){
            std::string relative,digest;if(!read_asset(assets,uri,bytes,relative,digest,error))return false;
            next.source_hashes_.push_back({relative,std::uint64_t(bytes.size()),digest});return true;};
        auto read_shared=[&](const char* uri,std::vector<std::uint8_t>& bytes){
            return read_same_creation_asset(assets,creation,uri,bytes,error);};

        std::vector<std::uint8_t> loot_records,loot_names,loot_schema;
        if(!read_shared("data/pydata/loot_table_pyarray.bin",loot_records)||
           !read_shared("data/pydata/loot_table_pyarraynames.bin",loot_names)||
           !read_shared("data/pydata/loot_table_pystructnames.bin",loot_schema))return false;
        std::vector<std::uint8_t> item_records,item_names,item_schema,monopoly_records,monopoly_names,monopoly_schema;
        std::vector<std::uint8_t> audiovisual_records,audiovisual_names,audiovisual_schema;
        std::vector<std::uint8_t> design_records,design_names,design_schema;
        if(!read_extra("data/pydata/item_powers_pyarray.bin",item_records)||
           !read_extra("data/pydata/item_powers_pyarraynames.bin",item_names)||
           !read_extra("data/pydata/item_powers_pystructnames.bin",item_schema)||
           !read_extra("data/pydata/item_powers_monopoly_pyarray.bin",monopoly_records)||
           !read_extra("data/pydata/item_powers_monopoly_pyarraynames.bin",monopoly_names)||
           !read_extra("data/pydata/item_powers_monopoly_pystructnames.bin",monopoly_schema)||
           !read_extra("data/pydata/loot_audiovisual_pyarray.bin",audiovisual_records)||
           !read_extra("data/pydata/loot_audiovisual_pyarraynames.bin",audiovisual_names)||
           !read_extra("data/pydata/loot_audiovisual_pystructnames.bin",audiovisual_schema)||
           !read_extra("data/pydata/design_pyarray.bin",design_records)||
           !read_extra("data/pydata/design_pyarraynames.bin",design_names)||
           !read_extra("data/pydata/design_pystructnames.bin",design_schema))return false;
        std::vector<std::uint8_t> constants;
        if(!read_shared("data/pydata/design_pycst.bin",constants)||
           !load_character_caps(constants,next.character_max_level_,error))return false;

        std::vector<std::uint8_t> quantities;
        if(!decode_v88_suffix(Bytes{loot_records.data(),loot_records.size()},
             Bytes{loot_names.data(),loot_names.size()},Bytes{loot_schema.data(),loot_schema.size()},
             next.loot_.consumed(),next.source_merchant_rows_,quantities,error))return false;
        auto to_bytes=[](const auto& data){return Bytes{data.data(),data.size()};};
        next.item_power_tables_=std::make_shared<dh2::data::ItemPowerTablesV5>();
        if(!next.item_power_tables_->load(to_bytes(item_records),to_bytes(item_names),to_bytes(item_schema),error))return false;
        next.power_resources_=std::make_shared<dh2::data::LootPowerResourcesV7>();
        const dh2::data::LootPowerInputsV7 power_input{
            to_bytes(item_records),to_bytes(item_names),to_bytes(item_schema),
            to_bytes(monopoly_records),to_bytes(monopoly_names),to_bytes(monopoly_schema),
            Bytes{quantities.data(),quantities.size()},to_bytes(loot_names),to_bytes(loot_schema)};
        if(!next.power_resources_->load(power_input,next.item_power_tables_->borrow(),error))return false;
        next.audiovisual_=std::make_shared<dh2::data::LootAudioVisualV8>();
        if(!next.audiovisual_->load(to_bytes(audiovisual_records),to_bytes(audiovisual_names),to_bytes(audiovisual_schema),error))return false;
        next.design_owner_=std::make_shared<dh2::data::DesignSettingsOwner>();
        if(!next.design_owner_->load(to_bytes(design_records),to_bytes(design_names),to_bytes(design_schema),error))return false;
        next.source_hashes_.insert(next.source_hashes_.begin(),creation.source_hashes.begin(),creation.source_hashes.end());
        if(!next.valid()){error="Runtime loot source owners did not pass completeness checks";return false;}
        *this=std::move(next);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}

bool source_player_class_count_v1(
    const dh::foundation::PlayableActorWorld& world,
    const OriginalPropertyDatabase& properties,
    dh2::data::LootEntryOperationV8 operation,
    std::int32_t& output,
    std::string& error) {
    struct SourceClass { dh2::data::LootEntryOperationV8 operation; std::int32_t row; const char* name; };
    static constexpr SourceClass source_classes[] = {
        {dh2::data::LootEntryOperationV8::warrior_count,263,"KnightPlayerBase"},
        {dh2::data::LootEntryOperationV8::mage_count,290,"MagePlayerBase"},
        {dh2::data::LootEntryOperationV8::rogue_count,325,"RoguePlayerBase"}};
    const auto found_class=std::find_if(std::begin(source_classes),std::end(source_classes),
        [operation](const SourceClass& candidate){return candidate.operation==operation;});
    if(found_class==std::end(source_classes)){
        error="Loot operation is not one of the original PlayerManager class-count queries";
        return false;
    }
    const auto& characters=properties.characters;
    if(characters.names.size()!=characters.rows.size()||
       std::size_t(found_class->row)>=characters.names.size()||
       characters.names[std::size_t(found_class->row)]!=found_class->name){
        error="Original CharacterTable does not match PlayerManager cached base-id classification";
        return false;
    }
    std::int32_t count{};
    for(const auto& pair:world.actors()){
        const auto* traits=world.traits(pair.first);
        if(!traits||!traits->is_player)continue;
        const auto& definition=pair.second.definition_id;
        if(definition.empty()){
            error="Same-world loot player has no source CharacterTable definition ID";
            return false;
        }
        const auto row=std::find(characters.names.begin(),characters.names.end(),definition);
        if(row==characters.names.end()){
            error="Same-world loot player definition is absent from original CharacterTable: "+definition;
            return false;
        }
        if(row-characters.names.begin()==found_class->row){
            if(count==std::numeric_limits<std::int32_t>::max()){
                error="Source PlayerManager class count overflow";
                return false;
            }
            ++count;
        }
    }
    output=count;
    error.clear();
    return true;
}

} // namespace dh::foundation::loot

