// Batch v1-vendor-gameswf-gameswf_tu_string-39e5bd-0: DH2 bodies for gameswf::tu_string.
//
// The vendored header base/container.h declares tu_string at global scope (the binary's symbols are
// gameswf::tu_string, see the ledger notes). The bodies below are members of that global class.
// They use only length(), c_str(), get_buffer() and resize(), never the byte offsets, so they follow
// the upstream union in container.h.
//
// DH2 binary layout (ARM32 listings of the functions below; 20 bytes; offsets in hex):
//   +00 byte: 0xFF = heap text; otherwise length + 1, and the inline text starts at +01
//   +04 int: heap length + 1
//   +08 int: heap capacity (a multiple of 16)
//   +0C char*: heap text
//   +10 uint32: hash cache. Low 24 bits: hash, 0xFFFFFF = not computed. Top byte: flags
//       (the constructors clear bit 0 of byte +13).
// The hash cache is not declared in the vendored header. Bodies that write it are partial.
//
// Upstream rows with no body here (ledger.d/v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.csv):
// 0x751d10 stricmp, 0x7520e0 and 0x7521c8 encode_utf8_from_wchar (thunks to the generic encoder),
// 0x78aebc operator==(char const*). Their bodies are the upstream ones in base/container.cpp and container.h.

#include <cstddef>
#include <cstring>

#include "base/container.h"

// 0x7521cc gameswf::tu_string::operator+=(char const*)   (_ZN7gameswf9tu_stringpLEPKc)
// Binary: n = strlen(src); old = length(); resize(old + n); strcpy(text + old, src); then the hash
// cache at +10 gets its low 24 bits set to 0xFFFFFF. The binary returns strcpy's result; the header
// declares void, as upstream.
// Partial: the cache write is not modelled. get_hashi() recomputes its value, so the visible result
// is the same.
void tu_string::operator+=(const char* str)
{
	const int add_length = static_cast<int>(std::strlen(str));
	const int old_length = length();
	resize(old_length + add_length);
	// strcpy also copies the terminator. get_buffer() is read after resize(), as in the binary.
	std::memcpy(get_buffer() + old_length, str, static_cast<std::size_t>(add_length) + 1);
}

// 0x7a3cf4 gameswf::tu_string::operator+=(char)   (_ZN7gameswf9tu_stringpLEc)
// Binary: a zero character does nothing (no resize, no cache write). Otherwise old = length();
// resize(old + 1); strncpy(text + old, &ch, 1); then the cache is invalidated as in 0x7521cc.
// Partial for the same reason. The binary returns char* (this, or the strncpy result); here void.
void tu_string::operator+=(char ch)
{
	if (ch == 0) {
		return;
	}
	const int old_length = length();
	resize(old_length + 1);
	get_buffer()[old_length] = ch;
}

// 0x753120 gameswf::tu_string::operator+=(gameswf::tu_string const&)   (_ZN7gameswf9tu_stringpLERKS0_)
// Binary: n = length(src); old = length(); resize(old + n); then the source text is read again, after
// the resize, and strcpy'd to text + old. The cache is invalidated as in 0x7521cc.
// strcpy stops at the first NUL of the source, so memcpy of strlen + 1 bytes keeps that rule.
// Appending a string to itself overlaps the copy, as with the binary's strcpy.
// Partial for the cache write.
void tu_string::operator+=(const tu_string& str)
{
	const int add_length = str.length();
	const int old_length = length();
	resize(old_length + add_length);
	const char* src = str.c_str();
	std::memcpy(get_buffer() + old_length, src, std::strlen(src) + 1);
}

// 0x796f18 gameswf::tu_string::operator==(gameswf::tu_string const&) const   (_ZNK7gameswf9tu_stringeqERKS0_)
// Binary: the same object compares equal. Otherwise strcmp of the two texts == 0, so the comparison
// stops at the first NUL. The vendored upstream body (size check, then memcmp) differs only for texts
// with embedded NULs. Reconstructed.
bool tu_string::operator==(const tu_string& str) const
{
	if (this == &str) {
		return true;
	}
	return std::strcmp(c_str(), str.c_str()) == 0;
}

// 0x7635c8 gameswf::tu_string::get_hashi() const   (_ZNK7gameswf9tu_string9get_hashiEv)
// Gameloft addition (no upstream counterpart). Binary: if the cache at +10 holds a computed hash,
// return its low 24 bits, sign-extended. Otherwise, for length > 0, run
//   h = 5381; for each byte from the last to the first: c = byte with 'A'..'Z' folded to 'a'..'z';
//             h = c ^ (33 * h)                       (32-bit wrap-around)
// and return h's low 24 bits, sign-extended. Length 0 returns 5381. The binary stores the value in
// the cache in both cases.
// Partial: the cache is not modelled, so every call recomputes. The value equals the binary's while
// its cache is valid.
int tu_string::get_hashi() const
{
	const int text_length = length();
	if (text_length <= 0) {
		return 5381;
	}
	const char* text = c_str();
	unsigned int h = 5381u;
	for (int i = text_length - 1; i >= 0; --i) {
		unsigned int c = static_cast<unsigned char>(text[i]);
		if (c - 'A' <= 25u) {  // 'A'..'Z' (unsigned compare: bytes below 'A' wrap to large values)
			c += 'a' - 'A';
		}
		h = c ^ (33u * h);
	}
	int low24 = static_cast<int>(h & 0xFFFFFFu);
	if ((low24 & 0x800000) != 0) {
		low24 -= 0x1000000;  // sign-extend the 24-bit value
	}
	return low24;
}
