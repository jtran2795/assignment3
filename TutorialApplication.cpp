/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "TutorialApplication.h"
#include <OgreWindowEventUtilities.h>
#include <iostream>

Mix_Chunk *bell = NULL;
int rotate = 0;
int cooldown = 0;

CEGUI::Window *sheet;
CEGUI::Window *score;
CEGUI::Window *hiscore;
//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::initSDL()
{
	sound = new SoundSystem();
	sound -> addChunk("swing.wav");
	sound -> addChunk("ball.wav");

}

void TutorialApplication::quitSDL()
{
	sound -> quitSound();
}

void TutorialApplication::initCEGUI() {
	CEGUI::OgreRenderer& myRenderer =
    CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
}

CEGUI::MouseButton TutorialApplication::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}

void TutorialApplication::createScene(void)
{
	initSDL();
	initCEGUI();
	sim = new Simulator();
	state = new GameState();

	CEGUI::WindowManager &mwmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *menu = mwmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::Window *start = mwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");
	CEGUI::Window *startMulti = mwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");

	start->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	start->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.4, 0)));

	startMulti->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	startMulti->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.5, 0)));

	start->setText("Start Game");
	menu->addChild(start);

	startMulti->setText("Play Online");
	menu->addChild(startMulti);

	CEGUI::System::getSingleton( ).getDefaultGUIContext().setRootWindow(menu);

	start->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::gameLoop, this));

	startMulti->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::netMenu, this));



	
	
    // Create your scene here :)
}

