// Batch v1-vendor-gameswf-pack4-6d36ef-0 (vendor-gameswf). Owners: gameswf::fill_style, gameswf::array<line_style>,
// gameswf::morph2_character_def, gameswf::movie_def_impl. 19 binary rows, ledgered in
// ledger.d/v1-vendor-gameswf-pack4-6d36ef-0.csv.
//
// Compiled here (no gameswf.h dependency):
//   array<T>::push_back<U> (0x7619b8 for U = line_style; array is global, as in base/container.h). The member
//   template is declared in base/container.h (DH2) and defined below. The batch test instantiates it for a counting
//   element type. Its line_style specialization is behind VENDOR_GAMESWF_DH2_CORE_HEADERS.
//   gameswf::dh2::array_push_* (include/vendor-gameswf/gameswf_array_grow_dh2_helpers.hpp), tested in the batch test.
//
// Behind VENDOR_GAMESWF_DH2_CORE_HEADERS (PARTIAL; not built by default, as in fddef6-1 and the shape batch):
//   every fill_style, morph2_character_def and movie_def_impl body. gameswf_styles.h, gameswf_morph2.h and
//   gameswf_movie_def.h include gameswf_impl.h, which pulls in gameswf.h. That header fails in this tree
//   (DECLARE_GC_TYPES, base/tu_gc_singlethreaded_refcount.h), so the guard-on check below is a syntax check only.
//   With the guard on (g++ -fsyntax-only), the fill_style, array<line_style> specialization, has_init_actions,
//   get_labeled_frame, movie_def_impl and export_resource bodies report no error of their own. The morph2 bodies
//   fail on the by-value m_shape1 / m_shape2 (layout request) and on operator new(size, swf::MemHint), which this
//   TU does not declare (fddef6-0 and the as_value batch use the same call).
//
// Skipped (compiler-generated deleting destructors, emitted from delete): 0x784d4c fill_style D0,
//   0x762184 morph2_character_def D0, 0x7663b8 movie_def_impl D0.
//
// Upstream definitions this file replaces are wrapped in #if 0 /* DH2 */ in:
//   upstream/gameswf-r1714/gameswf/gameswf_styles.cpp     (fill_style ctor and dtor)
//   upstream/gameswf-r1714/gameswf/gameswf_morph2.cpp     (morph2_character_def ctor and dtor)
//   upstream/gameswf-r1714/gameswf/gameswf_movie_def.cpp  (movie_def_impl ctor, export_resource)
//
// Layouts. 32-bit offsets are comments only; members are declared in the owner headers with // DH2 comments.
//   fill_style (84 bytes in the binary): vptr +0x00, int m_type +0x04, rgba m_color +0x08, matrix m_gradient_matrix
//     +0x0c, array<gradient_record> m_gradients +0x24, gc_ptr m_gradient_bitmap_info +0x34, gc_ptr
//     m_bitmap_character +0x38, matrix m_bitmap_matrix +0x3c. No m_blend_mode: the byte at +0x30 is the trailing
//     byte of the 16-byte array (see below).
//   array<T> (16 bytes in the binary): m_buffer +0x00, m_size +0x04, m_buffer_size +0x08, a byte at +0x0c that
//     the constructors zero and no copy copies. Upstream container.h has 12 bytes. Layout request (no member
//     declared for the byte).
//   morph2_character_def: shape_character_def base (0x88 bytes), then heap shapes m_shape1 +0x88 and m_shape2 +0x8c
//     (operator new 0x88 each), an unidentified run +0x90..+0x9b, float m_last_ratio +0x9c, unsigned m_offset
//     +0xa0, mesh_set* m_mesh +0xa4. Layout request: upstream declares m_shape1 and m_shape2 by value.

#include "base/container.h"                      // gameswf::array<T> (compiles on its own in this tree)
#include "vendor-gameswf/gameswf_alloc.hpp"      // gameswf::free_internal
#include "vendor-gameswf/gameswf_dh2_runtime.hpp"  // gameswf::destruct<T>
#include "vendor-gameswf/gameswf_array_grow_dh2_helpers.hpp"

