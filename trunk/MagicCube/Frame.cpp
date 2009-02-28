/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <Commdlg.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include "Frame.h"
#include "MathCast.h"
#include "StateArchive.h"
#include "PlaybackPlayer.h"
#include "LuaPlayer.h"
#include "PythonPlayer.h"


//#define	DISPLAY_DOT_INDICATOR


static const Real UNIT_CUBE_SIZE = 1;

static const Real SKYBOX_ROTATE_SPEED = 0.003f;

static std::string	getCurrentDirectory()
{
	char path[MAX_PATH] = "";
	GetCurrentDirectory(MAX_PATH, path);

	return path;
}

static std::string	s_CubeMaps[] =
{
	"stevecube.jpg",
	"cubescene.jpg",
	"early_morning.jpg",
	"cubemap.jpg",
	"morning.jpg",
	"cloudy_noon.jpg",
	"evening.jpg",
	"stormy.jpg",
};

static const std::string	s_CurrentDirectory = getCurrentDirectory();


void createCube(const std::string& name, const float size = 1)
{
	MeshPtr cube = MeshManager::getSingleton().createManual(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh* faceVertex[6] =
	{
		cube->createSubMesh("face0"),
		cube->createSubMesh("face1"),
		cube->createSubMesh("face2"),
		cube->createSubMesh("face3"),
		cube->createSubMesh("face4"),
		cube->createSubMesh("face5"),
	};

	cube->sharedVertexData = new VertexData();
	VertexData* vertexData = cube->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	currOffset += VertexElement::getTypeSize(VET_FLOAT2);

	// allocate the vertex buffer
	vertexData->vertexCount = 24;
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	Real* pVertex = static_cast<Real*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	unsigned short* pFaceIndices[6];
	HardwareIndexBufferSharedPtr iFaceBuf[6];
	for(size_t i = 0; i < 6; ++ i)
	{
		faceVertex[i]->indexData->indexCount = 6;
		faceVertex[i]->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, faceVertex[i]->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		iFaceBuf[i] = faceVertex[i]->indexData->indexBuffer;
		pFaceIndices[i] = static_cast<unsigned short*>(iFaceBuf[i]->lock(HardwareBuffer::HBL_DISCARD));
	}

	const float halfsize = size / 2;
	static const float vertexBuffer[][8] =
	{	// x, y, z,							normal,		texcoord
		// right side
		{+halfsize, -halfsize, +halfsize,	1,0,0,		0,1,},
		{+halfsize, -halfsize, -halfsize,	1,0,0,		1,1,},
		{+halfsize, +halfsize, -halfsize,	1,0,0,		1,0,},
		{+halfsize, +halfsize, +halfsize,	1,0,0,		0,0,},

		// left side
		{-halfsize, -halfsize, -halfsize,	-1,0,0,		0,1,},
		{-halfsize, -halfsize, +halfsize,	-1,0,0,		1,1,},
		{-halfsize, +halfsize, +halfsize,	-1,0,0,		1,0,},
		{-halfsize, +halfsize, -halfsize,	-1,0,0,		0,0,}, 

		// up side
		{-halfsize, +halfsize, +halfsize,	0,1,0,		0,1,},
		{+halfsize, +halfsize, +halfsize,	0,1,0,		1,1,},
		{+halfsize, +halfsize, -halfsize,	0,1,0,		1,0,},
		{-halfsize, +halfsize, -halfsize,	0,1,0,		0,0,},

		// down side
		{-halfsize, -halfsize, -halfsize,	0,-1,0,		0,1,},
		{+halfsize, -halfsize, -halfsize,	0,-1,0,		1,1,},
		{+halfsize, -halfsize, +halfsize,	0,-1,0,		1,0,},
		{-halfsize, -halfsize, +halfsize,	0,-1,0,		0,0,},

		// front side
		{-halfsize, -halfsize, +halfsize,	0,0,1,		0,1,},
		{+halfsize, -halfsize, +halfsize,	0,0,1,		1,1,},
		{+halfsize, +halfsize, +halfsize,	0,0,1,		1,0,},
		{-halfsize, +halfsize, +halfsize,	0,0,1,		0,0,},

		// back side
		{+halfsize, -halfsize, -halfsize,	0,0,-1,		0,1,},
		{-halfsize, -halfsize, -halfsize,	0,0,-1,		1,1,},
		{-halfsize, +halfsize, -halfsize,	0,0,-1,		1,0,},
		{+halfsize, +halfsize, -halfsize,	0,0,-1,		0,0,},
	};

	static const unsigned short faceIndexBuffer[] =
	{
		// front
		0,1,2,
		0,2,3,

		// back
		4,5,6,
		4,6,7,

		// left
		8,9,10,
		8,10,11,

		// right
		12,13,14,
		12,14,15,

		// up
		16,17,18,
		16,18,19,

		// down
		20,21,22,
		20,22,23
	};

	memcpy(pVertex, vertexBuffer, sizeof(vertexBuffer));
	for(size_t i = 0; i < 6; ++ i)
		memcpy(pFaceIndices[i], &(faceIndexBuffer[i * 6]), sizeof(faceIndexBuffer) / 6);

	// Unlock
	vBuf->unlock();
	for(size_t i = 0; i < 6; ++ i)
	{
		iFaceBuf[i]->unlock();

		// Generate face list
		faceVertex[i]->useSharedVertices = true;
	}

	// the original code was missing this line:
	cube->_setBounds( AxisAlignedBox( Vector3(-halfsize, -halfsize, -halfsize), Vector3(halfsize, halfsize, halfsize) ), false );
	cube->_setBoundingSphereRadius(halfsize);
	// this line makes clear the mesh is loaded (avoids memory leakes)
	cube->load();
}

std::string	coordinateToString(size_t x, size_t y, size_t z)
{
	std::string result;
	result += char('0' + x);
	result += char('0' + y);
	result += char('0' + z);

	return result;
}

CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
    switch (buttonID)
    {
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
    }
}

bool	rightDeflect(const Vector2& p1, const Vector2& p2, const Vector2& p3)
{
	Vector2 sub1 = p2 - p1;
	Vector2 sub2 = p3 - p2;

	if(sub1 == Vector2::ZERO || sub2 == Vector2::ZERO)
		return false;

	float sin1 = sub1.y / sqrt(sub1.x * sub1.x + sub1.y * sub1.y);
	float cos1 = sub1.x / sqrt(sub1.x * sub1.x + sub1.y * sub1.y);
	float sin2 = sub2.y / sqrt(sub2.x * sub2.x + sub2.y * sub2.y);
	float cos2 = sub2.x / sqrt(sub2.x * sub2.x + sub2.y * sub2.y);

	return sin1 * cos2 - cos1 * sin2 > 0;
}

bool	inTriangle(const Vector2& p, const Vector2& t1, const Vector2& t2, const Vector2& t3)
{
	bool right = rightDeflect(t1, t2, t3);
	const Vector2& rt2 = right ? t2 : t3;
	const Vector2& rt3 = right ? t3 : t2;

	return rightDeflect(t1, rt2, p) && rightDeflect(rt2, rt3, p) && rightDeflect(rt3, t1, p);
}

