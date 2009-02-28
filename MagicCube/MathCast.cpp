/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <stdexcept>

#include "MathCast.h"


Ogre::Real			positionScalarToReal(MagicCube::PositionScalar ps)
{
	switch(ps)
	{
	case MagicCube::Negative:	return -1;
	case MagicCube::Zero:		return 0;
	case MagicCube::Positive:	return 1;
	}

	throw std::logic_error("positionScalarToReal: invlid ps");
}

Ogre::Real			quaternionScalarToReal(MagicCube::QuaternionScalar qs)
{
	switch(qs)
	{
	case MagicCube::QS_0:		return 0;
	case MagicCube::QS_Half:	return 0.5f;
	case MagicCube::QS_hr2:		return 0.70710678f;
	case MagicCube::QS_1:		return 1;
	case MagicCube::QS_n_Half:	return -0.5f;
	case MagicCube::QS_n_hr2:	return -0.70710678f;
	case MagicCube::QS_n_1:		return -1;
	}

	throw std::logic_error("quaternionScalarToReal: invlid qs");
}

Ogre::Vector3		positionToVector3(MagicCube::Position p)
{
	return Ogre::Vector3(positionScalarToReal(p.x), positionScalarToReal(p.y), positionScalarToReal(p.z));
}

Ogre::Quaternion	orientationToQuaternion(MagicCube::Orientation o)
{
	return Ogre::Quaternion(quaternionScalarToReal(o.w), quaternionScalarToReal(o.x), quaternionScalarToReal(o.y),
		quaternionScalarToReal(o.z));
}

Ogre::Vector3		faceToVector3(MagicCube::Face face)
{
	switch(face)
	{
	case MagicCube::Face_pX:	return Ogre::Vector3::UNIT_X;
	case MagicCube::Face_nX:	return -Ogre::Vector3::UNIT_X;
	case MagicCube::Face_pY:	return Ogre::Vector3::UNIT_Y;
	case MagicCube::Face_nY:	return -Ogre::Vector3::UNIT_Y;
	case MagicCube::Face_pZ:	return Ogre::Vector3::UNIT_Z;
	case MagicCube::Face_nZ:	return -Ogre::Vector3::UNIT_Z;
	}

	throw std::logic_error("faceToVector3: invlid face");
}

MagicCube::PositionScalar		realToPositionScalar(Ogre::Real r)
{
	if(abs(r - (-1)) < 0.01f)
		return MagicCube::Negative;
	else if(abs(r - 0) < 0.01f)
		return MagicCube::Zero;
	else if(abs(r - 1) < 0.01f)
		return MagicCube::Positive;

	throw std::logic_error("realToPositionScalar: invalid r");
}

MagicCube::QuaternionScalar		realToQuaternionScalar(Ogre::Real r)
{
	static const float s_Scalars[] =
	{
		0, 0.5f, 0.70710678f, 1, -0.5f, -0.70710678f, -1,
	};

	for(int i = MagicCube::QS_0; i < MagicCube::QS_END; ++ i)
		if(abs(r - s_Scalars[i]) < 0.01f)
			return MagicCube::QuaternionScalar(i);

	throw std::logic_error("realToQuaternionScalar: invalid r");
}

MagicCube::Position		vector3ToPosition(Ogre::Vector3 v)
{
	return MagicCube::Position(realToPositionScalar(v.x), realToPositionScalar(v.y), realToPositionScalar(v.z));
}

MagicCube::Orientation	quaternionToOrientation(Ogre::Quaternion q)
{
	return MagicCube::Orientation(realToQuaternionScalar(q.w), realToQuaternionScalar(q.x),
		realToQuaternionScalar(q.y), realToQuaternionScalar(q.z));
}
