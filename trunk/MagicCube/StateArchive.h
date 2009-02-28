/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__STATEARCHIVE_H__
#define	__STATEARCHIVE_H__



#include <string>

#include "MagicCube.h"


namespace MagicCube
{
	class MAGICCUBE_API	StateArchive
	{
	public:
		struct UnitState
		{
			size_t PositionIndex;
			size_t OrientationIndex;
		};

	public:
		StateArchive(const MagicCube& mc);
		StateArchive(const std::string& code);

		const std::string&	getSerializationCode() const	{return m_SerializationCode;};

		const UnitState& getCornerState(size_t i) const	{return m_CornerState[i];};
		const UnitState& getEdgeState(size_t i) const	{return m_EdgeState[i];};

		bool	valid() const;

	public:
		static std::string	generateSerializationCode(const StateArchive& sa);

	private:
		UnitState	m_CornerState[8];
		UnitState	m_EdgeState[12];

		std::string	m_SerializationCode;
	};
}



#endif	// !defined(__STATEARCHIVE_H__)
