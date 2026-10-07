#pragma once
#include <string>
namespace dh2::ui {
// Synchronous source MenuManager.consumeEvent loan. The stack restores any
// enclosing delivery; no movie/reset retains a callback into a dead manager.
struct SourceMenuConsumptionV121 {
 void* context{};
 bool (*consume)(void*,std::string&){};
 int x{},y{};
 inline static thread_local SourceMenuConsumptionV121* current{};
 SourceMenuConsumptionV121* previous{};
 SourceMenuConsumptionV121(void* c,bool(*f)(void*,std::string&)):
  context(c),consume(f),previous(current){current=this;}
 ~SourceMenuConsumptionV121(){current=previous;}
 SourceMenuConsumptionV121(const SourceMenuConsumptionV121&)=delete;
 SourceMenuConsumptionV121& operator=(const SourceMenuConsumptionV121&)=delete;
};
}
