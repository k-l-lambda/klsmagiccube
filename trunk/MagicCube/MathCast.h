/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__MATHCAST_H__
#define	__MATHCAST_H__



#include "MagicCube.h"
#include <Ogre.h>


// MagicCube to Ogre
MAGICCUBE_API	Ogre::Real			positionScalarToReal(MagicCube::PositionScalar ps);
MAGICCUBE_API	Ogre::Real			quaternionScalarToReal(MagicCube::QuaternionScalar qs);
MAGICCUBE_API	Ogre::Vector3		positionToVector3(MagicCube::Position p);
MAGICCUBE_API	Ogre::Quaternion	orientationToQuaternion(MagicCube::Orientation o);
MAGICCUBE_API	Ogre::Vector3		faceToVector3(MagicCube::Face face);

// Ogre to MagicCube
MAGICCUBE_API	MagicCube::PositionScalar		realToPositionScalar(Ogre::Real r);
MAGICCUBE_API	MagicCube::QuaternionScalar		realToQuaternionScalar(Ogre::Real r);
MAGICCUBE_API	MagicCube::Position				vector3ToPosition(Ogre::Vector3 v);
MAGICCUBE_API	MagicCube::Orientation			quaternionToOrientation(Ogre::Quaternion q);



#endif	// !defined(__MATHCAST_H__)
