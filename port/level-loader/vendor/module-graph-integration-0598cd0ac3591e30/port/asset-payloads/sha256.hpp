#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
namespace dh2::assets {
using Sha256Digest=std::array<std::uint8_t,32>;
// New port byte utility, not a reconstructed original-engine function.
// The caller supplies a readable span; this does not validate allocation pages.
// nullptr/0 is valid. Null/nonzero, wrapping address spans and SHA256 bit-length
// overflow reject without changing output. Input/output overlap is supported:
// the complete message is consumed before committing the owned digest.
// No allocation, borrowed result, platform crypto service or VM state.
bool sha256(const std::uint8_t* bytes,std::size_t size,Sha256Digest& out)noexcept;
}
