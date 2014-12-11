#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ofxLeapMotion.h"
#include "ClothParticle.h"
#include "ClothSpring.h"

#define NUM 100

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
    
        // Leap Motionのコントローラー
        Controller controller;
        // カメラ
        ofCamera camera;
        // 球体の描画処理
        void drawSphere(Leap::Vector vector, float radius);
    
        float camdistance;
        float camdegree;
        void drawFinger(Finger finger);
        void drawPoint(ofPoint point);
        void drawFingerBox(Finger finger, ofPoint tip, ofPoint base);
        void drawPalm(Hand hand);
    
    //-----------------------------------------------------Cloth Particle
        vector <ClothParticle> ps;
        vector <ClothSpring> sp;
    
};