bool	inQuadrangle(const Vector2& p, const Vector2& t1, const Vector2& t2, const Vector2& t3, const Vector2& t4)
{
	bool right1 = rightDeflect(t1, t2, t3);
	const Vector2& rt1 = right1 ? t1 : t2;
	const Vector2& rt2 = right1 ? t2 : t1;

	bool right2 = rightDeflect(rt2, t3, t4);
	const Vector2& rt3 = right2 ? t3 : t4;
	const Vector2& rt4 = right2 ? t4 : t3;

	bool right3 = rightDeflect(rt1, rt3, rt4);
	const Vector2& rrt1 = right3 ? rt1 : rt4;
	const Vector2& rrt4 = right3 ? rt4 : rt1;

	return rightDeflect(rrt1, rt2, p) && rightDeflect(rt2, rt3, p) && rightDeflect(rt3, rrt4, p)
		&& rightDeflect(rrt4, rrt1, p);
}

int	roundRadian(Real r)
{
	int i = r / half_pi;

	if(abs(r - i * half_pi) > half_pi / 2)
		i += r > i * half_pi ? 1 : -1;

	/*i %= 4;

	if(i > 2)
		i -= 4;

	if(i < -1)
		i += 4;*/

	return i;
}

bool	showFileDialog(std::string& filename, bool openorsave, const std::string& initialdir, const std::string& title, const std::string& filter = "All Files(*)\1*\1\1")
{
	char filenamebuffer[MAX_PATH] = "";
	for(size_t i = 0; i < filename.length() && i < MAX_PATH; ++ i)
		filenamebuffer[i] = filename[i];
	if(filename.length() < MAX_PATH - 1)
		filenamebuffer[filename.length()] = 0;

	// '\0' may be treated as tail in std::string, use '\1' instead. Now replace '\1' by '\0'.
	struct Equal1
	{
		bool operator () (char v) const
		{
			return v == '\1';
		};
	};
	std::vector<char> filterbuffer(filter.begin(), filter.end());
	std::replace_if(filterbuffer.begin(), filterbuffer.end(), Equal1(), '\0');

	OPENFILENAME of =
	{
		sizeof(OPENFILENAME),					// DWORD        lStructSize;
		NULL,									// HWND         hwndOwner;
		GetModuleHandle(NULL),					// HINSTANCE    hInstance;
		&(filterbuffer.front()),				// LPCWSTR      lpstrFilter;
		NULL,									// LPWSTR       lpstrCustomFilter;
		0,										// DWORD        nMaxCustFilter;
		0,										// DWORD        nFilterIndex;
		filenamebuffer,							// LPWSTR       lpstrFile;
		MAX_PATH,								// DWORD        nMaxFile;
		NULL,									// LPWSTR       lpstrFileTitle;
		0,										// DWORD        nMaxFileTitle;
		initialdir.data(),						// LPCWSTR      lpstrInitialDir;
		title.data(),							// LPCWSTR      lpstrTitle;
		openorsave ? OFN_FILEMUSTEXIST : 0,		// DWORD        Flags;
		0,										// WORD         nFileOffset;
		0,										// WORD         nFileExtension;
		NULL,									// LPCWSTR      lpstrDefExt;
		0,										// LPARAM       lCustData;
		0,										// LPOFNHOOKPROC lpfnHook;
		NULL,									// LPCWSTR      lpTemplateName;
	};

	bool result = (openorsave ? GetOpenFileName(&of) : GetSaveFileName(&of)) != FALSE;
	if(result)
		filename = filenamebuffer;

	return result;
}


// Listener class for frame updates
class OgreListener
	: public ExampleFrameListener
	, public OIS::KeyListener
	, public OIS::MouseListener
{
	friend class Frame;


public:
	OgreListener(Frame* frame, RenderWindow* win, Camera* cam)
		: ExampleFrameListener(win, cam, true, true)
		, m_Frame(frame)
		, m_Exit(false)
	{
		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);
		const_cast<OIS::MouseState&>(mMouse->getMouseState()).X.abs = mMouse->getMouseState().width / 2;
		const_cast<OIS::MouseState&>(mMouse->getMouseState()).Y.abs = mMouse->getMouseState().height / 2;

		mDebugOverlay->getChild("Core/LogoPanel")->hide();
#ifndef	_DEBUG
		mDebugOverlay->hide();
#endif	// !defined(_DEBUG)
	};

	~OgreListener()
	{
	};

	Vector2 getCursorPosition() const
	{
		if(mMouse)
		{
			const OIS::MouseState& mstate = mMouse->getMouseState();
			return Vector2(mstate.X.abs, mstate.Y.abs);
		}

		return Vector2::ZERO;
	};

	unsigned int getWindowWidth() const
	{
		return mMouse ? mMouse->getMouseState().width : 0;
	};

	unsigned int getWindowHeight() const
	{
		return mMouse ? mMouse->getMouseState().height : 0;
	};

	bool mouseIsMoving() const
	{
		return mMouse && (mMouse->getMouseState().X.rel || mMouse->getMouseState().Y.rel || mMouse->getMouseState().Z.rel);
	};

	void exit()
	{
		m_Exit = true;
	};

private:
	virtual bool frameStarted(const FrameEvent& evt)
	{
		if(m_Exit)
			return false;

		m_Frame->frameStarted(evt);

		//if(mKeyboard && mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		//	return false;

		// Call superclass
		return ExampleFrameListener::frameStarted(evt);
	};

	virtual bool frameEnded(const FrameEvent& evt)
	{
		m_Frame->frameEnded(evt);

		return ExampleFrameListener::frameEnded(evt);
	}

	virtual bool keyPressed(const OIS::KeyEvent& e)
	{
		if(e.key == OIS::KC_F8)
			mDebugOverlay->isVisible() ? mDebugOverlay->hide() : mDebugOverlay->show();

		return m_Frame->keyPressed(e);
	};

	virtual bool keyReleased(const OIS::KeyEvent& e)
	{
		return m_Frame->keyReleased(e);
	};


	virtual bool mouseMoved(const OIS::MouseEvent& e)
	{
		return m_Frame->mouseMoved(e);
	};

	virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		return m_Frame->mousePressed(e, id);
	};

	virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		return m_Frame->mouseReleased(e, id);
	};

private:
	Frame* m_Frame;

	bool	m_Exit;
};


const Real	Frame::ManipulatationTransition::SPEED = 4.8f;

Frame::Frame()
	: m_nodeLight(NULL)
	, m_nodeMagicCube(NULL)
	, m_FocusFace(MagicCube::Face_INVALID)
	, m_RotatingMagicCube(false)
	, m_ManipulatingMagicCube(false)
	, m_SkyBoxAngle((timeGetTime() % DWORD(pi * 2000 / SKYBOX_ROTATE_SPEED)) * SKYBOX_ROTATE_SPEED / 1000)
	//, m_CurrentCubMap(0)
	, m_PlayerBar(NULL)
	, m_PlayerIsPlaying(false)
{
	// test
	assert(inTriangle(Vector2(0, 0), Vector2(-1, -1), Vector2(1, -1), Vector2(0, 1)));
	assert(inTriangle(Vector2(0, 0), Vector2(-1, -1), Vector2(0, 1), Vector2(1, 0)));
	assert(!inTriangle(Vector2(3, 0), Vector2(-1, -1), Vector2(0, 1), Vector2(1, 0)));
	assert(!inTriangle(Vector2(0, -2), Vector2(-1, -1), Vector2(0, 1), Vector2(1, 0)));
	assert(inQuadrangle(Vector2(0, 0), Vector2(-1, -1), Vector2(1, -1), Vector2(-1, 1), Vector2(1, 1)));
	assert(inQuadrangle(Vector2(0, 0), Vector2(-1, -1), Vector2(1, 1), Vector2(-1, 1), Vector2(1, -1)));
	assert(!inQuadrangle(Vector2(2, 0), Vector2(-1, -1), Vector2(1, 1), Vector2(-1, 1), Vector2(1, -1)));

	m_ManipulatationTransition.reset();
	m_ManipulatationTransition.face = MagicCube::Face_nX;
}

