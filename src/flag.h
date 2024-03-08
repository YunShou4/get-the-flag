#ifndef __FLAG_AGENT__H
#define __FLAG_AGENT__H 

#include "enviro.h"

using namespace enviro;

//! The main controller of Flag object
//! Flag is a static object
class FlagController : public Process, public AgentInterface {

    public:
        FlagController() : Process(), AgentInterface() {}

        // Flag has nothing to do
        void init() {}
        void start() {}
        void update() {}
        void stop() {}
};

class Flag : public Agent {
    
    public:
        Flag(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
    private:
        FlagController c;
};

DECLARE_INTERFACE(Flag)

#endif