#include <cstring>
#include <new>

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf_styles.h"
#include "gameswf/gameswf_morph2.h"
#include "gameswf/gameswf_movie_def.h"
#endif

// array<> is in the global namespace (base/container.h), so its member template is defined here, outside gameswf.

// 0x7619b8 _ZN7gameswf5arrayINS_10line_styleEE9push_backIS1_EEvRKT_
// 0x7619b8 void gameswf::array<gameswf::line_style>::push_back<gameswf::line_style>(gameswf::line_style const&)
// Member template declared in base/container.h (DH2). The binary copy-constructs the new slot in place: it does
// not default-construct and assign, as the upstream push_back does through resize(). The binary calls reserve
// only when the new count is above the buffer size, with new_size + new_size / 2 (gameswf::dh2::array_push_*).
// The parameter is const U& (the binary mangles it as RKT_, the function template's first parameter), so it is
// declared with U, not with T: a const T& signature mangles as RKS1_ and does not match 0x7619b8. The line_style
// case is the explicit specialization below (behind VENDOR_GAMESWF_DH2_CORE_HEADERS). The batch test instantiates
// this generic definition for a counting element type.
template <class T>
template <class U>
void array<T>::push_back(const U& val)
{
	int new_size = m_size + 1;
	if (gameswf::dh2::array_push_needs_reserve(new_size, m_buffer_size))
	{
		reserve(gameswf::dh2::array_push_reserve_size(new_size));
	}
	new (m_buffer + m_size) T(val);	// copy-construct into the open slot
	m_size = new_size;
}

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
// Explicit specialization for line_style, not an explicit instantiation: the upstream non-template
// push_back(const T&) has the same parameter type, so `template void ...::push_back<line_style>(const line_style&)`
// is ambiguous (GCC). The specialization emits the strong symbol of 0x7619b8, with the body of the generic
// definition above.
template <>
template <>
void array<gameswf::line_style>::push_back<gameswf::line_style>(const gameswf::line_style& val)
{
	int new_size = m_size + 1;
	if (gameswf::dh2::array_push_needs_reserve(new_size, m_buffer_size))
	{
		reserve(gameswf::dh2::array_push_reserve_size(new_size));
	}
	new (m_buffer + m_size) gameswf::line_style(val);	// copy-construct into the open slot
	m_size = new_size;
}
#endif

namespace gameswf
{

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)

// 0x761878 _ZN7gameswf10fill_styleC1ERKS0_
// 0x761878 gameswf::fill_style::fill_style(gameswf::fill_style const&)
// Partial, not built. Explicit copy (the upstream class has none). Copies type, color and gradient matrix, resizes
// the gradient array to the source count and copies each 5-byte gradient_record (binary: memcpy of 5 bytes), then
// takes references to the bitmap info and the bitmap character, and copies the bitmap matrix. The trailing byte of
// the array (+0x30) is zeroed in the binary and not copied here (no member is declared for it).
fill_style::fill_style(const fill_style& other)
	: base_fill_style()
{
	m_type = other.m_type;
	m_color = other.m_color;
	m_gradient_matrix = other.m_gradient_matrix;

	m_gradients.resize(other.m_gradients.size());
	for (int i = 0; i < m_gradients.size(); i++)
	{
		std::memcpy(&m_gradients[i], &other.m_gradients[i], sizeof(gradient_record));
	}

	m_gradient_bitmap_info = other.m_gradient_bitmap_info;	// add_ref through gc_ptr copy
	m_bitmap_character = other.m_bitmap_character;
	m_bitmap_matrix = other.m_bitmap_matrix;
}

