//
//  CircleDetection.h
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/18.
//
//

#pragma once
#include "ofMain.h"

#define LOG_NUM 60
#define END_DISTANCE 10

class CircleDetection{
    public:
        void getLog(ofVec3f log);
        bool detection(ofVec3f *fingerLog);
        CircleDetection();
        virtual ~CircleDetection(){};
    private:
        int currentLogCount;
        ofVec2f circleLog[LOG_NUM];
    protected:
};