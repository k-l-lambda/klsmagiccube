/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__FRAME_H__
#define	__FRAME_H__



#define	SKIP_CONFIG_DIALOG

#include "ExampleApplication.h"
#include "OgreCEGUIRenderer.h"
#include "MagicCube.h"
#include "Player.h"


class Frame : public ExampleApplication
{
	friend class OgreListener;


public:
	Frame();
	~Frame();

private:

	virtual void createFrameListener();


	// Just override the mandatory create scene method
	virtual void createScene(void);

private:
	void	frameStarted(const FrameEvent& evt);
	void	frameEnded(const FrameEvent& evt);

	bool	keyPressed( const OIS::KeyEvent &e );
	bool	keyReleased( const OIS::KeyEvent& /*arg*/ );

	bool	mouseMoved(const OIS::MouseEvent &arg);
	bool	mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool	mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	bool	onGuiMouseMove(const CEGUI::EventArgs& e);
	bool	onGuiShut(const CEGUI::EventArgs& e);
	bool	onGuiResetMagicCube(const CEGUI::EventArgs& e);
	bool	onGuiSaveSnapshot(const CEGUI::EventArgs& e);
	bool	onGuiLoadSnapshot(const CEGUI::EventArgs& e);
	bool	onGuiUndoManipulatation(const CEGUI::EventArgs& e);
	bool	onGuiRedoManipulatation(const CEGUI::EventArgs& e);
	bool	onGuiNextSkyBox(const CEGUI::EventArgs& e);
	bool	onGuiLoadPlayback(const CEGUI::EventArgs& e);
	bool	onGuiSavePlayback(const CEGUI::EventArgs& e);
	bool	onGuiLoadPythonScript(const CEGUI::EventArgs& e);
	bool	onGuiLoadLuaScript(const CEGUI::EventArgs& e);

	bool	onGuiPlayerStop(const CEGUI::EventArgs& e);
	bool	onGuiPlayerHome(const CEGUI::EventArgs& e);
	bool	onGuiPlayerPrev(const CEGUI::EventArgs& e);
	bool	onGuiPlayerPlayPause(const CEGUI::EventArgs& e);
	bool	onGuiPlayerNext(const CEGUI::EventArgs& e);

	bool	onGuiChangeMirrorEffect(const CEGUI::EventArgs& e);
	bool	onGuiChangeShininess(const CEGUI::EventArgs& e);

private:
	void	setupGui();

	void	setMagicCube(const MagicCube::MagicCube& mc);
	void	updateMagicCube();

	void	resetTransitionNode();

	void	highlightCube(size_t i, size_t j, size_t k, bool highlight = true);

	Vector2	getCursorPosition() const;

	Vector2	worldToViewport(const Vector3& p) const;
	CEGUI::UVector2	oisToCegui(const Vector2& p) const;

	void	manipulatationTransition(MagicCube::Face face, int dimension, bool needUpdate = true);

	void	setCubMap(const std::string& name);

	bool	playerOn() const;
	void	togglePlayPause(bool play);

	template<typename PlayerClass>
	void	loadPlayer(const std::string& directory, const std::string& title, const std::string& filter = "All Files(*)\1*\1\1");

	void	updateMirrorEffect();
	void	updateShininess();

private:
	// UI commands
	void	onResetMagicCube();
	void	onSaveSnapshot();
	void	onLoadSnapshot();
	void	onUndoManipulatation();
	void	onRedoManipulatation();
	void	onNextSkyBox();
	void	onPlayerStop();

	bool	onPlayerSynchronize(MagicCube::Manipulation m);

private:
	SceneNode*	m_nodeLight;
	SceneNode*	m_nodeLight2;
	SceneNode*	m_nodeMagicCube;
	SceneNode*	m_nodeCameraRoot;
	SceneNode*	m_nodeCamera;

	SceneNode*	m_CubeArray[3][3][3];
	MagicCube::MagicCube	m_MagicCube;

	MagicCube::Face		m_FocusFace;

	struct ManipulatationTransition
	{
		bool			on;
		MagicCube::Face	face;
		Real			angle;
		int				target;
		bool			needUpdate;

		static const Real	SPEED;

		void	reset()
		{
			on = false;
			angle = 0;
		};

		void	set(MagicCube::Face _face, int _target, bool _needUpdate = true)
		{
			on = true;
			face = _face;
			//angle = -target * Ogre::half_pi;
			target = _target;
			needUpdate = _needUpdate;

			//assert(target != 0);
		};
	}	m_ManipulatationTransition;

	boost::shared_ptr<CEGUI::Renderer>	m_GuiRenderer;
	boost::shared_ptr<CEGUI::System>	m_GuiSystem;

	CEGUI::Point		m_GuiCursorPostion;

	CEGUI::Window*		m_cDot[9];
	CEGUI::Window*		m_PlayerBar;
	CEGUI::Window*		m_PlayerSoureName;
	CEGUI::Window*		m_PlayerStep;

	bool		m_RotatingMagicCube;
	bool		m_ManipulatingMagicCube;

	Radian		m_SkyBoxAngle;

	std::string	m_SaveSnapshotFileName;

	boost::shared_ptr<MagicCube::Player>	m_Player;
	bool	m_PlayerIsPlaying;
};




#endif	// !defined(__FRAME_H__)
