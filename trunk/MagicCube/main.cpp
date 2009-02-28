/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <ctime>

#include <Ogre.h>

#ifdef	_DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#	pragma comment(lib, "OIS_d.lib")
#	pragma comment(lib, "CEGUIBase_d.lib")
#	pragma comment(lib, "OgreGUIRenderer_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#	pragma comment(lib, "OIS.lib")
#	pragma comment(lib, "CEGUIBase.lib")
#	pragma comment(lib, "OgreGUIRenderer.lib")
#endif


#if defined(DEBUG) | defined(_DEBUG)
#	include <crtdbg.h>
#endif


#define	_CONSOLE


#include "MagicCube.h"
#include "StateArchive.h"
#include "Frame.h"
#include "PlaybackPlayer.h"

#ifdef	NDEBUG
#	pragma comment(lib, "MagicCube.lib")
//#	pragma comment(lib, "MagicCube_pyd.lib")
#endif	// defined(NDEBUG)



bool testCase1()
{
	MagicCube::MagicCube cube;

	srand(std::clock());

	for(size_t i = 0; i < 1024 && cube.zeroableValid(); ++ i)
	{
		cube.doManipulation(MagicCube::Manipulation(rand() % MagicCube::Manipulation_END));
	}

	for(size_t i = 0; i < 20; ++ i)
	{
		std::cout << i << ":\t" << cube.getUnitOrientationStateIndex(i) << std::endl;
	}

	MagicCube::StateArchive sa(cube);
	std::cout << "Serialization code:\t\t" << sa.getSerializationCode() << std::endl;
	std::cout << "Serialization code of Zero:\t" << MagicCube::StateArchive(MagicCube::MagicCube::ZERO).getSerializationCode() << std::endl;

	MagicCube::MagicCube cube2(sa);
	assert(cube.logicEqual(cube2));

	MagicCube::StateArchive sa2(MagicCube::MagicCube(sa.getSerializationCode()));
	assert(sa.getSerializationCode() == sa2.getSerializationCode());

	MagicCube::PlaybackPlayer pp(cube);
	pp.save("test.txt");

	return cube.zeroableValid();
}

bool testCase2()
{
	MagicCube::MagicCube cube;

	cube.doManipulation(MagicCube::Rotate_X180);
	cube.doManipulation(MagicCube::Rotate_nX180);
	cube.doManipulation(MagicCube::Rotate_Z180);
	cube.doManipulation(MagicCube::Rotate_nZ180);
	cube.doManipulation(MagicCube::Rotate_X180);
	cube.doManipulation(MagicCube::Rotate_nX180);
	cube.doManipulation(MagicCube::Rotate_Z180);
	cube.doManipulation(MagicCube::Rotate_nZ180);

	return cube.zeroableValid() && (cube == MagicCube::MagicCube());
}

bool testCase3()
{
	bool result = true;

	for(size_t i = 0; i < 20 && result; ++ i)
	{
		MagicCube::Manipulation m[10] =
		{
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
			MagicCube::Manipulation(rand() % MagicCube::Manipulation_END),
		};

		MagicCube::MagicCube cube;

		for(size_t j = 0; j < 64; ++ j)
			cube.doManipulation(MagicCube::Manipulation(rand() % MagicCube::Manipulation_END));

		MagicCube::MagicCube cube2(cube);

		for(size_t j = 0; j < 10; ++ j)
			cube.doManipulation(m[j]);

		for(size_t j = 0; j < 10; ++ j)
			cube.undoManipulation(m[9 - j]);

		result &= (cube.zeroableValid() && (cube.logicEqual(cube2)));
	}

	return result;
}

bool testCase4()
{
	for(size_t i = 0; i < 20; ++ i)
	{
		assert(MagicCube::MagicCube::ZERO.getUnitOrientationStateIndex(i) == 0);
	}

	return true;
}


