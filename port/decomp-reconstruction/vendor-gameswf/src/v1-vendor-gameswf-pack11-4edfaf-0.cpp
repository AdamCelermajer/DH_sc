// Batch v1-vendor-gameswf-pack11-4edfaf-0 (vendor-gameswf). Owners: gameswf::as_property, gameswf::as_array,
// gameswf::standard_array_sorter, gameswf::custom_array_sorter, gameswf::as_listener, gameswf::as_color.
// 19 binary rows, ledgered in ledger.d/v1-vendor-gameswf-pack11-4edfaf-0.csv.
//
// Compiled here (no as_object or gc_ptr<as_function> is instantiated, so gameswf_object.h is not needed):
//   standard_array_sorter::operator() (0x7986e4). Uses as_value::to_number and to_tu_string, tu_string::stricmp
//   and strcmp. Declared in vendor-gameswf/include/vendor-gameswf/as_array_sorter.hpp.
//   gameswf::dh2::as_array_is_index_text (the text test under as_array::is_index) is in
//   vendor-gameswf/include/vendor-gameswf/as_array_dh2_helpers.hpp.
//
// Written but not compiled: PARTIAL, behind VENDOR_GAMESWF_DH2_CORE_HEADERS, as in pack4 and fddef6-1. These need
// gameswf_action.h, gameswf_environment.h and gameswf_as_classes/*.h, which do not compile in this tree
// (gc_ptr<as_object> instantiated in base/tu_gc_singlethreaded_refcount.h:120 and 123, as_object incomplete):
//   custom_array_sorter::operator(), as_array::is_index (member wrapper), pop, this_alive (stub), enumerate,
//   as_listener C1 and C2 (same body), as_listener D1 (stub), as_color C1 and C2 (same body).
//
// Not defined here:
//   as_property D1 and D2 (0x796c28, 0x796c9c). The body is the upstream empty destructor in gameswf_value.cpp;
//     the binary's explicit releases are the inlined gc_ptr destructors. Ledgered upstream.
//   as_array D1 (0x79a440) and as_color D1 (0x79c0a0). Compiler-generated implicit destructors of the upstream
//     classes. Ledgered upstream with a layout caveat.
//   The deleting destructors D0 (0x796c80, 0x79a4bc, 0x79b6b4, 0x79c0fc) are emitted from delete. Skipped.
//
// Upstream headers changed (DH2 lines only, each marked "// DH2:"):
//   upstream/gameswf-r1714/gameswf/gameswf_as_classes/as_array.h   is_index, pop, enumerate, this_alive
//   upstream/gameswf-r1714/gameswf/gameswf_as_classes/as_broadcaster.h   ~as_listener
// Those two headers fail to compile in this tree before and after the change (gameswf_action.h, tu_gc).
// Upstream definitions replaced by the constructors below, wrapped in "#if 0 /* DH2: replaced by ... */":
//   upstream/gameswf-r1714/gameswf/gameswf_as_classes/as_broadcaster.cpp   as_listener::as_listener(player*)
//   upstream/gameswf-r1714/gameswf/gameswf_as_classes/as_color.cpp         as_color::as_color(player*, character*)

#include <cstring>
#include <functional>

#include "base/container.h"                          // tu_string, tu_stringi (compiles on its own)
#include "vendor-gameswf/as_value.hpp"               // as_value (DH2)
#include "vendor-gameswf/as_array_sorter.hpp"        // standard_array_sorter, custom_array_sorter (DH2)
#include "vendor-gameswf/as_array_dh2_helpers.hpp"   // dh2::as_array_is_index_text

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf_action.h"              // call_method (DH2 six-argument form), fn_call
#include "gameswf/gameswf_environment.h"         // as_environment
#include "gameswf/gameswf_as_classes/as_array.h"
#include "gameswf/gameswf_as_classes/as_broadcaster.h"
#include "gameswf/gameswf_as_classes/as_color.h"
#include "gameswf/gameswf_character.h"
#endif

