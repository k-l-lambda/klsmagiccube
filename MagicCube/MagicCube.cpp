/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include <stdexcept>
#include <iostream>
#include <assert.h>

#include "MagicCube.h"
#include "Manipulation.h"
#include "StateArchive.h"


namespace MagicCube
{
	static const size_t s_CornerCount = 8;
	static const size_t s_EdgeCount = 12;
	static const size_t s_AxisCount = 6;

	static const Position	CORNER_POSITION[s_CornerCount] =
	{
		Position(Negative,	Positive,	Positive),
		Position(Positive,	Positive,	Positive),
		Position(Negative,	Positive,	Negative),
		Position(Positive,	Positive,	Negative),
		Position(Negative,	Negative,	Positive),
		Position(Positive,	Negative,	Positive),
		Position(Negative,	Negative,	Negative),
		Position(Positive,	Negative,	Negative),
	};

	static const Position	EDGE_POSITION[s_EdgeCount] =
	{
		Position(Zero,		Positive,	Positive),
		Position(Positive,	Positive,	Zero),
		Position(Zero,		Positive,	Negative),
		Position(Negative,	Positive,	Zero),
		Position(Negative,	Zero,		Positive),
		Position(Positive,	Zero,		Positive),
		Position(Positive,	Zero,		Negative),
		Position(Negative,	Zero,		Negative),
		Position(Zero,		Negative,	Positive),
		Position(Positive,	Negative,	Zero),
		Position(Zero,		Negative,	Negative),
		Position(Negative,	Negative,	Zero),
	};

	static const Position	AXIS_POSITION[s_AxisCount] =
	{
		Position(Positive,	Zero,		Zero),
		Position(Negative,	Zero,		Zero),
		Position(Zero,		Positive,	Zero),
		Position(Zero,		Negative,	Zero),
		Position(Zero,		Zero,		Positive),
		Position(Zero,		Zero,		Negative),
	};

	typedef	std::vector<size_t>	OrientationIdArray;

	template<size_t from, size_t to>
	struct ValidOrientationIds
	{
		static OrientationIdArray	corner()
		{
			OrientationIdArray result;

			for(size_t i = 0; i < Orientation::s_ValideValueCount; ++ i)
			{
				Face f = Face_pX;
				for(; f < Face_END; ++ f)
				{
					bool out1 = MagicCube::isOuterFace(CORNER_POSITION[from], f);
					bool out2 = MagicCube::isOuterFace(CORNER_POSITION[to], f * Orientation::VALID_VALUE[i]);

					if(out1 != out2)
						break;
				}

				if(f == Face_END)
					result.push_back(i);
			}

			assert(result.size() == 3);

			return result;
		};

		static OrientationIdArray	edge()
		{
			OrientationIdArray result;

			for(size_t i = 0; i < Orientation::s_ValideValueCount; ++ i)
			{
				Face f = Face_pX;
				for(; f < Face_END; ++ f)
				{
					bool out1 = MagicCube::isOuterFace(EDGE_POSITION[from], f);
					bool out2 = MagicCube::isOuterFace(EDGE_POSITION[to], f * Orientation::VALID_VALUE[i]);

					if(out1 != out2)
						break;
				}

				if(f == Face_END)
					result.push_back(i);
			}

			assert(result.size() == 2);

			return result;
		};

		static const OrientationIdArray&	getCorner()
		{
			static const OrientationIdArray ids = corner();

			return ids;
		};

		static const OrientationIdArray&	getEdge()
		{
			static const OrientationIdArray ids = edge();

			return ids;
		};
	};

	typedef	const OrientationIdArray&	(*GetValidOrientationIdsFunction)();

	template<size_t from>
	struct GetValidCornerOrientationIdsFunctionArray
	{
		static const GetValidOrientationIdsFunction*	function()
		{
			static const GetValidOrientationIdsFunction	functions[] =
			{
				&ValidOrientationIds<from, 0>::getCorner,
				&ValidOrientationIds<from, 1>::getCorner,
				&ValidOrientationIds<from, 2>::getCorner,
				&ValidOrientationIds<from, 3>::getCorner,
				&ValidOrientationIds<from, 4>::getCorner,
				&ValidOrientationIds<from, 5>::getCorner,
				&ValidOrientationIds<from, 6>::getCorner,
				&ValidOrientationIds<from, 7>::getCorner,
			};

			//if(to > s_CornerCount)
			//	throw std::logic_error("GetValidCornerOrientationIdsFunctionArray::function: \'to\' out of range");

			return functions;
		};
	};

