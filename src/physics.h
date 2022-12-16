#ifndef PYHSICS_H
#define PHYSICS_H

// bullet3 (physical engine)
#include <btBulletDynamicsCommon.h>
#include "object.h"




class PhysicalWorld
{
public:
    btDiscreteDynamicsWorld* dynamicsWorld;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    std::vector<Object*> glObjects;

    // constructor
    PhysicalWorld();

    void initializeEngine();
    void createGround(float length, float width);
    void addSphere(Object *obj);
    void animate();
    void clear();
};

#endif