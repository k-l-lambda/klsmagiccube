/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__PLAYBACKPLAYER_H__
#define	__PLAYBACKPLAYER_H__



#include "Player.h"


namespace MagicCube
{
	class PlaybackPlayer : public Player
	{
	public:
		PlaybackPlayer(MagicCube& mc);
		~PlaybackPlayer();

		void	load(const std::string& filename);
		void	save(const std::string& filename) const;

	private:
		virtual void	doNext();

	private:
		std::deque<Manipulation>	m_PlaybackManipulations;
	};
}



#endif	// !defined(__PLAYBACKPLAYER_H__)
