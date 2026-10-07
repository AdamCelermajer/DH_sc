#include "objective_gathering_registration_v11.hpp"
namespace dh2::character {namespace {
bool deliver(const ObjectiveGatheringBorrowV11& b,const ObjectiveGatheringServicesV11& s,bool adding,std::string& e){
 if(!b.identity||!b.enabled8){e="Required actual gathering Objective identity/byte8";return false;}
 auto base=adding?s.base_register:s.base_unregister;
 if(!base||!base(s.context,b.identity,e)){if(e.empty())e="Required actual Objective base registration lifecycle";return false;}
 if(!*b.enabled8)return true;
 if(!b.character10||!*b.character10||!b.item_id24){e="Required actual enabled gathering Objective Character10/data24";return false;}
 auto inventory=adding?s.inventory_register:s.inventory_unregister;
 if(!inventory||!inventory(s.context,*b.character10,*b.item_id24,e)){if(e.empty())e="Required SAME inventory gathering-ID registration receiver";return false;}return true;
}
}
bool objective_gathering_register_v11(const ObjectiveGatheringBorrowV11& b,const ObjectiveGatheringServicesV11& s,std::string& e){return deliver(b,s,true,e);}
bool objective_gathering_unregister_v11(const ObjectiveGatheringBorrowV11& b,const ObjectiveGatheringServicesV11& s,std::string& e){return deliver(b,s,false,e);}
}
