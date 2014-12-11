//
//  ClothSpring.h
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/11.
//
//

#pragma once
#include "ofMain.h"
#include "ClothParticle.h"

struct ClothSpring {
    ClothParticle *a, *b;
    float k;    //バネ係数
    float rest_length;
    
    ClothSpring(ClothParticle *a, ClothParticle *b, float k = 0.2) : a(a), b(b), k(k) {
        rest_length = (b->pos - a->pos).length();
    }
    
    void update() {
        ofVec3f dir = b->pos - a->pos;
        float dist = dir.length();
        if (dist == 0.0) dist = 0.0001; // prevent division by zero
        float f = (rest_length - dist) * k; // linear force spring
        dir.normalize();
        a->addForce(dir * -f);
        b->addForce(dir * f);
    }
};
