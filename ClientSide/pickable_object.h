#ifndef PICKABLEOBJECT_H
#define PICKABLEOBJECT_H

#include "puzzle_object.h"
#include "player.h"
#include "camera.h"

#define PICK_RADIUS 50

class PickableObject : public PuzzleObject
{
    public:
        PickableObject(btDynamicsWorld* world,
                       const std::string& name,
                       const std::vector<Entity*>& entities,
                       const std::function<void(PuzzleObject*)>& entitiesRelationshipFunction,
                       const std::function<void(PuzzleObject*, unsigned int)>& actionFunction);
        void update();
        void setIsTouched(bool isTouched){
            this->isTouched = isTouched;
        }
        bool getIsTouched(){
            return isTouched;
        }
        bool getIsPickedUp(){
            return isPickedUp;
        }
        void setIsPickedUp(bool isPickedUp){
            this->isPickedUp = isPickedUp;
        }
        btRigidBody *getInteractionSphereBody(){
            return interactionSphereBody;
        }
        static void setPlayer(Player *player){
            PickableObject::player = player;
        }
        static void setCamera(Camera *camera){
            PickableObject::camera = camera;
        }
        void pickUp();
        void release();
        virtual ~PickableObject();
    protected:
    private:
        btRigidBody *interactionSphereBody;
        bool isTouched, isPickedUp;
        static Player *player;
        static Camera *camera;
};

#endif // PICKABLEOBJECT_H
