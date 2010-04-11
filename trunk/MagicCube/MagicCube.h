/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__MAGICCUBE_H__
#define	__MAGICCUBE_H__



#include <vector>
#include <stack>


#ifdef	_MAGICCUBEDLL
#	define	MAGICCUBE_API	__declspec(dllexport)
#else
#	define	MAGICCUBE_API	__declspec(dllimport)
#endif	// defined(_MAGICCUBEDLL)

#ifdef	_LUADLL
#	define	LUAPLAYER_API	__declspec(dllexport)
#else
#	define	LUAPLAYER_API	__declspec(dllimport)
#endif	// defined(_LUADLL)


namespace MagicCube
{
	enum PositionScalar;
	enum QuaternionScalar;
	enum Face;
	enum Manipulation;

	class StateArchive;


	template<typename T>
	T&	plusAsInt(T& i)
	{
		return reinterpret_cast<T&>(++ reinterpret_cast<int&>(i));
	}

	inline PositionScalar&		operator ++ (PositionScalar& f)		{return plusAsInt<PositionScalar>(f);}
	inline QuaternionScalar&	operator ++ (QuaternionScalar& f)	{return plusAsInt<QuaternionScalar>(f);}
	inline Face&				operator ++ (Face& f)				{return plusAsInt<Face>(f);}
	inline Manipulation&		operator ++ (Manipulation& f)		{return plusAsInt<Manipulation>(f);}


	enum PositionScalar
	{
		Negative,
		Zero,
		Positive,
	};

	inline bool isValidPositionScalar(PositionScalar ps)
	{
		return (ps == Negative) || (ps == Zero) || (ps == Positive);
	}

	enum QuaternionScalar
	{
		QS_0,
		QS_Half,
		QS_hr2,
		QS_1,
		QS_n_Half,
		QS_n_hr2,
		QS_n_1,

		QS_END,
	};

	inline bool isValidQuaternionScalar(QuaternionScalar qs)
	{
		return (qs >= QS_0) && (qs < QS_END);
	}

	inline QuaternionScalar inverseQuaternionScalar(QuaternionScalar qs)
	{
		switch(qs)
		{
		case QS_Half:		return QS_n_Half;
		case QS_hr2:		return QS_n_hr2;
		case QS_1:			return QS_n_1;
		case QS_n_Half:		return QS_Half;
		case QS_n_hr2:		return QS_hr2;
		case QS_n_1:		return QS_1;
		}

		return qs;
	}

	enum Face
	{
		Face_INVALID = -1,

		Face_pX,
		Face_nX,
		Face_pY,
		Face_nY,
		Face_pZ,
		Face_nZ,

		Face_END,
	};

	inline bool isValidFace(Face f)
	{
		return (f >= 0) && (f < 6);
	}


	struct MAGICCUBE_API	Position
	{
		Position()
			: x(Zero)
			, y(Zero)
			, z(Zero)
		{
		};

		Position(PositionScalar _x, PositionScalar _y, PositionScalar _z)
			: x(_x)
			, y(_y)
			, z(_z)
		{
		};

		bool operator == (const Position& p) const
		{
			return (x == p.x) && (y == p.y) && (z == p.z);
		};

		bool valid() const
		{
			return isValidPositionScalar(x) && isValidPositionScalar(y) && isValidPositionScalar(z);
		};

		PositionScalar	x, y, z;
	};
	static const Position IDENTITY_POSITION;

	inline Position faceToPosition(Face f)
	{
		switch(f)
		{
		case Face_pX:	return Position(Positive, Zero, Zero);
		case Face_nX:	return Position(Negative, Zero, Zero);
		case Face_pY:	return Position(Zero, Positive, Zero);
		case Face_nY:	return Position(Zero, Negative, Zero);
		case Face_pZ:	return Position(Zero, Zero, Positive);
		case Face_nZ:	return Position(Zero, Zero, Negative);
		}

		throw std::logic_error("faceToPosition: invlid f");
	}

	inline Face positionToFace(const Position& p)
	{
		for(Face f = Face_pX; f < Face_END; ++ f)
			if(faceToPosition(f) == p)
				return f;

		throw std::logic_error("positionToFace: invlid p");
	}

