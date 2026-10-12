#pragma once
#include <cstddef>
namespace dh2::loader::test_v46 {
// Test-only malloc/free-backed libstdc++ vector allocation observer. Link the
// .cpp only into the isolated host probe, never production/shared libraries.
void watch_final_byte_delete_v46(const void*) noexcept;
bool final_byte_delete_observed_v46() noexcept;
void clear_final_byte_delete_watch_v46() noexcept;
}
