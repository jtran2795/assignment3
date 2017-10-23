#include "GameObject.h"

void GameObject::buildObject(Ogre::Entity* g, btCollisionShape* s, btScalar m, btTransform t, btVector3 inert)
{
	geom = g;
	shape = s;
	mass = m;
	tr = t;
	inertia = inert;

	node -> attachObject(geom);
	motionState = new btDefaultMotionState(tr);
	shape -> calculateLocalInertia(mass, inertia);
	simulator -> getCollisionShapes().push_back(shape);
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(mass, motionState, shape, inertia);
	body = new btRigidBody(bodyInfo);
	body->setActivationState(DISABLE_DEACTIVATION);
	simulator -> getDynamicsWorld() -> addRigidBody(body);
	body -> setUserPointer(node);
}

void GameObject::setScale(Ogre::Vector3 v)
{
	shape -> setLocalScaling(btVector3(v.x, v.y, v.z));
	node -> setScale(v);
}