void TutorialApplication::gameLoop(void) {
	unsigned int iframes;
	float rX , rY, rZ;

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr -> setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    
	Ogre::Light* light1 = mSceneMgr->createLight("light1");
    light1->setPosition(0,100,0);
    light1->setCastShadows(true);

	// Ogre::Light* light2 = mSceneMgr->createLight("light2");
 //    light2->setPosition(50,30,50);
	// light2->setCastShadows(true);

	createWalls();

	Ogre::Entity* floor_plane = mSceneMgr->createEntity("floor");
	Ogre::Entity* north_plane = mSceneMgr->createEntity("north");

	floor_plane->setMaterialName("WoodPallet");
	//Ogre::SceneNode* floor_plane_node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//floor_plane_node->attachObject(floor_plane);


	//Attatch floor to bullet

	btTransform floorTransform;
	floorTransform.setIdentity();
	floorTransform.setOrigin(btVector3(0,-50,0)); //Change later.

	btScalar floorMass(0.0);
	btVector3 localFloorInertia(0,0,0);

	GameObject *newFloor = new GameObject("newFloor", mSceneMgr, sim);
	btCollisionShape *floorShape = new btBoxShape(btVector3(btScalar(50.0), btScalar(0), btScalar(50.0)));

	newFloor -> buildObject(floor_plane, floorShape, floorMass, floorTransform, localFloorInertia);
	newFloor -> getBody() -> setRestitution(0.95f);

	north_plane->setMaterialName("WoodPallet");
	//Ogre::SceneNode* north_plane_node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//north_plane_node->attachObject(north_plane);

	GameObject* newWall = new GameObject("newWall", mSceneMgr, sim);
	btCollisionShape *wallShape = new btBoxShape(btVector3(btScalar(50.0), btScalar(50.0), btScalar(0.0)));

	floorTransform.setOrigin(btVector3(0,0,50));

	newWall -> buildObject(north_plane, wallShape, floorMass, floorTransform, localFloorInertia);
	newWall -> getBody() -> setRestitution(0.95f);




	Ogre::Entity* ball = mSceneMgr->createEntity("ball", Ogre::SceneManager::PT_SPHERE);
	ball->setMaterialName("Glass");

	btCollisionShape *ballShape = new btSphereShape(btScalar(50.0f));

	sim ->  getCollisionShapes().push_back(ballShape);

	btTransform ballTransform;
	ballTransform.setIdentity();
	ballTransform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0.0f));

	btScalar ballMass = 10.0f;
	btVector3 ballInertia(0,0,0);

	ballTransform.setOrigin(btVector3(0,0,0));

	GameObject* newBall = new GameObject("newBall", mSceneMgr, sim);
	sim -> addObject(newBall);

	newBall -> buildObject(ball, ballShape, ballMass, ballTransform, ballInertia);
	newBall -> getBody() -> setRestitution(1.0f);
	float randY = rand()% 1;
	float randX = (rand()% 1-0.5)*3.0;
	newBall -> getBody() -> setLinearVelocity(btVector3(randX,randY, 20.0f));
	newBall -> setScale(Ogre::Vector3(0.1f,0.1f,0.1f));

	Ogre::Entity* paddleEntity = mSceneMgr->createEntity("paddle", Ogre::SceneManager::PT_CUBE);
	mSceneMgr -> setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	paddleEntity->setMaterialName("Examples/BumpyMetal");
	btCollisionShape *paddleShape = new btBoxShape(btVector3(50,50,50));
	btScalar paddleMass = 800000.0f;
	btVector3 paddleIntertia(0,0,0);

	GameObject* paddle = new GameObject("paddle",mSceneMgr, sim);
	btTransform paddleTransform;
	paddleTransform.setIdentity();
	paddleTransform.setOrigin(btVector3(0.0f,0.0f,-50.0f));

	//paddle -> getNode() -> setScale(Ogre::Vector3(0.1f,0.05f,0.01f));
	paddle -> buildObject(paddleEntity, paddleShape, paddleMass, paddleTransform, paddleIntertia);
	paddle -> setScale(Ogre::Vector3(0.2,0.2,0.01));
	paddle -> getBody() -> setGravity(btVector3(0,0,0));
	paddle -> getBody() -> setRestitution(0.95f);
	paddle -> getBody() -> setAngularFactor(btVector3(1,1,0));
	paddle -> getBody() -> setLinearFactor(btVector3(1,1,0)); 

	sim -> addObject(paddle);
	mCamera->setPosition(Ogre::Vector3(0, 100, -150));
	mCamera->lookAt(Ogre::Vector3(0, 0, 50));

	sim -> getDynamicsWorld() -> setInternalTickCallback(tickCallBack);
	bool collisionWait = false;

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	score = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");
	hiscore = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/HiScore");
	score->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	hiscore->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    hiscore->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85, 0), CEGUI::UDim( 0, 0)));
	sheet->addChild(score);
	sheet->addChild(hiscore);
	CEGUI::System::getSingleton( ).getDefaultGUIContext().setRootWindow(sheet);
	score->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::resetGame, this));

	while(true)
	{
		Ogre::WindowEventUtilities::messagePump();
		sim -> getDynamicsWorld() -> stepSimulation(1.0f/240.0f);
		// Check that paddle doesn't go out of bounds
		if(!(state -> isGameOver())){
			char score_string[32];
			sprintf(score_string, "Score: %d", state -> getScore());
			score->setText(score_string);
			sprintf(score_string, "Hi-Score: %d", state -> getHiscore());
			hiscore->setText(score_string);
			}
			else
			{
				score -> setText("Game Over");
			}
		
		Ogre::Vector3 paddlePos = paddle -> getNode() -> getPosition();
		// if(paddlePos[0] < -100.0f || paddlePos[0] > 100.0f
		//    || paddlePos[1] < -39.0f || paddlePos[1] > 100.0f
		//    || paddlePos[2] < -100.0f || paddlePos[2] > 100.0f)
		// {
		// 	paddle -> getBody() -> setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		// }

	 	btVector3 cur_vel = paddle -> getBody() -> getLinearVelocity();

	 	btScalar x = cur_vel.x();
	 	btScalar y = cur_vel.y();
	 	btScalar z = cur_vel.z();

		if(paddlePos.x < -62.0f)
		{
			x = std::max(0.0f, x);
		}

		else if(paddlePos.x > 63.0f)
		{
			x = std::min(0.0f, x);
		}

		if(paddlePos.y < -39.0f)
		{
			y = std::max(0.0f, y);
		}

		else if(paddlePos.y > 31.0f)
		{
			y = std::min(0.0f, y);
		}

		paddle -> getBody() -> setLinearVelocity(btVector3(x, y, z));
		mCamera -> lookAt(paddle -> getNode() -> getPosition());
		//paddle -> getNode() -> lookAt(Ogre::Vector3(0,-50.0f,100.0f));
		collisionWait = collisionHandler(collisionWait);

		
		for(int i = 0; i < sim -> getDynamicsWorld() -> getCollisionObjectArray().size(); i++)
		{
			btCollisionObject* o = sim -> getDynamicsWorld() -> getCollisionObjectArray()[i];
			btRigidBody* rb = btRigidBody::upcast(o);

			if(rb && rb -> getMotionState())
			{
				btTransform tr;
				rb -> getMotionState() -> getWorldTransform(tr);

				void *usrp = rb -> getUserPointer();
				if(usrp)
				{
					btQuaternion rot = tr.getRotation();
					Ogre::SceneNode *sn = static_cast<Ogre::SceneNode *>(usrp);
					if((sn -> getName() == "newBall") && ((tr.getOrigin()).getY() <= -60.0f || (tr.getOrigin()).getY() > 250.0f))
						{
							//sim -> getDynamicsWorld() -> removeRigidBody(rb);
							if( cooldown >= 60 && !(state -> isGameOver()))
							{
								std::cout <<" Out of bounds! \n";
								state -> setGameOver(true);
								
								break;
							}
						}
					else{
						sn -> setPosition(Ogre::Vector3(tr.getOrigin().getX(), tr.getOrigin().getY(), tr.getOrigin().getZ()));
						sn -> setOrientation(Ogre::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
					}
				}
			}
		}
		//checkWalls(velocity,ball_node->getPosition());
		//ball_node->setPosition( Ogre::Vector3(ball_node->getPosition() + velocity));
		if(cooldown < 60) {cooldown++;}
		if(!mRoot->renderOneFrame()) 
			{
				break;
			}
	}
}

