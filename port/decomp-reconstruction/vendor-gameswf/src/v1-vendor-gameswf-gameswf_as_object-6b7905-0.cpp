// batch v1-vendor-gameswf-gameswf_as_object-6b7905-0  category=vendor-gameswf  owner=gameswf::as_object
//
// Gameloft functions of gameswf::as_object (libDungeonHunter2.so). Their declarations are the
// "// DH2:" lines in upstream/gameswf-r1714/gameswf/gameswf_object.h. The upstream definitions that
// differ from the binary are wrapped in #if 0 in gameswf_object.h (advance) and gameswf_object.cpp
// (as_object(player*), find_target(const as_value&), create_proto).
//
// Not defined here: the destructors 0x769a9c and 0x769b70 and get_player 0x780374 are upstream code
// (see the ledger). find_target(char const*) 0x76b284 is defined by its own batch.

#include "gameswf/gameswf_object.h"

namespace gameswf
{

// 0x752c0c _ZN7gameswf9as_object7_typeofEv
const char* as_object::_typeof()
{
	return "object";
}

// 0x752c24 _ZN7gameswf9as_object17notify_set_memberERKNS_10tu_stringiERKNS_8as_valueE
// Empty virtual hook (bx lr in the binary).
void as_object::notify_set_member(const tu_stringi&, const as_value&)
{
}

// 0x768b50 _ZN7gameswf9as_object7advanceEf
// Empty virtual in the binary (bx lr). Upstream asserts here; the DH2 build does not.
void as_object::advance(float)
{
}

// 0x768b78 _ZN7gameswf9as_object10set_memberEiRKNS_8as_valueE
int as_object::set_member(int, const as_value&)
{
	return 0;	// MOV R0, #0
}

// 0x768b80 _ZN7gameswf9as_object10get_memberEiPNS_8as_valueE
int as_object::get_member(int, as_value*)
{
	return 0;	// MOV R0, #0
}

// 0x76b820 _ZN7gameswf9as_objectC1EPNS_6playerE
// 0x76bce0 _ZN7gameswf9as_objectC2EPNS_6playerE (same body)
// The binary registers the new object in player+0x0c (array<smart_ptr<as_object>>::push_back).
as_object::as_object(player* p)
	:
	m_watch(NULL),
	m_player(p)
{
	if (p)
	{
		p->unk_0c.push_back(smart_ptr<as_object>(this));	// player +0x0c
	}
}

// 0x76b45c _ZN7gameswf9as_object11find_targetERKNS_8as_valueE
// The binary tests the type byte at +0x01: types 3 and 4 (both strings, as_value::_typeof returns
// "string" for them) go through to_tu_string() to find_target(char const*) 0x76b284. Type 5 (object)
// returns the object pointer at +0x04, and every other type returns NULL. Numbers (type 2) are not
// text here, so they return NULL.
as_object* as_object::find_target(const as_value& target)
{
	if (target.is_string())
	{
		return find_target(target.to_tu_string().c_str());
	}
	if (target.is_object())
	{
		return target.to_object();
	}
	return NULL;
}

// 0x76b8b0 _ZN7gameswf9as_object12create_protoERKNS_8as_valueE
as_object* as_object::create_proto(const as_value& constructor)
{
	m_proto = new as_object(get_player());
	m_proto->m_this_ptr = m_this_ptr;

	if (constructor.is_object())
	{
		as_object* ctor_object = constructor.to_object();
		if (ctor_object)
		{
			as_value val;
			ctor_object->get_member("prototype", &val);
			as_object* prototype = val.is_object() ? val.to_object() : NULL;
			// The binary does not test this pointer; it calls through it.
			assert(prototype != NULL);

			// Keep a copy of this object's members, copy the prototype's members onto this object,
			// then put back each saved member that the copy changed.
			stringi_hash<as_value> own(m_members);
			prototype->copy_to(this);
			for (auto it = own.begin(); it != own.end(); ++it)
			{
				const tu_stringi& key = it->first;
				const as_value& saved = it->second;
				as_value current;
				if (get_member(key, &current))
				{
					if (current.is_property() == false || saved.is_property())
					{
						if (current != saved)
						{
							set_member(key, saved);
						}
					}
					else
					{
						current.set_property(saved);
					}
				}
			}

			as_value prototype_ctor;
			if (prototype->get_ctor(&prototype_ctor))
			{
				// The binary sets the constructor on the new prototype; it does not recurse.
				m_proto->set_ctor(prototype_ctor);
			}
		}
	}

	set_ctor(constructor);
	return m_proto.get_ptr();
}

}	// namespace gameswf
