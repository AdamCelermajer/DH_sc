#include "source_android_notify_trophy_v114.hpp"
#include <array>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <android/log.h>
namespace dh2::android_ui {
bool source_android_notify_trophy_v114(const std::string& directory,std::int32_t id,std::string& e){
 // Actual GLiveMain.<clinit> cS fill-array payload55c has100 int entries127.
 // NotifyTrophy uses its LENGTH only, initializes a LOCAL array with127,
 // reads existing lines, stores1 at id, then truncates/writes all100 lines.
 constexpr std::size_t count=100;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","NotifyTrophy %d",id);
 if(id<0||id>static_cast<std::int32_t>(count)){e.clear();return true;}
 if(directory.empty()||directory.front()!='/'){e="Required actual Android package files directory for NotifyTrophy";return false;}
 const auto file=directory+"/androidTrophy.dat";
 try{
  std::array<std::int32_t,count> values;values.fill(127);
  std::ifstream in(file);std::string line;std::size_t index{};
  if(in.is_open()){
   while(std::getline(in,line)){
    if(!line.empty()&&line.back()=='\r')line.pop_back();
    if(index>=count)throw std::out_of_range("Original Android trophy line array bound");
    std::size_t start=(line.size()&&(line[0]=='-'||line[0]=='+'))?1:0;
    if(start==line.size())throw std::invalid_argument("Original Integer.parseInt empty value");
    for(std::size_t i=start;i<line.size();++i)if(line[i]<'0'||line[i]>'9')throw std::invalid_argument("Original Integer.parseInt nondecimal value");
    const auto value=std::stoll(line);if(value<std::numeric_limits<std::int32_t>::min()||value>std::numeric_limits<std::int32_t>::max())throw std::out_of_range("Original Integer.parseInt int32 range");
    values[index++]=static_cast<std::int32_t>(value);
   }
   if(in.bad())throw std::runtime_error("Original Android FileReader failure");in.close();
  }else{
   // Genuine createNewFile before array store; no invented populated defaults.
   std::ofstream create(file,std::ios::app);if(!create)throw std::runtime_error("Original Android createNewFile failure");create.close();
  }
  if(id==static_cast<std::int32_t>(count))throw std::out_of_range("Original NotifyTrophy accepted size then caught array index");
  values[static_cast<std::size_t>(id)]=1;
  std::ofstream out(file,std::ios::trunc);if(!out)throw std::runtime_error("Original Android FileWriter failure");
  for(auto value:values)out<<value<<'\n';out.flush();if(!out)throw std::runtime_error("Original Android trophy flush failure");out.close();
 }catch(const std::exception&){
  // Original Java catch falls through to returnvoid. Real I/O was attempted;
  // this does not report file persistence as verified, or undo a partial file.
 }
 e.clear();return true;
}
}
