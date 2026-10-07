#include "canonical_object_manager_v1.hpp"
#include <iostream>
#include <stdexcept>
using dh2::world::CanonicalObjectManagerV1;
static unsigned checks;
static void ck(bool v,const char* why){++checks;if(!v)throw std::runtime_error(why);}
int main(){try{
 std::string e;auto manager=std::make_shared<CanonicalObjectManagerV1>(dh2::world::CanonicalObjectManagerServicesV1{});
 CanonicalObjectManagerV1::FreshSourceBorrowV50 fresh;
 ck(manager->borrow_fresh_source_v50(manager,fresh,e)&&fresh.same_fresh_producer(manager),"Authentic completed source C1 receipt");
 ck(manager->source_next_key4c()==1&&manager->source_count50()==0&&manager->source_map_size1c_v38()==1,"Exact source constructor scalars preserved");
 dh2::target_providers::Handle16 named{};
 ck(manager->by_name("SourceConfig",-1,true,nullptr,named,e),"Actual source name node producer");
 ck(manager->source_count50()==0&&manager->source_init_phase7c_v38()==0,"No actor/Init shortcut");
 ck(!fresh.same_fresh_producer(manager)&&!manager->borrow_fresh_source_v50(manager,fresh,e),"phase0 plus count0 is insufficient after source mutation");
 manager->source_init_phase7c_v38()=0;
 ck(!fresh.same_fresh_producer(manager),"Resetting a source phase cannot restore native constructor provenance");
 auto second=std::make_shared<CanonicalObjectManagerV1>(dh2::world::CanonicalObjectManagerServicesV1{});
 ck(second->borrow_fresh_source_v50(second,fresh,e),"Actual new source constructor produces new receipt");
 auto unrelated=std::make_shared<int>(0);std::shared_ptr<CanonicalObjectManagerV1> counterfeit(unrelated,second.get());
 ck(!fresh.same_fresh_producer(counterfeit),"Same pointer with foreign ownership block rejected");
 dh2::target_providers::Handle16 missing{};missing.key=42;const dh2::world::CanonicalObjectBorrowV1* out{};
 ck(second->resolve_handle_v4(missing,false,out,{},e)&&!out,"Original absent handle map insertion");
 ck(!fresh.same_fresh_producer(second)&&second->source_count50()==0,"Inserted NULL map node consumes constructor provenance without fake actor count");
 auto third=std::make_shared<CanonicalObjectManagerV1>(dh2::world::CanonicalObjectManagerServicesV1{});
 ck(third->borrow_fresh_source_v50(third,fresh,e),"Fresh third C1");third->begin_frame(0);
 ck(!fresh.same_fresh_producer(third),"Prior source frame producer cannot become fresh by phase0");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_manager_cpp\":true,\"phase_count_not_freshness_authority\":true}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
