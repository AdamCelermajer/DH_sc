// as_mcloader.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script MovieClipLoader implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_MCLOADER_H
#define GAMESWF_AS_MCLOADER_H

#include "gameswf/gameswf_action.h"	// for as_object
#include "gameswf/gameswf_listener.h"	// for listener

namespace gameswf
{

	void	as_global_mcloader_ctor(const fn_call& fn);

	// DH2: built-in methods that the constructor registers (defined in as_mcloader.cpp)
	void	as_mcloader_addlistener(const fn_call& fn);	// DH2
	void	as_mcloader_removelistener(const fn_call& fn);	// DH2
	void	as_mcloader_loadclip(const fn_call& fn);	// DH2
	void	as_mcloader_unloadclip(const fn_call& fn);	// DH2
	void	as_mcloader_getprogress(const fn_call& fn);	// DH2

	struct as_mcloader : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_MCLOADER };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		struct loadable_movie
		{
			loadable_movie() :
				m_ch(NULL)
			{
			}

			gc_ptr<movie_def_impl> m_def;
			weak_ptr<character> m_target;
			character* m_ch;
		};

		// DH2: binary layout (as_object is 56 bytes). Listener block +0x38 (16 bytes: array part,
		// then a flag at +0x44); m_lm +0x48; unk_54 +0x54. The listener class is not DH2-defined yet.
#if 0 /* DH2: replaced by the listener block members below (binary +0x38) */
		listener m_listeners;
#endif
		array< weak_ptr<as_object> >	m_listener_items;	// DH2: +0x38, listener block array part (12 bytes)
		bool	unk_44;	// DH2: +0x44, listener block flag; the binary never releases it
		array<loadable_movie> m_lm;	// +0x48
		bool	unk_54;	// DH2: +0x54, cleared by the constructor; no batch reads it yet

		as_mcloader(player* player);
		~as_mcloader();
		
		virtual void	advance(float delta_time);
	};

}	// end namespace gameswf


#endif // GAMESWF_AS_XMLSOCKET_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