void TutorialApplication::netMenu() {

	netm = new NetManager();
	netm -> initNetManager();
	netm -> addNetworkInfo();
	netm -> startServer();

	CEGUI::WindowManager &nwmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *nMenu = nwmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::Window *host = nwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");
	CEGUI::Window *broadcast = nwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");
	CEGUI::Window *join = nwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");

	CEGUI::Window *send = nwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");
	CEGUI::Window *recv = nwmgr.createWindow("TaharezLook/Button", "CEGUIDemo/Score");

	host->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	host->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.4, 0)));

	join->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	join->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.5, 0)));

	broadcast->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	broadcast->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.6, 0)));

	send->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	send->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.7, 0)));

	recv->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	recv->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim( 0.8, 0)));

	host->setText("Host Game");
	nMenu->addChild(host);

	join->setText("Join Game");
	nMenu->addChild(join);

	broadcast->setText("UDP Broadcast");
	nMenu->addChild(broadcast);

	send->setText("Send Msg");
	nMenu->addChild(send);

	recv->setText("Receive Msg");
	nMenu->addChild(recv);

	CEGUI::System::getSingleton( ).getDefaultGUIContext().setRootWindow(nMenu);

	host->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::initHost, this));

	join->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::lobbyMenu, this));

	broadcast->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::broadcastUDP, this));

	send->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::sendMsg, this));

	recv->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::recvMsg, this));

}

void TutorialApplication::initHost() {

	netm -> multiPlayerInit(1);
	// if(netm -> pollForActivity(1000)) {
	// 	std::cout << "Connection Established" << "\n";
	// }
	int count = 0;
	while((netm -> pollForActivity(1000)) == 0 && count < 10) {
		count++;
	}
	if(count < 10) {
		std::cout << netm -> getClients() << "\n";
	}
	netm -> pollForActivity(1000);
	std::cout << netm -> getClients() << "\n";





}
void TutorialApplication::lobbyMenu() {

	std::cout << netm -> scanForActivity() << "\n";
	CEGUI::WindowManager &lwmgr = CEGUI::WindowManager::getSingleton();

}

