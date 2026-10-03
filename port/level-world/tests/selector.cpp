#include "selector.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>
using namespace dh2::selector;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated selector reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
};
void require(bool condition,const char* error){if(!condition)throw std::runtime_error(error);}
bool equal(const void* a,const void* b,std::size_t size){
 for(std::size_t i=0;i<size;i+=4){unsigned x,y;std::memcpy(&x,static_cast<const unsigned char*>(a)+i,4);std::memcpy(&y,static_cast<const unsigned char*>(b)+i,4);
  const auto nan=[](unsigned v){return (v&0x7f800000)==0x7f800000&&(v&0x7fffff);};if(x!=y&&!(nan(x)&&nan(y)))return false;}
 return true;
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x314c4553,"Invalid selector reference");const unsigned inverse_count=r.word(),builds=r.word();require(inverse_count<=100000&&builds<=10000,"Too many references");
  unsigned singular=0,boxes=0,rays=0,floors=0,selected=0,ray_hits=0,floor_hits=0,rejections=0;
  for(unsigned i=0;i<inverse_count;++i){
   Matrix matrix,expected;r.read(&matrix,68);const unsigned status=r.word();r.read(&expected,68);
   require(dh2_selector_inverse(&matrix)==int(status)&&equal(&matrix,&expected,68),"Matrix inverse mismatch");singular+=!status;
  }
  for(unsigned batch=0;batch<builds;++batch){
   const unsigned count=r.word(),leaf=r.word();require(count>0&&count<=4096,"Invalid geometry count");
   std::vector<dh2::collision::Triangle> geometry(count),selected_triangles(count);r.read(geometry.data(),count*36);
   std::vector<dh2::octree::Node> nodes(4096);std::vector<unsigned> indices(32768),scratch(4096),selected_ids(count);
   dh2::octree::Tree tree{nodes.data(),indices.data(),scratch.data(),geometry.data(),count,0,0,4096,32768,4096,leaf,0,0};
   require(dh2_octree_build(&tree,geometry.data(),count,leaf)==0,"Selector tree build failed");
   Workspace workspace{selected_ids.data(),selected_triangles.data(),count,0};const unsigned rows=r.word();require(rows<=100000,"Too many query references");
   for(unsigned q=0;q<rows;++q){
    const unsigned mode=r.word(),present=r.word(),baked=r.word();Matrix node;r.read(&node,68);Selector selector{&tree,present?&node:nullptr,baked,0};
    if(mode==0){
     const unsigned extra_present=r.word();Matrix extra;r.read(&extra,68);dh2::octree::Box box;r.read(&box,24);const unsigned expected_count=r.word();require(expected_count<=count,"Invalid query output count");
     std::vector<dh2::collision::Triangle> expected(expected_count);r.read(expected.data(),36*expected_count);
     require(dh2_selector_triangles(&workspace,&selector,&box,extra_present?&extra:nullptr)==expected_count&&equal(workspace.triangles,expected.data(),36*expected_count),"Selector query mismatch");++boxes;selected+=expected_count;
    }else{
     dh2::collision::Result result,expected;std::memset(&result,0xcc,sizeof(result));int hit;
     if(mode==1){dh2::collision::Ray ray;r.read(&ray,24);hit=dh2_selector_raycast(&result,&selector,&workspace,&ray);}
     else{require(mode==2,"Invalid selector mode");Floor floor{selector,{},&workspace};float point[3];r.read(&floor.bounds,24);r.read(point,12);hit=dh2_selector_floor(&result,&floor,point);}
     r.read(&expected,56);require(hit==int(expected.hit)&&equal(&result,&expected,56),"Coupled selector collision mismatch");
     if(mode==1){++rays;ray_hits+=hit;}else{++floors;floor_hits+=hit;}
    }
   }
   Selector selector{&tree,nullptr,1,0};dh2::collision::Result result;std::memset(&result,0xcc,sizeof(result));const auto saved=result;dh2::collision::Ray ray{};
   --workspace.capacity;require(dh2_selector_raycast(&result,&selector,&workspace,&ray)==-1&&std::memcmp(&result,&saved,sizeof(result))==0,"Capacity rejection modified result");++rejections;
   ++workspace.capacity;workspace.reserved=1;require(dh2_selector_raycast(&result,&selector,&workspace,&ray)==-1&&std::memcmp(&result,&saved,sizeof(result))==0,"Reserved rejection modified result");++rejections;
  }
  require(r.at==r.bytes.size(),"Trailing selector reference bytes");Matrix invalid{};invalid.identity=2;const auto saved=invalid;
  require(dh2_selector_inverse(&invalid)==-1&&std::memcmp(&invalid,&saved,68)==0,"Invalid matrix changed output");++rejections;
  std::cout<<"{\"matrix_inverse_comparisons\":"<<inverse_count<<",\"singular_inverses\":"<<singular<<",\"octree_builds\":"<<builds<<",\"selector_box_comparisons\":"<<boxes<<",\"selected_triangle_comparisons\":"<<selected<<",\"coupled_ray_comparisons\":"<<rays<<",\"coupled_floor_comparisons\":"<<floors<<",\"hits\":{\"ray\":"<<ray_hits<<",\"floor\":"<<floor_hits<<"},\"storage_rejection_checks\":"<<rejections<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
