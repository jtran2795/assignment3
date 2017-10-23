#include "BaseApplication.h"
#include "OgreMotionState.h"
#include "Simulator.h"

class GameObject{
protected:
	Ogre::String name;
	Ogre::SceneManager* sceneMgr;
	Simulator* simulator;
	Ogre::SceneNode* rootNode;
	Ogre::SceneNode* node;

	Ogre::Entity* geom;
	btCollisionShape* shape;
	btScalar mass;
	btRigidBody* body;
	btTransform tr;
	btVector3 inertia;
	btDefaultMotionState* motionState;

public:
	GameObject();
	GameObject(Ogre::String n, Ogre::SceneManager* sm, Simulator* sim)
	{
		name = n;
		sceneMgr = sm;
		simulator = sim;
		rootNode = sceneMgr -> getRootSceneNode();
		node = rootNode -> createChildSceneNode(n);
	}

	void buildObject(Ogre::Entity* g, btCollisionShape* s, btScalar m, btTransform t, btVector3 inert);

	btRigidBody* getBody()
	{
		return body;
	}

	Ogre::SceneNode* getNode(){
		return node;
	}

	Ogre::String getName(){
		return name;
	}

	void setScale(Ogre::Vector3 v);

	btTransform getTransform(){
		return tr;
	}
	void setTransform(btTransform t){
		tr = t;
	}
};
