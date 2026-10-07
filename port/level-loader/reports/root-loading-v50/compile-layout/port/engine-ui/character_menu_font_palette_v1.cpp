#include "character_menu_font_palette_v1.hpp"
#include <cstring>
namespace dh2::ui {
bool CharacterMenuFontPaletteV1::load(LocalizationBytes records,LocalizationBytes schema,std::string& e){
 auto word=[](const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;};
 if(!records.data||records.size<4||!schema.data||schema.size!=30||word(schema.data)!=2||word(schema.data+4)!=9||std::memcmp(schema.data+8,"glowcolor",9)||word(schema.data+17)!=9||std::memcmp(schema.data+21,"textcolor",9)){e="Genuine FontPalette source schema required";return false;}
 const auto count=word(records.data);if(count>(records.size-4)/8||records.size!=4+std::size_t(count)*8){e="FontPalette source rows truncated";return false;}
 std::vector<std::array<std::uint32_t,2>> rows(count);for(unsigned j=0;j<count;++j)rows[j]={word(records.data+4+8*j),word(records.data+8+8*j)};rows_=std::move(rows);return true;
}
bool CharacterMenuFontPaletteV1::text_color(std::int32_t row,std::uint32_t& out,std::string& e)const{if(row<0||std::size_t(row)>=rows_.size()){e="Source FontPalette index outside actual rows";return false;}out=rows_[row][1];return true;}
}
