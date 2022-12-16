#include "physics.h"
// #define GRAVITY -9.81f;
const float GRAVITY = -3.81f;
const float FLOOR_LENGTH = 50.f; //x
const float FLOOR_WIDTH = 50.f; //y

PhysicalWorld::PhysicalWorld(){
    initializeEngine();
    createGround(FLOOR_LENGTH, FLOOR_WIDTH);
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


void PhysicalWorld::createGround(float length, float width){
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(length), btScalar(1.), btScalar(width)));

    collisionShapes.push_back(groundShape);
    glObjects.push_back(NULL); // Generalize (link to openGL)

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, -1, 0));

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

    //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    btCollisionShape* colShape = new btSphereShape(btScalar(obj->scale));
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


    startTransform.setOrigin(btVector3(obj->position.x, obj->position.y, obj->position.z)); // Position

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    dynamicsWorld->addRigidBody(body);
}

void PhysicalWorld::animate(){
    ///-----stepsimulation_start-----
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);
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
        // vecPtr->operator[](index)
        // vecPtr->at(index)
        Object* glObj = glObjects[j];
        if (glObj != NULL) {
            glObj->model = glm::translate(glObj->model, glm::vec3(
            (float(trans.getOrigin().getX()) - glObj->position.x),
            (float(trans.getOrigin().getY()) - glObj->position.y), 
            (float(trans.getOrigin().getZ()) - glObj->position.z))/glObj->scale);

            glObj->position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            // TODO check is a copy or not ?
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
