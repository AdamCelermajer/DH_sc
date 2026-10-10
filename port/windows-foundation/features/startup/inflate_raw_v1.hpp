#pragma once

// Raw DEFLATE (RFC 1951) decoder. Plain C++, no platform calls. Written for
// the intro stream frames; output must not exceed `expected` bytes.

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation::startup {

// Decodes one complete raw deflate stream. Fails on truncated input, invalid
// codes, distances before the start of output, or output larger than expected.
// On success out holds exactly the decoded bytes (size may be < expected only
// if the stream is shorter; the caller checks the size it requires).
bool inflate_raw_v1(const std::uint8_t* in, std::size_t size,
                    std::vector<std::uint8_t>& out, std::size_t expected,
                    std::string& error);

} // namespace dh::foundation::startup
