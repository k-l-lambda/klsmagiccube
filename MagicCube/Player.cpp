/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include <boost\bind.hpp>
#include <boost\thread.hpp>

#include <windows.h>

#include "Player.h"


namespace MagicCube
{
	Player::Player(MagicCube& mc)
		: m_MagicCube(mc)
		, m_CurrentPointer(0)
		, m_StepResult(Manipulation_END)
		//, m_EngineHandle(NULL)
		, m_EngineMutex(new boost::mutex())
	{
	}

	Player::~Player()
	{
	}

	void Player::reset()
	{
		m_ManipulationSequence.clear();
		m_CurrentPointer = 0;
	}

	void Player::load(const std::string& /*source*/)
	{
		reset();
		//m_ManipulationSequence.push_back(Manipulation_Identity);
	}

	void Player::tick()
	{
		boost::mutex::scoped_lock scoped_lock(*m_EngineMutex);

		//static int s_time = 0;

		if((m_StepResult != Manipulation_END) /*&& (s_time <= 0)*/)
		{
			m_ManipulationSequence.push_back(m_StepResult);
			++ m_CurrentPointer;

			if(ready() /*&& !atBack()*/)
			{
				Manipulation m = m_ManipulationSequence[m_CurrentPointer];
				if(!m_fnSynchronize || m_fnSynchronize(m))
					m_MagicCube.doManipulation(m);
				else
					// cancel if synchronize failed.
					-- m_CurrentPointer;
			}

			m_StepResult = Manipulation_END;
			m_fnSynchronize.clear();
			m_Engine.reset();

			//s_time = 100;
		}

		// yield CPU to engine thread
		if(m_Engine)
			boost::thread::yield();

		//-- s_time;
	}

	bool Player::ready() const
	{
		return !m_ManipulationSequence.empty();
	}

	bool Player::busy() const
	{
		return m_Engine;
	}

	bool Player::atFront() const
	{
		return m_CurrentPointer == 0;
	}

	bool Player::atBack() const
	{
		return !atFront() && !(m_CurrentPointer < m_ManipulationSequence.size()
			&& isValidManipulation(m_ManipulationSequence[m_CurrentPointer])
			&& m_ManipulationSequence[m_CurrentPointer] > Manipulation_Identity);
	}

	size_t Player::step() const
	{
		return m_CurrentPointer - (atBack() ? 1 : 0);
	}

	void Player::advance(const SynchronizeFunctor& fnSynchronize)
	{
		if(ready() && !atBack())
		{
			++ m_CurrentPointer;

			if(m_CurrentPointer >= m_ManipulationSequence.size())
			{
				-- m_CurrentPointer;
				m_fnSynchronize = fnSynchronize;
				m_Engine.reset(new boost::thread(boost::bind(&Player::next, this)));

				// asynchronous notify step result in 'tick'
				return;
			}

			if(ready() && !atBack())
			{
				Manipulation m = m_ManipulationSequence[m_CurrentPointer];
				if(!fnSynchronize || fnSynchronize(m))
					m_MagicCube.doManipulation(m);
				else
					// cancel if synchronize failed.
					-- m_CurrentPointer;

				return;
			}
		}

		if(fnSynchronize)
			fnSynchronize(Manipulation_Identity);

		//return Manipulation_Identity;
	}

	void Player::backoff(const SynchronizeFunctor& fnSynchronize)
	{
		if(ready() && !atFront())
		{
			if(atBack())
				-- m_CurrentPointer;

			if(!atFront())
			{
				Manipulation m = m_ManipulationSequence[m_CurrentPointer];
				-- m_CurrentPointer;

				if(!fnSynchronize || fnSynchronize(reverseManipulation(m)))
				{
					m_MagicCube.undoManipulation(m);
				}
				else
					// cancel if synchronize failed.
					++ m_CurrentPointer;

				return;
			}
		}

		if(fnSynchronize)
			fnSynchronize(Manipulation_Identity);

		//return Manipulation_Identity;
	}

	void Player::setStepResult(Manipulation m)
	{
		boost::mutex::scoped_lock scoped_lock(*m_EngineMutex);

		if(m_ManipulationSequence.empty())
			m_ManipulationSequence.push_back(m);
		else
			m_StepResult = m;
	}

	void Player::clearEngine()
	{
		//boost::mutex::scoped_lock scoped_lock(*m_EngineMutex);

		if(m_Engine)
			m_Engine->join();
	}

	void Player::next()
	{
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

		// handle 'next' thread exceptions
		try
		{
			doNext();
		}
		catch(const std::exception& e)
		{
			MessageBox( NULL, e.what(), "An standard exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
	}
}
