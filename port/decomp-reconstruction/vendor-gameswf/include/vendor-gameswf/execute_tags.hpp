// gameswf execute tags as the DH2 binary has them: gameswf::execute_tag and the three tag classes that
// batch v1-vendor-gameswf-pack2-6d7f71-0 touches (place_object_2, remove_object_2, set_background_color).
//
// Replaces the upstream struct execute_tag in vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_impl.h
// (wrapped in #if 0 there). The other tag classes are defined in upstream gameswf_impl.cpp, which Track B does
// not compile. Only the vtable shape and the field that the batch reads are recovered here. The data members of
// remove_object_2 and set_background_color are not recovered; place_object_2 has only m_depth.
//
// execute_tag vtable (vtables-001.json, 52 bytes; offsets from the address point, after offset-to-top and typeinfo):
//   +0x00 D1 (0x759ac4)     +0x04 D0 (0x759ff0, compiler-generated)   +0x08 execute
//   +0x0c execute_state     +0x10 execute_state_reverse               +0x14 is_remove_tag
//   +0x18 is_action_tag     +0x1c get_depth_id_of_replace_or_add_tag  +0x20 get_depth
//   +0x24 get_action_offset +0x28 is_place_tag
// Upstream r1714 declares an execute_now slot before execute_state. The binary has no such slot.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack2-6d7f71-0.cpp.

#pragma once

#include <cstdint>

namespace gameswf {

struct character;

struct execute_tag {
	virtual ~execute_tag();                                            // +0x00 D1 0x759ac4
	virtual void execute(character* m);                                // +0x08 0x759ac8
	virtual void execute_state(character* m);                          // +0x0c 0x759acc
	virtual void execute_state_reverse(character* m, int frame);       // +0x10
	virtual bool is_remove_tag() const;                                // +0x14
	virtual bool is_action_tag() const;                                // +0x18
	virtual std::uint32_t get_depth_id_of_replace_or_add_tag() const;  // +0x1c
	virtual int get_depth() const;                                     // +0x20 0x759afc, returns -1
	virtual int get_action_offset() const;                             // +0x24 0x759b04, returns -1
	virtual bool is_place_tag() const;                                 // +0x28 0x759b0c, returns false
};

// Upstream defines place_object_2 in gameswf_impl.cpp. Vtable overrides are listed in slot order. The
// remaining members are not recovered: +0x04..+0x0b is padding here, and nothing after +0x0e is declared.
struct place_object_2 : public execute_tag {
	~place_object_2() override;                                         // +0x00 D1 (not in this batch)
	void execute(character* m) override;                                // +0x08 (not in this batch)
	void execute_state(character* m) override;                          // +0x0c (not in this batch)
	void execute_state_reverse(character* m, int frame) override;       // +0x10 (not in this batch)
	std::uint32_t get_depth_id_of_replace_or_add_tag() const override;  // +0x1c (not in this batch)
	int get_depth() const override;                                     // +0x20 0x759b14, returns the u16 at +0x0c
	bool is_place_tag() const override;                                 // +0x28 0x759b1c, returns true

	std::uint8_t pad_0x04[8];  // +0x04..+0x0b: not recovered
	std::uint16_t m_depth;     // +0x0c: read by get_depth() (upstream name m_depth)
};

// Upstream defines remove_object_2 in gameswf_impl.cpp. Only the vtable is recovered.
struct remove_object_2 : public execute_tag {
	~remove_object_2() override;                                        // +0x00 D1 0x759fe8
	void execute(character* m) override;                                // +0x08 (not in this batch)
	void execute_state(character* m) override;                          // +0x0c (not in this batch)
	void execute_state_reverse(character* m, int frame) override;       // +0x10 (not in this batch)
	bool is_remove_tag() const override;                                // +0x14 (not in this batch)
};

// Upstream defines set_background_color in gameswf_impl.cpp (with an rgba m_color). Only the vtable is recovered.
struct set_background_color : public execute_tag {
	~set_background_color() override;                                   // +0x00 D1 0x759fec
	void execute(character* m) override;                                // +0x08 (not in this batch)
	void execute_state(character* m) override;                          // +0x0c (not in this batch)
};

}  // namespace gameswf