	template<size_t from>
	struct GetValidEdgeOrientationIdsFunctionArray
	{
		static const GetValidOrientationIdsFunction*	function()
		{
			static const GetValidOrientationIdsFunction	functions[] =
			{
				&ValidOrientationIds<from, 0>::getEdge,
				&ValidOrientationIds<from, 1>::getEdge,
				&ValidOrientationIds<from, 2>::getEdge,
				&ValidOrientationIds<from, 3>::getEdge,
				&ValidOrientationIds<from, 4>::getEdge,
				&ValidOrientationIds<from, 5>::getEdge,
				&ValidOrientationIds<from, 6>::getEdge,
				&ValidOrientationIds<from, 7>::getEdge,
				&ValidOrientationIds<from, 8>::getEdge,
				&ValidOrientationIds<from, 9>::getEdge,
				&ValidOrientationIds<from, 10>::getEdge,
				&ValidOrientationIds<from, 11>::getEdge,
			};

			//if(to > s_EdgeCount)
			//	throw std::logic_error("GetValidEdgeOrientationIdsFunctionArray::function: \'to\' out of range");

			return functions;
		};
	};

	static const GetValidOrientationIdsFunction*	s_GetValidCornerOrientationIdsFunction[] =
	{
		GetValidCornerOrientationIdsFunctionArray<0>::function(),
		GetValidCornerOrientationIdsFunctionArray<1>::function(),
		GetValidCornerOrientationIdsFunctionArray<2>::function(),
		GetValidCornerOrientationIdsFunctionArray<3>::function(),
		GetValidCornerOrientationIdsFunctionArray<4>::function(),
		GetValidCornerOrientationIdsFunctionArray<5>::function(),
		GetValidCornerOrientationIdsFunctionArray<6>::function(),
		GetValidCornerOrientationIdsFunctionArray<7>::function(),
	};

	static const GetValidOrientationIdsFunction*	s_GetValidEdgeOrientationIdsFunction[] =
	{
		GetValidEdgeOrientationIdsFunctionArray<0>::function(),
		GetValidEdgeOrientationIdsFunctionArray<1>::function(),
		GetValidEdgeOrientationIdsFunctionArray<2>::function(),
		GetValidEdgeOrientationIdsFunctionArray<3>::function(),
		GetValidEdgeOrientationIdsFunctionArray<4>::function(),
		GetValidEdgeOrientationIdsFunctionArray<5>::function(),
		GetValidEdgeOrientationIdsFunctionArray<6>::function(),
		GetValidEdgeOrientationIdsFunctionArray<7>::function(),
		GetValidEdgeOrientationIdsFunctionArray<8>::function(),
		GetValidEdgeOrientationIdsFunctionArray<9>::function(),
		GetValidEdgeOrientationIdsFunctionArray<10>::function(),
		GetValidEdgeOrientationIdsFunctionArray<11>::function(),
	};

	const OrientationIdArray&	getValidCornerOrientationIds(size_t from, size_t to)
	{
		return s_GetValidCornerOrientationIdsFunction[from][to]();
	}

	const OrientationIdArray&	getValidEdgeOrientationIds(size_t from, size_t to)
	{
		return s_GetValidEdgeOrientationIdsFunction[from][to]();
	}


	const MagicCube	MagicCube::ZERO;

	MagicCube::MagicCube()
	{
		reset();
	}

	MagicCube::MagicCube(const MagicCube& mc)
	{
		setStateArchives(StateArchive(mc));
	}

	MagicCube::MagicCube(const StateArchive& sa)
	{
		setStateArchives(sa);
	}

	MagicCube& MagicCube::operator = (const MagicCube& mc)
	{
		setStateArchives(StateArchive(mc));

		return *this;
	}