void TutorialApplication::broadcastUDP() {
	std::cout << netm -> getPort() << "\n";
	netm -> broadcastUDPInvitation(1);
}

void TutorialApplication::sendMsg() {
	const char* buf = "Testing";
	netm -> messageClients(PROTOCOL_ALL, buf, strlen(buf));
}

void TutorialApplication::recvMsg() {
	if(netm -> scanForActivity()) {
		std::cout << "Got Message" << "\n";
	}
	else {
		std::cout << "No Messages Received" << "\n";
	}
}

void TutorialApplication::resetBall(Ogre::SceneNode *sn, btRigidBody *rb)
{
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0,0,0));
	sn -> setPosition(Ogre::Vector3(tr.getOrigin().getX(), tr.getOrigin().getY(), tr.getOrigin().getZ()));
	rb -> setWorldTransform(tr);
	float randY = rand() % 1;
	float randX = (rand() % 1 -0.5)*3.0;
	rb -> setLinearVelocity(btVector3(randX,randY, 20.0f));
	rb -> activate();
}
void TutorialApplication::resetPaddle(Ogre::SceneNode *sn, btRigidBody *rb){
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0,0,-50.f));
	sn -> setPosition(Ogre::Vector3(tr.getOrigin().getX(), tr.getOrigin().getY(), tr.getOrigin().getZ()));
	rb -> setWorldTransform(tr);
	rb -> activate();
}
void TutorialApplication::resetGame(){
	for(int i = 0; i < sim -> getDynamicsWorld() -> getCollisionObjectArray().size(); i++)
	{
		btCollisionObject* o = sim -> getDynamicsWorld() -> getCollisionObjectArray()[i];
			btRigidBody* rb = btRigidBody::upcast(o);

			if(rb && rb -> getMotionState())
			{
				btTransform tr;
				rb -> getMotionState() -> getWorldTransform(tr);

				void *usrp = rb -> getUserPointer();
				if(usrp)
				{
					Ogre::SceneNode *sn = static_cast<Ogre::SceneNode *>(usrp);
					if(sn -> getName() == "newBall")
						{
							resetBall(sn, rb);
						}
					if(sn -> getName() == "paddle")
						{
							resetPaddle(sn, rb);
						}
					}
			}
	}
	state -> resetScore();
	state -> resetBounces();
	score -> setText("0");
	cooldown = 0;
	rotate = 0;
	state -> setGameOver(false);
}
void TutorialApplication::tickCallBack(btDynamicsWorld *world, btScalar timeStep) 
{
	

	btRigidBody* paddle;
	btCollisionObjectArray coArray = world -> getCollisionObjectArray();
	for(int i = 0; i < coArray.size();i++) {
		void* usrp = coArray[i] -> getUserPointer();
		if(usrp) 
		{
			Ogre::SceneNode *sn = static_cast<Ogre::SceneNode *>(usrp);
			if(sn -> getName() == "paddle") 
			{
				paddle = btRigidBody::upcast(coArray[i]);
			}
		}
	}

	//paddle -> getBody() -> setAngularVelocity(btVector3(3.0f,0.0f,0.0f));
	//paddle -> getMotionState() -> getWorldTransform() -> getRotation()

	//std::cout << "rotate: " << rotate << " val: " << paddle -> getOrientation().getAngle() << "\n";

	if(rotate == 1)
	{
		if(paddle -> getOrientation().getAngle() >= 1.0f)
		{
			paddle -> setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
			rotate = 6;
		}
	} 
	else if(rotate == 2)
	{
		if(paddle -> getOrientation().getAngle() <= 0.0f)
		{
			paddle -> setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
			rotate = 0;
		}
	}
	else if(rotate == 3)
	{
		if(paddle -> getOrientation().getAngle() >= 1.0f)
		{
			paddle -> setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
			rotate = 7;
		}
	}
	else if(rotate == 4)
	{
		if(paddle -> getOrientation().getAngle() <= 0.0f)
		{
			paddle -> setAngularVelocity(btVector3(0.0f,0.0f,0.0f));
			rotate = 0;
		}
	}
}

