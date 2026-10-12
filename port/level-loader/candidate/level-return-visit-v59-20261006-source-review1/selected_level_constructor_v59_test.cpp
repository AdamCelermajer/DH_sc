#include <selected_level_constructor_v59.hpp>
#include <stdexcept>
#include <iostream>
using namespace dh2::loader;
static void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
int main(){
 // Explicit synthetic native adapter fixture; row/file spelling comes from
 // the separately inventoried original SWAMP/SWAMP_02 rows. This test does
 // not replace original-cache constructor proof or execute C1/restoration.
 dh2::data::LevelTables tables;tables.level_names.resize(43);tables.levels.resize(43);
 for(unsigned i=0;i<43;++i){tables.level_names[i]="fixture_"+std::to_string(i);tables.levels[i].file="fixture_"+std::to_string(i)+".mlx";}
 tables.level_names[41]="SWAMP";tables.levels[41].file="001_swamp.mlx";
 tables.level_names[42]="SWAMP_02";tables.levels[42].file="022_swamp2.rule.xml";
 auto pin=std::make_shared<unsigned>(0);std::string error;std::shared_ptr<CanonicalLevelContextV1> swamp,swamp2;
 LevelSourceRequestV1 request;request.identity="SWAMP";request.definition="data/scene/001_swamp.mlx";request.seed=7;
 check(CanonicalLevelContextV1::create(request,pin,swamp,error),error);
 request.identity="SWAMP_02";request.definition="data/scene/022_swamp2.rule.xml";request.kind=LevelSourceKindV1::procedural;
 check(CanonicalLevelContextV1::create(request,pin,swamp2,error),error);
 LevelConstructorServicesV3 services;services.levels=&tables;unsigned calls=0;
 services.construct_script=[&calls](auto,bool,auto&,auto&){++calls;return false;};
 LevelConstructorArgumentsV3 arguments;arguments.name="001_swamp.mlx";arguments.seed=7;
 check(validate_selected_level_constructor_v59(*swamp,arguments,services,error),error);
 arguments.name="data/scene/001_swamp.mlx";check(validate_selected_level_constructor_v59(*swamp,arguments,services,error),error);
 check(!construct_selected_level_v59(swamp2,arguments,services,error)&&!swamp2->constructor_owner_v3()&&calls==0,"foreign filename reached C1");
 arguments.name="022_swamp2.rule.xml";check(validate_selected_level_constructor_v59(*swamp2,arguments,services,error),error);
 arguments.seed=8;check(!construct_selected_level_v59(swamp2,arguments,services,error)&&!swamp2->constructor_owner_v3()&&calls==0,"foreign saved seed reached C1");
 arguments.seed=7;arguments.name="SWAMP_02";check(!validate_selected_level_constructor_v59(*swamp2,arguments,services,error),"logical identity used as source filename");
 check(swamp->source_request().identity=="SWAMP"&&swamp2->source_request().identity=="SWAMP_02"&&swamp->identity()!=swamp2->identity(),"selected identity collapsed");
 check(!swamp->constructor_fields_v3().save_ec&&!swamp2->constructor_fields_v3().save_ec,"preflight manufactured save");
 std::cout<<"PASS synthetic_selected_preflight distinct_rows=1 original_filename_substring=1 foreign_filename_seed_before_C1=1 no_save_or_restore_or_Eval=1\n";
}