	void MagicCube::reset()
	{
		// 8 corner blocks
		m_CubeArray[0]	= UnitCube(CORNER_POSITION[0],	IDENTITY_ORIENTATION);
		m_CubeArray[1]	= UnitCube(CORNER_POSITION[1],	IDENTITY_ORIENTATION);
		m_CubeArray[2]	= UnitCube(CORNER_POSITION[2],	IDENTITY_ORIENTATION);
		m_CubeArray[3]	= UnitCube(CORNER_POSITION[3],	IDENTITY_ORIENTATION);
		m_CubeArray[4]	= UnitCube(CORNER_POSITION[4],	IDENTITY_ORIENTATION);
		m_CubeArray[5]	= UnitCube(CORNER_POSITION[5],	IDENTITY_ORIENTATION);
		m_CubeArray[6]	= UnitCube(CORNER_POSITION[6],	IDENTITY_ORIENTATION);
		m_CubeArray[7]	= UnitCube(CORNER_POSITION[7],	IDENTITY_ORIENTATION);

		// 12 edge blocks
		m_CubeArray[8]	= UnitCube(EDGE_POSITION[0],	IDENTITY_ORIENTATION);
		m_CubeArray[9]	= UnitCube(EDGE_POSITION[1],	IDENTITY_ORIENTATION);
		m_CubeArray[10]	= UnitCube(EDGE_POSITION[2],	IDENTITY_ORIENTATION);
		m_CubeArray[11]	= UnitCube(EDGE_POSITION[3],	IDENTITY_ORIENTATION);
		m_CubeArray[12]	= UnitCube(EDGE_POSITION[4],	IDENTITY_ORIENTATION);
		m_CubeArray[13]	= UnitCube(EDGE_POSITION[5],	IDENTITY_ORIENTATION);
		m_CubeArray[14]	= UnitCube(EDGE_POSITION[6],	IDENTITY_ORIENTATION);
		m_CubeArray[15]	= UnitCube(EDGE_POSITION[7],	IDENTITY_ORIENTATION);
		m_CubeArray[16]	= UnitCube(EDGE_POSITION[8],	IDENTITY_ORIENTATION);
		m_CubeArray[17]	= UnitCube(EDGE_POSITION[9],	IDENTITY_ORIENTATION);
		m_CubeArray[18]	= UnitCube(EDGE_POSITION[10],	IDENTITY_ORIENTATION);
		m_CubeArray[19]	= UnitCube(EDGE_POSITION[11],	IDENTITY_ORIENTATION);

		// 6 axis blocks
		m_CubeArray[20]	= UnitCube(AXIS_POSITION[0],	IDENTITY_ORIENTATION);
		m_CubeArray[21]	= UnitCube(AXIS_POSITION[1],	IDENTITY_ORIENTATION);
		m_CubeArray[22]	= UnitCube(AXIS_POSITION[2],	IDENTITY_ORIENTATION);
		m_CubeArray[23]	= UnitCube(AXIS_POSITION[3],	IDENTITY_ORIENTATION);
		m_CubeArray[24]	= UnitCube(AXIS_POSITION[4],	IDENTITY_ORIENTATION);
		m_CubeArray[25]	= UnitCube(AXIS_POSITION[5],	IDENTITY_ORIENTATION);

		m_HistoryManipulation.c.clear();
		m_CancelledManipulation.c.clear();

		std::cout << "[MagicCube] reset." << std::endl;
	}

	void MagicCube::setStateArchives(const StateArchive& sa)
	{
		// 8 corner blocks
		for(size_t i = 0; i < s_CornerCount; ++ i)
		{
			m_CubeArray[i].position = CORNER_POSITION[sa.getCornerState(i).PositionIndex];

			const OrientationIdArray& ids = getValidCornerOrientationIds(ZERO.getUnitPositionStateIndex(i),
				sa.getCornerState(i).PositionIndex);
			m_CubeArray[i].orientation = Orientation::VALID_VALUE[ids[sa.getCornerState(i).OrientationIndex]];
		}

		// 12 edge blocks
		for(size_t i = 0; i < s_EdgeCount; ++ i)
		{
			m_CubeArray[i + s_CornerCount].position = EDGE_POSITION[sa.getEdgeState(i).PositionIndex];

			const OrientationIdArray& ids = getValidEdgeOrientationIds(ZERO.getUnitPositionStateIndex(i + s_CornerCount),
				sa.getEdgeState(i).PositionIndex);
			m_CubeArray[i + s_CornerCount].orientation = Orientation::VALID_VALUE[ids[sa.getEdgeState(i).OrientationIndex]];
		}

		// 6 axis blocks
		m_CubeArray[20]	= UnitCube(AXIS_POSITION[0],	IDENTITY_ORIENTATION);
		m_CubeArray[21]	= UnitCube(AXIS_POSITION[1],	IDENTITY_ORIENTATION);
		m_CubeArray[22]	= UnitCube(AXIS_POSITION[2],	IDENTITY_ORIENTATION);
		m_CubeArray[23]	= UnitCube(AXIS_POSITION[3],	IDENTITY_ORIENTATION);
		m_CubeArray[24]	= UnitCube(AXIS_POSITION[4],	IDENTITY_ORIENTATION);
		m_CubeArray[25]	= UnitCube(AXIS_POSITION[5],	IDENTITY_ORIENTATION);

		m_HistoryManipulation.c.clear();
		m_CancelledManipulation.c.clear();

		//assert(physicalValid());
		if(!physicalValid())
			std::cerr << "MagicCube::setStateArchives: the StateArchive is physical invalid." << std::endl;

		std::cout << "[MagicCube] set state archive:" << sa.getSerializationCode() << std::endl;
	}