	inline std::vector<Position> faceToVertices(Face f)
	{
		std::vector<Position> vertices;

		switch(f)
		{
		case Face_pX:
			vertices.push_back(Position(Positive, Negative, Negative));
			vertices.push_back(Position(Positive, Negative, Positive));
			vertices.push_back(Position(Positive, Positive, Negative));
			vertices.push_back(Position(Positive, Positive, Positive));

			break;
		case Face_nX:
			vertices.push_back(Position(Negative, Negative, Negative));
			vertices.push_back(Position(Negative, Negative, Positive));
			vertices.push_back(Position(Negative, Positive, Negative));
			vertices.push_back(Position(Negative, Positive, Positive));

			break;
		case Face_pY:
			vertices.push_back(Position(Negative, Positive, Negative));
			vertices.push_back(Position(Negative, Positive, Positive));
			vertices.push_back(Position(Positive, Positive, Negative));
			vertices.push_back(Position(Positive, Positive, Positive));

			break;
		case Face_nY:
			vertices.push_back(Position(Negative, Negative, Negative));
			vertices.push_back(Position(Negative, Negative, Positive));
			vertices.push_back(Position(Positive, Negative, Negative));
			vertices.push_back(Position(Positive, Negative, Positive));

			break;
		case Face_pZ:
			vertices.push_back(Position(Negative, Negative, Positive));
			vertices.push_back(Position(Negative, Positive, Positive));
			vertices.push_back(Position(Positive, Negative, Positive));
			vertices.push_back(Position(Positive, Positive, Positive));

			break;
		case Face_nZ:
			vertices.push_back(Position(Negative, Negative, Negative));
			vertices.push_back(Position(Negative, Positive, Negative));
			vertices.push_back(Position(Positive, Negative, Negative));
			vertices.push_back(Position(Positive, Positive, Negative));

			break;

		default:
			throw std::logic_error("faceToPosition: invlid f");
		}

		return vertices;
	}

	struct MAGICCUBE_API	Orientation
	{
		static const size_t			s_ValideValueCount = 24;
		static const Orientation	VALID_VALUE[s_ValideValueCount];

		Orientation()
			: w(QS_1)
			, x(QS_0)
			, y(QS_0)
			, z(QS_0)
		{
		};

		Orientation(QuaternionScalar _w, QuaternionScalar _x, QuaternionScalar _y, QuaternionScalar _z)
			: w(_w)
			, x(_x)
			, y(_y)
			, z(_z)
		{
		};

		bool operator == (const Orientation& q) const
		{
			return ((w == q.w) && (x == q.x) && (y == q.y) && (z == q.z)) || ((w == inverseQuaternionScalar(q.w))
				&& (x == inverseQuaternionScalar(q.x)) && (y == inverseQuaternionScalar(q.y))
				&& (z == inverseQuaternionScalar(q.z)));
		};

		bool valid() const
		{
			for(size_t i = 0; i < s_ValideValueCount; ++ i)
				if(*this == VALID_VALUE[i])
					return true;

			return false;
		};

		size_t validId() const
		{
			for(size_t i = 0; i < s_ValideValueCount; ++ i)
				if(*this == VALID_VALUE[i])
					return i;

			throw std::logic_error("Orientation::validId: this is invalid.");
		};

		QuaternionScalar	w, x, y, z;
	};
	static const Orientation	IDENTITY_ORIENTATION;

	MAGICCUBE_API	Orientation operator * (const Orientation& o1, const Orientation& o2);
	MAGICCUBE_API	Position operator * (const Position& p, const Orientation& o);
	MAGICCUBE_API	Face operator * (Face f, const Orientation& o);

	struct MAGICCUBE_API	UnitCube
	{
		UnitCube()
			: position(IDENTITY_POSITION)
			, orientation(IDENTITY_ORIENTATION)
		{
		};

		UnitCube(Position p, Orientation o)
			: position(p)
			, orientation(o)
		{
		};

		bool operator == (const UnitCube& uc) const
		{
			return (position == uc.position) && (orientation == uc.orientation);
		};

