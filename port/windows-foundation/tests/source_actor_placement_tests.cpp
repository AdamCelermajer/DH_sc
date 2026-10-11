#include "../source_actor_placement.hpp"
#include <cstdlib>
#include <iostream>
using dh::foundation::source_actor_draw_scale;
static void check(bool v,const char* m){if(!v){std::cerr<<m<<'\n';std::exit(1);}}
int main(){
    const std::array<float,3> gameplay{.9f,.9f,1.f};
    const auto ordinary=source_actor_draw_scale(false,gameplay);
    check(ordinary==gameplay,"ordinary pose must keep the actor scale");
    const auto scripted=source_actor_draw_scale(true,gameplay);
    check(scripted[0]==1.f&&scripted[1]==1.f&&scripted[2]==1.f,"scripted clip pose draws at unit scale");
    // Root offset of the witch hero clip (local y 1278.77 at 0.5 s): world offset equals the authored offset, not 0.9 times it.
    check(scripted[1]*1278.77f==1278.77f&&ordinary[1]*1278.77f<1278.77f,"scripted offset must not shrink toward the waypoint");
    std::cout<<"source_actor_placement ok\n";return 0;
}
