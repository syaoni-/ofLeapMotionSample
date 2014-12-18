//
//  CircleDetection.cpp
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/18.
//
//

#include "CircleDetection.h"

CircleDetection::CircleDetection(){
    currentLogCount = 0;
}

bool CircleDetection::detection(ofVec3f *fingerLog){
    

    return false;
    
}

void CircleDetection::getLog(ofVec3f log){
    
    circleLog[currentLogCount].x = log.x;
    circleLog[currentLogCount].y = log.y;
    
    if (currentLogCount < LOG_NUM) {
        currentLogCount++;
    } else {
        currentLogCount = 0;
    }
    
}