// gameswf_object.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A generic bag of attributes.	 Base-class for ActionScript
// script-defined objects.

#ifndef GAMESWF_OBJECT_H
#define GAMESWF_OBJECT_H

#include "gameswf/gameswf_value.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_player.h"
#include "base/container.h"
#include "base/weak_ptr.h"
#include "base/tu_loadlib.h"

namespace gameswf
{
	exported_module void	as_object_addproperty(const fn_call& fn);
	exported_module void	as_object_registerclass( const fn_call& fn );
	exported_module void	as_object_hasownproperty(const fn_call& fn);
	exported_module void	as_object_watch(const fn_call& fn);
	exported_module void	as_object_unwatch(const fn_call& fn);
	exported_module void	as_object_dump(const fn_call& fn);
	exported_module void	as_global_object_ctor(const fn_call& fn);

	// flash9
	exported_module void	as_object_add_event_listener(const fn_call& fn);

	struct instance_info;

	struct as_object : public as_object_interface
	{
		// Unique id of a gameswf resource
		enum	{ m_class_id = AS_OBJECT };
		exported_module virtual bool is(int class_id) const
		{
			return m_class_id == class_id;
		}

		stringi_hash<as_value>	m_members;

		// DH2: 12-byte as_value at +0x10 in the binary (ctor clears its flags and type, dtor drops its refs).
		as_value unk_10;	// DH2: +0x10, name unknown

		// It is used to register an event handler to be invoked when
		// a specified property of object changes.
		// TODO: create container based on stringi_hash<as_value>
		// watch should be coomon
		struct as_watch
		{
			as_watch() :	m_func(NULL)
			{
			}

			as_function* m_func;
			as_value m_user_data;
		};

		// primitive data type has no dynamic members
		stringi_hash<as_watch>*	m_watch;

		// it's used for passing new created object pointer to constructors chain
		weak_ptr<as_object> m_this_ptr;

		// We can place reference to __proto__ into members but it's used very often
		// so for optimization we place it into instance
		#if 0 /* DH2: replaced by smart_ptr below (the DH2 ref_counted has no gc_object base) */
		gc_ptr<as_object> m_proto;	// for optimization
		#endif /* DH2 */
		smart_ptr<as_object> m_proto;	// DH2: binary +0x28, set_ref in create_proto 0x76b8b0, drop_ref in the destructors

		// pointer to owner
		weak_ptr<player> m_player;

		weak_ptr<instance_info> m_instance;

		exported_module as_object(player* player);
		exported_module virtual ~as_object();
		
		exported_module virtual const char*	to_string() { return "[object Object]"; }
		exported_module virtual double	to_number();
		exported_module virtual bool to_bool() { return true; }
		exported_module virtual const char*	type_of() { return "object"; }
		exported_module virtual const char*	_typeof();	// DH2: binary name of type_of (vtable +0x18), 0x752c0c

		exported_module void	builtin_member(const tu_stringi& name, const as_value& val); 
		exported_module void	call_watcher(const tu_stringi& name, const as_value& old_val, as_value* new_val);
		exported_module virtual bool	set_member(const tu_stringi& name, const as_value& val);
		exported_module virtual bool	get_member(const tu_stringi& name, as_value* val);
		exported_module virtual int	set_member(int id, const as_value& val);	// DH2: vtable +0x24, 0x768b78, returns 0
		exported_module virtual int	get_member(int id, as_value* val);	// DH2: vtable +0x28, 0x768b80, returns 0
		exported_module virtual void	notify_set_member(const tu_stringi& name, const as_value& val);	// DH2: vtable +0x60, 0x752c24, empty
		exported_module virtual bool	find_property( const tu_stringi & name, as_value * val );
		exported_module virtual bool	on_event(const event_id& id);
		exported_module virtual	void enumerate(as_environment* env);
		exported_module virtual as_object* get_proto() const;
		exported_module virtual bool watch(const tu_string& name, as_function* callback, const as_value& user_data);
		exported_module virtual bool unwatch(const tu_string& name);
		exported_module virtual void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr);
		exported_module virtual void this_alive();
		exported_module virtual void alive() {}
		exported_module virtual void copy_to(as_object* target);
		exported_module virtual void dump(tu_string& tabs);
		exported_module virtual void dump();
		exported_module as_object* find_target(const as_value& target);
		exported_module as_object* find_target(const char* path);	// DH2: binary overload 0x76b284 (defined by its own batch)
		exported_module virtual root* get_root() const;
		exported_module	virtual as_environment*	get_environment() { return 0; }
		#if 0 /* DH2: replaced by vendor-gameswf/src/v1-vendor-gameswf-gameswf_as_object-6b7905-0.cpp */
		exported_module virtual void advance(float delta_time) { assert(0); }
#endif /* DH2 */
		exported_module virtual void advance(float delta_time);	// DH2: 0x768b50, empty in the binary (bx lr)

		player* get_player() const { return m_player.get_ptr(); }
		bool	is_instance_of(const as_function* constructor) const;
		as_object* get_global() const;

		// get/set constructor of object
		bool get_ctor(as_value* val) const;
		void set_ctor(const as_value& val);
		void set_instance(instance_info * info);

		as_object* create_proto(const as_value& constructor);

	};

}

#endif
