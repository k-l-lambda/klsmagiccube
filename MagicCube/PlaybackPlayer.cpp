/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include <iostream>
#include <fstream>
#include <boost\lexical_cast.hpp>

#include "PlaybackPlayer.h"
#include "StateArchive.h"


namespace MagicCube
{
	PlaybackPlayer::PlaybackPlayer(MagicCube& mc)
		: Player(mc)
	{
	}

	PlaybackPlayer::~PlaybackPlayer()
	{
		clearEngine();
	}

	void PlaybackPlayer::load(const std::string& filename)
	{
		m_PlaybackManipulations.clear();

		std::ifstream file(filename.data());
		if(file.is_open())
		{
			char buffer[64];
			file.getline(buffer, 64);
			StateArchive sa(buffer);
			if(sa.valid())
			{
				m_MagicCube.setStateArchives(sa);

				while(!file.eof())
				{
					file.getline(buffer, 64);
					try
					{
						if(buffer[0])
						{
							Manipulation m = Manipulation(boost::lexical_cast<int>(std::string(buffer)));
							if(isValidManipulation(m))
								m_PlaybackManipulations.push_back(m);
							else
							{
								std::cout << "PlaybackPlayer::load: there is invalid manipulation: " << int(m)
									<< " in file \"" << filename << "\"." << std::endl;

								break;
							}
						}
					}
					catch(const boost::bad_lexical_cast& /*e*/)
					{
						std::cout << "PlaybackPlayer::load: there is invalid manipulation:" << buffer << "." << std::endl;

						break;
					}
				}

				reset();
				//m_ManipulationSequence.push_back(Manipulation_Identity);
				setStepResult(Manipulation_Identity);
			}
			else
			{
				std::cout << "PlaybackPlayer::load: the file doesnot contain valid playback data: \"" << filename << "\"." << std::endl;
				throw std::logic_error("the file doesnot contain valid playback data.");
			}
		}
		else
		{
			std::cout << "PlaybackPlayer::load: cannot open file \"" << filename << "\"." << std::endl;
			throw std::logic_error("cannot open file.");
		}
	}

	void PlaybackPlayer::save(const std::string& filename) const
	{
		std::ofstream file(filename.data());

		if(file.is_open())
		{
			const std::deque<Manipulation>& mhistory = m_MagicCube.getHistoryManipulation();

			StateArchive sa(m_MagicCube);
			MagicCube cube(sa);
			for(std::deque<Manipulation>::const_reverse_iterator it = mhistory.rbegin(); it != mhistory.rend(); ++ it)
				cube.undoManipulation(*it);

			file << StateArchive(cube).getSerializationCode() << std::endl;
			for(std::deque<Manipulation>::const_iterator it = mhistory.begin(); it != mhistory.end(); ++ it)
				if(*it != Manipulation_Identity)
					file << int(*it) << std::endl;
		}
		else
			std::cout << "PlaybackPlayer::save: cannot write file \"" << filename << "\"." << std::endl;
	}

	void PlaybackPlayer::doNext()
	{
		if(!m_PlaybackManipulations.empty())
		{
			//m_ManipulationSequence.push_back(m_PlaybackManipulations.front());
			Manipulation m = m_PlaybackManipulations.front();
			m_PlaybackManipulations.erase(m_PlaybackManipulations.begin());
			setStepResult(m);
		}
		else
			setStepResult(Manipulation_Identity);
	}
}
