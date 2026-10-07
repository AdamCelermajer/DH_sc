#include "../native_unused_sweep_v50.hpp"
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2::resources;
int main(){try{
 unsigned checks=0;
 auto check=[&](bool b){++checks;if(!b)throw std::runtime_error("native unused sweep regression");};
 struct Owner{unsigned generation;};
 std::map<std::uint32_t,Owner> actual{{1,{7}},{2,{7}},{3,{7}},{4,{7}}};
 std::set<std::uint32_t> retained{1,3}; // owning image and independent raw Draw
 std::vector<std::uint32_t> deleted;
 auto validate=[](auto name,const auto& owner,std::string& error){if(!name||owner.generation!=7){error="stale";return false;}return true;};
 auto release=[&](auto name,std::string&){deleted.push_back(name);return actual.erase(name)==1;};
 std::string error;NativeUnusedSweepV50 report;
 check(sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(deleted==std::vector<std::uint32_t>({2,4}));
 check(actual.size()==2&&actual.count(1)&&actual.count(3));
 check(report.inspected==4&&report.preserved==2&&report.released==2);
 deleted.clear();
 check(sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(deleted.empty()&&report.inspected==2&&report.preserved==2&&report.released==0);
 // A stale last allocation aborts the entire pass before deleting an earlier
 // otherwise-unused name. Preserve the caller's previous completed receipt.
 actual.emplace(2,Owner{7});actual.emplace(5,Owner{6});
 check(!sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(deleted.empty()&&actual.size()==4&&report.inspected==2);
 actual.erase(5);error.clear();
 auto failure=[&](auto name,std::string& e){deleted.push_back(name);e="actual release failed";return false;};
 check(!sweep_native_unused_v50(actual,retained,validate,failure,report,error));
 check(deleted==std::vector<std::uint32_t>({2})&&report.released==0&&error=="actual release failed");
 actual.clear();deleted.clear();
 check(sweep_native_unused_v50(actual,retained,validate,release,report,error));
 check(report.inspected==0&&report.released==0&&deleted.empty());
 std::cout<<"PASS "<<checks<<" native registry/ownership/preflight/release checks\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
