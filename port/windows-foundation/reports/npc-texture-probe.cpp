#include "../texture_loader.hpp"
#include <iostream>
#include <fstream>
int main(int argc,char**argv){
 for(int i=1;i<argc;++i){dh::foundation::TextureImage t;std::string e;if(!dh::foundation::load_texture(argv[i],t,e)){std::cerr<<e;return 1;}
 unsigned z=0,p=0,o=0,rzero=0,rpartial=0,ropaque=0;
 for(unsigned y=0;y<t.height;++y)for(unsigned x=0;x<t.width;++x){auto a=t.rgba[(std::size_t(y)*t.width+x)*4+3];a==0?++z:a==255?++o:++p;
  bool region=i==1 ? x>=t.width*.125756f&&x<=t.width*.249233f&&y>=t.height*.001137f&&y<=t.height*.124482f : x>=t.width*.125977f&&x<=t.width*.375f&&y>=t.height*.500238f&&y<=t.height*.625f;
  if(region){a==0?++rzero:a==255?++ropaque:++rpartial;}}
 std::cout<<argv[i]<<" dimensions="<<t.width<<"x"<<t.height<<" global_alpha_zero="<<z<<" partial="<<p<<" opaque="<<o<<" region_alpha_zero="<<rzero<<" partial="<<rpartial<<" opaque="<<ropaque<<"\n";
 }
}
