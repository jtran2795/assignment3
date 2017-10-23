#include "BaseApplication.h"
class GameObject;
class Simulator{
protected:
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	std:: deque<GameObject*> objList;
public:
	Simulator();
	~Simulator();

	void addObject(GameObject* o);
	btDiscreteDynamicsWorld* getDynamicsWorld()
	{
		return dynamicsWorld;
	}

	btAlignedObjectArray<btCollisionShape*> getCollisionShapes()
	{
		return collisionShapes;
	}

	int getCollisionObjectCount()
	{
		return collisionShapes.size();
	}

	std:: deque<GameObject*> getObjList() {
		return objList;
	}
	// bool removeObject(GameObject* o);
	// void stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps = 1, 
	// 	const Ogre::Real fixedTimestep = 1.0f/60.0f);
};
