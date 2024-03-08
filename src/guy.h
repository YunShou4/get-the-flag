#ifndef __GUY_AGENT__H
#define __GUY_AGENT__H 

#include "enviro.h"
#include <math.h>

using namespace enviro;

//! Simple implementation for a MIN(a,b) method
double min(double a, double b) {
    if (a < b) { 
        return a;
    } else {
        return b;
    }
}

//! The main controller of Guy object
//! Guy is a dynamic object
//! Guy is in control of the player with keyboard
//! Guy aims to get the red flag in the maze
//! Guy can move left and right, and jump
//! Guy will be killed by Ghost if colliding with a Ghost
//! Guy will be killed by Bumper if colliding with a Bumper
//! Guy can tp between the doors with a same color
class GuyController : public Process, public AgentInterface {

    public:
        GuyController() : Process(), AgentInterface() {
            LEFT = false;
            RIGHT = false; 
            JUMP = false;
        }

        //! Get the height (distance to a platform) of Guy
        double height() {
            return min(sensor_value(0), sensor_value(1));
        }

        //! To judge whether Guy is jumping
        double airborne() { 
            return height() > H_MIN;
        }

        void init() {
            // Prevents the agent from rotating
            prevent_rotation();

            // Keyboard mapping: 
            // A: go left    
            // D: go right    
            // Space: jump
            watch("keydown", [&](Event& e) {
                std::string k = e.value()["key"];
                if (k == " " && !airborne()) {
                    JUMP = true;
                } else if (k == "a") {
                    LEFT = true;
                } else if (k == "d") {
                    RIGHT = true;
                } 
            });
            watch("keyup", [&](Event& e) {
                std::string k = e.value()["key"];
                if (k == "a") {
                    LEFT = false;
                } else if (k == "d") {
                    RIGHT = false;
                }
            });

            // If the Guy makes a collision with a Ghost or a Bumper or the Flag
            // then go back to the starting point
            notice_collisions_with("Ghost", [&](Event &e) {
                teleport(-40,135,0);
            });  
            notice_collisions_with("Bumper", [&](Event &e) {
                teleport(-40,135,0);
            }); 
            notice_collisions_with("Flag", [&](Event &e) {
                teleport(-40,135,0);
            });    

            // If the Guy makes a collision with a Door
            // then tp to the door with the same color
            notice_collisions_with("Door", [&](Event &e) {
                teleport(-315,-120,0);
            });           
        }

        void start() {}

        void update() {
            double fx;
            double fy = JUMP ? JUMP_F : 0;
            // Update the velocity
            if (!airborne()) {
                // If Guy is not jumping
                if (RIGHT) {
                    vx = VEL_X;
                } if (LEFT) {
                    vx = -VEL_X;
                } else if (!RIGHT && !LEFT) {
                    vx = 0;
                }
                fx = -K_X * (velocity().x - vx);
            } else {
                // If Guy is jumping
                // Guy will get less velocity of X-axis
                if (RIGHT) {
                    vx = 0.2 * VEL_X;
                } if (LEFT) {
                    vx = -0.2 * VEL_X;
                }            
                fx = 0;
                fx = -K_X * (velocity().x - vx);
            }
            omni_apply_force(fx, G + fy);
            JUMP = false;
        }

        void stop() {}

        double vx;
        // Keyboard control
        bool LEFT, RIGHT, JUMP;
        // Velocity of X-axis
        const double VEL_X = 20;
        // Velocity of jumping (Y-axis)
        const double JUMP_F = -2200;
        // Acceleration
        const double K_X = 15;
        // Simulate gravity
        const double G = 500;
        // Judge whether Guy is jumping
        const double H_MIN = 1.0;
};

class Guy : public Agent {

    public:
        Guy(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }

    private:
        GuyController c;
};

DECLARE_INTERFACE(Guy)

#endif