// 0x77a780 _ZN7gameswf10fill_styleaSERKS0_
// 0x77a780 gameswf::fill_style::operator=(gameswf::fill_style const&)
// Partial, not built. The same copy as the copy constructor, into an existing object. Returns *this. The binary
// does not test for self-assignment; the copy is the same bytes, so memmove is used to keep it defined.
fill_style& fill_style::operator=(const fill_style& other)
{
	m_type = other.m_type;
	m_color = other.m_color;
	m_gradient_matrix = other.m_gradient_matrix;

	m_gradients.resize(other.m_gradients.size());
	for (int i = 0; i < m_gradients.size(); i++)
	{
		std::memmove(&m_gradients[i], &other.m_gradients[i], sizeof(gradient_record));
	}

	m_gradient_bitmap_info = other.m_gradient_bitmap_info;	// set_ref
	m_bitmap_character = other.m_bitmap_character;
	m_bitmap_matrix = other.m_bitmap_matrix;
	return *this;
}

// 0x784a20 _ZN7gameswf10fill_styleD2Ev
// 0x784cac _ZN7gameswf10fill_styleD1Ev (identical body)
// Partial, not built. The binary releases m_bitmap_character (+0x38), then m_gradient_bitmap_info (+0x34), then
// clears the gradient array (resize 0, reserve 0). Member destruction in reverse declaration order gives the same
// sequence, so the explicit releases below are redundant but reproduce the binary's order; the member destructors
// that run after the body find null pointers. The vptr reset is the compiler's.
fill_style::~fill_style()
{
	m_bitmap_character = nullptr;
	m_gradient_bitmap_info = nullptr;
	m_gradients.resize(0);
	m_gradients.reserve(0);
}

// 0x784a8c _ZN7gameswf10fill_styleC1Ev
// 0x784c18 _ZN7gameswf10fill_styleC2Ev (identical body)
// Partial, not built. Default values the binary writes: type 0, color white and opaque (bytes 0xff), gradient
// matrix and bitmap matrix identity, empty gradient array (+0x24..+0x30 zero), null bitmap references.
fill_style::fill_style()
	: m_type(0)
{
	m_color.set(255, 255, 255, 255);	// +0x08, the binary stores four 0xff bytes
	m_gradient_matrix.set_identity();	// +0x0c
	m_gradient_bitmap_info = nullptr;	// +0x34
	m_bitmap_character = nullptr;		// +0x38
	m_bitmap_matrix.set_identity();		// +0x3c
}

// 0x7621f0 _ZN7gameswf20morph2_character_defC1EPNS_6playerE
// 0x76227c _ZN7gameswf20morph2_character_defC2EPNS_6playerE (identical body)
// Partial, not built, layout request (see the file comment). Base shape_character_def(player) first. Then the
// binary writes -1.0f to m_last_ratio (+0x9c), the morph vtable at +0 and the second-base vtable at +0x20, and
// 0 to m_offset (+0xa0). Two shapes are allocated with operator new(0x88) and constructed in place: the first
// goes to m_shape1 (+0x88), the second to m_shape2 (+0x8c). m_mesh (+0xa4) is not written by the binary's
// constructor and is left unset here (open: the upstream constructor sets it to 0).
morph2_character_def::morph2_character_def(player* p)
	: shape_character_def(p)
{
	m_last_ratio = -1.0f;	// +0x9c
	m_offset = 0;			// +0xa0

	void* mem1 = operator new(0x88u, static_cast<swf::MemHint>(0));
	m_shape1 = new (mem1) shape_character_def(p);	// +0x88

	void* mem2 = operator new(0x88u, static_cast<swf::MemHint>(0));
	m_shape2 = new (mem2) shape_character_def(p);	// +0x8c
}

// 0x76212c _ZN7gameswf20morph2_character_defD1Ev
// 0x7621a0 _ZN7gameswf20morph2_character_defD2Ev (identical body)
// Partial, not built, layout request. Destroys the second shape (+0x8c) and then the first (+0x88) through
// gameswf::destruct<shape_character_def>, as the binary does. The shape_character_def base destructor runs
// after the body. m_mesh (+0xa4) is not released here, as in the binary.
morph2_character_def::~morph2_character_def()
{
	destruct<shape_character_def>(m_shape2);
	destruct<shape_character_def>(m_shape1);
}