	bool MagicCube::operator == (const MagicCube& mc) const
	{
		bool result = true;

		for(size_t i = 0; (i < s_ActiveCubeCount) && result; ++ i)
			result &= (m_CubeArray[i] == mc.m_CubeArray[i]);

		return result;
	}

	bool MagicCube::logicEqual(const MagicCube& mc) const
	{
		bool result = true;

		for(size_t i = 0; (i < s_CornerCount + s_EdgeCount) && result; ++ i)
			result &= (m_CubeArray[i] == mc.m_CubeArray[i]);

		return result;
	}

	bool MagicCube::physicalValid() const
	{
		bool result = true;

		try
		{
			for(PositionScalar i = Negative; i <= Positive; ++ i)
				for(PositionScalar j = Negative; j <= Positive; ++ j)
					for(PositionScalar k = Negative; k <= Positive; ++ k)
					{
						result &= getUnit(Position(i, j, k)).valid();
					}
		}
		catch(const std::logic_error& /*e*/)
		{
			return false;
		}

		return result;
	}

	bool MagicCube::inoutValid() const
	{
		if(physicalValid())
		{
			for(size_t i = 0; i < s_ActiveCubeCount; ++ i)
				for(Face f = Face_pX; f < Face_END; ++ f)
				{
					Face fLocal = f * m_CubeArray[i].orientation;
					if(isOuterFace(i, fLocal) != ZERO.isOuterFace(i, f))
						return false;
				}

			return true;
		}

		return false;
	}

	bool MagicCube::zeroableValid() const
	{
		if(inoutValid())
		{
			// TODO:
			return true;
		}

		return false;
	}

	void MagicCube::doManipulation(Manipulation m)
	{
		_doManipulation(m);

		m_HistoryManipulation.push(m);
		m_CancelledManipulation.c.clear();
	}

	void MagicCube::undoManipulation(Manipulation m)
	{
		doManipulation(reverseManipulation(m));
	}

	Manipulation MagicCube::manipulationBackOff()
	{
		if(!m_HistoryManipulation.empty())
		{
			Manipulation m = m_HistoryManipulation.top();
			_undoManipulation(m);
			m_CancelledManipulation.push(m);
			m_HistoryManipulation.pop();

			return m;
		}

		return Manipulation_Identity;
	}

	Manipulation MagicCube::manipulationAdvance()
	{
		if(!m_CancelledManipulation.empty())
		{
			Manipulation m = m_CancelledManipulation.top();
			_doManipulation(m);
			m_HistoryManipulation.push(m);
			m_CancelledManipulation.pop();

			return m;
		}

		return Manipulation_Identity;
	}

	/*Manipulation MagicCube::lastManipulation() const
	{
		return m_HistoryManipulation.empty() ? Manipulation_Identity : m_HistoryManipulation.top();
	}

	Manipulation MagicCube::nextManipulation() const
	{
		return m_CancelledManipulation.empty() ? Manipulation_Identity : m_CancelledManipulation.top();
	}*/

	const UnitCube& MagicCube::getUnit(size_t index) const
	{
		if(index >= 0 && index < s_ActiveCubeCount)
			return m_CubeArray[index];

		return ORIGIN_UNITCUBE;
	}

	const UnitCube& MagicCube::getUnit(const Position& pos) const
	{
		return getUnit(getUnitIndex(pos));
	}

	size_t MagicCube::getUnitIndex(const Position& pos) const
	{
		if(!pos.valid())
			throw std::logic_error("MagicCube::getUnitIndex: invalid pos.");

		// center block
		if(pos == IDENTITY_POSITION)
			return -1;

		for(size_t i = 0; i < s_ActiveCubeCount; ++ i)
			if(m_CubeArray[i].position == pos)
				return i;

		throw std::logic_error("MagicCube::getUnitIndex: inner state is invalid, cannot find match unit cube.");
	}

	bool MagicCube::isOuterFace(const Position& pos, Face f)
	{
		switch(f)
		{
		case Face_pX:	return pos.x == Positive;
		case Face_nX:	return pos.x == Negative;
		case Face_pY:	return pos.y == Positive;
		case Face_nY:	return pos.y == Negative;
		case Face_pZ:	return pos.z == Positive;
		case Face_nZ:	return pos.z == Negative;
		}

		throw std::logic_error("faceToPosition: invlid f");
	}