Frame::~Frame()
{
	m_CubeCode = MagicCube::StateArchive(m_MagicCube).getSerializationCode();
}

void Frame::createFrameListener()
{
	// This is where we instantiate our own frame listener
	mFrameListener= new OgreListener(this, mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);

}

void Frame::createScene()
{
	{
#pragma message("Update version info: file:"__FILE__" date:"__DATE__" time:"__TIME__)
#define	DO_TO_STRING(s)	#s
#define	TO_STRING(s)	DO_TO_STRING(s)
		static const char* const VERSIONINFO = "[VERSION] MagicCube version: File:"TO_STRING(__FILE__)
			", Date:"TO_STRING(__DATE__)", Time:"TO_STRING(__TIME__);
#undef	TO_STRING
#undef	DO_TO_STRING
		LogManager::getSingleton().logMessage(VERSIONINFO, LML_CRITICAL);
	}

	m_nodeCameraRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	// Create 2 point lights
	Light* l1 = mSceneMgr->createLight("MainLight");
	l1->setAttenuation(100000, 0.4f, 0, 0);
	//l1->setSpecularColour(0.4f, 0.4f, 0.4f);
	m_nodeLight = m_nodeCameraRoot->createChildSceneNode();
	m_nodeLight->createChildSceneNode(Vector3(40, 20, 100))->attachObject(l1);

	Light* l2 = mSceneMgr->createLight("AssistLight");
	l2->setDiffuseColour(0.2f, 0.2f, 0.2f);
	//l2->setSpecularColour(0.9f, 0.9f, 0.9f);
	m_nodeLight2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	m_nodeLight2->createChildSceneNode(Vector3(-40, 60, 30))->attachObject(l2);

	mSceneMgr->setAmbientLight(ColourValue(0.6f, 0.6f, 0.6f));

	createCube("Cube", UNIT_CUBE_SIZE * 0.93f);
	m_nodeMagicCube = m_nodeCameraRoot->createChildSceneNode();

	for(size_t i = 0; i < 3; ++ i)
		for(size_t j = 0; j < 3; ++ j)
			for(size_t k = 0; k < 3; ++ k)
			{
				std::string coordname = coordinateToString(i, j, k);

				MaterialPtr materials[] =
				{
					dynamic_cast<Material*>(MaterialManager::getSingleton().getByName("MagicCube/Red").get())->clone("MagicCube/Red" + coordname),
					dynamic_cast<Material*>(MaterialManager::getSingleton().getByName("MagicCube/Cyan").get())->clone("MagicCube/Cyan" + coordname),
					dynamic_cast<Material*>(MaterialManager::getSingleton().getByName("MagicCube/Green").get())->clone("MagicCube/Green" + coordname),
					dynamic_cast<Material*>(MaterialManager::getSingleton().getByName("MagicCube/Purple").get())->clone("MagicCube/Purple" + coordname),
					dynamic_cast<Material*>(MaterialManager::getSingleton().getByName("MagicCube/Blue").get())->clone("MagicCube/Blue" + coordname),
					dynamic_cast<Material*>(MaterialManager::getSingleton().getByName("MagicCube/Yellow").get())->clone("MagicCube/Yellow" + coordname),
				};

				for(size_t f = 0; f < 6; ++ f)
				{
					if(!m_MagicCube.isOuterFace(MagicCube::Position(MagicCube::PositionScalar(i),
						MagicCube::PositionScalar(j), MagicCube::PositionScalar(k)), MagicCube::Face(f)))
					{
						materials[f]->setDiffuse(ColourValue::Black);
						materials[f]->getTechnique(0)->removePass(1);
					}
					else
					{
						//ColourValue diffuse = materials[f]->getTechnique(0)->getPass(0)->getDiffuse();
						materials[f]->setSpecular(ColourValue::White);
						materials[f]->setShininess(108);
					}
				}

				Entity *ent = mSceneMgr->createEntity("cube" + coordname, "Cube");
				ent->getSubEntity(0)->setMaterialName("MagicCube/Red" + coordname);
				ent->getSubEntity(1)->setMaterialName("MagicCube/Cyan" + coordname);
				ent->getSubEntity(2)->setMaterialName("MagicCube/Green" + coordname);
				ent->getSubEntity(3)->setMaterialName("MagicCube/Purple" + coordname);
				ent->getSubEntity(4)->setMaterialName("MagicCube/Blue" + coordname);
				ent->getSubEntity(5)->setMaterialName("MagicCube/Yellow" + coordname);

				SceneNode* nodeTransition = m_nodeMagicCube->createChildSceneNode();
				m_CubeArray[i][j][k] = nodeTransition->createChildSceneNode();
				m_CubeArray[i][j][k]->attachObject(ent);
			}

	m_nodeCameraRoot->setOrientation(Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));
	m_nodeCamera = m_nodeCameraRoot->createChildSceneNode(Vector3(3, 0, 15));
	mCamera->setPosition(m_nodeCamera->getWorldPosition());
	mCamera->lookAt(0,0,0);

	mSceneMgr->setSkyBox(true, "MagicCube/SkyBox");

	setCubMap(s_CubeMaps[m_CurrentCubMap]);

	mWindow->getViewport(0)->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));

	MagicCube::StateArchive sa(m_CubeCode);
	if(sa.valid())
		m_MagicCube.setStateArchives(sa);
	updateMagicCube();

	setupGui();

	updateMirrorEffect();
	updateShininess();
}

