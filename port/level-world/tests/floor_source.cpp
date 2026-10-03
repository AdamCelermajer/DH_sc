#include "floor_source.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated floor-source reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> blob(unsigned n){if(n>1000000)throw std::runtime_error("Floor-source stream exceeds limit");std::vector<unsigned char> result(n);read(result.data(),n);return result;}
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
  Reader r(argv[1]);require(r.word()==0x32534d46,"Invalid floor-source reference");unsigned meshes=r.word(),flags=r.word(),bounds=r.word();require(meshes<=10000&&flags<=10000&&bounds<=10000,"Too many floor-source cases");unsigned triangles=0,rejections=0;
  for(unsigned batch=0;batch<meshes;++batch){
   unsigned count=r.word(),present=r.word(),bake=r.word();require(count<=10000&&present<=1&&bake<=1,"Invalid mesh case");dh2::selector::Matrix matrix;r.read(&matrix,68);
   std::vector<dh2::floor_source::Part> parts(count);std::vector<std::vector<unsigned char>> vertices(count),indices(count);
   for(unsigned i=0;i<count;++i){auto& part=parts[i];part.position.vertices=r.word();part.draw_count=r.word();part.position.type=r.word();part.position.components=r.word();part.position.stride=r.word();part.primitive_type=r.word();unsigned vn=r.word(),in=r.word();vertices[i]=r.blob(vn);indices[i]=r.blob(in);part.position.data=vertices[i].data();part.indices=in?indices[i].data():nullptr;}
   unsigned expected_count=r.word();require(expected_count<=100000,"Too many floor triangles");std::vector<dh2::collision::Triangle> expected(expected_count),output(expected_count+1);r.read(expected.data(),36*expected_count);unsigned written=0xcccccccc;
   require(dh2_floor_mesh_triangles(output.data(),expected_count,&written,parts.data(),count,present?&matrix:nullptr,bake)==0&&written==expected_count&&equal(output.data(),expected.data(),36*expected_count),"Floor mesh mismatch");triangles+=written;
   if(expected_count){std::memset(output.data(),0xcc,output.size()*36);auto saved=output;written=0xcccccccc;
    require(dh2_floor_mesh_triangles(output.data(),expected_count-1,&written,parts.data(),count,present?&matrix:nullptr,bake)==2&&written==0xcccccccc&&std::memcmp(saved.data(),output.data(),output.size()*36)==0,"Capacity rejection modified output");++rejections;}
  }
  for(unsigned i=0;i<flags;++i){dh2::floor_source::Flags value,expected;r.read(&value,8);auto tags=r.blob(r.word());r.read(&expected,8);require(dh2_floor_source_flags(&value,reinterpret_cast<const char*>(tags.data()),tags.size())==0&&std::memcmp(&value,&expected,8)==0,"Floor tags mismatch");}
  for(unsigned i=0;i<bounds;++i){dh2::octree::Box local,world,expanded,output;dh2::selector::Matrix matrix;r.read(&local,24);r.read(&matrix,68);r.read(&world,24);r.read(&expanded,24);
   require(dh2_floor_transform_bounds(&output,&local,&matrix)==0&&equal(&output,&world,24),"World bounds mismatch");require(dh2_floor_source_bounds(&output,&output)==0&&equal(&output,&expanded,24),"Expanded floor bounds mismatch");
   require(dh2_floor_transform_bounds(&local,&local,&matrix)==0&&equal(&local,&world,24),"In-place bounds mismatch");
  }
  require(r.at==r.bytes.size(),"Trailing floor-source reference bytes");
  float vertex[9]{};unsigned short index[3]{0,1,3};dh2::floor_source::Part bad{{reinterpret_cast<const unsigned char*>(vertex),6,3,12,3},index,3,6};dh2::collision::Triangle output;std::memset(&output,0xcc,36);auto saved=output;unsigned written=0xcccccccc;
  require(dh2_floor_mesh_triangles(&output,1,&written,&bad,1,nullptr,0)==1&&written==0xcccccccc&&std::memcmp(&output,&saved,36)==0,"Out-of-range index rejection modified output");++rejections;
  index[2]=2;bad.draw_count=2;require(dh2_floor_mesh_triangles(&output,1,&written,&bad,1,nullptr,0)==1&&written==0xcccccccc&&std::memcmp(&output,&saved,36)==0,"Incomplete topology rejection modified output");++rejections;
  std::cout<<"{\"mesh_constructor_comparisons\":"<<meshes<<",\"triangle_comparisons\":"<<triangles<<",\"floor_tag_comparisons\":"<<flags<<",\"world_and_floor_bounds_comparisons\":"<<bounds<<",\"storage_rejection_checks\":"<<rejections<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
