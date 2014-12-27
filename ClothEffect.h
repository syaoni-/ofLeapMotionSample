//
//  ClothEffect.h
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/27.
//
//

#pragma once
#include "ofMain.h"
#include "ClothParticle.h"
#include "ClothSpring.h"

#define ROWS 20
#define COLS 20

class ClothEffect : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();
    
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
    private:
        //-----------------------------------------------------Cloth Particle
        vector <ClothParticle> ps;
        vector <ClothSpring> sp;
    
    protected:
};
