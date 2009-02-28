/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include <stdexcept>
#include <iostream>
#include <assert.h>

#include <Integer.h>

#pragma comment(lib, "Integer.lib")


#include "StateArchive.h"


namespace MagicCube
{
	void	appendToInteger(size_t code, size_t max, integer::Integer& i)
	{
		i = i * max + code;
	}

	size_t	fetchFromInteger(size_t max, integer::Integer& i)
	{
		size_t code = size_t((i % max).toInt());
		i /= max;

		return code;
	}


	StateArchive::StateArchive(const MagicCube& mc)
	{
		for(size_t i = 0; i < 8; ++ i)
		{
			m_CornerState[i].PositionIndex = mc.getUnitPositionStateIndex(i);
			m_CornerState[i].OrientationIndex = mc.getUnitOrientationStateIndex(i);
		}

		for(size_t i = 0; i < 12; ++ i)
		{
			m_EdgeState[i].PositionIndex = mc.getUnitPositionStateIndex(8 + i);
			m_EdgeState[i].OrientationIndex = mc.getUnitOrientationStateIndex(8 + i);
		}

		m_SerializationCode = generateSerializationCode(*this);
	}

	StateArchive::StateArchive(const std::string& code)
		: m_SerializationCode(code)
	{
		integer::Integer ii(code, 16);

		for(size_t i = 0; i < 12; ++ i)
		{
			m_EdgeState[11 - i].OrientationIndex = fetchFromInteger(2, ii);
			m_EdgeState[11 - i].PositionIndex = fetchFromInteger(12, ii);
		}

		for(size_t i = 0; i < 8; ++ i)
		{
			m_CornerState[7 - i].OrientationIndex = fetchFromInteger(3, ii);
			m_CornerState[7 - i].PositionIndex = fetchFromInteger(8, ii);
		}

		assert(integer::Integer(generateSerializationCode(*this), 16) == integer::Integer(code, 16));
	}

	bool StateArchive::valid() const
	{
		return MagicCube(*this).physicalValid();
	}

	std::string StateArchive::generateSerializationCode(const StateArchive& sa)
	{
		integer::Integer ii;

		for(size_t i = 0; i < 8; ++ i)
		{
			appendToInteger(sa.m_CornerState[i].PositionIndex, 8, ii);
			appendToInteger(sa.m_CornerState[i].OrientationIndex, 3, ii);
		}

		for(size_t i = 0; i < 12; ++ i)
		{
			appendToInteger(sa.m_EdgeState[i].PositionIndex, 12, ii);
			appendToInteger(sa.m_EdgeState[i].OrientationIndex, 2, ii);
		}

		return ii.print16();
	}
}
