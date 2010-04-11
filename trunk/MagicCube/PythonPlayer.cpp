/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include <iostream>
#include <fstream>

#pragma warning(disable: 4819)
#pragma warning(disable: 4244)
#include <boost/python.hpp>
#pragma warning(default: 4244)
#pragma warning(default: 4819)

#include "PythonPlayer.h"
#include "StateArchive.h"

#ifdef	NDEBUG
#	pragma comment(lib, "MagicCube.lib")
#endif	// defined(NDEBUG)


namespace MagicCube
{
	namespace PythonSymbols
	{
		Face	mul(Face f, const Orientation& o)
		{
			return f * o;
		}

		std::string	StateArchive_getSerializationCode(const StateArchive* self)
		{
			return self->getSerializationCode();
		}
	}

	BOOST_PYTHON_MODULE(MagicCube)
	{
		using namespace boost::python;
		using namespace PythonSymbols;

		enum_<PositionScalar>("PositionScalar")
			.value("Negative",	Negative)
			.value("Zero",		Zero)
			.value("Positive",	Positive)
		;
		enum_<QuaternionScalar>("QuaternionScalar")
			.value("_0",		QS_0)
			.value("_Half",		QS_Half)
			.value("_hr2",		QS_hr2)
			.value("_1",		QS_1)
			.value("_n_Half",	QS_n_Half)
			.value("_n_hr2",	QS_n_hr2)
			.value("_n_1",		QS_n_1)
		;
		enum_<Face>("Face")
			.value("pX",		Face_pX)
			.value("nX",		Face_nX)
			.value("pY",		Face_pY)
			.value("nY",		Face_nY)
			.value("pZ",		Face_pZ)
			.value("nZ",		Face_nZ)
		;
		enum_<Manipulation>("Manipulation")
			.value("X90",		Rotate_X90)
			.value("Xn90",		Rotate_Xn90)
			.value("X180",		Rotate_X180)
			.value("nX90",		Rotate_nX90)
			.value("nXn90",		Rotate_nXn90)
			.value("nX180",		Rotate_nX180)
			.value("Y90",		Rotate_Y90)
			.value("Yn90",		Rotate_Yn90)
			.value("Y180",		Rotate_Y180)
			.value("nY90",		Rotate_nY90)
			.value("nYn90",		Rotate_nYn90)
			.value("nY180",		Rotate_nY180)
			.value("Z90",		Rotate_Z90)
			.value("Zn90",		Rotate_Zn90)
			.value("Z180",		Rotate_Z180)
			.value("nZ90",		Rotate_nZ90)
			.value("nZn90",		Rotate_nZn90)
			.value("nZ180",		Rotate_nZ180)
		;
		class_<Position>("Position")
			.def(init<const Position&>())
			.def(init<PositionScalar, PositionScalar, PositionScalar>())
			.def(self == Position())
			.def(self * Orientation())
			.def("valid",							&Position::valid)
			.def_readwrite("x",						&Position::x)
			.def_readwrite("y",						&Position::y)
			.def_readwrite("z",						&Position::z)
		;
		class_<Orientation>("Orientation")
			.def(init<const Orientation&>())
			.def(init<QuaternionScalar, QuaternionScalar, QuaternionScalar, QuaternionScalar>())
			.def(self == Orientation())
			.def(self * Orientation())
			.def("valid",							&Orientation::valid)
			.def("validId",							&Orientation::validId)
			.def_readwrite("w",						&Orientation::w)
			.def_readwrite("x",						&Orientation::x)
			.def_readwrite("y",						&Orientation::y)
			.def_readwrite("z",						&Orientation::z)
		;
		class_<UnitCube>("UnitCube")
			.def(init<const UnitCube&>())
			.def(init<Position, Orientation>())
			.def(self == UnitCube())
			.def("valid",							&UnitCube::valid)
			.def_readwrite("position",				&UnitCube::position)
			.def_readwrite("orientation",			&UnitCube::orientation)
		;
		class_<MagicCube>("MagicCube")
			.def(init<const MagicCube&>())
			.def(init<const StateArchive&>())
			.def("reset",							&MagicCube::reset)
			.def("setStateArchives",				&MagicCube::setStateArchives)
			.def(self == MagicCube())
			.def("logicEqual",						&MagicCube::logicEqual)
			.def("physicalValid",					&MagicCube::physicalValid)
			.def("inoutValid",						&MagicCube::inoutValid)
			.def("zeroableValid",					&MagicCube::zeroableValid)
			//.def("connectiveWith",					&MagicCube::connectiveWith)
			.def("doManipulation",					&MagicCube::doManipulation)
			.def("undoManipulation",				&MagicCube::undoManipulation)
			.def("manipulationBackOff",				&MagicCube::manipulationBackOff)
			.def("manipulationAdvance",				&MagicCube::manipulationAdvance)
			.def("getUnit",							(const UnitCube& (MagicCube::*)(size_t) const)&MagicCube::getUnit, return_internal_reference<>())
			.def("getUnit",							(const UnitCube& (MagicCube::*)(const Position&) const)&MagicCube::getUnit, return_internal_reference<>())
			.def("getUnitIndex",					&MagicCube::getUnitIndex)
			.def("isOuterFace",						(bool (*)(const Position&, Face))&MagicCube::isOuterFace)
			.def("isOuterFace",						(bool (MagicCube::*)(size_t, Face) const)&MagicCube::isOuterFace)
			.def("getUnitPositionStateIndex",		&MagicCube::getUnitPositionStateIndex)
			.def("getUnitOrientationStateIndex",	&MagicCube::getUnitOrientationStateIndex)
			.def("getHistoryManipulation",			&MagicCube::getHistoryManipulation, return_internal_reference<>())
		;
		class_<StateArchive>("StateArchive", init<const MagicCube&>())
			.def(init<const std::string&>())
			.def("getSerializationCode",	&StateArchive_getSerializationCode)
			.def("getCornerState",			&StateArchive::getCornerState, return_internal_reference<>())
			.def("getEdgeState",			&StateArchive::getEdgeState, return_internal_reference<>())
			.def("valid",					&StateArchive::valid)
		;

		def("faceToPosition",		&faceToPosition);
		def("isValidManipulation",	&isValidManipulation);
		def("manipulationToFace",	&manipulationToFace);
		def("faceToManipulation",	&faceToManipulation);
		def("mul",	&mul);

		// constants
		scope().attr("CUBE0") = boost::cref(MagicCube::ZERO);
		scope().attr("MagicCube").attr("ZERO") = boost::cref(MagicCube::ZERO);

		scope().attr("L")	= Rotate_X90;
		scope().attr("iL")	= Rotate_Xn90;
		scope().attr("L2")	= Rotate_X180;
		scope().attr("R")	= Rotate_nX90;
		scope().attr("iR")	= Rotate_nXn90;
		scope().attr("R2")	= Rotate_nX180;
		scope().attr("U")	= Rotate_Y90;
		scope().attr("iU")	= Rotate_Yn90;
		scope().attr("U2")	= Rotate_Y180;
		scope().attr("D")	= Rotate_nY90;
		scope().attr("iD")	= Rotate_nYn90;
		scope().attr("D2")	= Rotate_nY180;
		scope().attr("F")	= Rotate_Z90;
		scope().attr("iF")	= Rotate_Zn90;
		scope().attr("F2")	= Rotate_Z180;
		scope().attr("B")	= Rotate_nZ90;
		scope().attr("iB")	= Rotate_nZn90;
		scope().attr("B2")	= Rotate_nZ180;
	}