#ifdef	_CONSOLE
int main(int argc, char **argv)
#else
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#endif
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	/*Ogre::Quaternion x_0(Ogre::Radian(0),		Ogre::Vector3(1, 0, 0));
	Ogre::Quaternion x_1(Ogre::Degree(90),		Ogre::Vector3(1, 0, 0));
	Ogre::Quaternion x_n1(Ogre::Degree(-90),	Ogre::Vector3(1, 0, 0));
	Ogre::Quaternion y_0(Ogre::Radian(0),		Ogre::Vector3(0, 1, 0));
	Ogre::Quaternion y_1(Ogre::Degree(90),		Ogre::Vector3(0, 1, 0));
	Ogre::Quaternion y_n1(Ogre::Degree(-90),	Ogre::Vector3(0, 1, 0));
	Ogre::Quaternion z_0(Ogre::Radian(0),		Ogre::Vector3(0, 0, 1));
	Ogre::Quaternion z_1(Ogre::Degree(90),		Ogre::Vector3(0, 0, 1));
	Ogre::Quaternion z_n1(Ogre::Degree(-90),	Ogre::Vector3(0, 0, 1));
	Ogre::Quaternion x_2(Ogre::Degree(180),		Ogre::Vector3(1, 0, 0));
	Ogre::Quaternion y_2(Ogre::Degree(180),		Ogre::Vector3(0, 1, 0));
	Ogre::Quaternion z_2(Ogre::Degree(180),		Ogre::Vector3(0, 0, 1));
	Ogre::Quaternion x_1_y_1(x_1 * y_1);
	Ogre::Quaternion norm_x_1_y_1(x_1_y_1);
	norm_x_1_y_1.normalise();
	x_2.normalise();
	Ogre::Quaternion x_3(Ogre::Degree(270),		Ogre::Vector3(1, 0, 0));
	Ogre::Quaternion x_4(Ogre::Degree(360),		Ogre::Vector3(1, 0, 0));
	x_3.normalise();
	bool b = (x_1 == x_3);

	Ogre::Quaternion s24[] =
	{
		x_0,
		y_1,
		y_2,
		y_n1,

		z_1,
		z_1 * y_1,
		z_1 * y_2,
		z_1 * y_n1,

		z_n1,
		z_n1 * y_1,
		z_n1 * y_2,
		z_n1 * y_n1,

		x_1,
		x_1 * y_1,
		x_1 * y_2,
		x_1 * y_n1,

		x_n1,
		x_n1 * y_1,
		x_n1 * y_2,
		x_n1 * y_n1,

		x_2,
		x_2 * y_1,
		x_2 * y_2,
		x_2 * y_n1,
	};

	for(size_t i = 0; i < 24; ++ i)
	{
		std::cout << (i / 4) << ','<< (i % 4) << ":\t";

		for(size_t j = 0; j < 4; ++ j)
		{
			std::string o = "other";
			if(s24[i].ptr()[j] > -0.01f && s24[i].ptr()[j] < 0.01f)
				o = "0";
			else if(s24[i].ptr()[j] > 0.49f && s24[i].ptr()[j] < 0.51f)
				o = "1/2";
			else if(s24[i].ptr()[j] > 0.7f && s24[i].ptr()[j] < 0.71f)
				o = "1/^";
			else if(s24[i].ptr()[j] > 0.99f && s24[i].ptr()[j] < 1.01f)
				o = "1";
			else if(s24[i].ptr()[j] > -0.51f && s24[i].ptr()[j] < -0.49f)
				o = "-1/2";					  					 
			else if(s24[i].ptr()[j] > -0.71f && s24[i].ptr()[j] < -0.7f )
				o = "-1/^";					  					 
			else if(s24[i].ptr()[j] > -1.01f && s24[i].ptr()[j] < -0.99f)
				o = "-1";

			std::cout << o << '\t';
		}

		std::cout << std::endl;
	}*/

#ifdef	_DEBUG
	assert(testCase1());
	std::cout << "test case 1 passed." << std::endl;
	assert(testCase2());
	std::cout << "test case 2 passed." << std::endl;
	assert(testCase3());
	std::cout << "test case 3 passed." << std::endl;
	assert(testCase4());
	std::cout << "test case 4 passed." << std::endl;

#define	ASSERT_FACE_MANIPULATION_CAST_CORRECT(m) \
	assert(MagicCube::m == MagicCube::faceToManipulation(manipulationToFace(MagicCube::m), dimensionOfManipulation(MagicCube::m)));
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_X90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Xn90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_X180)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nX90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nXn90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nX180)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Y90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Yn90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Y180)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nY90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nYn90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nY180)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Z90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Zn90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_Z180)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nZ90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nZn90)
	ASSERT_FACE_MANIPULATION_CAST_CORRECT(Rotate_nZ180)
#endif

	// Create application object
	Frame frame;

	try
	{
		frame.go();
	}
	catch( Exception& e )
	{
		MessageBox( NULL, e.getFullDescription().c_str(), "An OGRE exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
	catch(const std::exception& e)
	{
		MessageBox( NULL, e.what(), "An standard exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}


	return 0;
}
