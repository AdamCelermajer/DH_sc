#include "textures.hpp"
#include <cstdio>
#include <cstdint>
#include <vector>
struct Crop{const char* name;const char* texture;int x,y,w,h;};
static const Crop crops[]{
#include "../app/src/main/cpp/gameplay_icon_catalog.inc"
};
int main(int argc,char** argv){
 if(argc!=2)return 2;auto* file=std::fopen(argv[1],"rb");if(!file)return 3;
 std::fseek(file,0,SEEK_END);auto size=std::ftell(file);std::rewind(file);
 std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
 if(std::fread(bytes.data(),1,bytes.size(),file)!=bytes.size())return 4;std::fclose(file);
 dh2::textures::View view{};if(dh2_texture_open(bytes.data(),bytes.size(),&view)!=dh2::textures::Error::ok)return 5;
 std::vector<std::uint8_t> pixels(std::size_t(view.width)*view.height*4);
 if(dh2_texture_decode(&view,pixels.data(),pixels.size())!=dh2::textures::Error::ok)return 6;
 unsigned checked=0,visible=0;std::uint64_t hash=1469598103934665603ull;
 for(const auto& c:crops){
  if(c.x<0||c.y<0||c.w<=0||c.h<=0||c.x+c.w>int(view.width)||c.y+c.h>int(view.height)){std::printf("bad crop %s\n",c.name);return 7;}
  unsigned opaque=0;
  for(int y=0;y<c.h;++y)for(int x=0;x<c.w;++x){auto at=(std::size_t(c.y+y)*view.width+c.x+x)*4;
   opaque+=pixels[at+3]!=0;for(int ch=0;ch<4;++ch){hash^=pixels[at+ch];hash*=1099511628211ull;}}
  ++checked;if(opaque)++visible;
 }
 std::printf("PASS original atlas %ux%u | authored crops %u | visible %u | RGBA hash %016llx\n",view.width,view.height,checked,visible,static_cast<unsigned long long>(hash));
 return visible==checked?0:8;
}
