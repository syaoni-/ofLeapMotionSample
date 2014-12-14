//
//  FingerAcceleration.h
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/14.
//
//


#pragma once
#include "ofMain.h"

class FingerAcceleration{
    public:
        ofVec3f fingerPos;
        ofVec3f preFingerPos; //前フレームの指の位置
        float fingerPosLog; //指に落ちのログ(前フレームと現在のフレームの距離を取る)
        float logGetInterval;
};
