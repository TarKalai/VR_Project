#include "physics.h"

PhysicalWorld::PhysicalWorld()
{
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
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

btCollisionShape* PhysicalWorld::getShape(Object *obj) {
    if (obj->type==geometry::domino)
        return new btBoxShape(btVector3((dominoDim::thick/2)*obj->scale.x, (dominoDim::height/2)*obj->scale.y, (dominoDim::width/2)*obj->scale.z));
    else if (obj->type==geometry::plane)
        return new btBoxShape(btVector3(obj->scale.x, general::floorThickness, obj->scale.z));
    else if (obj->type==geometry::cube)
        return new btBoxShape(btVector3(obj->scale.x, obj->scale.y, obj->scale.z));
    else if (obj->type==geometry::sphere)
        return new btSphereShape(btScalar(obj->scale.x));
    else if (obj->type==geometry::bunny) {
        return new btBoxShape(btVector3((bunnyDim::thick/2)*obj->scale.x, (bunnyDim::height/2)*obj->scale.y, (bunnyDim::width/2)*obj->scale.z));
    }
    return new btBoxShape(btVector3(obj->scale.x, obj->scale.y, obj->scale.z));
}

void PhysicalWorld::addObject(Object *obj){
    btCollisionShape* colShape = getShape(obj);
    
    collisionShapes.push_back(colShape);
    glObjects.insert({obj->id, obj}); // Generalize (link to openGL)

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    btScalar mass;
    if (obj->physicType==PHYSIC::UNMOVABLE) 
        mass = btScalar(0.);
    else
        mass = btScalar(obj->scale.x * obj->scale.y * obj->scale.z);
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
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
    //rbInfo.m_initialLinearVelocity(btVector3(velocity.x, velocity.y,velocity.z)); // set initial velocity 

    btRigidBody* body = new btRigidBody(rbInfo);
    body->setUserIndex(obj->id); // >0 used for classical collisionable object with openGL display (e.g. dominos)
    if (obj->physicType==PHYSIC::UNMOVABLE) 
        setType(body, PHYSIC::UNMOVABLE);
    else
        setType(body, PHYSIC::MOVABLE);
    //body->setLinearVelocity(btVector3(velocity.x, velocity.y,velocity.z)); // set initial velocity 
    dynamicsWorld->addRigidBody(body);
}

void PhysicalWorld::RayCastPush(glm::vec3 from, glm::vec3 to, int type, int power) {
    btVector3 rayFrom = btVector3(from.x, from.y, from.z);
    btVector3 rayTo = btVector3(to.x, to.y, to.z);
    btCollisionWorld::AllHitsRayResultCallback callback(rayFrom, rayTo);
    dynamicsWorld->rayTest(rayFrom, rayTo, callback);

    btRigidBody* hitRigidbody = nullptr;
    btScalar closestHitFraction = 1;
    int closestIdx = -1;

    if (callback.hasHit())
    {
        for (int i = 0; i < callback.m_collisionObjects.size(); i++)
        {
            btCollisionObject* object = const_cast<btCollisionObject*>(callback.m_collisionObjects[i]);
            if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY) {
                hitRigidbody = btRigidBody::upcast(object);
                if (getType(hitRigidbody) == PHYSIC::ANY_TYPE || getType(hitRigidbody) == type) { // Check for object type
                    if (callback.m_hitFractions[i] < closestHitFraction) {
                        closestHitFraction = callback.m_hitFractions[i];
                        closestIdx = i;
                    }
                }
            }
        }

        if (closestIdx != -1) {
            btCollisionObject* object = const_cast<btCollisionObject*>(callback.m_collisionObjects[closestIdx]);
            hitRigidbody = btRigidBody::upcast(object);
            btVector3 hitPoint = callback.m_hitPointWorld[closestIdx];
            btVector3 dir = rayTo-rayFrom;
            btVector3 impulse = (dir/dir.length()) * (power * pow(hitRigidbody->getMass(),.5));//pow(power, general::pushingExponent); // impulse vector
            btVector3 relativePosition = hitPoint - hitRigidbody->getCenterOfMassPosition(); // relative position

            hitRigidbody->activate(true);
            hitRigidbody->applyImpulse(impulse, relativePosition);
            //hitRigidbody->applyForce(impulse, relativePosition);
        }
    }
}


