/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <stdexcept>
#include "Manipulation.h"
#include "MagicCube.h"
#include "MathCast.h"

#ifdef	_DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#endif


namespace MagicCube
{
	static const Ogre::Quaternion	s_Rotates[] =
	{
		Ogre::Quaternion::IDENTITY,											// Manipulation_Identity,
		Ogre::Quaternion(Ogre::Degree(90),		Ogre::Vector3::UNIT_X),		// Rotate_X90,
		Ogre::Quaternion(Ogre::Degree(-90),		Ogre::Vector3::UNIT_X),		// Rotate_Xn90,
		Ogre::Quaternion(Ogre::Degree(180),		Ogre::Vector3::UNIT_X),		// Rotate_X180,
		Ogre::Quaternion(Ogre::Degree(90),		-Ogre::Vector3::UNIT_X),	// Rotate_nX90,
		Ogre::Quaternion(Ogre::Degree(-90),		-Ogre::Vector3::UNIT_X),	// Rotate_nXn90,
		Ogre::Quaternion(Ogre::Degree(180),		-Ogre::Vector3::UNIT_X),	// Rotate_nX180,
		Ogre::Quaternion(Ogre::Degree(90),		Ogre::Vector3::UNIT_Y),		// Rotate_Y90,
		Ogre::Quaternion(Ogre::Degree(-90),		Ogre::Vector3::UNIT_Y),		// Rotate_Yn90,
		Ogre::Quaternion(Ogre::Degree(180),		Ogre::Vector3::UNIT_Y),		// Rotate_Y180,
		Ogre::Quaternion(Ogre::Degree(90),		-Ogre::Vector3::UNIT_Y),	// Rotate_nY90,
		Ogre::Quaternion(Ogre::Degree(-90),		-Ogre::Vector3::UNIT_Y),	// Rotate_nYn90,
		Ogre::Quaternion(Ogre::Degree(180),		-Ogre::Vector3::UNIT_Y),	// Rotate_nY180,
		Ogre::Quaternion(Ogre::Degree(90),		Ogre::Vector3::UNIT_Z),		// Rotate_Z90,
		Ogre::Quaternion(Ogre::Degree(-90),		Ogre::Vector3::UNIT_Z),		// Rotate_Zn90,
		Ogre::Quaternion(Ogre::Degree(180),		Ogre::Vector3::UNIT_Z),		// Rotate_Z180,
		Ogre::Quaternion(Ogre::Degree(90),		-Ogre::Vector3::UNIT_Z),	// Rotate_nZ90,
		Ogre::Quaternion(Ogre::Degree(-90),		-Ogre::Vector3::UNIT_Z),	// Rotate_nZn90,
		Ogre::Quaternion(Ogre::Degree(180),		-Ogre::Vector3::UNIT_Z),	// Rotate_nZ180,
	};


	const Orientation	Orientation::VALID_VALUE[] =
	{
		quaternionToOrientation(s_Rotates[Manipulation_Identity]),
		quaternionToOrientation(s_Rotates[Rotate_Y90]),
		quaternionToOrientation(s_Rotates[Rotate_Y180]),
		quaternionToOrientation(s_Rotates[Rotate_Yn90]),

		quaternionToOrientation(s_Rotates[Rotate_X180]),
		quaternionToOrientation(s_Rotates[Rotate_X180] * s_Rotates[Rotate_Y90]),
		quaternionToOrientation(s_Rotates[Rotate_X180] * s_Rotates[Rotate_Y180]),
		quaternionToOrientation(s_Rotates[Rotate_X180] * s_Rotates[Rotate_Yn90]),

		quaternionToOrientation(s_Rotates[Rotate_Z90]),
		quaternionToOrientation(s_Rotates[Rotate_Z90] * s_Rotates[Rotate_Y90]),
		quaternionToOrientation(s_Rotates[Rotate_Z90] * s_Rotates[Rotate_Y180]),
		quaternionToOrientation(s_Rotates[Rotate_Z90] * s_Rotates[Rotate_Yn90]),

		quaternionToOrientation(s_Rotates[Rotate_Zn90]),
		quaternionToOrientation(s_Rotates[Rotate_Zn90] * s_Rotates[Rotate_Y90]),
		quaternionToOrientation(s_Rotates[Rotate_Zn90] * s_Rotates[Rotate_Y180]),
		quaternionToOrientation(s_Rotates[Rotate_Zn90] * s_Rotates[Rotate_Yn90]),

		quaternionToOrientation(s_Rotates[Rotate_X90]),
		quaternionToOrientation(s_Rotates[Rotate_X90] * s_Rotates[Rotate_Y90]),
		quaternionToOrientation(s_Rotates[Rotate_X90] * s_Rotates[Rotate_Y180]),
		quaternionToOrientation(s_Rotates[Rotate_X90] * s_Rotates[Rotate_Yn90]),

		quaternionToOrientation(s_Rotates[Rotate_Xn90]),
		quaternionToOrientation(s_Rotates[Rotate_Xn90] * s_Rotates[Rotate_Y90]),
		quaternionToOrientation(s_Rotates[Rotate_Xn90] * s_Rotates[Rotate_Y180]),
		quaternionToOrientation(s_Rotates[Rotate_Xn90] * s_Rotates[Rotate_Yn90]),
	};


	Orientation operator * (const Orientation& o1, const Orientation& o2)
	{
		return quaternionToOrientation(orientationToQuaternion(o1) * orientationToQuaternion(o2));
	}

	Position operator * (const Position& p, const Orientation& o)
	{
		Ogre::Matrix3 rotate;
		Ogre::Quaternion q = orientationToQuaternion(o).Inverse();
		q.ToRotationMatrix(rotate);

		Ogre::Vector3 position = positionToVector3(p) * rotate;

		return vector3ToPosition(position);
	}

	Face operator * (Face f, const Orientation& o)
	{
		return positionToFace(faceToPosition(f) * o);
	}


	void manipulation(UnitCube& uc, Manipulation m)
	{
		if(!isValidManipulation(m))
			throw std::logic_error("manipulation: invalid m");

		Ogre::Matrix3 rotate;
		s_Rotates[m].ToRotationMatrix(rotate);

		Ogre::Vector3 position = positionToVector3(uc.position) * rotate;
		Ogre::Quaternion orientation = s_Rotates[m].Inverse() * orientationToQuaternion(uc.orientation);
		orientation.normalise();
		if(orientation.w < 0)
			orientation = orientation * (-1);

		uc.position = vector3ToPosition(position);
		uc.orientation = quaternionToOrientation(orientation);
	}
}
