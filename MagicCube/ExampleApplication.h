/*
**	This source file is part of K.L.'s MagicCube.
**
**	Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
**	This program is free software without any warranty.
*/

/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef __ExampleApplication_H__
#define __ExampleApplication_H__

#include <boost\lexical_cast.hpp>

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "ExampleFrameListener.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert(mainBundle);

	CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
	assert(mainBundleURL);

	CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
	assert(cfStringRef);

	CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

	CFRelease(mainBundleURL);
	CFRelease(cfStringRef);

	return std::string(path);
}
#endif

using namespace Ogre;

static const char* s_GuiConfigSectionName = "MagicCubeGui";


/** Base class which manages the standard startup of an Ogre application.
Designed to be subclassed for specific examples if required.
*/
class ExampleApplication
{
public:
	/// Standard constructor
	ExampleApplication()
		: m_OpenMirrorEffect(true)
		, m_OpenShininess(true)
		, m_CurrentCubMap(0)
	{
		mFrameListener = 0;
		mRoot = 0;
		// Provide a nice cross platform solution for locating the configuration files
		// On windows files are searched for in the current working directory, on OS X however
		// you must provide the full path, the helper function macBundlePath does this for us.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = macBundlePath() + "/Contents/Resources/";
#else
		mResourcePath = "";
#endif
	}
	/// Standard destructor
	virtual ~ExampleApplication()
	{
		if (mFrameListener)
			delete mFrameListener;
		if (mRoot)
			delete mRoot;

		saveGuiConfig();
	}

	/// Start the example
	virtual void go(void)
	{
		if (!setup())
			return;

		mRoot->startRendering();

		// clean up
		destroyScene();
	}

protected:
	Root *mRoot;
	Camera* mCamera;
	SceneManager* mSceneMgr;
	ExampleFrameListener* mFrameListener;
	RenderWindow* mWindow;
	Ogre::String mResourcePath;

	bool	m_OpenMirrorEffect;
	bool	m_OpenShininess;

	size_t		m_CurrentCubMap;

	std::string	m_CubeCode;

