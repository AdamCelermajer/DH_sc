#include "../native_batch_compiler_v111.hpp"
#include <cassert>

int main(){
 dh2::world::NativeBatchPartV111 part;
 part.vertices.emplace_back();
 const auto& default_color=part.vertices.front().color;
 assert(default_color[0]==1.f&&default_color[1]==1.f&&default_color[2]==1.f&&default_color[3]==1.f);
 assert(dh2::world::native_batch_default_color_v113(part));

 auto& color=part.attributes[2];
 color.components=4;
 color.values.push_back({.25f,.5f,.75f,1.f});
 assert(!dh2::world::native_batch_default_color_v113(part));
 color.values.clear();
 color.components=0;
 color.quantized_components.push_back({1,2,3,4});
 assert(!dh2::world::native_batch_default_color_v113(part));
}
