// gameswf::traits_info and gameswf::instance_info as the DH2 binary has them (Gameloft AVM2 support). Replaces the
// upstream structs in vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_avm2.h (traits_info) and gameswf_abc.h
// (instance_info), wrapped in #if 0 there. The upstream instance_info holds m_trait as gc_array; the binary holds an
// array<smart_ptr<traits_info>>, which the destructor calls resize and reserve on.
//
// Only the members the batch touches are named. The other bytes are padding until their functions are reconstructed.
//
//   traits_info (ref_counted base, 12 bytes: vptr +0x00, count +0x04, weak proxy +0x08)
//     +0x0c..+0x23  pad_0c   upstream name, kind, attr and the trait union (not recovered)
//     +0x24         m_metadata  array<int> (buffer +0x24, size +0x28, buffer size +0x2c)
//
//   instance_info (ref_counted base)
//     +0x0c..+0x1b  pad_0c   upstream name, super name, flags and protected namespace (not recovered)
//     +0x1c         m_interface  array<int>
//     +0x28..+0x2f  pad_28   not recovered (upstream m_iinit and m_abc)
//     +0x30         m_trait      array<smart_ptr<traits_info>>
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack17-c88d12-0.cpp (0x7b9938, 0x7b9c18; the deleting destructors
// 0x7b99d0 and 0x7ba5e4 are compiler-generated).

#pragma once

#include <cstdint>

#include "base/container.h"         // array<> (DH2 destructor: clear() and reserve(0))
#include "base/smart_ptr.h"         // smart_ptr<T> (global in upstream)
#include "vendor-gameswf/ref_counted.hpp"  // gameswf::ref_counted (DH2)

namespace gameswf {

struct traits_info : public ref_counted {
	~traits_info() override;  // 0x7b9c18 (D1). Destroys m_metadata. The base destructor runs after this body.

	std::uint8_t pad_0c[0x18];  // +0x0c..+0x23
	array<int> m_metadata;      // +0x24
};

struct instance_info : public ref_counted {
	~instance_info() override;  // 0x7b9938 (D1). Destroys m_trait, then m_interface. The base destructor runs after this body.

	std::uint8_t pad_0c[0x10];                // +0x0c..+0x1b
	array<int> m_interface;                   // +0x1c
	std::uint8_t pad_28[0x08];                // +0x28..+0x2f
	array<smart_ptr<traits_info> > m_trait;   // +0x30
};

}  // namespace gameswf
