#include "budget-extracted-v39.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
namespace {unsigned checks{};void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
template<class Callback>void stops(Callback callback,const char* text){try{callback();throw std::runtime_error("budget accepted invalid work");}catch(const QaBudgetStopV39& stopped){check(std::string(stopped.reason)==text,"wrong named budget stop");}}
}
int main(){try{
 QaBudgetV39 budget;budget.check_graph(64,4096,65536);check(budget.units==0,"graph validation performed an iteration");
 stops([&]{budget.check_graph(65,1,1);},"floor cap64");stops([&]{budget.check_graph(1,4097,1);},"node cap4096");stops([&]{budget.check_graph(1,1,65537);},"edge cap65536");
 for(unsigned i=0;i<25000;++i){budget.step();}
 check(budget.units==25000,"iteration accounting");stops([&]{budget.step();},"iteration cap25000");check(budget.units==25000,"rejected iteration accounted as done");
 budget.start=std::chrono::steady_clock::now()-std::chrono::milliseconds(1001);stops([&]{budget.check_time();},"elapsed cap1000ms");budget.units=0;stops([&]{budget.step();},"elapsed cap1000ms");check(budget.units==0,"timed-out unit executed");
 std::cout<<"PASS actual extracted QA guard: graph caps,25000 exact units and named iteration/time stops before work; checks="<<checks<<"\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
