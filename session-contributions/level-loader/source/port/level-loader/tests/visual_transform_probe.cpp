#include "visual_transform_v1.hpp"
#include <iomanip>
#include <iostream>
int main() {
    while(std::cin.peek()!=std::char_traits<char>::eof()) {
    dh2::loader::VisualTransformV1 value;
    for(float& f:value.position)if(!(std::cin>>f))return 2;
    for(float& f:value.rotation_degrees)if(!(std::cin>>f))return 2;
    for(float& f:value.scale)if(!(std::cin>>f))return 2;
    std::string error;
    if(!dh2::loader::project_visual_transform_v1(value,error)){std::cerr<<error<<'\n';return 1;}
    std::cout<<std::setprecision(9);
    for(float f:value.position)std::cout<<f<<' ';
    for(float f:value.quaternion)std::cout<<f<<' ';
    for(float f:value.scale)std::cout<<f<<' ';
    std::cout<<'\n';
    std::cin>>std::ws;
    }
}
