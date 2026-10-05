#include "../hud_freetype_font_v2.hpp"
#include "../gfnt.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("Actual SCT GFNT required");
 std::ifstream f(argv[1],std::ios::binary);if(!f)throw std::runtime_error("GFNT unavailable");
 std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(f),{}};
 dh2::ui::GfntFont gfnt;std::string error;if(!gfnt.load(bytes.data(),bytes.size(),error))throw std::runtime_error(error);
 dh2::ui::HudFreetypeFontV2 ft;if(ft.load(bytes.data(),bytes.size(),error))throw std::runtime_error("GFNT manufactured a FreeType face");
 if(error.empty())throw std::runtime_error("Real FT rejected without diagnostic");
 std::cout<<"{\"validation\":\"PASS\",\"actual_GFNT_valid\":true,\"real_FT_no_face\":true,\"FT_version\":\""<<ft.version()<<"\",\"diagnostic\":\""<<error<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
