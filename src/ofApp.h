#pragma once

#include "ofMath.h"
#include "Particle.h"
#include "ofxLeapMotion.h"
#include "LeapMotion.h"
#include "ClothEffect.h"

#define FINGER_NUM 10
#define LOG_NUM 10

class ofApp : public ofBaseApp{

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
    
        /* Particle */
        vector <Particle> particles;

    //----------------------------------------------------LeapMotion
    LeapMotion leapMotion;
    
    //-----------------------------------------------------Cloth Particle
    
        ClothEffect clotheffect;
};
