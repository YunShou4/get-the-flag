#ifndef __DOOR_AGENT__H
#define __DOOR_AGENT__H 

#include "enviro.h"

using namespace enviro;

//! The main controller of Door object
//! Door is a static object
class DoorController : public Process, public AgentInterface {

    public:
        DoorController() : Process(), AgentInterface() {}

        // Door has nothing to do
        void init() {}
        void start() {}
        void update() {}
        void stop() {}
};

class Door : public Agent {
    
    public:
        Door(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
    private:
        DoorController c;
};

DECLARE_INTERFACE(Door)

#endif