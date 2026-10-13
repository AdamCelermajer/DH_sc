// batch v1-vendor-gameswf-gameswf_player-e0b31b-0  category=vendor-gameswf  owner=gameswf::player
//
// Gameloft functions of gameswf::player (libDungeonHunter2.so). Their declarations are the
// "// DH2:" lines in upstream/gameswf-r1714/gameswf/gameswf_player.h. The upstream definitions of
// action_clear and set_as_garbage differ from the binary and are wrapped in #if 0 in
// upstream/gameswf-r1714/gameswf/gameswf_player.cpp.
//
// Not defined here: get_root and notify_key_event (upstream r1714, see the ledger) and the
// deleting destructor 0x771c38 (compiler-generated).

#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_impl.h"        // generic_character
#include "gameswf/gameswf_sprite.h"      // sprite_instance
#include "gameswf/gameswf_text.h"        // edit_text_character
#include "gameswf/gameswf_movie_def.h"   // movie_definition_sub

namespace gameswf
{

// 0x76c7d8 _ZN7gameswf6player12action_clearEv
void player::action_clear()
{
}

// 0x76c7e4 _ZNK7gameswf6player9get_stageEv
int player::get_stage() const
{
	return m_stage;	// +0x38
}

// 0x76c808 _ZN7gameswf6player14set_as_garbageEv
void player::set_as_garbage()
{
	++m_garbage_count;	// +0x30
}

// 0x76c9d8 _ZN7gameswf6player22clear_unused_instancesEv
void player::clear_unused_instances()
{
	m_unused_sprites.resize(0);	// +0xb0
	m_unused_generic_characters.resize(0);	// +0xc0
	m_unused_edit_text_characters.resize(0);	// +0xd0
}

// 0x76cb44 _ZN7gameswf6player26create_edit_text_characterEPNS_23edit_text_character_defEPNS_9characterEi
// PARTIAL: unk_34 (+0x34) has no evidence for its meaning, and the unk_0c keep-alive push is
// not reproduced (see the note in the body). Otherwise the pool logic follows the binary: reuse the
// last pooled object, or construct a new one.
edit_text_character* player::create_edit_text_character(edit_text_character_def* def, character* parent, int id)
{
	int count = m_unused_edit_text_characters.size();	// +0xd4
	edit_text_character* obj;
	if (count <= 0)
	{
		obj = new edit_text_character(this, parent, def, id);
	}
	else
	{
		obj = static_cast<edit_text_character*>(m_unused_edit_text_characters[count - 1].get_ptr());	// +0xd0
		obj->m_def = def;	// +0xa0
		obj->recycle(parent, id);	// vtable +0x64
		// PARTIAL: the binary pushes obj into unk_0c (+0x0c) when its ref count word (+0x04) is 1,
		// which keeps the object alive after the pool drops its reference. ref_counted::m_ref_count
		// is private in vendor-gameswf/include/vendor-gameswf/ref_counted.hpp with no getter, so this
		// test is not reproduced (layout request: a const ref-count getter on ref_counted).
		obj->unk_34 = m_garbage_count;	// +0x34 <- player +0x30
		m_unused_edit_text_characters.resize(m_unused_edit_text_characters.size() - 1);
	}
	return obj;
}

// 0x76cc00 _ZN7gameswf6player24create_generic_characterEPNS_13character_defEPNS_9characterEi
// PARTIAL: unk_34 (+0x34) has no evidence for its meaning; the unk_0c keep-alive push is not reproduced.
generic_character* player::create_generic_character(character_def* def, character* parent, int id)
{
	int count = m_unused_generic_characters.size();	// +0xc4
	generic_character* obj;
	if (count <= 0)
	{
		obj = new generic_character(this, def, parent, id);
	}
	else
	{
		obj = static_cast<generic_character*>(m_unused_generic_characters[count - 1].get_ptr());	// +0xc0
		obj->m_def = def;	// +0xa0
		obj->recycle(parent, id);	// vtable +0x64
		// PARTIAL: the binary pushes obj into unk_0c (+0x0c) when its ref count word (+0x04) is 1,
		// which keeps the object alive after the pool drops its reference. ref_counted::m_ref_count
		// is private in vendor-gameswf/include/vendor-gameswf/ref_counted.hpp with no getter, so this
		// test is not reproduced (layout request: a const ref-count getter on ref_counted).
		obj->unk_34 = m_garbage_count;	// +0x34 <- player +0x30
		m_unused_generic_characters.resize(m_unused_generic_characters.size() - 1);
	}
	return obj;
}

// 0x76ccf0 _ZN7gameswf6player22create_sprite_instanceEPNS_20movie_definition_subEPNS_4rootEPNS_9characterEi
// PARTIAL: unk_34 (+0x34) has no evidence for its meaning; the unk_0c keep-alive push is not reproduced.
sprite_instance* player::create_sprite_instance(movie_definition_sub* def, root* r, character* parent, int id)
{
	int count = m_unused_sprites.size();	// +0xb4
	sprite_instance* obj;
	if (count <= 0)
	{
		obj = new sprite_instance(this, def, r, parent, id);
	}
	else
	{
		obj = static_cast<sprite_instance*>(m_unused_sprites[count - 1].get_ptr());	// +0xb0
		obj->m_root = r;	// +0xa4
		obj->m_def = def;	// +0xa0
		obj->recycle(parent, id);	// vtable +0x64
		// PARTIAL: the binary pushes obj into unk_0c (+0x0c) when its ref count word (+0x04) is 1,
		// which keeps the object alive after the pool drops its reference. ref_counted::m_ref_count
		// is private in vendor-gameswf/include/vendor-gameswf/ref_counted.hpp with no getter, so this
		// test is not reproduced (layout request: a const ref-count getter on ref_counted).
		obj->unk_34 = m_garbage_count;	// +0x34 <- player +0x30
		m_unused_sprites.resize(m_unused_sprites.size() - 1);
	}
	return obj;
}

// 0x773268 _ZN7gameswf6player22notify_unused_instanceEPNS_9characterE
// PARTIAL: the class-kind byte at +0x98 (unk_98) is only named here; its meaning as a class id
// is inferred from the is(kind) call. The binary calls set_ref / set_text_value on a null
// pointer when is() fails; this version skips that call instead (unreachable when the byte
// matches the class id).
void player::notify_unused_instance(character* ch)
{
	ch->detach_weak_proxy();
	switch (ch->unk_98)
	{
	case 3:
	{
		generic_character* obj = ch->is(3) ? static_cast<generic_character*>(ch) : nullptr;
		if (obj)
		{
			obj->m_def = nullptr;	// +0xa0
		}
		m_unused_generic_characters.push_back(ch);	// +0xc0
		break;
	}
	case 32:
	{
		edit_text_character* obj = ch->is(32) ? static_cast<edit_text_character*>(ch) : nullptr;
		if (obj)
		{
			obj->set_text_value(tu_string(""), false);
		}
		m_unused_edit_text_characters.push_back(ch);	// +0xd0
		break;
	}
	case 2:
		m_unused_sprites.push_back(ch);	// +0xb0
		break;
	default:
		return;
	}
	ch->m_members.clear();	// +0x0c, as_object member hash
	hash<as_object*, bool> visited;
	ch->clear_refs(&visited, ch);	// vtable +0x40
	visited.clear();
}

}  // namespace gameswf
