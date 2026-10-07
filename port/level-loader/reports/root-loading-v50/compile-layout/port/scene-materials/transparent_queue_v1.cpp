#include "transparent_queue_v1.hpp"
#include <utility>
namespace dh2::scene {
namespace {bool missing(std::string& e,const char* message){e=message;return false;}}
bool transparent_entry_v1(TransparentEntryV1& entry,const std::array<float,3>& camera,
 const std::array<float,3>* supplied,std::int32_t priority,const TransparentQueueServicesV1& s,std::string& error){
 if(!entry.node||!entry.node_owner||(entry.material&&!entry.material_owner))return missing(error,"Transparent entry requires retained actual node/material");
 std::int32_t selected=priority;
 if(priority==0x7fffffff){if(!s.node_priority_d8)return missing(error,"Required source transparent node priority d8");if(!s.node_priority_d8(entry.node,selected,error))return false;}
 // Source publishes priority before requesting node position or distance bias.
 entry.priority=selected;
 std::array<float,3> actual{};
 if(supplied)actual=*supplied;
 else {if(!s.node_position_38)return missing(error,"Required source transparent node transform 38");if(!s.node_position_38(entry.node,actual,error))return false;}
 const float x=actual[0]-camera[0],y=actual[1]-camera[1],z=actual[2]-camera[2];
 const float xx=x*x,yy=y*y,zz=z*z;const float xy=xx+yy;const float squared=xy+zz;
 if(!s.node_distance_bias_d0)return missing(error,"Required source transparent node distance bias d0");
 float bias{};if(!s.node_distance_bias_d0(entry.node,bias,error))return false;
 entry.distance=squared+bias;return true;
}
bool transparent_less_v1(const TransparentEntryV1& a,const TransparentEntryV1& b,
 const TransparentQueueServicesV1& s,bool& result,std::string& error){
 if(a.priority>b.priority){result=true;return true;}
 if(a.priority!=b.priority){result=false;return true;}
 if(a.distance>b.distance){result=true;return true;}
 if(a.distance!=b.distance){result=false;return true;} // Includes unordered NaNs.
 if(a.material&&b.material){
  if(!s.material_equal_3537b0)return missing(error,"Required source transparent material equality 3537b0");
  bool equal{};if(!s.material_equal_3537b0(a.material,b.material,equal,error))return false;
  if(!equal){if(!s.material_less_3537e4)return missing(error,"Required source transparent material order 3537e4");return s.material_less_3537e4(a.material,b.material,result,error);}
  if(!s.node_suborder_20)return missing(error,"Required source transparent node suborder 20");
  std::int32_t left{},right{};if(!s.node_suborder_20(a.node,a.part,left,error))return false;
  if(!s.node_suborder_20(b.node,b.part,right,error))return false;
  result=left<right;return true;
 }
 if(a.material||b.material){result=a.material<b.material;return true;}
 result=a.node<b.node;return true;
}
bool TransparentQueueV1::append(TransparentEntryV1 e,const std::array<float,3>& c,
 const std::array<float,3>* p,std::int32_t priority,const TransparentQueueServicesV1& s,std::string& error){
 if(!transparent_entry_v1(e,c,p,priority,s,error))return false;
 entries_.push_back(std::move(e));return true;
}
}
