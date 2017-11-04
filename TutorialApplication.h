/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include "GameObject.h"
#include "SoundSystem.h"
#include "GameState.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication  
{
public:
    TutorialApplication(void);
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    virtual ~TutorialApplication(void);

protected:
  SoundSystem *sound;
	Simulator *sim;
  GameState *state;
  NetManager *netm;
    virtual void createScene(void);
    void gameLoop(void);
    void gameLoopMP(void);
    void initSDL();
    void quitSDL();
    void initCEGUI();
	virtual void createWalls(void);
  static void tickCallBack(btDynamicsWorld *world, btScalar timeStep);
  bool collisionHandler(bool wait);
  void createFrameListener(void);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
  bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  bool mouseMoved(const OIS::MouseEvent &arg);
  void resetBall(Ogre::SceneNode *sn, btRigidBody *rb);
  void resetPaddle(Ogre::SceneNode *sn, btRigidBody *rb);
  void resetGame();
  void netMenu(void);
  void initHost(void);
  void lobbyMenu(void);
  void checkMessages();
  void sendMessage();
  void resetConnection();
  void broadcastUDP(void);
  void quitGame(void);
  void recvMsg(void);
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