// 0x7636b4 _ZN7gameswf14movie_def_impl16has_init_actionsEv
// 0x7636b4 gameswf::movie_def_impl::has_init_actions()
// Partial, not built. Non-const and virtual (declared so in gameswf_movie_def.h, DH2 comment): the binary's
// symbol is _ZN...16has_init_actionsEv (no const), and the function is stored at vtable 0x990048 +0x88 (0x9900d0),
// the same slot as sprite_definition::has_init_actions. Returns the byte at +0xec (236) as an int. The member is
// declared bool, so the value is 0 or 1 here; the binary returns the raw byte.
int movie_def_impl::has_init_actions()
{
	return m_has_init_actions;
}

// 0x764da0 _ZN7gameswf14movie_def_impl17get_labeled_frameERKNS_10tu_stringiEPi
// 0x764da0 gameswf::movie_def_impl::get_labeled_frame(gameswf::tu_stringi const&, int*)
// Partial, not built. The binary looks the label up with hash::find_index (returns -1 when absent: result 0) and,
// when frame_number is non-null, writes the entry's int value (entry +0x14 from the table base, 16-byte entries)
// and returns 1. The value is at table +0x14 + 16 * index (16-byte entries after an 8-byte table header). The DH2
// table is keyed by stringi_pointer; upstream m_named_frames is a stringi_hash<int>. This body uses the public
// hash::get, which does the same find and store in one call (upstream container.h, hash::get at line 914): it returns
// false and leaves *value alone when the label is absent, and when frame_number is NULL it only reports presence.
// find_index is private in hash, so it is not called here.
bool movie_def_impl::get_labeled_frame(const tu_stringi& label, int* frame_number)
{
	return m_named_frames.get(label, frame_number);
}

// 0x765ec8 _ZN7gameswf14movie_def_implC1EPNS_6playerENS_19create_bitmaps_flagENS_23create_font_shapes_flagE
// 0x76603c _ZN7gameswf14movie_def_implC2EPNS_6playerENS_19create_bitmaps_flagENS_23create_font_shapes_flagE
// Partial, not built, layout request. The member initialisation matches the upstream constructor for every field
// upstream names (create flags at +0xac and +0xb0, frame rate 30.0f at +0xc4, version 0, loaded length 0, jpeg
// input, stream and zlib/origin pointers null). The binary also writes, and this body does not yet: m_has_init_actions
// (+0xec, written here as false), a per-movie unique id (+0xf0, read from the int at +0x24 of the record at player
// +0xac, which is then incremented), a flag byte at +0x104 set to 1 and one at +0x105 set to 0, the background
// colour at +0x114 (rgb 0xff, alpha from the old value & 0xfe), and zeros in the other fields the binary clears.
// These need DH2 field names (layout requests) before they can be written.
movie_def_impl::movie_def_impl(player* p, create_bitmaps_flag cbf, create_font_shapes_flag cfs)
	: movie_definition_sub(p),
	  m_create_bitmaps(cbf),
	  m_create_font_shapes(cfs),
	  m_frame_rate(30.0f),
	  m_version(0),
	  m_loaded_length(0),
	  m_jpeg_in(0),
	  m_str(NULL),
	  m_file_end_pos(0),
	  m_zlib_in(NULL),
	  m_origin_in(NULL)
{
	m_has_init_actions = false;	// +0xec
}

// 0x7689b0 _ZN7gameswf14movie_def_impl15export_resourceERKNS_9tu_stringEPNS_13character_defE
// 0x7689b0 gameswf::movie_def_impl::export_resource(gameswf::tu_string const&, gameswf::character_def*)
// Partial, not built. The binary takes a counted reference to res, stores it in the export hash (+0x78) under a
// copy of the symbol name, and drops its temporary reference. m_exports.set does the same reference handling, so
// it is the body here. The key copy is freed by the binary when it owns the buffer; the upstream key is a value.
void movie_def_impl::export_resource(const tu_string& symbol, character_def* res)
{
	m_exports.set(symbol, res);
}

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

}  // namespace gameswf
