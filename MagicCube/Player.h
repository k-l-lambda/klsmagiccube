/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__PLAYER_H__
#define	__PLAYER_H__



#include <boost\shared_ptr.hpp>
#include <boost\function.hpp>

#include "MagicCube.h"


namespace boost
{
	class mutex;
	class thread;
}


namespace MagicCube
{
	class MAGICCUBE_API	Player
	{
	public:
		typedef	boost::function<bool (Manipulation)>	SynchronizeFunctor;

	public:
		Player(MagicCube& mc);
		virtual ~Player();

		void	reset();
		virtual void	load(const std::string& source);
		void	tick();

		bool	ready() const;
		bool	busy() const;
		bool	atFront() const;
		bool	atBack() const;

		size_t	step() const;

		void	advance(const SynchronizeFunctor& fnSynchronize = SynchronizeFunctor());
		void	backoff(const SynchronizeFunctor& fnSynchronize = SynchronizeFunctor());

	protected:
		void	setStepResult(Manipulation m);

		void	clearEngine();

	private:
		// asynchronous function
		void	next();
		virtual void	doNext() = 0;

	protected:
		MagicCube&	m_MagicCube;

	private:
		size_t	m_CurrentPointer;

		std::vector<Manipulation>	m_ManipulationSequence;

		Manipulation	m_StepResult;
		SynchronizeFunctor	m_fnSynchronize;

		boost::shared_ptr<boost::thread>	m_Engine;
		//void*	m_EngineHandle;
		boost::shared_ptr<boost::mutex>		m_EngineMutex;
	};
}



#endif	// !defined(__PLAYER_H__)
