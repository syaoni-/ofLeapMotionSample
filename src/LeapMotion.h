//
//  LeapMotion.h
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/27.
//
//

#pragma once
#include "ofMain.h"
#include "ofxLeapMotion.h"

#define FINGER_NUM 10
#define LOG_NUM 10

class LeapMotion : public ofBaseApp{
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
    
        /* 手の検知 */
        bool guuDic(ofVec3f *fPos);
        bool chokiDic(ofVec3f hPos, ofVec3f *fPos);
        bool paaDic(ofVec3f hPos, ofVec3f *fPos);
    
        void getController(Controller customCon);
    
    private:
        // Leap Motionのコントローラー
        Leap::Controller controller;
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
        ofVec3f fingerPos[FINGER_NUM];
        ofVec3f preFingerPos[FINGER_NUM]; //前フレームの指の位置
    
        /* 加速度 */
        float fingerAcceleration[FINGER_NUM];
    
        /* グー・チョキ・パーの画像（デバッグ用） */
        ofImage guu;
        ofImage choki;
        ofImage paa;
    
        /* 動いている方向 */
        ofVec3f preMoveDirection;
        ofVec3f moveDirection;
        ofVec3f aboutMoveDirection;
    
        /* 叩き */
        bool slapDec();
    
        /* 拍 */
        bool beatDetection();

    
    protected:
};