namespace gameswf {

namespace {

// as_global_array::option (upstream gameswf_as_classes/as_array.h). The sorters test these bits.
constexpr int kCaseInsensitive = 1;
constexpr int kDescending = 2;
constexpr int kNumeric = 16;

}  // namespace

// 0x7986e4 gameswf::standard_array_sorter::operator()(gameswf::as_value const&, gameswf::as_value const&)
// Checks the option bits in this order. NUMERIC compares to_number() values and returns. Otherwise, CASEINSENSITIVE
// compares the two strings with tu_string::stricmp, when the two values hold different string objects and the
// comparison is not equal. Otherwise (or when the case-insensitive comparison is equal), the strings are compared
// with strcmp, again only when they are different objects and the comparison is not equal. When that is also equal
// (or the objects are the same), the result is the address order of the two values. DESCENDING negates the result
// of each branch (the binary returns result ^ 1, or this >= a3 for the address branch).
bool standard_array_sorter::operator()(const as_value& lhs, const as_value& rhs) {
	const int options = m_options;  // +0x0c
	const bool descending = (options & kDescending) != 0;

	if ((options & kNumeric) != 0) {
		const double lhs_number = lhs.to_number();
		const bool less = lhs_number < rhs.to_number();
		return descending ? !less : less;
	}

	const tu_string& lhs_text = lhs.to_tu_string();
	const tu_string& rhs_text = rhs.to_tu_string();

	if ((options & kCaseInsensitive) != 0 && &lhs_text != &rhs_text) {
		const int c = tu_string::stricmp(lhs_text.c_str(), rhs_text.c_str());
		if (c != 0) {
			const bool less = c < 0;
			return descending ? !less : less;
		}
	}

	if (&lhs_text != &rhs_text) {
		const int c = std::strcmp(lhs_text.c_str(), rhs_text.c_str());
		if (c != 0) {
			const bool less = c < 0;
			return descending ? !less : less;
		}
	}

	const bool less = std::less<const as_value*>()(&lhs, &rhs);
	return descending ? !less : less;
}

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)

// The as_color method bodies live in other batches (0x79d538 getRGB, 0x79c118 setRGB, 0x79cf14 getTransform,
// 0x79c924 setTransform). Declared here for the guarded constructor below.
void as_color_getRGB(const fn_call& fn);
void as_color_setRGB(const fn_call& fn);
void as_color_gettransform(const fn_call& fn);
void as_color_settransform(const fn_call& fn);

// PARTIAL. Not compiled in this tree (see the file comment).
// 0x799514 gameswf::custom_array_sorter::operator()(gameswf::as_value const&, gameswf::as_value const&)
// The binary pushes lhs, then rhs, on the environment stack (the environment's first member is the array). It calls
// the comparison function with method = *m_compare_fn, this = *m_compare_fn, two arguments, first argument bottom
// index = stack size - 1 (read after the pushes), and the debug label "???". Then it resizes the stack to size - 2 and
// returns true when the result converts (truncating) to 1. The result is destroyed at the end, as in the binary.
bool custom_array_sorter::operator()(const as_value& lhs, const as_value& rhs) {
	array<as_value>& stack = *reinterpret_cast<array<as_value>*>(m_env);
	stack.push_back(lhs);
	stack.push_back(rhs);
	as_value result = call_method(*m_compare_fn, m_env, *m_compare_fn, 2, stack.size() - 1, "???");
	stack.resize(stack.size() - 2);
	return static_cast<int>(result.to_number()) == 1;
}

// PARTIAL. 0x79806c gameswf::as_array::is_index(gameswf::tu_stringi const&)
// Non-const member. The binary never reads this (its first argument is unused); the string is the second argument.
// The text test is gameswf::dh2::as_array_is_index_text, compiled and tested in this batch's test.
bool as_array::is_index(const tu_stringi& name) {
	return dh2::as_array_is_index_text(name.c_str());
}

// PARTIAL. 0x7980cc gameswf::as_array::pop(gameswf::as_value*)
// Copies the last element to *val, then resizes the array to one fewer element. The binary does not test for an empty
// array, and this build does not either (callers keep the count above zero).
void as_array::pop(as_value* val) {
	*val = m_array[m_array.size() - 1];
	m_array.resize(m_array.size() - 1);
}

