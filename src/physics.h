#ifndef PYHSICS_H
#define PHYSICS_H

// bullet3 (physical engine)
#include <btBulletDynamicsCommon.h>
#include <map>
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
    std::map<int, Object*> glObjects;

    // constructor
    PhysicalWorld(Object *obj);

    void initializeEngine();
    void createGround(Object *obj, float width=50., float depth=50.);
    void push(glm::vec3 position, glm::vec3 direction, int power);
    void addSphere(Object *obj);
    void addCube(Object *obj);
    void addDomino(Object *obj);
    void addObject(Object *objbt, btCollisionShape* colShape);

    void animate();
    void clear();
};

#endif