#include "physics.h"
#include <math.h>
#include "glm/ext.hpp" 
#include "glm/gtx/string_cast.hpp"
// #define GRAVITY -9.81f;
const float GRAVITY = -3.81f;
PhysicalWorld::PhysicalWorld(Object *obj)
{
    initializeEngine();
    createGround(obj, 50., 50.);
}

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL

void PhysicalWorld::initializeEngine(){
    ///-----initialization_start-----
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = new btDefaultCollisionConfiguration();
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));
}


void PhysicalWorld::createGround(Object *obj, float width, float depth){
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(width), btScalar(0.), btScalar(depth)));

    collisionShapes.push_back(groundShape);
    glObjects.push_back(obj); // Generalize (link to openGL)

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 0, 0));

    btScalar mass(0.);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape->calculateLocalInertia(mass, localInertia);

    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    dynamicsWorld -> addRigidBody(body);
}


void PhysicalWorld::addSphere(Object *obj){
    //create a dynamic rigidbody
    if (obj->scale.x!=obj->scale.y || obj->scale.y!=obj->scale.z) { // Check if it's a sphere
        std::cout << "The sphere has not spherical dimensions !" << std::endl;
        exit(-1);
    }
    btCollisionShape* colShape = new btSphereShape(btScalar(obj->scale.x));
    addObject(obj, colShape);
}

void PhysicalWorld::addCube(Object *obj){
    //create a dynamic rigidbody
    btCollisionShape* colShape = new btBoxShape(btVector3(obj->scale.x, obj->scale.y, obj->scale.z));
    addObject(obj, colShape);
}

void PhysicalWorld::addObject(Object *obj, btCollisionShape* colShape){
    //btCollisionShape* colShape = new btSphereShape(btScalar(obj->scale));
    collisionShapes.push_back(colShape);
    glObjects.push_back(obj);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(1.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass, localInertia);


    startTransform.setOrigin(btVector3(obj->position.x, obj->position.y, obj->position.z)); // Initial Position
    
    // Initial Rotation with quaternion
    float roll = obj->rotation.x; // roll = x-axis rotation
    float pitch = obj->rotation.y; // pitch = y-axis rotation
    float yaw = obj->rotation.z; // yaw = z-axis rotation
    float qx = sin(roll/2) * cos(pitch/2) * cos(yaw/2) - cos(roll/2) * sin(pitch/2) * sin(yaw/2);
    float qy = cos(roll/2) * sin(pitch/2) * cos(yaw/2) + sin(roll/2) * cos(pitch/2) * sin(yaw/2);
    float qz = cos(roll/2) * cos(pitch/2) * sin(yaw/2) - sin(roll/2) * sin(pitch/2) * cos(yaw/2);
    float qw = cos(roll/2) * cos(pitch/2) * cos(yaw/2) + sin(roll/2) * sin(pitch/2) * sin(yaw/2);
    startTransform.setRotation(btQuaternion(qx,qy,qz,qw)); 

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    dynamicsWorld->addRigidBody(body);
}

void PhysicalWorld::animate()
{
    ///-----stepsimulation_start-----
    dynamicsWorld->stepSimulation(1.f / 60.f, 1);
    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) // go through all the objects
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }

        Object* glObj = glObjects[j];
        if (glObj != NULL) {
            btScalar roll, pitch, yaw;
            trans.getRotation().getEulerZYX(yaw,pitch,roll);
            glm::vec3 translation = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            glObj->setPosRot(translation, glm::vec3(roll, pitch, yaw));
        }
    }
}

void PhysicalWorld::clear(){
        //cleanup in the reverse order of creation/initialization

        ///-----cleanup_start-----

        //remove the rigidbodies from the dynamics world and delete them
        for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        //delete collision shapes
        for (int j = 0; j < collisionShapes.size(); j++)
        {
            btCollisionShape* shape = collisionShapes[j];
            collisionShapes[j] = 0;
            delete shape;
        }

        //delete dynamics world
        delete this->dynamicsWorld;

        //delete solver
        delete solver;

        //delete broadphase
        delete overlappingPairCache;

        //delete dispatcher
        delete dispatcher;

        delete collisionConfiguration;

        //next line is optional: it will be cleared by the destructor when the array goes out of scope
        collisionShapes.clear();
        glObjects.clear();
}