	// These internal methods package up the stages in the startup process
	/** Sets up the application - returns false if the user chooses to abandon configuration. */
	virtual bool setup(void)
	{

		String pluginsPath;
		// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
#	ifdef	_DEBUG
		pluginsPath = mResourcePath + "plugins_d.cfg";
#	else
		pluginsPath = mResourcePath + "plugins.cfg";
#	endif
#endif

		mRoot = new Root(pluginsPath, 
			mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

#ifndef	_DEBUG
		LogManager::getSingleton().setLogDetail(LL_LOW);
#endif	// !defined(_DEBUG)

		setupResources();

		bool carryOn = configure();
		if (!carryOn) return false;

		chooseSceneManager();
		createCamera();
		createViewports();

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Create any resource listeners (for loading screens)
		createResourceListener();
		// Load resources
		loadResources();

		// Create the scene
		createScene();

		createFrameListener();

		return true;

	}
	/** Configures the application - returns false if the user chooses to abandon configuration. */
	virtual bool configure(void)
	{
		// Show the configuration dialog and initialise the system
		// You can skip this and use root.restoreConfig() to load configuration
		// settings if you were sure there are valid ones saved in ogre.cfg
#ifdef	SKIP_CONFIG_DIALOG
		if(mRoot->restoreConfig())
		{
			loadGuiConfig();
		}
		else
		{
			if(!mRoot->showConfigDialog())
				return false;
		}
#else
		if(!mRoot->showConfigDialog())
			return false
#endif
		{
			// If returned true, user clicked OK so initialise
			// Here we choose to let the system create a default rendering window by passing 'true'
			mWindow = mRoot->initialise(true, "Magic Cube");
		}
		return true;
	}

	virtual void chooseSceneManager(void)
	{
		// Create the SceneManager, in this case a generic one
		mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
	}
	virtual void createCamera(void)
	{
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");

		// Position it at 500 in Z direction
		mCamera->setPosition(Vector3(0,0,500));
		// Look back along -Z
		mCamera->lookAt(Vector3(0,0,-300));
		mCamera->setNearClipDistance(5);

	}
	virtual void createFrameListener(void)
	{
		mFrameListener= new ExampleFrameListener(mWindow, mCamera);
		mFrameListener->showDebugOverlay(true);
		mRoot->addFrameListener(mFrameListener);
	}

	virtual void createScene(void) = 0;    // pure virtual - this has to be overridden

	virtual void destroyScene(void){}    // Optional to override this

	virtual void createViewports(void)
	{
		// Create one viewport, entire window
		Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(ColourValue(0,0,0));

		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(
			Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	}

	/// Method which will define the source of resources (other than current folder)
	virtual void setupResources(void)
	{
		// Load resource paths from config file
		ConfigFile cf;
		cf.load(mResourcePath + "resources.cfg");

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app,
				// In order to make things portable on OS X we need to provide
				// the loading with it's own bundle path location
				ResourceGroupManager::getSingleton().addResourceLocation(
					String(macBundlePath() + "/" + archName), typeName, secName);
#else
				ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
#endif
			}
		}
	}

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener(void)
	{

	}

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources(void)
	{
		// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	}


private:
	void	loadGuiConfig()
	{
		ConfigFile cfg;
		cfg.load("ogre.cfg"/*, "\t:=", false*/);
		std::string s = cfg.getSetting("OpenMirrorEffect", s_GuiConfigSectionName);
		m_OpenMirrorEffect = boost::lexical_cast<bool>(s.empty() ? "1" : s);
		s = cfg.getSetting("OpenShininess", s_GuiConfigSectionName);
		m_OpenShininess = boost::lexical_cast<bool>(s.empty() ? "1" : s);
		s = cfg.getSetting("CurrentCubMap", s_GuiConfigSectionName);
		m_CurrentCubMap = boost::lexical_cast<size_t>(s.empty() ? "0" : s);
		m_CubeCode = cfg.getSetting("CubeCode", s_GuiConfigSectionName);
	};

	void	saveGuiConfig()
	{
		std::ifstream cfg("ogre.cfg", std::ios::in);
		if(cfg.is_open())
		{
			//bool found = false;

			std::stringstream ss;

			// find begin position of the GUI config section
			char buffer[128];
			while(!cfg.eof() && !cfg.fail())
			{
				//std::cout << "g:\t" << cfg.tellg() << std::endl;
				//std::cout << "p:\t" << cfg.tellp() << std::endl;
				cfg.getline(buffer, 128);
				if(std::string(buffer) == std::string("[") + s_GuiConfigSectionName + "]")
				{
					//found = true;
					break;
				}

				ss << buffer << std::endl;
			}

			/*cfg << std::endl;

			if(!found)
				cfg << "[" << s_GuiConfigSectionName << "]" << std::endl;

			cfg << "OpenMirrorEffect=" << boost::lexical_cast<std::string>(m_OpenMirrorEffect) << std::endl;
			cfg << "OpenShininess=" << boost::lexical_cast<std::string>(m_OpenShininess) << std::endl;
			cfg << "CurrentCubMap=" << boost::lexical_cast<std::string>(m_CurrentCubMap) << std::endl;

			cfg.flush();*/

			cfg.close();
			std::fstream ocfg("ogre.cfg", std::ios::out | std::ios::trunc);
			if(ocfg.is_open())
			{
				ocfg << ss.str();

				//if(!found)
				{
					//ocfg << std::endl;
					ocfg << "[" << s_GuiConfigSectionName << "]" << std::endl;
				}

				ocfg << "OpenMirrorEffect=" << boost::lexical_cast<std::string>(m_OpenMirrorEffect) << std::endl;
				ocfg << "OpenShininess=" << boost::lexical_cast<std::string>(m_OpenShininess) << std::endl;
				ocfg << "CurrentCubMap=" << boost::lexical_cast<std::string>(m_CurrentCubMap) << std::endl;
				ocfg << "CubeCode=" << m_CubeCode << std::endl;
			}
		}
	};

};


#endif