void Frame::frameStarted(const FrameEvent& evt)
{
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	{
		Radian delta = Radian(evt.timeSinceLastFrame * SKYBOX_ROTATE_SPEED);
		m_SkyBoxAngle += delta;

		// rotate sky box
		//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 10000, true, Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));

		// light animation
		//m_nodeLight2->yaw(delta);
		//m_nodeLight->yaw(delta);

		// rotate camera
		m_nodeCameraRoot->setOrientation(Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));
		mCamera->setPosition(m_nodeCamera->getWorldPosition());
		mCamera->lookAt(m_nodeMagicCube->getWorldPosition());
	}

	// find focus face
	if(!m_RotatingMagicCube && !m_ManipulatationTransition.on && !m_ManipulatingMagicCube)
	{
		m_FocusFace = MagicCube::Face_INVALID;
		if(!playerOn())
		{
			Vector2 cursor = getCursorPosition();
			for(size_t f = 0; f < 6; ++ f)
			{
				Vector3 faceNormal = positionToVector3(MagicCube::faceToPosition(MagicCube::Face(f)));
				Matrix4 world;
				m_nodeMagicCube->getWorldTransforms(&world);
				faceNormal = world * faceNormal;

				Vector3 cameraPos = -mCamera->getDirection();

				//std::cout << f << std::endl;
				//std::cout << "faceNormal:\t" << faceNormal.x << ",\t" << faceNormal.y << ",\t" << faceNormal.z << std::endl;
				//std::cout << "cameraPos:\t" << cameraPos.x << ",\t" << cameraPos.y << ",\t" << cameraPos.z << std::endl;

				if(cameraPos.dotProduct(faceNormal) > 0)
				{
					//std::cout << "face: " << f << std::endl;

					std::vector<MagicCube::Position> pvertices = MagicCube::faceToVertices(MagicCube::Face(f));
					Vector2 vertices2d[4] =
					{
						worldToViewport(world * (positionToVector3(pvertices[0]) * 1.5f)),
						worldToViewport(world * (positionToVector3(pvertices[1]) * 1.5f)),
						worldToViewport(world * (positionToVector3(pvertices[2]) * 1.5f)),
						worldToViewport(world * (positionToVector3(pvertices[3]) * 1.5f)),
					};

					if(inQuadrangle(cursor, vertices2d[0], vertices2d[1], vertices2d[2], vertices2d[3]))
					{
						m_FocusFace = MagicCube::Face(f);

						/*std::cout << "p:\t(" << cursor.x << "," << cursor.y << ") \t("
							<< vertices2d[0].x << "," << vertices2d[0].y << ") \t("
							<< vertices2d[1].x << "," << vertices2d[1].y << ") \t("
							<< vertices2d[2].x << "," << vertices2d[2].y << ") \t("
							<< vertices2d[3].x << "," << vertices2d[3].y << ") \t"
							<< std::endl;*/
					}
				}
			}
		}
	}

	for(size_t i = 0; i < 3; ++ i)
		for(size_t j = 0; j < 3; ++ j)
			for(size_t k = 0; k < 3; ++ k)
				highlightCube(i, j, k, false);
	if(MagicCube::isValidFace(m_FocusFace) && !m_ManipulatationTransition.on)
	{
		for(size_t i = 0; i < 3; ++ i)
			for(size_t j = 0; j < 3; ++ j)
			{
				MagicCube::Position p;

				switch(m_FocusFace)
				{
				case MagicCube::Face_pX:	p = MagicCube::Position(MagicCube::Positive, MagicCube::PositionScalar(i), MagicCube::PositionScalar(j));	break;
				case MagicCube::Face_nX:	p = MagicCube::Position(MagicCube::Negative, MagicCube::PositionScalar(i), MagicCube::PositionScalar(j));	break;
				case MagicCube::Face_pY:	p = MagicCube::Position(MagicCube::PositionScalar(i), MagicCube::Positive, MagicCube::PositionScalar(j));	break;
				case MagicCube::Face_nY:	p = MagicCube::Position(MagicCube::PositionScalar(i), MagicCube::Negative, MagicCube::PositionScalar(j));	break;
				case MagicCube::Face_pZ:	p = MagicCube::Position(MagicCube::PositionScalar(i), MagicCube::PositionScalar(j), MagicCube::Positive);	break;
				case MagicCube::Face_nZ:	p = MagicCube::Position(MagicCube::PositionScalar(i), MagicCube::PositionScalar(j), MagicCube::Negative);	break;
				}

				highlightCube(p.x, p.y, p.z);
			}
	}

	// Manipulatation transit
	{
		if(m_ManipulatationTransition.on)
		{
			Real target = m_ManipulatationTransition.target * Ogre::half_pi;

			Real delta = evt.timeSinceLastFrame * ManipulatationTransition::SPEED
				* ((target - m_ManipulatationTransition.angle < 0) ? -1 : 1);

			// remove redundant loops
			int loops = int((target - m_ManipulatationTransition.angle) / (Ogre::pi * 2));
			if(loops != 0)
				m_ManipulatationTransition.angle += loops * Ogre::pi * 2;

			if(abs(target - m_ManipulatationTransition.angle) < delta)
			{
				if(m_ManipulatationTransition.needUpdate)
				{
					MagicCube::Manipulation m = MagicCube::faceToManipulation(m_ManipulatationTransition.face, m_ManipulatationTransition.target);
					m_MagicCube.doManipulation(m);
				}

				m_ManipulatationTransition.reset();
				updateMagicCube();
			}
			else
			{
				m_ManipulatationTransition.angle += delta;
			}
		}

		if(m_ManipulatationTransition.face != MagicCube::Face_INVALID && m_ManipulatationTransition.angle != 0)
			for(MagicCube::PositionScalar i = MagicCube::Negative; i <= MagicCube::Positive; ++ i)
				for(MagicCube::PositionScalar j = MagicCube::Negative; j <= MagicCube::Positive; ++ j)
				{
					MagicCube::Position p;
					switch(m_ManipulatationTransition.face)
					{
					case MagicCube::Face_pX:	p = MagicCube::Position(MagicCube::Positive, i, j);	break;
					case MagicCube::Face_nX:	p = MagicCube::Position(MagicCube::Negative, i, j);	break;
					case MagicCube::Face_pY:	p = MagicCube::Position(i, MagicCube::Positive, j);	break;
					case MagicCube::Face_nY:	p = MagicCube::Position(i, MagicCube::Negative, j);	break;
					case MagicCube::Face_pZ:	p = MagicCube::Position(i, j, MagicCube::Positive);	break;
					case MagicCube::Face_nZ:	p = MagicCube::Position(i, j, MagicCube::Negative);	break;
					}

					SceneNode* nodeCube = m_CubeArray[p.x][p.y][p.z];
					Node* nodeTransition = nodeCube->getParent();

					//nodeTransition->rotate(faceToVector3(m_ManipulatationTransition.face), Radian(delta), Node::TS_PARENT);
					nodeTransition->setOrientation(Quaternion(Radian(-m_ManipulatationTransition.angle), faceToVector3(m_ManipulatationTransition.face)));
				}
	}

	// player
	if(playerOn() && m_PlayerIsPlaying && !m_ManipulatationTransition.on && m_Player && !m_Player->busy())
	{
		try
		{
			/*MagicCube::Manipulation m = m_Player->advance();
			if(m != MagicCube::Manipulation_Identity)
				manipulatationTransition(MagicCube::manipulationToFace(m), MagicCube::dimensionOfManipulation(m), false);*/
			m_PlayerBar->disable();
			m_Player->advance(boost::bind(&Frame::onPlayerSynchronize, this, _1));

			//if(m_Player->atBack())
			//	togglePlayPause(false);
		}
		catch(const std::logic_error& e)
		{
			MessageBox(NULL, e.what(), "Logic Exception", MB_OK | MB_ICONERROR);
			togglePlayPause(false);
		}
	}

	if(m_Player && m_PlayerBar->isDisabled())
		m_Player->tick();

	if(playerOn() && m_Player && m_PlayerStep)
		m_PlayerStep->setText(boost::lexical_cast<std::string>(m_Player->step()).data());

	// set cursor
	if(m_RotatingMagicCube || m_ManipulatingMagicCube)
		m_GuiSystem->setDefaultMouseCursor(NULL);
	else
		m_GuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");

#ifdef	DISPLAY_DOT_INDICATOR
	// draw dots on vertices
	{
		Matrix4 world;
		m_nodeMagicCube->getWorldTransforms(&world);

		std::vector<MagicCube::Position> vertices1 = MagicCube::faceToVertices(MagicCube::Face(0));
		std::vector<MagicCube::Position> vertices2 = MagicCube::faceToVertices(MagicCube::Face(1));
		Vector2 vertices2d[8] =
		{
			worldToViewport(world * (positionToVector3(vertices1[0]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices1[1]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices1[2]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices1[3]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices2[0]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices2[1]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices2[2]) * 1.5f)),
			worldToViewport(world * (positionToVector3(vertices2[3]) * 1.5f)),
		};

		for(size_t i = 0; i < 8; ++ i)
			m_cDot[i]->setPosition(oisToCegui(vertices2d[i]));

		m_cDot[8]->setPosition(oisToCegui(getCursorPosition()));
	}
