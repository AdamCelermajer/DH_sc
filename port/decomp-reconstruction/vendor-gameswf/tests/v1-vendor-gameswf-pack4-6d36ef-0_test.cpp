// Test for batch v1-vendor-gameswf-pack4-6d36ef-0. Covers array<T>::push_back<U> (0x7619b8): the capacity rule, and the
// member template itself (one copy-construction into the open slot per push, values kept across reallocations).
// Returns non-zero on failure.
//
// The batch source is included so the member template's definition is visible to this translation unit. The host
// allocator stands in for the tu_malloc / tu_realloc / tu_free macros (base/tu_config.h, which otherwise routes to
// dlmalloc, not linked here).
#include <cstdio>
#include <cstdlib>

#if defined(__GNUC__) && !defined(__clang__)
// array<T> moves its elements bitwise on reallocation (upstream design, container.h). The counting element type below
// is not trivially copyable, so GCC warns about that move; the warning is expected here.
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

#define tu_malloc(size) std::malloc(size)
#define tu_realloc(old_ptr, new_size, old_size) std::realloc(old_ptr, new_size)
#define tu_free(old_ptr, old_size) std::free(old_ptr)
// On _WIN32, base/tu_config.h makes exported_module __declspec(dllexport). That emits every tu_string member in this
// TU and needs base/container.cpp, which is not linked. This test does not use tu_string, so the macro is empty here.
#define exported_module

#include "../src/v1-vendor-gameswf-pack4-6d36ef-0.cpp"
#include "vendor-gameswf/gameswf_array_grow_dh2_helpers.hpp"

using gameswf::dh2::array_push_needs_reserve;
using gameswf::dh2::array_push_reserve_size;

static int g_failures = 0;

#define CHECK(cond)                                                    \
	do {                                                               \
		if (!(cond)) {                                                 \
			std::printf("FAIL line %d: %s\n", __LINE__, #cond);        \
			++g_failures;                                              \
		}                                                              \
	} while (0)

namespace {

// Element type for the array test. The copy constructor counts copies. The default constructor is needed because
// array<T>::resize default-constructs new slots. Contents move by bitwise copy on reallocation, as upstream expects.
struct counted {
	int value;
	static int copies;

	counted() : value(-1) {}
	explicit counted(int v) : value(v) {}
	counted(const counted& o) : value(o.value) { ++copies; }
	counted& operator=(const counted& o) { value = o.value; return *this; }
};

int counted::copies = 0;

}  // namespace

int main()
{
	// reserve size is new_size + new_size / 2
	CHECK(array_push_reserve_size(1) == 1);
	CHECK(array_push_reserve_size(2) == 3);
	CHECK(array_push_reserve_size(3) == 4);
	CHECK(array_push_reserve_size(4) == 6);
	CHECK(array_push_reserve_size(7) == 10);

	// reserve only when the new count is above the buffer size
	CHECK(array_push_needs_reserve(1, 0));
	CHECK(!array_push_needs_reserve(3, 3));
	CHECK(array_push_needs_reserve(4, 3));

	// push ten elements into an empty array: capacity grows 1, 3, 6, 10 (four reserves)
	int size = 0;
	int capacity = 0;
	int reserves = 0;
	for (int i = 0; i < 10; ++i) {
		int new_size = size + 1;
		if (array_push_needs_reserve(new_size, capacity)) {
			capacity = array_push_reserve_size(new_size);
			++reserves;
		}
		size = new_size;
		CHECK(capacity >= size);
	}
	CHECK(size == 10);
	CHECK(capacity == 10);
	CHECK(reserves == 4);

	// The real template: push_back<counted> copy-constructs each element into the open slot (one copy per push, the
	// binary's line_style path). Ten pushes cross several reallocations; the values must survive them.
	{
		array<counted> a;
		counted::copies = 0;
		for (int i = 0; i < 10; ++i) {
			a.push_back<counted>(counted(i));
		}
		CHECK(a.size() == 10);
		CHECK(counted::copies == 10);
		for (int i = 0; i < 10; ++i) {
			CHECK(a[i].value == i);
		}
	}

	if (g_failures == 0) {
		std::printf("ok\n");
		return 0;
	}
	std::printf("%d failure(s)\n", g_failures);
	return 1;
}