		bool valid() const
		{
			return position.valid() && orientation.valid();
		};

		Position		position;
		Orientation		orientation;
	};
	static const UnitCube	ORIGIN_UNITCUBE;

	enum Manipulation
	{
		Manipulation_Identity,

		Rotate_X90,
		Rotate_Xn90,
		Rotate_X180,
		Rotate_nX90,
		Rotate_nXn90,
		Rotate_nX180,
		Rotate_Y90,
		Rotate_Yn90,
		Rotate_Y180,
		Rotate_nY90,
		Rotate_nYn90,
		Rotate_nY180,
		Rotate_Z90,
		Rotate_Zn90,
		Rotate_Z180,
		Rotate_nZ90,
		Rotate_nZn90,
		Rotate_nZ180,

		Manipulation_END,
	};

	inline bool isValidManipulation(Manipulation m)
	{
		return (m >= Manipulation_Identity) && (m < Manipulation_END);
	}

	inline Manipulation reverseManipulation(Manipulation m)
	{
		switch(m)
		{
		case Rotate_X90:	return Rotate_Xn90;
		case Rotate_Xn90:	return Rotate_X90;
		case Rotate_nX90:	return Rotate_nXn90;
		case Rotate_nXn90:	return Rotate_nX90;
		case Rotate_Y90:	return Rotate_Yn90;
		case Rotate_Yn90:	return Rotate_Y90;
		case Rotate_nY90:	return Rotate_nYn90;
		case Rotate_nYn90:	return Rotate_nY90;
		case Rotate_Z90:	return Rotate_Zn90;
		case Rotate_Zn90:	return Rotate_Z90;
		case Rotate_nZ90:	return Rotate_nZn90;
		case Rotate_nZn90:	return Rotate_nZ90;
		}

		return m;
	}

	inline int dimensionOfManipulation(Manipulation m)
	{
		switch(m)
		{
		case Rotate_X90:	return 1;
		case Rotate_Xn90:	return -1;
		case Rotate_X180:	return 2;
		case Rotate_nX90:	return 1;
		case Rotate_nXn90:	return -1;
		case Rotate_nX180:	return 2;
		case Rotate_Y90:	return 1;
		case Rotate_Yn90:	return -1;
		case Rotate_Y180:	return 2;
		case Rotate_nY90:	return 1;
		case Rotate_nYn90:	return -1;
		case Rotate_nY180:	return 2;
		case Rotate_Z90:	return 1;
		case Rotate_Zn90:	return -1;
		case Rotate_Z180:	return 2;
		case Rotate_nZ90:	return 1;
		case Rotate_nZn90:	return -1;
		case Rotate_nZ180:	return 2;
		}

		return 0;
	}

	inline Face manipulationToFace(Manipulation m)
	{
		switch(m)
		{
		case Rotate_X90:	return Face_pX;
		case Rotate_Xn90:	return Face_pX;
		case Rotate_X180:	return Face_pX;
		case Rotate_nX90:	return Face_nX;
		case Rotate_nXn90:	return Face_nX;
		case Rotate_nX180:	return Face_nX;
		case Rotate_Y90:	return Face_pY;
		case Rotate_Yn90:	return Face_pY;
		case Rotate_Y180:	return Face_pY;
		case Rotate_nY90:	return Face_nY;
		case Rotate_nYn90:	return Face_nY;
		case Rotate_nY180:	return Face_nY;
		case Rotate_Z90:	return Face_pZ;
		case Rotate_Zn90:	return Face_pZ;
		case Rotate_Z180:	return Face_pZ;
		case Rotate_nZ90:	return Face_nZ;
		case Rotate_nZn90:	return Face_nZ;
		case Rotate_nZ180:	return Face_nZ;
		}

		throw std::logic_error("manipulationToFace: invalid m");
	}

