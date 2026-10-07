#include "../campaign_save_filename_v45.hpp"
#include <array>
#include <fstream>
#include <cassert>
#include <iostream>
int main(){std::ifstream file("port/level-world/reference/campaign-save-v45/filename-gold.bin",std::ios::binary);unsigned count{};file.read(reinterpret_cast<char*>(&count),4);assert(count==24);for(unsigned i=0;i<count;++i){std::array<unsigned,4> row{};file.read(reinterpret_cast<char*>(row.data()),16);assert(file&&row[3]<64);std::string text(row[3],'\0');file.read(text.data(),row[3]);assert(file);assert(dh2::level::campaign_save_filename_v45(row[0],row[1],row[2])==text);}std::cout<<"Original SG_GetFilename replay PASS "<<count<<"; sprintf/CString leaves fixtures\n";}