bool TutorialApplication::collisionHandler(bool wait) {
	btDynamicsWorld* world = sim -> getDynamicsWorld();
	int contactManifolds = world -> getDispatcher() -> getNumManifolds();
	for(int i = 0; i < contactManifolds; i++) {
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
        btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());
        void * usrpA = obA -> getUserPointer();
        void * usrpB = obB -> getUserPointer();
        btRigidBody* rbA = btRigidBody::upcast(obA);
        btRigidBody* rbB = btRigidBody::upcast(obB); 
        if(usrpA && usrpB)
        {
        	Ogre::SceneNode *snA = static_cast<Ogre::SceneNode *>(usrpA);
        	Ogre::SceneNode *snB = static_cast<Ogre::SceneNode *>(usrpB);
        	int numContacts = contactManifold->getNumContacts();
        	for (int j = 0; j < numContacts; j++)
        	{
	            btManifoldPoint& pt = contactManifold->getContactPoint(j);
	                if(snA -> getName() == "newBall" || snB ->getName() == "newBall")
       				{
       					if(wait == false) 
       					{
	       					std::cout << snA -> getName() << " " << snB -> getName() << "\n";
	       					 if(snA ->getName() == "newFloor" || snB -> getName() == "newFloor")
	       					 {
	       					 	//replace with floor sound
	       					 	sound -> playChunk("ball.wav");
	       					 	state -> incrementBounces();
	       					 	std::cout << state -> getBounces() << "\n";
	       					 	if(!(state -> isGameOver()) && (state -> getBounces() >= 2))
	       					 	{
	       					 		std::cout << "Game Over Double Bounce" << "\n";
	       					 		state -> setGameOver(true);
	       					 		score->setText("Game Over");
	       					 	}
	       					 	if(snA -> getName() == "newBall" ) {
	       					 		rbA -> applyImpulse(btVector3(0.0f,20.0f,0.0f), btVector3(0.0f,0.0f,0.0f));
	       					 	}
	       					 	else {
	       					 		rbB -> applyImpulse(btVector3(0.0f,20.0f,0.0f), btVector3(0.0f,0.0f,0.0f));
	       					 	}
	       					 	return true;
	       					 }
	       					 else if(snA -> getName() == "paddle") 
	       					 {
	       					 	if(!(state -> isGameOver())) {state -> incrementScore();}
	       					 	sound -> playChunk("ball.wav");
	       					 	//rbB -> applyImpulse(btVector3(0.0f,0.0f,20.0f), btVector3(0.0f,0.0f,0.0f));
	       					 }
	       					 else if(snB -> getName() == "paddle") 
	       					 {
	       					 	if(!(state -> isGameOver())) {state -> incrementScore();}
	       					 	sound -> playChunk("ball.wav");
	       					 	//rbA -> applyImpulse(btVector3(0.0f,0.0f,20.0f), btVector3(0.0f,0.0f,0.0f));
	       					 }
	       					 else if(snA -> getName() == "newWall") 
	       					 {
	       					 	sound -> playChunk("ball.wav");
	       					 }
	       					 else if(snB -> getName() == "newWall") 
	       					 {
	       					 	sound -> playChunk("ball.wav");
	       					 }
	       					 state -> resetBounces();


       					}
       					return true;	
       				}
	        }
        }
	}
	return false;
}


void TutorialApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mRoot->addFrameListener(this);
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    if(mShutDown)
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
 
    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
 
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if (arg.key == OIS::KC_ESCAPE) {
		quitSDL();
    	mShutDown = true;
  	}

	std:: deque<GameObject*> objList = sim -> getObjList();
	GameObject* paddle = NULL;
	for(int i = 0; i < objList.size();i++) 
	{
		if(objList[i] -> getName() == Ogre::String("paddle")) 
		{
			paddle = objList[i];
		}
	}
	if(paddle == NULL) 
		return true;

	btVector3 cur_vel = paddle -> getBody() -> getLinearVelocity();

	btScalar x = cur_vel.x();
	btScalar y = cur_vel.y();
	btScalar z = cur_vel.z();

	if (arg.key == OIS::KC_A || arg.key == OIS::KC_LEFT)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(5.0f, y, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_W || arg.key == OIS::KC_UP)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(x, 5.0f, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(x, -5.0f, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(-5.0f, y, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_E)
	{
		//Mix_PlayChannel(-1, bell, 0);
		//sound -> playChunk("bell.wav");
		std::cout << state -> getScore() << "\n";
		// std::cout << paddle -> getBody() -> getOrientation().getAngle() << "\n";
		// std::cout << paddle -> getBody() -> getOrientation().getAxis().getX() << " "
		//  << paddle -> getBody() -> getOrientation().getAxis().getY() << " "
		//  << paddle -> getBody() -> getOrientation().getAxis().getZ() << "\n";
	}
  	if (arg.key == OIS::KC_SPACE && (rotate == 0 || rotate == 2))
	{
		paddle -> getBody() -> setAngularVelocity(btVector3(3.0f,0.0f,0.0f));
		paddle -> getBody() -> activate();
		rotate = 1;
		sound -> playChunk("swing.wav");
	}
	if ((arg.key == OIS::KC_RMENU || arg.key == OIS::KC_LMENU ) && (rotate == 0 || rotate == 4))
	{
		paddle -> getBody() -> setAngularVelocity(btVector3(-3.0f,0.0f,0.0f));
		paddle -> getBody() -> activate();
		rotate = 3;
		sound -> playChunk("swing.wav");
	}
	if (arg.key == OIS::KC_M) {
 		sound -> muteSound();
 	}

	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)arg.key);
	context.injectChar((CEGUI::Key::Scan)arg.text);
	return true;
}
bool TutorialApplication::keyReleased( const OIS::KeyEvent &arg )
{
	std:: deque<GameObject*> objList = sim -> getObjList();
	GameObject* paddle = NULL;
	for(int i = 0; i < objList.size();i++) {
		if(objList[i] -> getName() == "paddle") {
			paddle = objList[i];
		}
	}
	if(paddle == NULL)
		return true;

	btVector3 cur_vel = paddle -> getBody() -> getLinearVelocity();

	btScalar x = cur_vel.x();
	btScalar y = cur_vel.y();
	btScalar z = cur_vel.z();

	if (arg.key == OIS::KC_A || arg.key == OIS::KC_LEFT)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(0.0f, y, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_W || arg.key == OIS::KC_UP)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(x, 0.0f, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(x, 0.0f, z));
		paddle -> getBody() -> activate();
	}
	if (arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT)
	{
		paddle -> getBody() -> setLinearVelocity(btVector3(0.0f, y, z));
		paddle -> getBody() -> activate();
	}
  	if (arg.key == OIS::KC_SPACE && (rotate == 1 || rotate == 6))
	{
		paddle -> getBody() -> setAngularVelocity(btVector3(-3.0f,0.0f,0.0f));
		paddle -> getBody() -> activate();
		rotate = 2;
	}
	if ((arg.key == OIS::KC_RMENU || arg.key == OIS::KC_LMENU ) && (rotate == 3 || rotate == 7))
	{
		paddle -> getBody() -> setAngularVelocity(btVector3(3.0f,0.0f,0.0f));
		paddle -> getBody() -> activate();
		rotate = 4;
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	return true;
}

bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}

bool TutorialApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent &arg)
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
	    sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}

void TutorialApplication::createWalls(void)
{
	Ogre::Plane plane = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane(
		"floor", //name
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, //resource group name
		plane,  //plane object
		100, 100, //size wxh
		2, 2, //texture segments
		true,  //normals
		1, 5, 5, //texture coordinates u and v
		Ogre::Vector3::UNIT_Z); //up vector

	plane = Ogre::Plane(Ogre::Vector3::NEGATIVE_UNIT_Z, 0);

	Ogre::MeshManager::getSingleton().createPlane(
		"north",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 
		100, 100,
		2, 2, 
		true, 
		1, 5, 5, 
		Ogre::Vector3::UNIT_X);
}
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
