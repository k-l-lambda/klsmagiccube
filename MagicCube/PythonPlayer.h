/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__PYTHONPLAYER_H__
#define	__PYTHONPLAYER_H__



#ifdef	_PYDLL
#	define	PYTHONPLAYER_API	__declspec(dllexport)
#else
#	define	PYTHONPLAYER_API	__declspec(dllimport)
#endif	// defined(_PYDLL)

#include "Player.h"


namespace boost
{
	namespace python
	{
		class dict;
	}
}


namespace MagicCube
{
	class /*PYTHONPLAYER_API*/	PythonPlayer : public Player
	{
	public:
		PythonPlayer(MagicCube& mc);
		~PythonPlayer();

		void	load(const std::string& filename);

	private:
		virtual void	doNext();

	private:
		boost::shared_ptr<boost::python::dict>	m_PythonDict;
	};
}



#endif	// !defined(__PYTHONPLAYER_H__)
