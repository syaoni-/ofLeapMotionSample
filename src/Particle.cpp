//
//  Particle.cpp
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/04.
//
//

#include "Particle.h"

//初期化
Particle::Particle(){
    setInitialCondition(0,0,0,0);
    damping = 0.01f;
    outOffsetX = 100.0f;
    outOffsetY = 500.0f;
}

//力（加速度）をリセット
void Particle::resetForce(){
    frc.set(0, 0);
}

//力を加える
void Particle::addForce(float x, float y){
    frc.x += x;
    frc.y += y;
}

//抵抗力の計算
void Particle::addDampingForce(){
    frc.x -= vel.x * damping;
    frc.y -= vel.y * damping;
}

//初期状態を設定
void Particle::setInitialCondition(float px, float py, float vx, float vy){
    pos.set(px, py);
    vel.set(vx, vy);
}

//更新
void Particle::update(){
    vel += frc;
    pos += vel;
}

//描画
void Particle::draw(){
    ofCircle(pos.x, pos.y, 3);
}

bool Particle::outWindow(){
    if (pos.x < 0 - outOffsetX || pos.x > ofGetWidth() + outOffsetX || pos.y < 0 - outOffsetY || pos.y > ofGetHeight() + outOffsetY)
        return true;
    return false;
}