// PARTIAL, STUB. 0x79839c gameswf::as_array::this_alive()
// The binary calls weak_ptr<player>::check_proxy on as_object's m_player (+0x2c). If the object's word at +0x34 differs
// from the player's word at +0x30 + 48 (the player's counter), it calls as_object::this_alive() and then this_alive() on
// each element that is an object and whose +0x34 word differs in the same way. The +0x34 word and the player's counter
// are not declared in the tree, so the body is left empty. Layout request: the as_object generation word and the
// player counter.
void as_array::this_alive() {
}

// PARTIAL. 0x798bc8 gameswf::as_array::enumerate(gameswf::as_environment*)
// Calls as_object::enumerate first, then pushes the numbers 0 to size-1 onto the environment stack. The binary's
// return value is the last push_back's return register, not a result, so the DH2 signature is void.
void as_array::enumerate(as_environment* env) {
	as_object::enumerate(env);
	array<as_value>& stack = *reinterpret_cast<array<as_value>*>(env);
	const int count = m_array.size();
	for (int i = 0; i < count; ++i) {
		stack.push_back(as_value(i));
	}
}

// PARTIAL. 0x79b518 gameswf::as_listener::as_listener(gameswf::player*) [C1]
// PARTIAL. 0x79b570 gameswf::as_listener::as_listener(gameswf::player*) [C2, same body]
// Replaces the upstream constructor in as_broadcaster.cpp (wrapped in #if 0 there). The binary runs the as_object(player*)
// base constructor, sets the vtable, and zeroes +0x38..+0x54: the listener array (buffer +0x38, count +0x3c, capacity
// +0x40), a byte flag at +0x44, a byte flag at +0x48, the suspended-event queue at +0x4c (count +0x54), and +0x50. The
// upstream initialiser m_reentrance(false) is kept for the flag at +0x44; that offset is not verified. Upstream as_listener
// has a different member set (a listener wrapper), so the array, queue and +0x48 flag are left to default construction.
// Layout request: DH2 member names for +0x38..+0x54.
as_listener::as_listener(player* p) : as_object(p), m_reentrance(false) {
}

// PARTIAL, STUB. 0x79b5c8 gameswf::as_listener::~as_listener() [D1]
// The binary: vtable, pops the suspended-event queue while its count (+0x54) is non-zero, releases each listener weak
// proxy (count at +0x3c; a proxy whose count goes to 0 is freed), sets the count to 0, frees the buffer with reserve(0),
// then runs as_object::~as_object. Left empty: the member names are not declared (layout request).
as_listener::~as_listener() {
}

// PARTIAL. 0x79c2f0 gameswf::as_color::as_color(gameswf::player*, gameswf::character*) [C1]
// PARTIAL. 0x79c66c gameswf::as_color::as_color(gameswf::player*, gameswf::character*) [C2, same body]
// Replaces the upstream constructor in as_color.cpp (wrapped in #if 0 there). Order in the binary: as_object(player)
// base, vtable, weak_ptr<character> m_target = target (proxy at +0x38), the cxform defaults at +0x40..+0x5c, then eight
// words copied from the character's cxform pointer at character+0x48. This build copies the cxform with the upstream
// accessor character::get_cxform(). That the accessor returns the same eight words as the binary's +0x48 pointer is
// NOT verified. Upstream's assert(target) is not in the binary and is not written. Then four builtin members (getRGB,
// setRGB, getTransform, setTransform) and the constructor as_global_color_ctor.
as_color::as_color(player* p, character* target) : as_object(p) {
	m_target = target;
	m_cxform = target->get_cxform();

	as_value get_rgb;
	get_rgb.set_as_c_function(as_color_getRGB);
	builtin_member("getRGB", get_rgb);

	as_value set_rgb;
	set_rgb.set_as_c_function(as_color_setRGB);
	builtin_member("setRGB", set_rgb);

	as_value get_transform;
	get_transform.set_as_c_function(as_color_gettransform);
	builtin_member("getTransform", get_transform);

	as_value set_transform;
	set_transform.set_as_c_function(as_color_settransform);
	builtin_member("setTransform", set_transform);

	as_value ctor;
	ctor.set_as_c_function(as_global_color_ctor);
	set_ctor(ctor);
}

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

}  // namespace gameswf
