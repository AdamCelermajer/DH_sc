#include "source_assertion_process_v76.hpp"
#include <cstdio>
namespace dh2::world {
std::shared_ptr<SourceAssertionProcessV76> SourceAssertionProcessV76::borrow(){
 static const auto owner=std::shared_ptr<SourceAssertionProcessV76>(new SourceAssertionProcessV76());return owner;
}
bool SourceAssertionProcessV76::report(const char* file,std::int32_t line,const char* expression,std::string& e){
 if(!file||!expression){e="Original assertion diagnostic requires actual file/expression literals";return false;}
 if(std::fprintf(stderr,"ASSERT(%s) FAILED: %s:%d\n",expression,file,line)<0||std::fflush(stderr)!=0){
  e="Native assertion diagnostic sink write failed";return false;
 }
 e.clear();return true;
}
}
