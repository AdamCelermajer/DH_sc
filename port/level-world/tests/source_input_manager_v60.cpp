#include "source_input_manager_v60.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_services_owner_v5.hpp"
#include <cassert>

int main(){
 using namespace dh2;
 std::string error;
 auto input=std::make_shared<input::SourceInputManagerV60>();
 auto application=std::make_shared<application::ApplicationServicesOwnerV5>();
 assert(application->publish_source_input_manager_v60(input,error));
 assert(input->gamepad_count()==4&&input->mouse_count()==1&&input->keyboard_count()==1);
 assert(input->enabled()==1&&input->keyboard().device.type4==1&&input->mouse().device.type4==1);
 auto state=[](const input::InputStateC1V60& s){
  assert(s.word00==0&&s.word04==0&&s.word08==1&&s.word0c==0);
  assert(s.word10==0&&s.word14==0&&s.word18==1&&s.byte1c==0);
 };
 for(const auto& s:input->keyboard().states)state(s);
 for(const auto& s:input->mouse().first_states)state(s);
 for(const auto& s:input->mouse().second_states)state(s);
 for(int index=0;index<4;++index){
  std::shared_ptr<const input::GamepadC1V60> pad,again;
  assert(input->get_gamepad(index,pad,error)&&input->get_gamepad(index,again,error));
  assert(pad.get()==again.get()&&pad->device.type4==2&&pad->device.word8==0);
  assert(pad->connected758==0&&pad->byte74c==0&&pad->word750==0&&pad->word754==128);
  for(const auto& s:pad->states0c)state(s);
  for(const auto& s:pad->states5ac)state(s);
  for(std::size_t i=0;i<4;++i){
   assert(pad->indices6fc[i]==i&&pad->indices73c[i]==i);
   for(const auto v:pad->vectors6cc[i])assert(v==0);
   for(const auto v:pad->vectors70c[i])assert(v==0);
  }
 }
 std::shared_ptr<const input::GamepadC1V60> invalid;
 assert(!input->get_gamepad(-1,invalid,error)&&!invalid);
 assert(!input->get_gamepad(4,invalid,error)&&!invalid);
 auto services=input->first_local_services();
 std::int32_t count=-1;player::FirstLocalControllerBorrowV59 borrow;
 assert(services.count(services.context,count,error)&&count==4);
 assert(services.controller_zero(services.context,borrow,error));
 std::shared_ptr<const input::GamepadC1V60> original;
 assert(input->get_gamepad(0,original,error));
 assert(borrow.identity==reinterpret_cast<std::uintptr_t>(original.get()));
 assert(borrow.connected758==&original->connected758&&*borrow.connected758==0);
 assert(borrow.receiver.get()==original.get());
 assert(!application->publish_source_input_manager_v60(std::make_shared<input::SourceInputManagerV60>(),error));
 assert(application->source_input_manager_v60().get()==input.get());
 // App publication and controller borrows retain the same owner; releasing
 // the caller's reference cannot invalidate the field pointer.
 std::weak_ptr<input::SourceInputManagerV60> weak=input;input.reset();application.reset();
 assert(!weak.expired()&&*borrow.connected758==0);
 services={};original.reset();borrow={};assert(weak.expired());
}
