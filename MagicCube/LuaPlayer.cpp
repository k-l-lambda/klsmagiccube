/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include <iostream>
#include <fstream>

extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}
#ifdef	_DEBUG
#	pragma comment(lib, "Lua_Debug.lib")
#else
#	pragma comment(lib, "Lua_Release.lib")
#endif
#pragma warning(disable: 4996)
#pragma warning(disable: 4819)
#pragma warning(disable: 4267)
#include <luabind\luabind.hpp>
#include <luabind\operator.hpp>
#include <luabind\adopt_policy.hpp>
#pragma warning(default: 4267)
#pragma warning(default: 4819)
#pragma warning(default: 4996)
#ifdef	_DEBUG
#	pragma comment(lib, "luabind_Debug.lib")
#else
#	pragma comment(lib, "luabind_Release.lib")
#endif


#include "LuaPlayer.h"
#include "StateArchive.h"


namespace MagicCube
{
	namespace LuaSymbols
	{
		template<typename T>
		const T&	cref(const T& v)
		{
			return v;
		}

		Face	mul(Face f, const Orientation& o)
		{
			return f * o;
		}
	}

	extern "C" __declspec(dllexport) int registerLuaSymbols(lua_State* L)
	{
		using namespace LuaSymbols;
		using namespace luabind;

		open(L);

		module(L)
		[
			class_<PositionScalar>("PositionScalar")
				.enum_("constants")
				[
					value("Negative",	Negative),
					value("Zero",		Zero),
					value("Positive",	Positive)
				]
			,
			class_<QuaternionScalar>("QuaternionScalar")
				.enum_("constants")
				[
					value("_0",			QS_0),
					value("_Half",		QS_Half),
					value("_hr2",		QS_hr2),
					value("_1",			QS_1),
					value("_n_Half",	QS_n_Half),
					value("_n_hr2",		QS_n_hr2),
					value("_n_1",		QS_n_1)
				]
			,
			class_<Face>("Face")
				.enum_("constants")
				[
					value("pX",		Face_pX),
					value("nX",		Face_nX),
					value("pY",		Face_pY),
					value("nY",		Face_nY),
					value("pZ",		Face_pZ),
					value("nZ",		Face_nZ)
				]
				.def(self * other<const Orientation&>())
			,
			class_<Manipulation>("Manipulation")
				.enum_("constants")
				[
					value("X90",		Rotate_X90),
					value("Xn90",		Rotate_Xn90),
					value("X180",		Rotate_X180),
					value("nX90",		Rotate_nX90),
					value("nXn90",		Rotate_nXn90),
					value("nX180",		Rotate_nX180),
					value("Y90",		Rotate_Y90),
					value("Yn90",		Rotate_Yn90),
					value("Y180",		Rotate_Y180),
					value("nY90",		Rotate_nY90),
					value("nYn90",		Rotate_nYn90),
					value("nY180",		Rotate_nY180),
					value("Z90",		Rotate_Z90),
					value("Zn90",		Rotate_Zn90),
					value("Z180",		Rotate_Z180),
					value("nZ90",		Rotate_nZ90),
					value("nZn90",		Rotate_nZn90),
					value("nZ180",		Rotate_nZ180)
				]
			,
			class_<Position>("Position")
				.def(constructor<>())
				.def(constructor<const Position&>())
				.def(constructor<PositionScalar, PositionScalar, PositionScalar>())
				.def(const_self == other<const Position&>())
				.def(const_self * other<const Orientation&>())
				.def("valid",							&Position::valid)
				.def_readwrite("x",						&Position::x)
				.def_readwrite("y",						&Position::y)
				.def_readwrite("z",						&Position::z)
			,
			class_<Orientation>("Orientation")
				.def(constructor<>())
				.def(constructor<const Orientation&>())
				.def(constructor<QuaternionScalar, QuaternionScalar, QuaternionScalar, QuaternionScalar>())
				.def(const_self == other<const Orientation&>())
				.def(const_self * other<const Orientation&>())
				.def("valid",							&Orientation::valid)
				.def("validId",							&Orientation::validId)
				.def_readwrite("w",						&Orientation::w)
				.def_readwrite("x",						&Orientation::x)
				.def_readwrite("y",						&Orientation::y)
				.def_readwrite("z",						&Orientation::z)
			,
			class_<UnitCube>("UnitCube")
				.def(constructor<>())
				.def(constructor<const UnitCube&>())
				.def(constructor<Position, Orientation>())
				.def(const_self == other<const UnitCube&>())
				.def("valid",							&UnitCube::valid)
				.def_readwrite("position",				&UnitCube::position)
				.def_readwrite("orientation",			&UnitCube::orientation)
			,
			class_<MagicCube>("MagicCube")
				.def(constructor<>())
				.def(constructor<const MagicCube&>())
				.def(constructor<const StateArchive&>())
				.def("reset",							&MagicCube::reset)
				.def("setStateArchives",				&MagicCube::setStateArchives)
				.def(const_self == other<const MagicCube&>())
				.def("logicEqual",						&MagicCube::logicEqual)
				.def("physicalValid",					&MagicCube::physicalValid)
				.def("inoutValid",						&MagicCube::inoutValid)
				.def("zeroableValid",					&MagicCube::zeroableValid)
				//.def("connectiveWith",					&MagicCube::connectiveWith)
				.def("doManipulation",					&MagicCube::doManipulation)
				.def("undoManipulation",				&MagicCube::undoManipulation)
				.def("manipulationBackOff",				&MagicCube::manipulationBackOff)
				.def("manipulationAdvance",				&MagicCube::manipulationAdvance)
				.def("getUnit",							(const UnitCube& (MagicCube::*)(size_t) const)&MagicCube::getUnit)
				.def("getUnit",							(const UnitCube& (MagicCube::*)(const Position&) const)&MagicCube::getUnit)
				.def("getUnitIndex",					&MagicCube::getUnitIndex)
				.def("isOuterFace",						(bool (*)(const Position&, Face))&MagicCube::isOuterFace)
				.def("isOuterFace",						(bool (MagicCube::*)(size_t, Face) const)&MagicCube::isOuterFace)
				.def("getUnitPositionStateIndex",		&MagicCube::getUnitPositionStateIndex)
				.def("getUnitOrientationStateIndex",	&MagicCube::getUnitOrientationStateIndex)
				.def("getHistoryManipulation",			&MagicCube::getHistoryManipulation)
				.enum_("constants")
				[
					value("s_ActiveCubeCount",	MagicCube::s_ActiveCubeCount)
				]
			,
			class_<StateArchive>("StateArchive")
				.scope
				[
					class_<StateArchive::UnitState>("UnitState")
						.def_readwrite("PositionIndex",		&StateArchive::UnitState::PositionIndex)
						.def_readwrite("OrientationIndex",	&StateArchive::UnitState::OrientationIndex)
				]
				.def(constructor<const MagicCube&>())
				.def(constructor<const std::string&>())
				.def("getSerializationCode",	&StateArchive::getSerializationCode)
				.def("getCornerState",			&StateArchive::getCornerState)
				.def("getEdgeState",			&StateArchive::getEdgeState)
				.def("valid",					&StateArchive::valid)
			,
			def("faceToPosition",	&faceToPosition)
			,
			def("isValidManipulation",	&isValidManipulation)
			,
			def("manipulationToFace",	&manipulationToFace)
			,
			def("faceToManipulation",	&faceToManipulation)
			,
			def("cref",	&cref<MagicCube>)
			,
			def("cref",	&cref<Manipulation>)
			,
			def("mul",	&mul)
		];

		globals(L)["CUBE0"] = boost::cref(MagicCube::ZERO);
		globals(L)["MagicCube"]["ZERO"] = boost::cref(MagicCube::ZERO);

		globals(L)["L"]		= Rotate_X90;
		globals(L)["iL"]	= Rotate_Xn90;
		globals(L)["L2"]	= Rotate_X180;
		globals(L)["R"]		= Rotate_nX90;
		globals(L)["iR"]	= Rotate_nXn90;
		globals(L)["R2"]	= Rotate_nX180;
		globals(L)["U"]		= Rotate_Y90;
		globals(L)["iU"]	= Rotate_Yn90;
		globals(L)["U2"]	= Rotate_Y180;
		globals(L)["D"]		= Rotate_nY90;
		globals(L)["iD"]	= Rotate_nYn90;
		globals(L)["D2"]	= Rotate_nY180;
		globals(L)["F"]		= Rotate_Z90;
		globals(L)["iF"]	= Rotate_Zn90;
		globals(L)["F2"]	= Rotate_Z180;
		globals(L)["B"]		= Rotate_nZ90;
		globals(L)["iB"]	= Rotate_nZn90;
		globals(L)["B2"]	= Rotate_nZ180;

		return 0;
	}


