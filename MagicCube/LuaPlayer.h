/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__LUAPLAYER_H__
#define	__LUAPLAYER_H__



#include "Player.h"


struct lua_State;


namespace MagicCube
{
	class LUAPLAYER_API	LuaPlayer : public Player
	{
	public:
		LuaPlayer(MagicCube& mc);
		~LuaPlayer();

		void	load(const std::string& filename);

	private:
		virtual void	doNext();

	private:
		lua_State*	m_LuaState;
	};
}



#endif	// !defined(__LUAPLAYER_H__)
