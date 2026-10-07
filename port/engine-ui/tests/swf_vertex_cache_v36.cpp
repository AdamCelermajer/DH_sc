#include "../swf_vertex_cache_v36.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
namespace {unsigned checks{};void check(bool value){++checks;if(!value)throw std::runtime_error("cache check "+std::to_string(checks));}}
int main(){try{
 using namespace dh2::ui;SwfVertexCacheV36 cache(128,2);std::uintptr_t serial{},out{};unsigned uploads{},releases{};bool retained{};std::string error;
 auto upload=[&](const float* data,std::size_t n,std::uintptr_t& value,std::string&){check(data&&n==4);++uploads;value=++serial;return true;};
 auto release=[&](std::uintptr_t value){check(value!=0);++releases;};
 std::vector<float> a{0,1,2,3},b{1,2,3,4},c{2,3,4,5};
 check(cache.acquire(a,4,upload,release,out,retained,error)&&retained&&out==1&&uploads==1);
 for(unsigned i=0;i<1000;++i)check(cache.acquire(a,4,upload,release,out,retained,error)&&out==1);
 check(uploads==1&&cache.stats().hits==1000&&cache.stats().resident_entries==1);
 check(cache.acquire(b,4,upload,release,out,retained,error)&&out==2);
 check(cache.acquire(a,4,upload,release,out,retained,error)&&out==1);
 check(cache.acquire(c,4,upload,release,out,retained,error)&&out==3&&releases==1);
 check(cache.stats().resident_entries==2&&cache.stats().resident_bytes==32&&cache.stats().evictions==1);
 cache.abandon_context();check(releases==1);
 check(cache.acquire(a,4,upload,release,out,retained,error)&&out==4&&cache.stats().context_reuploads==1);
 check(cache.acquire(a,4,upload,release,out,retained,error)&&out==4&&uploads==4);
 check(cache.acquire(a,5,upload,release,out,retained,error)&&out==5&&releases==1);
 cache.clear(release);check(cache.stats().resident_entries==0&&cache.stats().resident_bytes==0&&releases==3);
 SwfVertexCacheV36 small(8,2);check(small.acquire(a,4,upload,release,out,retained,error)&&!retained&&!out&&uploads==5);
 check(!small.acquire({},4,upload,release,out,retained,error));
 auto fail=[&](const float*,std::size_t,std::uintptr_t& value,std::string& e){value=90;e="declared allocation failure";return false;};
 check(!cache.acquire(a,4,fail,release,out,retained,error)&&error=="declared allocation failure"&&releases==4&&cache.stats().resident_entries==0);
 std::vector<float> strip{0,0,0,0,1,0,1,0,0,1,0,1,1,1,1,1},triangles;
 check(swf_append_triangles_v36(triangles,strip,true,error)&&triangles.size()==24);
 const unsigned indices[]{0,1,2,2,1,3};for(unsigned i=0;i<6;++i)for(unsigned j=0;j<4;++j)check(triangles[i*4+j]==strip[indices[i]*4+j]);
 std::vector<float> signed_zero=a;signed_zero[0]=-0.f;check(SwfVertexCacheV36::fingerprint(a.data(),a.size(),4)!=SwfVertexCacheV36::fingerprint(signed_zero.data(),signed_zero.size(),4));
 std::cout<<"PASS bounded exact-content cache: warm reuse1000, mode/data keys, LRU/budgets, lost-context reupload, required failure cleanup, strip topology; checks="<<checks<<"\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