#endif	// defined(DISPLAY_DOT_INDICATOR)
}

void Frame::frameEnded(const FrameEvent& evt)
{
	// yield CPU when not busy
	if(/*!m_RotatingMagicCube && !m_ManipulatingMagicCube &&*/ !m_ManipulatationTransition.on
		&& !static_cast<const OgreListener*>(mFrameListener)->mouseIsMoving())
		Sleep(15);

	// save snapshot
	if(!m_SaveSnapshotFileName.empty())
	{
		mWindow->getViewport(0)->getTarget()->writeContentsToFile(m_SaveSnapshotFileName);

		std::cout << "Save snapshot: " << m_SaveSnapshotFileName << std::endl;

		m_SaveSnapshotFileName.clear();

		CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");
		root->show();
	}
}

bool Frame::keyPressed(const OIS::KeyEvent &e)
{
	MagicCube::Manipulation m;
	switch(e.key)
	{
	case OIS::KC_1:			m = MagicCube::Rotate_X90;		break;
	case OIS::KC_2:			m = MagicCube::Rotate_Xn90;		break;
	case OIS::KC_3:			m = MagicCube::Rotate_Y90;		break;
	case OIS::KC_4:			m = MagicCube::Rotate_Yn90;		break;
	case OIS::KC_5:			m = MagicCube::Rotate_Z90;		break;
	case OIS::KC_6:			m = MagicCube::Rotate_Zn90;		break;
	case OIS::KC_7:			m = MagicCube::Rotate_nX90;		break;
	case OIS::KC_8:			m = MagicCube::Rotate_nXn90;	break;
	case OIS::KC_9:			m = MagicCube::Rotate_nY90;		break;
	case OIS::KC_0:			m = MagicCube::Rotate_nYn90;	break;
	case OIS::KC_MINUS:		m = MagicCube::Rotate_nZ90;		break;
	case OIS::KC_EQUALS:	m = MagicCube::Rotate_nZn90;	break;
	default:
		m = MagicCube::Manipulation(-1);
	}

	if(m >= 0)
	{
		MagicCube::Face face = MagicCube::manipulationToFace(m);
		manipulatationTransition(face, MagicCube::dimensionOfManipulation(m));
	}

	const OgreListener* listener = static_cast<const OgreListener*>(mFrameListener);

	switch(e.key)
	{
	case OIS::KC_F2:
		onResetMagicCube();

		break;
	case OIS::KC_SYSRQ:
		onSaveSnapshot();

		break;
	case OIS::KC_L:
		if(listener->mKeyboard->isKeyDown(OIS::KC_LCONTROL) || listener->mKeyboard->isKeyDown(OIS::KC_RCONTROL))
			onLoadSnapshot();

		break;
	case OIS::KC_TAB:
		if(listener->mKeyboard->isKeyDown(OIS::KC_LSHIFT) || listener->mKeyboard->isKeyDown(OIS::KC_RSHIFT))
			onNextSkyBox();

		break;
	case OIS::KC_ESCAPE:
		onPlayerStop();

		break;
	}

	if(!m_ManipulatationTransition.on)
	{
		if(listener->mKeyboard->isKeyDown(OIS::KC_LCONTROL) || listener->mKeyboard->isKeyDown(OIS::KC_RCONTROL))
		{
			switch(e.key)
			{
			case OIS::KC_Z:
				onUndoManipulatation();

				break;
			case OIS::KC_Y:
				onRedoManipulatation();

				break;
			}
		}
	}

	return true;
}

bool Frame::keyReleased(const OIS::KeyEvent& /*arg*/)
{
	return true;
}

bool Frame::mouseMoved(const OIS::MouseEvent& e)
{
	if(m_RotatingMagicCube)
	{
		// rotate magic cube
		{
			m_nodeMagicCube->yaw(Radian(e.state.X.rel * 0.01f), Node::TS_PARENT);
			m_nodeMagicCube->pitch(Radian(e.state.Y.rel * 0.01f), Node::TS_PARENT);
		}
	}

	/*if(e.state.buttonDown(OIS::MB_Right))
	{
		m_nodeCamera->yaw(-Radian(e.state.X.rel * 0.01f), Node::TS_WORLD);
		m_nodeCamera->pitch(-Radian(e.state.Y.rel * 0.01f), Node::TS_WORLD);
	}*/

	if(e.state.Z.rel)
	{
		if(m_FocusFace == MagicCube::Face_INVALID)
		{
			static const Real NEAR_DISTANCE = 8;
			static const Real FAR_DISTANCE = 24;

			Vector3 camera = m_nodeCamera->getPosition() - m_nodeMagicCube->getPosition();
			camera *= exp(-e.state.Z.rel / 1600.0);
			if(camera.length() < NEAR_DISTANCE)
				camera = camera.normalisedCopy() * NEAR_DISTANCE;
			else if(camera.length() > FAR_DISTANCE)
				camera = camera.normalisedCopy() * FAR_DISTANCE;

			m_nodeCamera->setPosition(m_nodeMagicCube->getPosition() + camera);
		}
		else
		{
			/*if(m_ManipulatationTransition.face != m_FocusFace)
			{
				m_ManipulatationTransition.reset();
				updateMagicCube();
			}

			m_ManipulatationTransition.set(m_FocusFace, -int(e.state.Z.rel) / 120);*/
			manipulatationTransition(m_FocusFace, -int(e.state.Z.rel) / 120);
		}
	}

	if(m_ManipulatingMagicCube)
		m_ManipulatationTransition.angle += e.state.Y.rel * 0.01f;

	CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);

	return true;
}

bool Frame::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));

	if((m_FocusFace == MagicCube::Face_INVALID && id == OIS::MB_Left) || id == OIS::MB_Middle)
	{
		m_RotatingMagicCube = true;
		m_FocusFace = MagicCube::Face_INVALID;
	}

	if(m_FocusFace != MagicCube::Face_INVALID && id == OIS::MB_Left)
	{
		m_ManipulatingMagicCube = true;
		m_ManipulatationTransition.face = m_FocusFace;
		m_FocusFace = MagicCube::Face_INVALID;
	}

	return true;
}

bool Frame::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));

	if(m_RotatingMagicCube && (id == OIS::MB_Left || id == OIS::MB_Middle))
		m_RotatingMagicCube = false;

	if(m_ManipulatingMagicCube && id == OIS::MB_Left)
	{
		m_ManipulatingMagicCube = false;

		m_ManipulatationTransition.set(m_ManipulatationTransition.face,
			roundRadian(m_ManipulatationTransition.angle));
	}

	return true;
}

bool Frame::onGuiMouseMove(const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& me = static_cast<const CEGUI::MouseEventArgs&>(e);

	m_GuiCursorPostion = me.position;

	//std::cout << "gui:\t" << m_GuiCursorPostion.d_x << ",\t" << m_GuiCursorPostion.d_y << std::endl;
	//std::cout << "ogre:\t" << getCursorPosition().x << ",\t" << getCursorPosition().y << std::endl;

	return true;
}

bool Frame::onGuiShut(const CEGUI::EventArgs&)
{
	OgreListener* listener = static_cast<OgreListener*>(mFrameListener);
	listener->exit();

	return true;
}

