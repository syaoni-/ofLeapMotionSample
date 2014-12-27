//
//  ClothParticle.h
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/11.
//
//

#pragma once
#include "ofMain.h"

struct ClothParticle {
    ofVec3f pos,vel,forces;
    float mass,inverse_mass,drag;
    
    ClothParticle(ofVec3f pos, float mass, float drag = 0.96) : pos(pos), mass(mass), drag(drag) {
        if (mass==0.0f) inverse_mass = 0;
        else if (mass<0.001) mass=0.001;
        if (mass!=0.0f) inverse_mass = 1/mass;
    }
    
    void update() {
        forces *= inverse_mass;
        vel += forces;
        forces.x = 0;
        forces.y = 0;
        forces.z = 0;
        vel.limit(15);
        pos += vel;
        vel *= drag;
    }
    
    void addForce(ofVec3f oForce) {
        forces += oForce;
    }
    
};
