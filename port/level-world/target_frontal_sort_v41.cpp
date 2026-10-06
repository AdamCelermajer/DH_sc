#include "target_frontal_sort_v41.hpp"
namespace dh2::character {
int target_frontal_sort_v41(target_search::List40& list,std::string& error){
 error.clear();if(list.count>list.capacity||list.capacity>65536||list.sort>2){error="Invalid source target heap sort boundary";return -1;}
 while(list.count){target_search::Target24 unused{};if(target_search::dh2_target_pop(&list,&unused)){error="Required source target heap Pop before frontal sorting";return -1;}}
 list.sort=2;return 0;
}
}