bool Frame::onGuiResetMagicCube(const CEGUI::EventArgs&)
{
	if(m_PlayerBar)
		m_PlayerBar->hide();

	onResetMagicCube();

	return true;
}

bool Frame::onGuiSaveSnapshot(const CEGUI::EventArgs&)
{
	onSaveSnapshot();

	return true;
}

bool Frame::onGuiLoadSnapshot(const CEGUI::EventArgs&)
{
	if(m_PlayerBar)
		m_PlayerBar->hide();

	onLoadSnapshot();

	return true;
}

bool Frame::onGuiUndoManipulatation(const CEGUI::EventArgs&)
{
	onUndoManipulatation();

	return true;
}

bool Frame::onGuiRedoManipulatation(const CEGUI::EventArgs&)
{
	onRedoManipulatation();

	return true;
}

bool Frame::onGuiNextSkyBox(const CEGUI::EventArgs&)
{
	onNextSkyBox();

	return true;
}

bool Frame::onGuiLoadPlayback(const CEGUI::EventArgs& /*e*/)
{
	loadPlayer<MagicCube::PlaybackPlayer>("\\playback\\", "Load Playback");

	return true;
}

bool Frame::onGuiSavePlayback(const CEGUI::EventArgs& e)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);

	std::string name = boost::lexical_cast<std::string>(stime.wYear) + "-"
		+ boost::lexical_cast<std::string>(stime.wMonth) + "-"
		+ boost::lexical_cast<std::string>(stime.wDay) + " "
		+ boost::lexical_cast<std::string>(stime.wHour) + "."
		+ boost::lexical_cast<std::string>(stime.wMinute) + "."
		+ boost::lexical_cast<std::string>(stime.wSecond) + ".txt";
	std::string dir = s_CurrentDirectory + "\\playback\\";

	if(showFileDialog(name, false, dir, "Save Playback"))
	{
		MagicCube::PlaybackPlayer pp(m_MagicCube);
		pp.save(name);
	}

	return true;
}

bool Frame::onGuiLoadPythonScript(const CEGUI::EventArgs&)
{
	loadPlayer<MagicCube::PythonPlayer>("\\scripts\\", "Load Python Script", "Python Files(*.py)\1*.py\1All Files(*)\1*\1\1");

	return true;
}

bool Frame::onGuiLoadLuaScript(const CEGUI::EventArgs&)
{
	loadPlayer<MagicCube::LuaPlayer>("\\scripts\\", "Load Lua Script", "Lua Files(*.lua)\1*.lua\1All Files(*)\1*\1\1");

	return true;
}

bool Frame::onGuiPlayerStop(const CEGUI::EventArgs&)
{
	onPlayerStop();

	return true;
}

bool Frame::onGuiPlayerHome(const CEGUI::EventArgs&)
{
	togglePlayPause(false);

	if(!m_ManipulatationTransition.on)
	{
		while(!m_Player->atFront())
			m_Player->backoff();

		updateMagicCube();
	}

	return true;
}

bool Frame::onGuiPlayerPrev(const CEGUI::EventArgs&)
{
	togglePlayPause(false);

	if(!m_ManipulatationTransition.on)
	{
		/*MagicCube::Manipulation m = MagicCube::reverseManipulation(m_Player->backoff());
		if(m != MagicCube::Manipulation_Identity)
			manipulatationTransition(MagicCube::manipulationToFace(m), MagicCube::dimensionOfManipulation(m), false);*/
		m_PlayerBar->disable();
		m_Player->backoff(boost::bind(&Frame::onPlayerSynchronize, this, _1));
	}

	return true;
}

bool Frame::onGuiPlayerPlayPause(const CEGUI::EventArgs&)
{
	togglePlayPause(!m_PlayerIsPlaying);

	return true;
}

bool Frame::onGuiPlayerNext(const CEGUI::EventArgs&)
{
	togglePlayPause(false);

	if(!m_ManipulatationTransition.on)
	{
		try
		{
			/*MagicCube::Manipulation m = m_Player->advance();
			if(m != MagicCube::Manipulation_Identity)
				manipulatationTransition(MagicCube::manipulationToFace(m), MagicCube::dimensionOfManipulation(m), false);*/
			m_PlayerBar->disable();
			m_Player->advance(boost::bind(&Frame::onPlayerSynchronize, this, _1));
		}
		catch(const std::logic_error& e)
		{
			MessageBox(NULL, e.what(), "Logic Exception", MB_OK | MB_ICONERROR);
			togglePlayPause(false);
		}
	}

	return true;
}

bool Frame::onGuiChangeMirrorEffect(const CEGUI::EventArgs&)
{
	m_OpenMirrorEffect = static_cast<CEGUI::Checkbox*>(CEGUI::WindowManager::getSingleton()
		.getWindow((CEGUI::utf8*)"MagicCube/MirrorEffect"))->isSelected();

	updateMirrorEffect();

	return true;
}

bool Frame::onGuiChangeShininess(const CEGUI::EventArgs&)
{
	m_OpenShininess = static_cast<CEGUI::Checkbox*>(CEGUI::WindowManager::getSingleton()
		.getWindow((CEGUI::utf8*)"MagicCube/Shininess"))->isSelected();

	updateShininess();

	return true;
}

void Frame::setupGui()
{
	m_GuiRenderer.reset(new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr));
	m_GuiSystem.reset(new CEGUI::System(m_GuiRenderer.get()));
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	m_GuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	m_GuiSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
	m_GuiSystem->setDefaultTooltip("TaharezLook/Tooltip");

	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"MagicCube.layout"); 
	m_GuiSystem->setGUISheet(sheet);

	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");
	root->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&Frame::onGuiMouseMove, this));

	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/Shut")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiShut, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/ResetMagicCube")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiResetMagicCube, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/SaveSnapshot")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiSaveSnapshot, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/LoadSnapshot")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiLoadSnapshot, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/UndoManipulatation")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiUndoManipulatation, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/RedoManipulatation")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiRedoManipulatation, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/NextSkyBox")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiNextSkyBox, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/LoadPlayback")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiLoadPlayback, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/SavePlayback")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiSavePlayback, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/LoadPythonScript")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiLoadPythonScript, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/LoadLuaScript")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiLoadLuaScript, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerStop")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiPlayerStop, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerHome")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiPlayerHome, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerPrev")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiPlayerPrev, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerPlayPause")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiPlayerPlayPause, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerNext")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiPlayerNext, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/MirrorEffect")
		->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&Frame::onGuiChangeMirrorEffect, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/Shininess")
		->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&Frame::onGuiChangeShininess, this));

	m_PlayerBar = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerBar");
	if(!m_PlayerBar)
		std::cerr << "!	Cannot find GUI control MagicCube/PlayerBar." << std::endl;
	if(m_PlayerBar)
		m_PlayerBar->hide();

	m_PlayerSoureName = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerSourceName");
	m_PlayerStep = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerStep");

	CEGUI::Checkbox* cMirrorEffect = static_cast<CEGUI::Checkbox*>(CEGUI::WindowManager::getSingleton()
		.getWindow((CEGUI::utf8*)"MagicCube/MirrorEffect"));
	CEGUI::Checkbox* cShininess = static_cast<CEGUI::Checkbox*>(CEGUI::WindowManager::getSingleton()
		.getWindow((CEGUI::utf8*)"MagicCube/Shininess"));
	cMirrorEffect->setSelected(m_OpenMirrorEffect);
	cShininess->setSelected(m_OpenShininess);

	for(size_t i = 0; i < 9; ++ i)
	{
		std::string name = "MagicCube/Dot";
		name += char('0' + i);
		m_cDot[i] = CEGUI::WindowManager::getSingleton().getWindow(name);
#ifndef	DISPLAY_DOT_INDICATOR
		m_cDot[i]->hide();
#endif	// !defined(DISPLAY_DOT_INDICATOR)
	}
}