	bool MagicCube::isOuterFace(size_t index, Face f) const
	{
		if(index < s_ActiveCubeCount)
		{
			return isOuterFace(m_CubeArray[index].position, f);
		}
		else
			throw std::logic_error("MagicCube::isOuterFace: index out of range.");
	}

	size_t MagicCube::getUnitPositionStateIndex(size_t index) const
	{
		if(index < s_CornerCount)
		{
			for(size_t i = 0; i < s_CornerCount; ++ i)
				if(m_CubeArray[index].position == CORNER_POSITION[i])
					return i;
		}
		else if(index < s_CornerCount + s_EdgeCount)
		{
			for(size_t i = 0; i < s_EdgeCount; ++ i)
				if(m_CubeArray[index].position == EDGE_POSITION[i])
					return i;
		}
		else if(index < s_CornerCount + s_EdgeCount + s_AxisCount)
		{
			if(m_CubeArray[index].position == EDGE_POSITION[index - s_CornerCount + s_EdgeCount])
				return 0;
		}
		else
			throw std::logic_error("MagicCube::getUnitPositionStateIndex: index out of range");

		throw std::logic_error("MagicCube::getUnitPositionStateIndex: magic cube is in-out invalid");
	}

	size_t MagicCube::getUnitOrientationStateIndex(size_t index) const
	{
		if(index < s_CornerCount)
		{
			const OrientationIdArray& ids = getValidCornerOrientationIds(ZERO.getUnitPositionStateIndex(index),
				getUnitPositionStateIndex(index));
			assert(ids.size() == 3);

			for(size_t i = 0; i < 3; ++ i)
				if(m_CubeArray[index].orientation == Orientation::VALID_VALUE[ids[i]])
					return i;
		}
		else if(index < s_CornerCount + s_EdgeCount)
		{
			const OrientationIdArray& ids = getValidEdgeOrientationIds(ZERO.getUnitPositionStateIndex(index),
				getUnitPositionStateIndex(index));
			assert(ids.size() == 2);

			for(size_t i = 0; i < 2; ++ i)
				if(m_CubeArray[index].orientation == Orientation::VALID_VALUE[ids[i]])
					return i;
		}
		else
			throw std::logic_error("MagicCube::getUnitPositionStateIndex: index out of range");

		throw std::logic_error("MagicCube::getUnitOrientationStateIndex: magic cube is in-out invalid");
	}

	UnitCube& MagicCube::unit(PositionScalar x, PositionScalar y, PositionScalar z)
	{
		size_t index = getUnitIndex(Position(x, y, z));
		if(index >= 0 && index < s_ActiveCubeCount)
			return m_CubeArray[index];

		throw std::logic_error("MagicCube::unit: invalid position");
	}

	void MagicCube::_doManipulation(Manipulation m)
	{
		UnitCube* face[3][3] = {{NULL}};

		for(PositionScalar i = Negative; i <= Positive; ++ i)
			for(PositionScalar j = Negative; j <= Positive; ++ j)
			{
				switch(m)
				{
				case Manipulation_Identity:
					return;

				case Rotate_X90:
				case Rotate_Xn90:
				case Rotate_X180:
					face[i][j] = &(unit(Positive, i, j));

					break;
				case Rotate_nX90:
				case Rotate_nXn90:
				case Rotate_nX180:
					face[i][j] = &(unit(Negative, i, j));

					break;
				case Rotate_Y90:
				case Rotate_Yn90:
				case Rotate_Y180:
					face[i][j] = &(unit(i, Positive, j));

					break;
				case Rotate_nY90:
				case Rotate_nYn90:
				case Rotate_nY180:
					face[i][j] = &(unit(i, Negative, j));

					break;
				case Rotate_Z90:
				case Rotate_Zn90:
				case Rotate_Z180:
					face[i][j] = &(unit(i, j, Positive));

					break;
				case Rotate_nZ90:
				case Rotate_nZn90:
				case Rotate_nZ180:
					face[i][j] = &(unit(i, j, Negative));

					break;
				default:
					std::logic_error("MagicCube::doManipulation: invalid m");
				}
			}

		for(PositionScalar i = Negative; i <= Positive; ++ i)
			for(PositionScalar j = Negative; j <= Positive; ++ j)
				manipulation(*(face[i][j]), m);

		std::cout << "[MagicCube] Manipulate:" << manipulationToString(m) << std::endl;
	}

	void MagicCube::_undoManipulation(Manipulation m)
	{
		_doManipulation(reverseManipulation(m));
	}
}
