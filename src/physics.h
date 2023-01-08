#pragma once

// bullet3 (physical engine)
#include <btBulletDynamicsCommon.h>
#include <map>
#include "object.h"


class PhysicalWorld{
    
public:
    float speedAnimation = 1.;
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
    void createGround(Object *obj);
    void addSphere(Object *obj, glm::vec3 velocity=glm::vec3(0,0,0), int lifetime=-1);
    void addCube(Object *obj, glm::vec3 velocity=glm::vec3(0,0,0), int lifetime=-1);
    void addDomino(Object *obj, glm::vec3 velocity=glm::vec3(0,0,0), int lifetime=-1);
    void addObject(Object *obj, btCollisionShape* colShape, glm::vec3 velocity, int lifetime=-1);
    int getLifeTime(btRigidBody* body) { return body->getUserIndex2(); }
    void setLifeTime(btRigidBody* body, int lifetime) { body->setUserIndex2(lifetime); }
    void animate();
    void clear();
};