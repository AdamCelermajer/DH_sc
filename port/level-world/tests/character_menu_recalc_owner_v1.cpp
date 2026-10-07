#define main original_design_fixture_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_game_design.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_menu_recalc_owner_v1.hpp"
int main(int argc,char** argv){try{
 check(argc==2);auto actual=inputs(argv[1]);actual.refresh();CharacterGameDesign design;std::string error;check(design.initialize(actual.view,error));
 auto borrow=design.borrow();auto state=std::make_shared<dh2::data::PropertyState>();
 dh2::data::reset_properties(*borrow.rules(),*state,&borrow.characters()->rows[263]);
 auto live=dh2::data::property_view(*borrow.rules(),*state);dh2::data::PropertySheet buff=borrow.rules()->defaults;
 unsigned p=0;while(p<224&&!(std::uint32_t(live.types[p])&4))++p;check(p<224);buff[p]=256;
 const std::int32_t* sheet=buff.data();dh2::data::PropertyBuffGroup group{&sheet,1};live.groups=&group;live.group_count=1;
 auto expected=*state;auto expected_view=dh2::data::property_view(*borrow.rules(),expected);expected_view.groups=&group;expected_view.group_count=1;
 check(!dh2_class_recalc_base(borrow.class_rows()->data(),borrow.class_rows()->size(),expected.base.data(),&expected_view));
 CharacterMenuRecalcOwnerV1 owner(design.borrow(),state,live);check(owner.recalculate(live,1,error));
 check(state->base==expected.base&&state->resolved==expected.resolved&&live.groups==&group&&live.group_count==1);
 buff[p]=512;check(owner.recalculate(live,0,error));std::int32_t expected_value;check(!dh2_property_resolve(&expected_view,p,&expected_value));check(live.resolved[p]==expected_value);
 check(!owner.recalculate(expected_view,1,error));
 std::cout<<"PASS actual source classes + live buff-aware same-owner Recalc true/false; different view rejected\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
