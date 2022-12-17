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

    float size_x = 0.175, size_y = 1.0, size_z = 0.5;

    // constructor
    PhysicalWorld();

    void initializeEngine();
    void createGround(float length, float width);

    void addSphere(Object *obj);
    void addCube(Object *obj);
    void addObject(Object *objbt, btCollisionShape* colShape);

    void animate();
    void clear();
};

#endif