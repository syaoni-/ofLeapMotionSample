#pragma once

#include "ofMain.h"
#include "ofMath.h"
#include "Particle.h"
#include "ofxLeapMotion.h"
#include "ClothParticle.h"
#include "ClothSpring.h"
#include "FingerAcceleration.h"

#define NUM 100
#define FINGER_NUM 10
#define LOG_INTERVAL 0.5

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
        ofVec3f indexFingerPos;
        ofVec3f fingerPos[FINGER_NUM];
        ofVec3f preFingerPos[FINGER_NUM]; //前フレームの指の位置

        /* 加速度 */
        float preElapsedTime;
        float logTime;
        float fingerAcceleration[FINGER_NUM];
        //FingerAcceleration fingerLog[FINGER_NUM];
    
        /* 手の検知 */
        bool guuDic(ofVec3f *fPos);
        bool chokiDic(ofVec3f hPos, ofVec3f *fPos);
        bool paaDic(ofVec3f hPos, ofVec3f *fPos);
        ofImage guu;
        ofImage choki;
        ofImage paa;
    
        /* 動いている方向 */
        ofVec3f moveDirection;
        ofVec3f aboutMoveDirection;
    
    //-----------------------------------------------------Cloth Particle
        vector <ClothParticle> ps;
        vector <ClothSpring> sp;
    
};
