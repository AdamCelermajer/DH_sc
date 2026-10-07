#include "../../engine-animation/animation.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
int main(int argc,char** argv){try{if(argc!=5)throw std::runtime_error("actual three Knight animations and prince model required");unsigned delivered=0;
 std::ifstream model_file(argv[4],std::ios::binary);if(!model_file)throw std::runtime_error(argv[4]);std::vector<std::uint8_t> model{std::istreambuf_iterator<char>(model_file),{}};dh2::resources::BresView image{};if(dh2_bres_open(&image,model.data(),model.size())!=dh2::resources::BresError::ok)throw std::runtime_error("source prince image");dh2::scene::Scene bindings;std::string e;if(!dh2::scene::load(image,bindings,e))throw std::runtime_error(e);
 for(int a=1;a<4;++a){std::ifstream f(argv[a],std::ios::binary);if(!f)throw std::runtime_error(argv[a]);std::vector<std::uint8_t> raw{std::istreambuf_iterator<char>(f),{}};dh2::animation::Player player;if(!player.load(raw.data(),raw.size(),bindings,e,dh2::animation::MissingTargets::ignore))throw std::runtime_error(e);
  auto view=player.events.view();const int use=dh2_events_time(&view,"do_skill");std::cerr<<"source range "<<player.start<<' '<<player.end<<" eventtype "<<view.type<<" groups "<<view.count<<" use "<<use<<'\n';for(unsigned g=0;g<view.count;++g)for(unsigned n=0;n<view.groups[g].count;++n)std::cerr<<"group "<<g<<" name ["<<view.groups[g].names[n]<<"]\n";if(use<player.start||use>player.end||use<0)throw std::runtime_error("actual do_skill outside authored range");
  dh2::animation::EventCursor cursor;unsigned count=0;for(int ms=player.start;ms<=player.end+16;ms+=16){const int next=std::min(ms,player.end);if(!dh2_events_update(&view,&cursor,ms-16,next,player.start,player.end,[](const dh2::animation::TriggeredEvent* q,void* p){if(q->name&&std::string(q->name)=="do_skill")++*static_cast<unsigned*>(p);},&count))throw std::runtime_error("actual interval rejected");}
  if(count!=1)throw std::runtime_error("authored Use event expected exactly once");delivered+=count;std::cout<<"actual skill clip "<<argv[a]<<" start "<<player.start<<" end "<<player.end<<" do_skill "<<use<<" deliveries "<<count<<'\n';
 }std::cout<<"{\"validation\":\"PASS\",\"authored_use_deliveries\":"<<delivered<<",\"live_skill_damage\":false}"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