	void registerPythonSymbols(const boost::python::dict& global)
	{
		using namespace boost::python;

		if(PyImport_AppendInittab("MagicCube", initMagicCube) == -1)
			throw std::runtime_error("Failed to add MagicCube to python interpreter's builtin modules");

		exec("from MagicCube import *\n", global, global);
	}


	PythonPlayer::PythonPlayer(MagicCube& mc)
		: Player(mc)
	{
		Py_Initialize();
	}

	PythonPlayer::~PythonPlayer()
	{
		clearEngine();
	}

	void PythonPlayer::load(const std::string& filename)
	{
		m_PythonDict.reset(new boost::python::dict(boost::python::import("__main__").attr("__dict__")));

		registerPythonSymbols(*m_PythonDict);

		try
		{
			boost::python::exec_file(filename.data(), *m_PythonDict, *m_PythonDict);

			boost::python::object fnInitialize = boost::python::eval("initialize", *m_PythonDict, *m_PythonDict);
			fnInitialize(boost::cref(m_MagicCube));
		}
		catch(const boost::python::error_already_set& /*e*/)
		{
			throw std::logic_error("Python error.");
		}

		reset();
		setStepResult(Manipulation_Identity);
	}

	void PythonPlayer::doNext()
	{
		try
		{
			boost::python::object fnStep = boost::python::eval("step", *m_PythonDict, *m_PythonDict);
			Manipulation m = static_cast<Manipulation>(boost::python::extract<int>(fnStep()).operator boost::python::extract<int>::result_type());

			setStepResult(m);
		}
		catch(const boost::python::error_already_set& /*e*/)
		{
			throw std::logic_error("Python error.");
		}
	}
}