	LuaPlayer::LuaPlayer(MagicCube& mc)
		: Player(mc)
		, m_LuaState(NULL)
	{
	}

	LuaPlayer::~LuaPlayer()
	{
		clearEngine();

		if(m_LuaState)
			lua_close(m_LuaState);
	}

	void LuaPlayer::load(const std::string& filename)
	{
		if(m_LuaState)
			lua_close(m_LuaState);
		m_LuaState = lua_open();

		luaopen_base(m_LuaState);
		luaopen_math(m_LuaState);
		luaopen_string(m_LuaState);
		luaopen_table(m_LuaState);

		registerLuaSymbols(m_LuaState);

		if(luaL_loadfile(m_LuaState, filename.data()) || lua_pcall(m_LuaState, 0, 0, 0))
		{
			throw std::logic_error("parse lua script failed");
		}

		try
		{
			luabind::call_function<int>(m_LuaState, "initialize", boost::cref(m_MagicCube));
		}
		catch(const luabind::error& e)
		{
			throw std::logic_error(std::string("luabind error: ") + e.what());
		}
		catch(const luabind::cast_failed& e)
		{
			throw std::logic_error(std::string("luabind cast_failed: ") + e.what());
		}

		reset();
		setStepResult(Manipulation_Identity);
	}

	void LuaPlayer::doNext()
	{
		Manipulation m = Manipulation_Identity;
		try
		{
			m = luabind::call_function<Manipulation>(m_LuaState, "step")[luabind::adopt(_1)];
		}
		catch(const luabind::error& e)
		{
			throw std::logic_error(std::string("luabind error: ") + e.what());
		}
		catch(const luabind::cast_failed& e)
		{
			throw std::logic_error(std::string("luabind cast_failed: ") + e.what());
		}

		setStepResult(m);
	}
}
