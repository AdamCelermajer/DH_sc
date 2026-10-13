// Test for batch v1-vendor-gameswf-pack3-a95b99-0.
//   bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-pack3-a95b99-0.cpp
//
// Covered: array<swf_event*>::release_buffer (0x75d89c), permanent_string_cache::clear (0x76c8ac) and its
// destructor (0x76c990). as_environment::pop and get_player need a constructed as_environment, which needs
// as_object and the as_value bodies, so they are not run here.
//
// The test supplies the callees that are not in this batch:
//   gameswf::destruct<permanent_string> (0x76c870, another batch) records the call and deletes the object
//   gameswf::free_internal (0x752b38) frees with free()
//   gameswf::as_value::operator= and drop_refs (the as_value batch): no-ops, so the array copy is unused here
//   gameswf::string_pointer::operator== and string_pointer_hash_functor (not in this batch): text compare and hash

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "vendor-gameswf/as_value.hpp"
#include "vendor-gameswf/gameswf_alloc.hpp"
#include "vendor-gameswf/permanent_string_cache.hpp"

namespace {

int g_failures = 0;
int g_destruct_calls = 0;

#define CHECK(cond)                                                              \
	do {                                                                         \
		if (!(cond)) {                                                           \
			std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
			++g_failures;                                                        \
		}                                                                        \
	} while (0)

}  // namespace

namespace gameswf {

struct swf_event;  // only pointers are stored in the test array

struct permanent_string {
	int id;
};

// Stand-in for 0x76c870 (destruct<permanent_string>, skipped in the fddef6-2 batch).
template <class T>
void destruct(const T* object) {
	++g_destruct_calls;
	delete const_cast<T*>(object);
}

// clear() in the batch source calls this, so the test TU must emit the instantiation itself.
template void destruct<permanent_string>(const permanent_string* object);

void free_internal(void* p, unsigned int size) {
	(void)size;
	std::free(p);
}

void as_value::operator=(const as_value& v) {
	(void)v;
}

void as_value::drop_refs() {}

bool string_pointer::operator==(const string_pointer& other) const {
	return std::strcmp(m_string->c_str(), other.m_string->c_str()) == 0;
}

template <>
std::size_t string_pointer_hash_functor<string_pointer>::operator()(const string_pointer& key) const {
	std::size_t h = 5381;
	for (const char* p = key.m_string->c_str(); *p != 0; ++p) {
		h = h * 33 + static_cast<unsigned char>(*p);
	}
	return h;
}

}  // namespace gameswf

// release_buffer drops the buffer and the count. The array stays usable afterwards.
void test_release_buffer() {
	::array<gameswf::swf_event*> events;
	int a = 0;
	int b = 0;
	events.push_back(reinterpret_cast<gameswf::swf_event*>(&a));
	events.push_back(reinterpret_cast<gameswf::swf_event*>(&b));
	CHECK(events.size() == 2);

	events.release_buffer();
	CHECK(events.size() == 0);

	events.push_back(nullptr);
	CHECK(events.size() == 1);
	CHECK(events[0] == nullptr);
}

// clear() destructs the permanent string of each entry once, then the table is empty.
void test_clear_destructs_entries() {
	g_destruct_calls = 0;
	gameswf::permanent_string_cache cache;
	::tu_string t1("alpha");
	::tu_string t2("beta");
	::tu_string t3("gamma");

	cache.add(gameswf::string_pointer{&t1}, new gameswf::permanent_string{1});
	cache.add(gameswf::string_pointer{&t2}, new gameswf::permanent_string{2});
	cache.add(gameswf::string_pointer{&t3}, new gameswf::permanent_string{3});
	CHECK(cache.size() == 3);

	cache.clear();
	CHECK(g_destruct_calls == 3);
	CHECK(cache.size() == 0);
	CHECK(cache.is_empty());

	// The cache takes new entries after clear, and clear() on an empty cache destructs nothing.
	cache.add(gameswf::string_pointer{&t1}, new gameswf::permanent_string{4});
	CHECK(cache.size() == 1);
	cache.clear();
	CHECK(g_destruct_calls == 4);
	cache.clear();
	CHECK(g_destruct_calls == 4);
}

// The destructor runs clear(): each entry's permanent string is destructed once.
void test_destructor_destructs_entries() {
	g_destruct_calls = 0;
	::tu_string t1("one");
	::tu_string t2("two");
	gameswf::permanent_string_cache* cache = new gameswf::permanent_string_cache;
	cache->add(gameswf::string_pointer{&t1}, new gameswf::permanent_string{5});
	cache->add(gameswf::string_pointer{&t2}, new gameswf::permanent_string{6});
	delete cache;
	CHECK(g_destruct_calls == 2);
}

int main() {
	test_release_buffer();
	test_clear_destructs_entries();
	test_destructor_destructs_entries();

	if (g_failures == 0) {
		std::printf("OK v1-vendor-gameswf-pack3-a95b99-0 tests\n");
		return 0;
	}
	std::printf("%d failure(s)\n", g_failures);
	return 1;
}
