#ifndef __GHOST_AGENT__H
#define __GHOST_AGENT__H 

#include "enviro.h"

using namespace enviro;

//! The main controller of Ghost object
//! Ghost is a dynamic object
//! Ghost patrols between two Bumpers
//! Ghost will kill Guy if colliding with a Guy
class GhostController : public Process, public AgentInterface {

    public:
        GhostController() : Process(), AgentInterface() {
            // Init the velocity for the Ghost
            vx = 20;
        }

        void init() {
            // Prevents the agent from rotating
            prevent_rotation();

            // If the Ghost makes a collision with a Bumper
            // then change the direction
            notice_collisions_with("Bumper", [&](Event &e) {
                vx = -vx;
            });

            // Styling the object
            decorate(R"(<g>
                <circle cx=-5 cy=-3 r=2 style='fill:black'></circle>
                <circle cx=5 cy=-3 r=2 style='fill:black'></circle></g>)");
        }

        void start() {}

        void update() {
            // Update the velocity when changing the direction
            double fx = -30 * (velocity().x - vx);
            omni_apply_force(fx, 0);
        }

        void stop() {}

        double vx;
};

class Ghost : public Agent {

    public:
        Ghost(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
    private:
        GhostController c;
};

DECLARE_INTERFACE(Ghost)

#endif