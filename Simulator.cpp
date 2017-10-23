#include "Simulator.h"

Simulator::Simulator() {
		collisionConfig = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfig);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
		dynamicsWorld -> setGravity(btVector3(0.0,-0.75f, 0.0));
	}
void Simulator::addObject(GameObject* o) {
	objList.push_back(o);
}
// void Simulator::addObject(GameObject* o) {
// 	objList.push_back(o);
// 	dynamicsWorld -> addRigidBody(o -> getBody());
// }