void Frame::setMagicCube(const MagicCube::MagicCube& mc)
{
	for(MagicCube::PositionScalar i = MagicCube::Negative; i <= MagicCube::Positive; ++ i)
		for(MagicCube::PositionScalar j = MagicCube::Negative; j <= MagicCube::Positive; ++ j)
			for(MagicCube::PositionScalar k = MagicCube::Negative; k <= MagicCube::Positive; ++ k)
			{
				const MagicCube::UnitCube& cube = mc.getUnit(MagicCube::Position(i, j, k));
				Ogre::Quaternion orientation = orientationToQuaternion(cube.orientation);

				m_CubeArray[i][j][k]->setPosition((i - 1) * UNIT_CUBE_SIZE, (j - 1) * UNIT_CUBE_SIZE, (k - 1) * UNIT_CUBE_SIZE);
				m_CubeArray[i][j][k]->setOrientation(orientation);
				m_CubeArray[i][j][k]->detachObject(unsigned short(0));
			}

	for(MagicCube::PositionScalar i = MagicCube::Negative; i <= MagicCube::Positive; ++ i)
		for(MagicCube::PositionScalar j = MagicCube::Negative; j <= MagicCube::Positive; ++ j)
			for(MagicCube::PositionScalar k = MagicCube::Negative; k <= MagicCube::Positive; ++ k)
			{
				size_t index = mc.getUnitIndex(MagicCube::Position(i, j, k));
				MagicCube::Position pos = MagicCube::MagicCube::ZERO.getUnit(index).position;
				Entity* ent = mSceneMgr->getEntity("cube" + coordinateToString(pos.x, pos.y, pos.z));
				assert(ent);

				m_CubeArray[i][j][k]->attachObject(ent);
			}

	resetTransitionNode();
}

void Frame::updateMagicCube()
{
	setMagicCube(m_MagicCube);
}

void Frame::resetTransitionNode()
{
	for(size_t i = 0; i < 3; ++ i)
		for(size_t j = 0; j < 3; ++ j)
			for(size_t k = 0; k < 3; ++ k)
			{
				Node* nodeTransition = m_CubeArray[i][j][k]->getParent();
				nodeTransition->setOrientation(Quaternion::IDENTITY);
			}
}

void Frame::highlightCube(size_t i, size_t j, size_t k, bool highlight)
{
	static const ColourValue s_HighlightColor(0.4f, 0.4f, 0.4f);

	Entity* ent = dynamic_cast<Entity*>(m_CubeArray[i][j][k]->getAttachedObject(0));
	assert(ent);

	for(size_t i = 0; i < ent->getNumSubEntities(); ++ i)
		ent->getSubEntity(unsigned(i))->getMaterial()->setSelfIllumination(highlight ? s_HighlightColor : ColourValue::Black);
}

Vector2 Frame::getCursorPosition() const
{
	return static_cast<const OgreListener*>(mFrameListener)->getCursorPosition();
}

Vector2 Frame::worldToViewport(const Vector3& p) const
{
	Matrix4 view = mCamera->getViewMatrix();
	Matrix4 projection = mCamera->getProjectionMatrix();

	//Vector3 v = (view * projection) * p;
	Vector3 v = projection * view * p;

	const OgreListener* listener = static_cast<const OgreListener*>(mFrameListener);

	return Vector2((v.x + 1) * listener->getWindowWidth() / 2,
		(1 - v.y) * listener->getWindowHeight() / 2);
}

CEGUI::UVector2 Frame::oisToCegui(const Vector2& p) const
{
	const OgreListener* listener = static_cast<const OgreListener*>(mFrameListener);

	return CEGUI::UVector2(CEGUI::UDim((p.x - 5) / listener->getWindowWidth(), 0),
		CEGUI::UDim((p.y - 18) / listener->getWindowHeight(), 0));
}

void Frame::manipulatationTransition(MagicCube::Face face, int dimension, bool needUpdate)
{
	if(m_ManipulatationTransition.face != face && needUpdate)
	{
		m_ManipulatationTransition.reset();
		updateMagicCube();
	}

	m_ManipulatationTransition.set(face, dimension, needUpdate);
}

void Frame::setCubMap(const std::string& name)
{
	MaterialPtr matSkyBox = MaterialManager::getSingleton().getByName("MagicCube/SkyBox");
	matSkyBox->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setCubicTextureName(name, false);
	mSceneMgr->setSkyBox(true, "MagicCube/SkyBox");

	for(size_t i = 0; i < 3; ++ i)
		for(size_t j = 0; j < 3; ++ j)
			for(size_t k = 0; k < 3; ++ k)
			{
				std::string coordname = coordinateToString(i, j, k);

				MaterialPtr mat[] =
				{
					MaterialManager::getSingleton().getByName("MagicCube/Red" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Cyan" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Green" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Purple" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Blue" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Yellow" + coordname),
				};

				for(size_t m = 0; m < 6; ++ m)
					if(mat[m]->getTechnique(0)->getNumPasses() > 1)
						mat[m]->getTechnique(0)->getPass(1)->getTextureUnitState(0)->setCubicTextureName(name, false);
			}
}

bool Frame::playerOn() const
{
	return m_PlayerBar && m_PlayerBar->isVisible();
}

void Frame::togglePlayPause(bool play)
{
	m_PlayerIsPlaying = play;
	CEGUI::Window* bnPlay = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"MagicCube/PlayerPlayPause");
	bnPlay->setText(m_PlayerIsPlaying ? "||" : ">");
}

template<typename PlayerClass>
void Frame::loadPlayer(const std::string& directory, const std::string& title, const std::string& filter)
{
	std::string name;
	std::string dir = s_CurrentDirectory + directory;

	if(showFileDialog(name, false, dir, title, filter))
	{
		m_Player.reset(new PlayerClass(m_MagicCube));
		try
		{
			m_Player->load(name);

			if(m_Player->ready())
			{
				updateMagicCube();
				if(m_PlayerBar)
				{
					m_PlayerBar->show();

					size_t filenamep = name.find_last_of('\\');
					if(filenamep != std::string::npos)
						++ filenamep;
					else
						filenamep = 0;
					m_PlayerSoureName->setText(name.substr(filenamep).data());

					togglePlayPause(true);
				}
			}
			else
			{
				std::cout << "Frame::onGuiLoadPlayback: load playback file \"" << name << "\" failed." << std::endl;
			}
		}
		catch(const std::logic_error& e)
		{
			MessageBox(NULL, e.what(), "Logic Exception", MB_OK | MB_ICONERROR);
		}
	}
}

