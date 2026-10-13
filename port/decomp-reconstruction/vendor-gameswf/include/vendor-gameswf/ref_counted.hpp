// gameswf::ref_counted as the DH2 binary has it (Gameloft). This replaces the upstream struct in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf.h, which derives from gc_object and
// weak_pointee_mixin. Neither base exists in the binary: the constructor makes no base calls and
// the object has a single vptr.
//
// Layout, 12 bytes: vptr +0x00, m_ref_count +0x04 (intrusive count), m_weak_proxy +0x08.
// The weak proxy is a raw 8-byte block with the same two fields as weak_proxy in base/weak_ptr.h:
// int m_ref_count +0 and bool m_alive +4. It is allocated with operator new(8, swf::MemHint) and
// freed with gameswf::free_internal, not delete.
//
// The vtable has only the two destructor slots (D1 and D0), so there are no other virtual methods.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-gameswf_ref_counted-0eb785-0.cpp

#pragma once

#include "base/weak_ptr.h"  // weak_proxy (upstream)

namespace gameswf {

// Destroys an object whose count reached zero. This is 0x75a214, destruct<ref_counted> (lib-gameswf
// queue). It is not defined by this batch.
template <class T>
void destruct(const T* object);

// 0x752b38 gameswf::free_internal(void*, unsigned int) (lib-gameswf queue). It is not defined by this
// batch. The second argument is 0 at every call site in this batch.
void free_internal(void* p, unsigned int size);

struct ref_counted {
	ref_counted();                       // 0x759c04 (C2), 0x759c34 (C1)
	virtual ~ref_counted();              // 0x75dc28 (D1), 0x75dca4 (D2); D0 0x75dc88 is compiler-generated
	void add_ref() const;                // 0x759c64
	void drop_ref();                     // 0x75a240
	weak_proxy* get_weak_proxy() const;  // 0x75a88c
	void detach_weak_proxy();            // 0x75dbe4

private:
	mutable int m_ref_count;             // +0x04
	mutable weak_proxy* m_weak_proxy;    // +0x08
};

}  // namespace gameswf