	inline Manipulation faceToManipulation(Face f, int dimension)
	{
		dimension %= 4;
		if(dimension < 0)
			dimension += 4;

		if(dimension == 0)
			return Manipulation_Identity;

		switch(f)
		{
		case Face_pX:
			switch(dimension)
			{
			case 1:	return Rotate_X90;
			case 2:	return Rotate_X180;
			case 3:	return Rotate_Xn90;
			}

			break;
		case Face_nX:
			switch(dimension)
			{
			case 1:	return Rotate_nX90;
			case 2:	return Rotate_nX180;
			case 3:	return Rotate_nXn90;
			}

			break;
		case Face_pY:
			switch(dimension)
			{
			case 1:	return Rotate_Y90;
			case 2:	return Rotate_Y180;
			case 3:	return Rotate_Yn90;
			}

			break;
		case Face_nY:
			switch(dimension)
			{
			case 1:	return Rotate_nY90;
			case 2:	return Rotate_nY180;
			case 3:	return Rotate_nYn90;
			}

			break;
		case Face_pZ:
			switch(dimension)
			{
			case 1:	return Rotate_Z90;
			case 2:	return Rotate_Z180;
			case 3:	return Rotate_Zn90;
			}

			break;
		case Face_nZ:
			switch(dimension)
			{
			case 1:	return Rotate_nZ90;
			case 2:	return Rotate_nZ180;
			case 3:	return Rotate_nZn90;
			}

			break;
		}

		throw std::logic_error("manipulationToFace: invalid m");
	}

	inline const char* manipulationToString(Manipulation m)
	{
		switch(m)
		{
#define	EnumToString(e)		case e: return #e;
			EnumToString(Manipulation_Identity);
			EnumToString(Rotate_X90);
			EnumToString(Rotate_Xn90);
			EnumToString(Rotate_X180);
			EnumToString(Rotate_nX90);
			EnumToString(Rotate_nXn90);
			EnumToString(Rotate_nX180);
			EnumToString(Rotate_Y90);
			EnumToString(Rotate_Yn90);
			EnumToString(Rotate_Y180);
			EnumToString(Rotate_nY90);
			EnumToString(Rotate_nYn90);
			EnumToString(Rotate_nY180);
			EnumToString(Rotate_Z90);
			EnumToString(Rotate_Zn90);
			EnumToString(Rotate_Z180);
			EnumToString(Rotate_nZ90);
			EnumToString(Rotate_nZn90);
			EnumToString(Rotate_nZ180);
#undef	EnumToString
		}

		return "";
	}


	class MAGICCUBE_API	MagicCube
	{
	public:
		static const MagicCube	ZERO;

	public:
		MagicCube();
		MagicCube(const MagicCube& mc);
		MagicCube(const StateArchive& sa);

		MagicCube& operator = (const MagicCube& mc);

		void	reset();
		void	setStateArchives(const StateArchive& sa);

		bool	operator == (const MagicCube& mc) const;
		bool	logicEqual(const MagicCube& mc) const;

		bool	physicalValid() const;
		bool	inoutValid() const;
		bool	zeroableValid() const;

		bool	connectiveWith(const MagicCube& mc) const;

		void	doManipulation(Manipulation m);
		void	undoManipulation(Manipulation m);

		Manipulation	manipulationBackOff();
		Manipulation	manipulationAdvance();

		const UnitCube&	getUnit(size_t index) const;

		const UnitCube&	getUnit(const Position& pos) const;
		size_t	getUnitIndex(const Position& pos) const;

		static bool	isOuterFace(const Position& pos, Face f);
		bool	isOuterFace(size_t index, Face f) const;

		size_t	getUnitPositionStateIndex(size_t index) const;
		size_t	getUnitOrientationStateIndex(size_t index) const;

		const std::deque<Manipulation>&	getHistoryManipulation() const	{return m_HistoryManipulation._Get_container();};

		static const size_t	s_ActiveCubeCount = 26;

	private:
		UnitCube&	unit(PositionScalar x, PositionScalar y, PositionScalar z);

		void	_doManipulation(Manipulation m);
		void	_undoManipulation(Manipulation m);

	private:
		UnitCube	m_CubeArray[s_ActiveCubeCount];

		std::stack<Manipulation>	m_HistoryManipulation;
		std::stack<Manipulation>	m_CancelledManipulation;
	};
}



#endif	// !defined(__MAGICCUBE_H__)
