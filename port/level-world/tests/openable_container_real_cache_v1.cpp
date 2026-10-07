#include "openable_container_owner_v1.hpp"
#include "openable_container_real_cache_fixture_v1.hpp"
#include <cassert>
#include <iostream>
int main(){using namespace dh2::world;OpenableContainerTableV1 table;std::string e;
 assert(table.load(source_openable_records,sizeof(source_openable_records),source_openable_names,sizeof(source_openable_names),e));assert(table.size()==68);
 OpenableContainerRowV1 row;std::int32_t id=-1;
 assert(table.resolve("Swamp_Normal_Chest",id,row,e));assert(id==58&&row.visual==47&&row.loot==227&&row.sound==33&&row.keep_physics&&row.script.empty());
 assert(table.resolve("SwampCave_Normal_Chest",id,row,e));assert(id==55&&row.visual==47&&row.loot==223&&row.sound==33&&row.keep_physics&&row.script.empty());
 assert(table.resolve("swamp_normal_chest",id,row,e)&&id==-1); // source strcmp case-sensitive
 assert(!table.load(source_openable_records,sizeof(source_openable_records)-1,source_openable_names,sizeof(source_openable_names),e));assert(table.size()==68);
 assert(table.resolve("Swamp_Normal_Chest",id,row,e)&&id==58&&row.loot==227);
 std::cout<<"Actual original cache OpenableContainers68 rows + selected SWAMP source names/loot/visual/sound + bounded rejection PASS\n";
}
