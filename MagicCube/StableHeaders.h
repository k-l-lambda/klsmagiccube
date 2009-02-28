/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

// StableHeaders.h : included by for 'PrecompiledHeaders.cpp'
//

#pragma once



// TODO: 在此处引用程序要求的附加头文件

#include <boost\lexical_cast.hpp>
#include <boost\shared_ptr.hpp>

#pragma warning(disable: 4180)
#include <boost\bind.hpp>
#pragma warning(default: 4180)

#include <Ogre.h>
//#include <asm_math.h>
namespace Ogre
{
	static const float pi = 4.0 * atan( 1.0 );
	static const float half_pi = 0.5 * pi;
}
#include <CEGUI\CEGUISystem.h>
#include <CEGUI\CEGUILogger.h>
#include <CEGUI\CEGUISchemeManager.h>
#include <CEGUI\CEGUIWindowManager.h>
#include <CEGUI\CEGUIWindow.h>
#include <CEGUI\elements\CEGUIPushButton.h>
#include <CEGUI\elements\CEGUICheckbox.h>
