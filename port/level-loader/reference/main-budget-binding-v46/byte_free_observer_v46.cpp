#include "byte_free_observer_v46.hpp"
#include <cstdlib>
#include <new>
namespace {const void* watched{};bool observed{};
void release(void* p)noexcept {const bool target=p&&p==watched;std::free(p);if(target)observed=true;}
}
namespace dh2::loader::test_v46 {
void watch_final_byte_delete_v46(const void* p)noexcept {watched=p;observed=false;}
bool final_byte_delete_observed_v46()noexcept {return observed;}
void clear_final_byte_delete_watch_v46()noexcept {watched=nullptr;observed=false;}
}
// All are test-only host delete endpoints. Sized/array forms free exactly once.
void operator delete(void* p)noexcept {release(p);}
void operator delete(void* p,std::size_t)noexcept {release(p);}
void operator delete[](void* p)noexcept {release(p);}
void operator delete[](void* p,std::size_t)noexcept {release(p);}