Object* PhysicalWorld::RayCastObj(glm::vec3 from, glm::vec3 to, int type) {
    btRigidBody* body = RayCastBody(from, to, type);
    if (body!=nullptr) { return glObjects.at(body->getUserIndex()); }
    else { return nullptr; }

}

int PhysicalWorld::DeleteRayCastObj(glm::vec3 from, glm::vec3 to, int type) {
    btRigidBody* body = RayCastBody(from, to, type);
    if (body != nullptr) {
        dynamicsWorld->removeRigidBody(body);
        delete body;
        return body->getUserIndex();
    }
    return -1;
}

btRigidBody* PhysicalWorld::RayCastBody(glm::vec3 from, glm::vec3 to, int type) {
    btVector3 rayFrom = btVector3(from.x, from.y, from.z);
    btVector3 rayTo = btVector3(to.x, to.y, to.z);
    btCollisionWorld::AllHitsRayResultCallback callback(rayFrom, rayTo);
    dynamicsWorld->rayTest(rayFrom, rayTo, callback);

    btRigidBody* hitRigidbody = nullptr;
    btScalar closestHitFraction = 1;
    int closestIdx = -1;

    if (callback.hasHit())
    {
        for (int i = 0; i < callback.m_collisionObjects.size(); i++)
        {
            btCollisionObject* object = const_cast<btCollisionObject*>(callback.m_collisionObjects[i]);
            if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY) {
                hitRigidbody = btRigidBody::upcast(object);
                if (getType(hitRigidbody) == PHYSIC::ANY_TYPE || getType(hitRigidbody) == type) { // Check for object type
                    if (callback.m_hitFractions[i] < closestHitFraction) {
                        closestHitFraction = callback.m_hitFractions[i];
                        closestIdx = i;
                    }
                }
            }
        }
        if (closestIdx != -1) {
            btCollisionObject* object = const_cast<btCollisionObject*>(callback.m_collisionObjects[closestIdx]);
            return btRigidBody::upcast(object);
        }
    }
    return nullptr;   
}


glm::vec3 PhysicalWorld::RayCastPos(glm::vec3 from, glm::vec3 to, int type) {
    btVector3 rayFrom = btVector3(from.x, from.y, from.z);
    btVector3 rayTo = btVector3(to.x, to.y, to.z);
    btCollisionWorld::AllHitsRayResultCallback callback(rayFrom, rayTo);
    dynamicsWorld->rayTest(rayFrom, rayTo, callback);

    btRigidBody* hitRigidbody = nullptr;
    btScalar closestHitFraction = 1;
    int closestIdx = -1;

    if (callback.hasHit())
    {
        for (int i = 0; i < callback.m_collisionObjects.size(); i++)
        {
            btCollisionObject* object = const_cast<btCollisionObject*>(callback.m_collisionObjects[i]);
            if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY) {
                hitRigidbody = btRigidBody::upcast(object);
                if (getType(hitRigidbody) == PHYSIC::ANY_TYPE || getType(hitRigidbody) == type) { // Check for object type
                    if (callback.m_hitFractions[i] < closestHitFraction) {
                        closestHitFraction = callback.m_hitFractions[i];
                        closestIdx = i;
                    }
                }
            }
        }

        if (closestIdx != -1) {
            btVector3 hitPoint = callback.m_hitPointWorld[closestIdx];
            return glm::vec3(hitPoint.getX(), hitPoint.getY(), hitPoint.getZ());
        }
    }
    return glm::vec3(-1);
}


void PhysicalWorld::animate()
{
    ///-----stepsimulation_start-----
    dynamicsWorld->stepSimulation(Time::getSpeed()/60, 1, Time::getSpeed()/60); 
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
            // If collision
            trans = obj->getWorldTransform();
        }

        if (getType(body) != PHYSIC::UNMOVABLE){ // not ground
            Object* glObj = glObjects.at(body->getUserIndex());
            btScalar roll, pitch, yaw;
            trans.getRotation().getEulerZYX(yaw,pitch,roll);
            glm::vec3 translation = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
            glObj->setPosRot(translation, glm::vec3(roll, pitch, yaw));
        }
    }
}

void PhysicalWorld::reset() {
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
}

void PhysicalWorld::clear(){
        //cleanup in the reverse order of creation/initialization

        ///-----cleanup_start-----

        reset();

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

PhysicalWorld::~PhysicalWorld(){}
