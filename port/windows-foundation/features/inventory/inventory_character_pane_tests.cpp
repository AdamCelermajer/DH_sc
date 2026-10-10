#include "inventory_details.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation::inventory;

static void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
static bool near(float a,float b){return std::abs(a-b)<0.0002f;}

int main(){
 const auto& panes=original_inventory_character_panes_v1();
 check(panes.size()==2,"Expected separate main and Details source pane callbacks");
 const auto& main=panes[0];
 check(main.path=="_root.menu_InventorySheetMain.avatarpane"&&main.root_depth==571&&main.child_depth==1,
       "Main source avatarpane identity/order changed");
 check(main.after_role.empty()&&main.before_role=="menu_InventorySheetMain/3/",
       "Main source avatarpane insertion role changed");
 check(near(main.matrix[0],0.6887207f)&&near(main.matrix[3],4.1150055f)&&
       near(main.bounds[0],155.05f)&&near(main.bounds[1],87.7f)&&
       near(main.bounds[2],321.10056f)&&near(main.bounds[3],282.75126f),
       "Main source avatarpane matrix/bounds changed");

 const auto& details=panes[1];
 check(details.path=="_root.menu_InventorySheetDetails.avatarpane"&&details.root_depth==750&&details.child_depth==235,
       "Details source avatarpane identity/order changed");
 check(details.after_role=="menu_InventorySheetDetails/btn_AutoEquip/"&&
       details.before_role=="menu_InventorySheetDetails/237/",
       "Details source avatarpane insertion roles changed");
 check(near(details.matrix[0],0.4281069f)&&near(details.matrix[3],4.1039545f)&&
       near(details.bounds[0],219.2730f)&&near(details.bounds[1],66.26375f)&&
       near(details.bounds[2],322.48958f)&&near(details.bounds[3],260.7912f),
       "Details source avatarpane matrix/bounds changed");
 // RenderCharacterPane uses the physical viewport width / height for camera
 // aspect. This authored-coordinate ratio is retained for viewport scaling.
 const auto authored_aspect=(details.bounds[2]-details.bounds[0])/(details.bounds[3]-details.bounds[1]);
 check(near(authored_aspect,0.53060f),"Details source pane aspect basis changed");
 std::cout<<"inventory character pane API tests PASS\n";
}
