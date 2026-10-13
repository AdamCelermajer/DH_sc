// gameswf::html_reader (Gameloft; not in upstream GameSWF r1714). Binary: parse_tag 0x78e060, parse 0x78e354.
//
// The class is the HTML reader for edit_text fields. The binary's parse() treats html_reader itself as an array of
// edit_text_character::text_attributes (push_back, resize and element access on its own address), so the class
// has its array layout in the binary. That element type is not declared anywhere in the tree, so this header
// declares only the member functions that can be rebuilt. Layout request: the owner of html_reader declares the
// array base once edit_text_character::text_attributes exists (see ledger.d/v1-vendor-gameswf-pack10-9a32ad-0.csv).
//
// parse() (0x78e354) is not declared here: it is partial and has no body in this batch.

#ifndef VENDOR_GAMESWF_HTML_READER_HPP
#define VENDOR_GAMESWF_HTML_READER_HPP

#include "base/container.h"                    // stringi_hash<>, tu_string, tu_stringi (global in upstream)
#include "vendor-gameswf/gameswf_hash_dh2.hpp"  // the DH2 hash<tu_stringi, tu_string>::set_raw_capacity specialization

namespace gameswf {

struct html_reader {
	// 0x78e060 gameswf::html_reader::parse_tag(gameswf::stringi_hash<gameswf::tu_string>&, char const*)
	// Reads one tag body (the text between '<' and '>', without them) into attrs: "name" is the tag name, and every
	// name='value' or name="value" pair adds one entry. Returns 0 for a closing tag ('/' first) or an unmatched quote,
	// and 1 otherwise. The binary ignores its this pointer, so this is a non-static member with no state.
	int parse_tag(stringi_hash<tu_string>& attrs, const char* tag);
};

}  // namespace gameswf

#endif  // VENDOR_GAMESWF_HTML_READER_HPP
