#pragma once

// bullet3 (physical engine)
#include <btBulletDynamicsCommon.h>
#include <map>
#include "object.h"
#include <math.h>
#include "glm/ext.hpp" 

#include "glm/gtx/string_cast.hpp"
#include "utils.h"


class PhysicalWorld{
    
public:
    float speedAnimation = 1.;
    btDynamicsWorld* dynamicsWorld;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    std::map<int, Object*> glObjects;

    // constructor
    PhysicalWorld();
    PhysicalWorld(Object *obj);

    void initializeEngine();
    void createGround(Object *obj);
    void addSphere(Object *obj, glm::vec3 velocity=glm::vec3(0,0,0));
    void addCube(Object *obj, glm::vec3 velocity=glm::vec3(0,0,0));
    void addDomino(Object *obj, glm::vec3 velocity=glm::vec3(0,0,0));
    void addObject(Object *obj, btCollisionShape* colShape, glm::vec3 velocity);
    int getType(btRigidBody* body) { return body->getUserIndex2(); }
    void setType(btRigidBody* body, int type) { body->setUserIndex2(type); }
    
    Object* RayCastObj(glm::vec3 from, glm::vec3 to, int type);
    btRigidBody* RayCastBody(glm::vec3 from, glm::vec3 to, int type);
    glm::vec3 RayCastPos(glm::vec3 from, glm::vec3 to, int type);
    int DeleteRayCastObj(glm::vec3 from, glm::vec3 to, int type);
    void RayCastPush(glm::vec3 from, glm::vec3 to, int type, int power);
    
    void animate();
    void clear();
    ~PhysicalWorld();
};