void Frame::updateMirrorEffect()
{
	for(size_t i = 0; i < 3; ++ i)
		for(size_t j = 0; j < 3; ++ j)
			for(size_t k = 0; k < 3; ++ k)
			{
				std::string coordname = coordinateToString(i, j, k);

				MaterialPtr materials[] =
				{
					MaterialManager::getSingleton().getByName("MagicCube/Red" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Cyan" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Green" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Purple" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Blue" + coordname),
					MaterialManager::getSingleton().getByName("MagicCube/Yellow" + coordname),
				};

				for(MagicCube::Face f = MagicCube::Face_pX; f < MagicCube::Face_END; ++ f)
				{
					if(MagicCube::MagicCube::isOuterFace(MagicCube::Position(MagicCube::PositionScalar(i),
						MagicCube::PositionScalar(j), MagicCube::PositionScalar(k)), f))
					{
						if(m_OpenMirrorEffect)
						{
							materials[f]->getTechnique(0)->getPass(1)->setSceneBlending(SBF_ONE, SBF_ONE);
						}
						else
						{
							materials[f]->getTechnique(0)->getPass(1)->setSceneBlending(SBF_ZERO, SBF_ONE);
							//materials[f]->getTechnique(0)->getPass(1)->getTextureUnitState(0)->setBlank();
						}
						/*materials[f]->getTechnique(0)->getPass(1)->getTextureUnitState(0)
							->setEnvironmentMap(checkbox->isSelected(), TextureUnitState::ENV_REFLECTION);*/
					}
				}
			}

	updateShininess();
}

void Frame::updateShininess()
{
	Light* l1 = static_cast<Light*>(static_cast<SceneNode*>(m_nodeLight->getChild(0))->getAttachedObject(0));
	Light* l2 = static_cast<Light*>(static_cast<SceneNode*>(m_nodeLight2->getChild(0))->getAttachedObject(0));
	if(m_OpenShininess)
	{
		if(m_OpenMirrorEffect)
		{
			l1->setSpecularColour(0.16f, 0.16f, 0.16f);
			l2->setSpecularColour(0.4f, 0.4f, 0.4f);
		}
		else
		{
			l1->setSpecularColour(0.4f, 0.4f, 0.4f);
			l2->setSpecularColour(0.8f, 0.8f, 0.8f);
		}
	}
	else
	{
		l1->setSpecularColour(ColourValue::Black);
		l2->setSpecularColour(ColourValue::Black);
	}
}

void Frame::onResetMagicCube()
{
	if(!m_ManipulatationTransition.on && !m_ManipulatingMagicCube)
	{
		m_MagicCube.reset();
		updateMagicCube();
		m_ManipulatationTransition.reset();
	}
}

void Frame::onSaveSnapshot()
{
	static const char * const SNAPSHOT_EXT_NAME = ".png";
	static const char * const SNAPSHOT_DIR = ".\\snapshots\\";

	std::string	series = MagicCube::StateArchive(m_MagicCube).getSerializationCode();
	std::string	filename = std::string(SNAPSHOT_DIR) + "[" + series + "]" + SNAPSHOT_EXT_NAME;

	WIN32_FIND_DATAA data;
	if(FindFirstFile(filename.data(), &data) != INVALID_HANDLE_VALUE)
	{
		size_t i = 0;
		do
		{
			++ i;

			filename = std::string(SNAPSHOT_DIR) + "[" + series + "](" + boost::lexical_cast<std::string>(i) + ")" + SNAPSHOT_EXT_NAME;
		}while(FindFirstFile(filename.data(), &data) != INVALID_HANDLE_VALUE);
	}

	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");
	root->hide();

	m_SaveSnapshotFileName = s_CurrentDirectory + '\\' + filename;

	/*mWindow->getViewport(0)->getTarget()->writeContentsToFile(s_CurrentDirectory + '\\' + filename);

	root->show();

	std::cout << "Save snapshot: " << s_CurrentDirectory + '\\' + filename << std::endl;*/
}

void Frame::onLoadSnapshot()
{
	if(!m_ManipulatationTransition.on && !m_ManipulatingMagicCube)
	{
		std::string dir = s_CurrentDirectory + "\\snapshots\\";

		/*char filename[MAX_PATH] = "";

		OPENFILENAME of =
		{
			sizeof(OPENFILENAME),	// DWORD        lStructSize;
			NULL,					// HWND         hwndOwner;
			GetModuleHandle(NULL),	// HINSTANCE    hInstance;
			NULL,					// LPCWSTR      lpstrFilter;
			NULL,					// LPWSTR       lpstrCustomFilter;
			0,						// DWORD        nMaxCustFilter;
			0,						// DWORD        nFilterIndex;
			filename,				// LPWSTR       lpstrFile;
			MAX_PATH,				// DWORD        nMaxFile;
			NULL,					// LPWSTR       lpstrFileTitle;
			0,						// DWORD        nMaxFileTitle;
			dir.data(),				// LPCWSTR      lpstrInitialDir;
			"Select a snapshot",	// LPCWSTR      lpstrTitle;
			0,						// DWORD        Flags;
			0,						// WORD         nFileOffset;
			0,						// WORD         nFileExtension;
			NULL,					// LPCWSTR      lpstrDefExt;
			0,						// LPARAM       lCustData;
			0,						// LPOFNHOOKPROC lpfnHook;
			NULL,					// LPCWSTR      lpTemplateName;
		};

		if(GetOpenFileName(&of))
		{
			std::string name = filename;*/
		std::string name;
		if(showFileDialog(name, true, dir, "Select a snapshot"))
		{
			size_t filenamestart = name.find('\\');
			if(filenamestart != std::string::npos)
				name = name.substr(filenamestart);

			if(name.size() > 1)
			{
				size_t seriesstart = name.find('[');
				if(seriesstart == std::string::npos)
					seriesstart = 0;

				std::string series = name.substr(seriesstart + 1);
				series = series.substr(0, series.find(']'));
				MagicCube::StateArchive sa(series);
				if(sa.valid())
				{
					m_MagicCube.setStateArchives(sa);
					updateMagicCube();
					m_ManipulatationTransition.reset();
				}
			}
		}
	}
}

void Frame::onUndoManipulatation()
{
	if(!playerOn())
	{
		MagicCube::Manipulation m = m_MagicCube.manipulationBackOff();
		if(m > MagicCube::Manipulation_Identity)
		{
			m = MagicCube::reverseManipulation(m);
			MagicCube::Face face = MagicCube::manipulationToFace(m);
			manipulatationTransition(face, MagicCube::dimensionOfManipulation(m), false);
		}
	}
}

void Frame::onRedoManipulatation()
{
	if(!playerOn())
	{
		MagicCube::Manipulation m = m_MagicCube.manipulationAdvance();
		if(m > MagicCube::Manipulation_Identity)
		{
			MagicCube::Face face = MagicCube::manipulationToFace(m);
			manipulatationTransition(face, MagicCube::dimensionOfManipulation(m), false);
		}
	}
}

void Frame::onNextSkyBox()
{
	++ m_CurrentCubMap;
	if(m_CurrentCubMap >= sizeof(s_CubeMaps) / sizeof(std::string))
		m_CurrentCubMap = 0;
	setCubMap(s_CubeMaps[m_CurrentCubMap]);
}

void Frame::onPlayerStop()
{
	if(m_PlayerBar)
		m_PlayerBar->hide();
}

bool Frame::onPlayerSynchronize(MagicCube::Manipulation m)
{
	if(m != MagicCube::Manipulation_Identity)
		manipulatationTransition(MagicCube::manipulationToFace(m), MagicCube::dimensionOfManipulation(m), false);

	m_PlayerBar->enable();

	if(m_Player->atBack())
		togglePlayPause(false);

	return true;
}
